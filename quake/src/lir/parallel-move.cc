#include "lir/parallel-move.h"
#include "lir/assembler.h"

namespace quake {
namespace lir {

void ParallelMove::AddRefReg(Move* ins, const Register* reg) {
  auto idx = static_cast<std::size_t>(reg->register_kind());
  register_[idx].read_list.push_back(ins);
}

void ParallelMove::AddRefSpill(Move* ins, MemoryStack::Slot slot) {
  auto idx = static_cast<std::size_t>(slot);
  spill_[idx].read_list.push_back(ins);
}

void ParallelMove::AddOutReg(Move* ins, const Register* r) {
  auto idx = static_cast<std::size_t>(r->register_kind());
  CHECK(register_[idx].generate_by == nullptr);
  register_[idx].generate_by = ins;
}

void ParallelMove::AddOutSpill(Move* ins, MemoryStack::Slot slot) {
  auto idx = static_cast<std::size_t>(slot);
  CHECK(spill_[idx].generate_by == nullptr);
  spill_[idx].generate_by = ins;
}

// ----------------------------------------------------------------------------
// Building a dependnecy graph, via following method. For each instruction
// we visit its LHS or RHS node, if it is a register, then we just create an
// entry in output_register_ field, index by register's RegisterKind value.  If
// we can find a populated entry, we just link it.
void ParallelMove::BuildInput(Move* move) {
  auto slot = ra_->Resolve(move->from);
  switch (slot.index()) {
    case kReg:
      AddRefReg(move, base::Get<const Register*>(slot));
      break;

    default:
      AddRefSpill(move, base::Get<MemoryStack::Slot>(slot));
      break;
  }
}

void ParallelMove::BuildOutput(Move* move) {
  auto slot = ra_->ResolvePhi(move->to);
  switch (slot.index()) {
    case kReg:
      AddOutReg(move, base::Get<const Register*>(slot));
      break;

    default:
      AddOutSpill(move, base::Get<MemoryStack::Slot>(slot));
      break;
  }
}

void ParallelMove::BuildDPG() {
  // (0) iterate all phi node and visit its output
  for (auto& move : move_list_) {
    // visit its output
    BuildInput(&move);
  }

  // (1) iterate all the phi node visit its ref
  for (auto& move : move_list_) {
    BuildOutput(&move);
  }
}

void ParallelMove::AddMicroSpill(const Move& move) {
  auto& read_node = FindReadNode(ra_->ResolvePhi(move.to));
  CHECK(read_node.generate_by == &move);
  read_node.spill = &move;
  read_node.generate_by = nullptr;
  micro_spill_.push_back(&move);  // hidden from rest of the worlds
}

ParallelMove::ReadNode& ParallelMove::FindReadNode(
    const ParallelMove::Slot& slot) {
  if (slot.index() == kReg) {
    auto idx = static_cast<std::size_t>(
        base::Get<const Register*>(slot)->register_kind());
    return register_[idx];
  } else {
    auto idx = static_cast<std::size_t>(base::Get<MemoryStack::Slot>(slot));
    return spill_[idx];
  }
}

class ParallelMove::CycleFinder {
 public:
  using ReadNode = ParallelMove::ReadNode;
  using Move = ParallelMove::Move;
  using ReadNodeList = std::vector<ReadNode>;

 public:
  CycleFinder(ParallelMove* pm, const ReadNodeList& read_node_list)
      : pm_(pm), read_node_list_(read_node_list) {}

  void FindCycle();

 private:
  void VisitOne(const Move*, const ReadNode&);
  void VisitOne(const Move&);

 private:
  ParallelMove* pm_;
  const ReadNodeList& read_node_list_;

 private:
  static constexpr std::uint32_t kWhite =
      std::numeric_limits<std::uint32_t>::max();

  static constexpr std::uint32_t kGray =
      std::numeric_limits<std::uint32_t>::max() - 1;

  std::uint32_t dfs_ts_ = 0;
  std::vector<std::uint32_t> visit_num_;
};

void ParallelMove::CycleFinder::VisitOne(const Move* move,
                                         const ReadNode& rnode) {
  // (0) Check whether the node has been visited or not
  if (auto ins = rnode.generate_by_instruction(); ins) {
    auto current_ts = visit_num_[ins->id()];
    switch (current_ts) {
      case kWhite:
        break;  // not yet marked

      case kGray:
        CHECK(move);
        // Violation of color invariant, ie try to visit a been visited node
        // which means there's a cycle
        pm_->AddMicroSpill(*move);
        return;

      default:
        // Already visited, just do nothing
        return;
    }

    visit_num_[ins->id()] = kGray;
  }

  // (1) Visit its children
  for (const auto& r : rnode.read_list) {
    auto vdfs = visit_num_[r->id()];

    switch (vdfs) {
      case kWhite:
        // Unvisited node, just visit it
        VisitOne(*r);
        break;

      case kGray:
        // The node is been visiting, haven't finished yet. Which means we have
        // a cycle, ie the current node somehow go back to a node that is not
        // finish itself's visiting
        pm_->AddMicroSpill(*r);
        break;

      default:
        // The node has been fully visited already, and it means not loop
        break;
    }
  }

  // (2) Lastly assign a real visiting ts to the node which means the node has
  //     been fully visited already
  if (auto ins = rnode.generate_by_instruction(); ins) {
    visit_num_[ins->id()] = dfs_ts_++;
  }
}

void ParallelMove::CycleFinder::VisitOne(const Move& move) {
  auto pred = [&](const ReadNode& rnode) -> bool {
    return rnode.generate_by == &move;
  };

  if (auto itr = std::find_if(read_node_list_.begin(), read_node_list_.end(),
                              std::move(pred));
      itr != read_node_list_.end()) {
    VisitOne(&move, *itr);
  }
}

void ParallelMove::CycleFinder::FindCycle() {
  for (const auto& node : read_node_list_) {
    dfs_ts_ = 0;
    visit_num_.resize(pm_->as_->instruction_maxid(), kWhite);
    VisitOne(nullptr, node);
  }
}

// The builded PDG graph will have each read node along with some generated_by
// node. As long as we can sort out a TOP order, then the graph is linearizable
// and it is fine for us to generate code, otherwise we will have to spill
// some slots out to break the backedge
void ParallelMove::ResolveConflicts() {
  CycleFinder{this, register_}.FindCycle();
  CycleFinder{this, spill_}.FindCycle();
}

ParallelMove::Result ParallelMove::WriteOutput() {
  Result result{};
  std::vector<bool> visited;
  std::vector<bool> spill;
  visited.resize(as_->instruction_maxid(), false);
  spill.resize(as_->instruction_maxid(), false);

  // first generate phi node based on read node, currently after the conflicts
  // been resolved, there're no backedge, and everything should be fine
  auto gen_list = [&](const std::vector<ReadNode>& list) -> bool {
    auto mark = 0;
    for (const auto& rn : list) {
      if (rn.HasNoDependent()) continue;
      if (rn.spill) {
        spill[rn.spill->id()] = true;
      }
      auto rr = rn.dependent();
      CHECK(rr);
      if (visited[rr->id()]) continue;

      {
        auto do_not_mark = false;
        for (const auto& r : rn.read_list) {
          if (visited[r->id()] || spill[r->id()]) {
            continue;
          }
          do_not_mark = true;
          break;
        }

        if (!do_not_mark) {
          result.move_list.push_back(*rr);
          visited[rr->id()] = true;
          ++mark;
        }
      }
    }

    return mark != 0;
  };

  // the following method is kind of none optimal, we need to revisit it little
  // bit in the future by really constructing a graph. The only benefits of
  // current method is it does not need any external data structure.
  //
  // the basic idea is to find out dependency by first mark those node which
  // does not have read_list and visit them until we visit all the instruction

  auto has_change = false;

  do {
    has_change = false;
    if (gen_list(register_)) has_change = true;
    if (gen_list(spill_)) has_change = true;
  } while (has_change);

  // Lastly emit micro_spill if needed, notes micro_spill does not generate
  // any output entry inside of register_/spill_ table due to they are spilled
  // to random position we are not aware of during ParallelMove position.
  // Anyway, its fine, as long as we emit the micro-spill instruction at the
  // last
  for (auto s : micro_spill_) {
    if (!visited[s->id()]) {
      visited[s->id()] = true;
      result.move_list.push_back(*s);
    }
    result.micro_spill.push_back(*s);
  }

  BASE_DBG({
    auto dbg = [&](const std::vector<ReadNode>& list) {
      for (const auto& rn : list) {
        if (rn.generate_by && visited[rn.generate_by->id()]) continue;
        if (rn.spill) {
          spill[rn.spill->id()] = true;
        }
        if (rn.HasNoDependent()) continue;
        {
          auto rr = rn.dependent();
          CHECK(rr);
          if (!visited[rr->id()]) {
            LOG(TRACE) << rr->instruction->Print();
          }
        }
      }
    };

    dbg(register_);
    dbg(spill_);
  });

  return result;
}

ParallelMove::Result ParallelMove::Resolve() {
  BuildDPG();
  ResolveConflicts();
  return WriteOutput();
}

ParallelMove::ParallelMove(Quake* quake, HAssembler* as, RABroker* ra,
                           std::size_t max_spill, std::vector<Move>&& ins)
    : quake_(quake),
      as_(as),
      ra_(ra),
      move_list_(std::move(ins)),
      register_(),
      spill_(),
      micro_spill_() {
  register_.resize(static_cast<std::size_t>(RegisterKind::kSizeOfRegisterKind));
  spill_.resize(max_spill);
}

}  // namespace lir
}  // namespace quake
