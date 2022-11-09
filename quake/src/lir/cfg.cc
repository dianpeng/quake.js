#include "lir/cfg.h"

#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"

namespace quake {
namespace lir {

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
  for (auto ins = start(); ins != end(); ins = ins->next()) {
    if (!visitor(ins)) return false;
  }
  return true;
}

bool BasicBlock::ForeachInstruction(
    std::function<bool(const HInstruction*)>&& visitor) const {
  for (auto ins = start(); ins != end(); ins = ins->next()) {
    if (!visitor(ins)) return false;
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

// ----------------------------------------------------------------------------
// Construction of CFG from HAssembler
//
// The algorithm works in 2 passes, one pass is to discovery all the BB along
// with its starting HInstruction, and the second pass is to find each BB's
// range and link them together

BasicBlock* CFG::AddBB(HInstruction* start) {
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
  start_ = AddBB(as->start_instruction());
  start_->set_block_type(BasicBlock::kStart);

  end_ = AddBB(as->end_instruction());
  end_->set_block_type(BasicBlock::kEnd);

  deopt_ = NewBB();
  deopt_->set_block_type(BasicBlock::kDeoptBlock);

  end_->EmptyBlock(as);
  deopt_->EmptyBlock(as);

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
        auto jump_target = ins->Opr(0)->GetLabel()->bind();
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

}  // namespace lir
}  // namespace quake
