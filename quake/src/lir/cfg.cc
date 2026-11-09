#include "lir/cfg.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"

#include <numeric>

namespace quake {
namespace lir {

// Walk forward past any consecutive phi instructions to the first real
// instruction. Used to resolve label bind points that may still point at
// removed phi instructions after SSA destruction (VregPass): the phi
// instruction's memory is still valid (zone-allocated, freed only at zone
// clear), so IsPhi() / next() work even on unlinked instructions.
static HInstruction* SkipPhis(HInstruction* start) {
  while (start && start->IsPhi()) {
    start = start->next();
  }
  return start;
}

const char* BasicBlock::GetBlockTypeName(int bt) {
  switch (bt) {
    case kBlock:
      return "block";
    case kStart:
      return "start";
    case kEnd:
      return "end";
    case kDeoptBlock:
      return "deopt";
    default:
      UNREACHABLE();
      return "";
  }
}

std::size_t BasicBlock::GetInstructionSize() const {
  std::size_t cnt = 0;
  ForeachInstruction([&](auto) -> bool {
    cnt++;
    return true;
  });
  return cnt;
}

const HInstruction* BasicBlock::GetInstruction(std::size_t idx) const {
  const HInstruction* ins = nullptr;

  ForeachInstruction([&](auto x) {
    if (idx == 0) {
      ins = x;
      return false;
    }
    --idx;
    return true;
  });

  CHECK(ins);
  return ins;
}

HInstruction* BasicBlock::GetInstruction(std::size_t idx) {
  return const_cast<HInstruction*>(GetInstruction(idx));
}

bool BasicBlock::ForeachInstruction(
    std::function<bool(HInstruction*)>&& visitor) {
  for (auto ins = start(); ins != end();) {
    auto next = ins->next();
    if (!visitor(ins)) return false;
    ins = next;
  }
  return true;
}

bool BasicBlock::ForeachInstruction(
    std::function<bool(const HInstruction*)>&& visitor) const {
  for (auto ins = start(); ins != end();) {
    auto next = ins->next();
    if (!visitor(ins)) return false;
    ins = next;
  }
  return true;
}

bool BasicBlock::ForeachEdge(std::function<bool(const Edge&)>&& visitor) const {
  for (const auto& edge : prev_) {
    if (!visitor(edge)) return false;
  }
  return true;
}

std::string BasicBlock::PrintGraphName() const {
  return base::Fmt("bb_%d", block_id());
}

std::string BasicBlock::PrintGraphNode() const {
  // format the label information into box to allow we inspect the instruction
  std::stringstream fmt;
  fmt << "(" << block_type_name() << ")\n";
  ForeachInstruction([&](const auto ins) -> bool {
    fmt << ins->Print() << "\n";
    return true;
  });

  const auto label = GraphvizNode::EscapeLabelString(fmt.str());
  return base::Fmt("%s [color=blue shape=box label=\"%s\"]",
                   PrintGraphName().c_str(), label.c_str());
}

std::string BasicBlock::PrintGraphLink() const {
  std::stringstream fmt;

  auto name = PrintGraphName();
  if (left_) {
    auto l = left_->PrintGraphName();
    fmt << name << " -> " << l << "\n";
  }
  if (right_) {
    auto r = right_->PrintGraphName();
    fmt << name << " -> " << r << "\n";
  }
  return fmt.str();
}

std::string BasicBlock::Print() const {
  std::stringstream fmt;
  fmt << "#" << block_id() << " ";
  fmt << "(" << block_type_name() << ")";
  fmt << "{\n";

  // printing out all the instruction next to each other in the BB
  ForeachInstruction([&](auto instruction) -> bool {
    fmt << "  " << instruction->Print() << '\n';
    return true;
  });

  fmt << "}\n";

  if (left_) {
    fmt << "==>("
        << "#" << left_->block_id() << ";";
  } else {
    fmt << "==>(N/A"
        << ";";
  }

  if (right_) {
    fmt << "#" << right_->block_id() << ")";
  } else {
    fmt << "N/A)";
  }

  fmt << '\n';

  return fmt.str();
}

void BasicBlock::EmptyBlock(HAssembler* as) {
  start_ = end_ = as->end_instruction();
}

void BasicBlock::set_end(HInstruction* ins) {
  if (end_) {
    CHECK(end_->RemoveUse(this, 1));
  }
  end_ = ins;
  end_->AddUse(this, 1, &end_);
}

void BasicBlock::set_start(HInstruction* ins) {
  if (start_) {
    CHECK(start_->RemoveUse(this, 1));
  }
  start_ = ins;
  start_->AddUse(this, 1, &start_);
}

void BasicBlock::set_idom(BasicBlock* bb) {
  if (bb != this) {
    idom_ = bb;
    idom_->domtree_children_.push_back(this);
  }
}

// ----------------------------------------------------------------------------
// Construction of CFG from HAssembler
//
// The algorithm works in 2 passes, one pass is to discovery all the BB along
// with its starting HInstruction, and the second pass is to find each BB's
// range and link them together

BasicBlock* CFG::AddBB(HInstruction* start) {
  start = SkipPhis(start);
  if (!start) return nullptr;

  auto ret =
      bbmap_.insert(std::make_pair(start, static_cast<BasicBlock*>(nullptr)));

  if (ret.second) {
    ret.first->second = NewBB();
    auto bb = ret.first->second;
    bb->set_start(start);
  }

  return ret.first->second;
}

BasicBlock* CFG::GetBB(HInstruction* ins) {
  if (auto itr = bbmap_.find(ins); itr != bbmap_.end()) {
    return itr->second;
  } else {
    return nullptr;
  }
}

bool CFG::HasBB(HInstruction* ins) const {
  return bbmap_.find(ins) != bbmap_.end();
}

BasicBlock* CFG::NewBB() {
  return quake_->zone()->New<BasicBlock>(quake_, block_id_++);
}

void CFG::FindBB(HAssembler* as) {
  start_ = NewBB();
  start_->set_block_type(BasicBlock::kStart);
  start_->EmptyBlock(as);
  start_->LinkLeft(AddBB(as->start_instruction()));

  end_ = NewBB();
  end_->set_block_type(BasicBlock::kEnd);
  end_->EmptyBlock(as);

  deopt_ = NewBB();
  deopt_->set_block_type(BasicBlock::kDeoptBlock);
  deopt_->EmptyBlock(as);
  deopt_->LinkLeft(end_);

  as->ForeachInstruction([&](HInstruction* ins) {
    // Only jump/branch code will/can start a new BB, notes all the deopt will
    // be linked to a deopt BB block, ie special block
    switch (ins->opcode()) {
      case HOpcode::kJmpNe:
      case HOpcode::kJmpEq:
      case HOpcode::kJmpLt:
      case HOpcode::kJmpLe:
      case HOpcode::kJmpBt:
      case HOpcode::kJmpBe:
      case HOpcode::kJmpGt:
      case HOpcode::kJmpGe:
      case HOpcode::kJmpAt:
      case HOpcode::kJmpAe:
      case HOpcode::kJmpNz:
      case HOpcode::kJmpZ:
      case HOpcode::kJmpBoxIsHeap:
      case HOpcode::kJmpBoxIsPrimitive: {
        // conditional jump, ie will result in 2 blocks afterwards
        auto opr0 = ins->Opr(0);
        CHECK(opr0->IsLabel());
        auto label = opr0->GetLabel();
        CHECK(label->HasBind());

        AddBB(label->bind());  // jumpping target block
        AddBB(ins->next());
      } break;

      case HOpcode::kJmp: {
        auto opr0 = ins->Opr(0);
        CHECK(opr0->IsLabel());
        auto label = opr0->GetLabel();
        CHECK(label->HasBind());
        AddBB(label->bind());
      } break;

      case HOpcode::kDeopt:
      case HOpcode::kDeoptIfTrue:
      case HOpcode::kDeoptIfFalse:
      case HOpcode::kDeoptNe:
      case HOpcode::kDeoptEq:

      case HOpcode::kDeoptGt:
      case HOpcode::kDeoptGe:
      case HOpcode::kDeoptLt:
      case HOpcode::kDeoptLe:

      case HOpcode::kDeoptBt:
      case HOpcode::kDeoptBe:
      case HOpcode::kDeoptAt:
      case HOpcode::kDeoptAe:

      case HOpcode::kDeoptNz:
      case HOpcode::kDeoptZ: {
        AddBB(ins->next());  // next instruction forms a fallthrough basic block
      } break;

      default:
        // notes for deoptimization, we don't need to care about it
        // since it will be linked to a specialized end block which
        // labeled as deoptimization and finally link to the end block
        break;
    }
  });
}

void CFG::MaterializeBB(HAssembler* as) {
  for (const auto& entry : bbmap_) {
    auto bb = entry.second;

    auto ins = bb->start();
    auto end = as->end_instruction();

    for (ins = bb->start()->next(); ins != end; ins = ins->next()) {
      if (ins->IsJump() ||  // must be a jump
          ins->IsDeopt())   // or must be a deoptimization
        break;

      if (HasBB(ins)) {
        // this instruction starts a new BB
        ins = ins->prev();
        break;
      }
    }

    auto end_of_block = ins == end ? end : ins->next();

    bb->end_ = end_of_block;  // setup it one instruction off by

    // Perform BB linking, notes, for conditional jump the branch is a fork,
    // ie both left and right should be linked together
    if (ins->IsJump() || ins->IsDeopt()) {
      // the current basic block ends with a jump or deoptimization
      if (ins->IsJump()) {
        auto jump_target = SkipPhis(ins->Opr(0)->GetLabel()->bind());
        // If the jump target resolves to null (all phis with no successor
        // instruction — can happen with dead phi-only blocks after SSA
        // destruction), link to the shared end block as a safe fallback.
        if (!jump_target) jump_target = end_->start();
        auto next_bb = GetBB(jump_target);
        CHECK(next_bb);
        bb->LinkLeft(next_bb);
      } else {
        bb->LinkLeft(deopt_);
      }

      // If it is an conditional jump, link it to the right hand side, ie fork
      // branch
      if (ins->IsConditionalJump() || ins->IsConditionalDeopt()) {
        auto next_ins = end_of_block;
        auto fallthrough_bb = GetBB(next_ins);
        CHECK(fallthrough_bb);
        bb->LinkRight(fallthrough_bb);
      }
    } else if (ins->IsDeopt()) {
      auto next_bb = deopt_;
      bb->LinkLeft(next_bb);
    } else if (ins == end) {
      // The block runs off the tail of the instruction stream into the
      // sentry: this happens on real traces when the block LEADER is the
      // trace's loop-back jump (a leader whenever a deopt/conditional jump
      // precedes it) — the block then holds the loop-back jump, walks to the
      // kEnd marker and falls through with no in-stream successor. Link it
      // to the shared end pseudo block, the same sink the deopt pseudo
      // block flows into; nothing is live past the end of the trace.
      bb->LinkLeft(end_);
    } else {
      auto next_bb = GetBB(end_of_block);
      CHECK(next_bb);
      bb->LinkLeft(next_bb);
    }

    // Must alreay been setup
    CHECK(bb->start());
    CHECK(bb->end());
    CHECK(bb->left_bb());  // at least have left
  }

  // Lastly, linking the whole graph together
  {
    auto deopt_bb = deopt_;
    deopt_bb->LinkLeft(end_);
  }
}

void CFG::CreateCFG(HAssembler* as) {
  FindBB(as);
  MaterializeBB(as);
}

template <typename T>
bool CFG::ForeachBB(T&& visitor) {
  std::vector<bool> visited;
  std::deque<BasicBlock*> queue;
  visited.resize(block_id_, false);

  queue.push_back(start_);

  while (!queue.empty()) {
    auto top = queue.front();
    queue.pop_front();

    if (visited[top->block_id()]) continue;

    // visit the basic block
    if (!visitor(top)) return false;

    visited[top->block_id()] = true;

    // enqueue its end if needed
    auto lhs = top->left_bb();
    auto rhs = top->right_bb();

    if (lhs && !visited[lhs->block_id()]) {
      queue.push_back(lhs);
    }
    if (rhs && !visited[rhs->block_id()]) {
      queue.push_back(rhs);
    }
  }

  return true;
}

bool CFG::ForeachBasicBlock(
    std::function<bool(const BasicBlock*)>&& visitor) const {
  return const_cast<CFG*>(this)->ForeachBB(std::move(visitor));
}

bool CFG::ForeachBasicBlock(std::function<bool(BasicBlock*)>&& visitor) {
  return ForeachBB(std::move(visitor));
}

std::string CFG::Print() const {
  std::stringstream fmt;
  ForeachBasicBlock([&](const auto bb) -> bool {
    fmt << bb->Print();
    return true;
  });
  return fmt.str();
}

std::string CFG::PrintGraph() const {
  std::stringstream fmt;
  fmt << "digraph CFG {\n";
  ForeachBasicBlock([&](const auto bb) -> bool {
    fmt << bb->PrintGraphNode();
    fmt << bb->PrintGraphLink();
    return true;
  });
  fmt << "}\n";
  return fmt.str();
}

std::string CFG::PrintDom() const {
  std::stringstream fmt;
  fmt << "digraph domtree {\n";

  ForeachBasicBlock([&](const BasicBlock* bb) -> bool {
    // format the label text, which is a 2 line text, first line contains
    // the block type name and id, and the second line contains all the
    // dominator node's #
    auto first_line =
        base::Fmt("(%s)%d", bb->block_type_name(), bb->block_id());

    // dominator set information
    auto second_line =
        std::accumulate(bb->dom().begin(), bb->dom().end(), std::string{},
                        [&](std::string acc, BasicBlock* b) {
                          acc.append(base::Fmt("%d;", b->block_id()));
                          return acc;
                        });

    fmt << "idom_" << bb->block_id() << "[shape=box color=red label=\""
        << first_line << "\n"
        << second_line << "\"]\n";

    auto idom = bb->idom();
    if (!idom) return true;

    // skip idom that points to itself, ie start node
    if (idom == bb) return true;

    fmt << "idom_" << bb->block_id() << " -> "
        << "idom_" << idom->block_id() << "\n";

    return true;
  });

  fmt << "}\n";

  return fmt.str();
}

// ----------------------------------------------------------------------------
// Dominator Tree Analysis
// ----------------------------------------------------------------------------
//
// We run a simple data flow algorithm to find the dominator, ie dominator set
// of each BB is a union of its predecessor's dominator set
void CFG::RunDFSNumber() {
  std::stack<BasicBlock*> stack;
  const auto invalid_dfs_num = std::numeric_limits<std::uint32_t>::max();
  std::uint32_t dfs = 0;

  auto is_visited = [&](const BasicBlock* bb) -> bool {
    return bb->dfs_start_ != invalid_dfs_num;
  };

  stack.push(start_);

  while (!stack.empty()) {
    auto top = stack.top();
    if (top->dfs_start_ == invalid_dfs_num) {
      top->dfs_start_ = dfs++;  // start visiting this node

      // Enqueue successor only when they are not visiting yet, ie start ts is
      // not marked yet. Otherwise it is a grey node, ie been visiting
      if (auto lhs = top->left_bb(); lhs) {
        if (!is_visited(lhs)) {
          stack.push(lhs);
        }
      }

      if (auto rhs = top->right_bb(); rhs) {
        if (!is_visited(rhs)) {
          stack.push(rhs);
        }
      }
    } else {
      top->dfs_end_ = dfs++;
      stack.pop();
    }
  }
}

void CFG::RunDom() {
  // FIXME(dpeng): Optimize it to use a common dominator way to create the
  //   dom tree. The current set operation is too expensive.
  using DomSet = std::set<BasicBlock*>;
  std::vector<DomSet> dom;
  dom.resize(block_id_, DomSet{});
  auto has_change = false;

  do {
    has_change = false;
    ForeachBasicBlock([&](BasicBlock* bb) -> bool {
      auto& d = dom[bb->block_id()];
      if (d.empty()) {
        d.insert(bb);
      }

      // Now visiting all its predecessor node and union all the set together
      DomSet temp_dom;

      bb->ForeachEdge([&](const BasicBlock::Edge& e) -> bool {
        auto prev = e.prev;

        if (prev->dfs_start_ > bb->dfs_start_) return true;  // ignore backedge

        const auto& pd = dom[prev->block_id()];
        if (temp_dom.empty()) {
          temp_dom = pd;
        } else {
          DomSet tmp;
          std::set_intersection(temp_dom.begin(), temp_dom.end(), pd.begin(),
                                pd.end(), std::inserter(tmp, tmp.begin()));
          temp_dom = std::move(tmp);
        }
        return true;
      });

      temp_dom.insert(bb);

      // Check whether we are the same or not, if it is not same then we should
      // run fix point

      if (temp_dom != d) {
        has_change = true;
        d = std::move(temp_dom);
      }

      return true;
    });
  } while (has_change);

  // materialize it back to the block
  ForeachBasicBlock([&](BasicBlock* bb) -> bool {
    const auto& dom_set = dom[bb->block_id()];
    BasicBlock* idom = nullptr;

    CHECK(!dom_set.empty());

    for (const auto& d : dom_set) {
      if (d != bb) {
        bb->dom_.push_back(d);
      }

      if (!idom) {
        idom = d;
      } else {
        if (d != bb && idom->dfs_start_ < d->dfs_start_) {
          idom = d;
        }
      }
    }

    bb->set_idom(idom);
    return true;
  });
}

template <typename T>
bool CFG::ForeachDom(T&& visitor) {
  std::queue<BasicBlock*> q;
  q.push(start_);
  while (q.empty()) {
    auto front = q.front();
    q.pop();
    if (!visitor(front)) return false;
    for (auto& c : front->domtree_children()) {
      q.push(c);
    }
  }
  return true;
}

bool CFG::ForeachDomTree(
    std::function<bool(const BasicBlock*)>&& visitor) const {
  return const_cast<CFG*>(this)->ForeachDom(std::move(visitor));
}

bool CFG::ForeachDomTree(std::function<bool(BasicBlock*)>&& visitor) {
  return ForeachDom(std::move(visitor));
}

void CFG::CreateDom() {
  RunDFSNumber();  // when we need to have RPO, then number it as RPO
  RunDom();        // perform dominator
}

}  // namespace lir
}  // namespace quake
