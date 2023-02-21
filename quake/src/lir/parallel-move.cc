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

void ParallelMove::AddMicroSpill(Move* move) {
  auto& read_node = FindReadNode(ra_->ResolvePhi(move->to));
  CHECK(read_node.generate_by == move);
  read_node.spill = move;
  read_node.generate_by = nullptr;
  micro_spill_.push_back(move);  // hidden from rest of the worlds
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

// The builded PDG graph will have each read node along with some generated_by
// node. As long as we can sort out a TOP order, then the graph is linearizable
// and it is fine for us to generate code, otherwise we will have to spill
// some slots out to break the backedge
void ParallelMove::ResolveConflicts() {
  static const std::uint32_t kInvalid =
      std::numeric_limits<std::uint32_t>::max();

  std::uint32_t dfs_ts = 0;
  std::vector<std::uint32_t> visit_num;
  visit_num.resize(as_->instruction_maxid(), kInvalid);

  auto find_backedge_and_spill = [&](const std::vector<ReadNode>& list) {
    for (auto rn : list) {
      // foreach ReadNode, we just need to number the read_list and generate_by
      // as following order, ie the number of generate_by is lower than every
      // read_list. If any node in the read_list has been visited, we need to
      // check the variant is satisfied or not. Otherwise a spill is found.

      for (const auto& r : rn.read_list) {
        if (visit_num[r->id()] == kInvalid) {
          visit_num[r->id()] = dfs_ts++;
        } else if (rn.generate_by) {
          auto vdfs = visit_num[r->id()];
          // check variant
          if (auto gen_dfs_num = visit_num[rn.generate_by->id()];
              gen_dfs_num != kInvalid) {
            if (gen_dfs_num <= vdfs) {
              // invariant breaks, ie finding a cycle. We just spill the node
              // that backedge points to, ie the current visiting read node
              AddMicroSpill(r);
            }
          }
        }
      }

      if (rn.generate_by) {
        // mark the generate_by node
        visit_num[rn.generate_by->id()] = dfs_ts++;
      }
    }
  };

  find_backedge_and_spill(register_);
  find_backedge_and_spill(spill_);
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
