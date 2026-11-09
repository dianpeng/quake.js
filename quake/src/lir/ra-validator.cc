#include "lir/ra-validator.h"

#include "lir/arch-constraint.h"
#include "lir/assembler.h"
#include "lir/instruction.h"
#include "lir/operand.h"
#include "lir/parallel-copy.h"

#include "ir/checkpoint.h"
#include "ir/ssa-var.h"

#include "quake.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace quake {
namespace lir {

using arch::ArchOperandConstraint;
using arch::ArchOperandKind;

namespace {

// Abstract-slot view of a materialized operand for the parallel-copy
// simulation and boundarys check. Non-location operands
// (immediates, statics, labels) map to a distinguished "none" kind the
// checks skip.
struct LocatorSlot {
  enum Kind { kRegister, kSpill, kNone };
  Kind kind = kNone;
  const Register* reg = nullptr;
  MemoryStack::Slot spill = 0;

  static LocatorSlot Of(const HOperand* opr) {
    LocatorSlot s;
    if (opr == nullptr) return s;
    if (opr->IsRegister()) {
      s.kind = kRegister;
      s.reg = opr->GetRegister();
    } else if (opr->IsMemStack()) {
      s.kind = kSpill;
      s.spill = opr->GetMemStack().slot();
    }
    return s;
  }

  bool IsLocation() const { return kind != kNone; }

  bool operator==(const LocatorSlot& that) const {
    if (kind != that.kind) return false;
    if (kind == kRegister) return reg == that.reg;
    if (kind == kSpill) return spill == that.spill;
    return true;
  }
  bool operator!=(const LocatorSlot& that) const { return !(*this == that); }
};

const Register* RegisterFromKindId(std::uint32_t kind_id) {
  return Register::GetRegisterFromKind(static_cast<RegisterKind>(kind_id));
}

}  // namespace

RAValidator::RAValidator(Quake* quake, HAssembler* as)
    : quake_(quake), as_(as), cfg_(quake) {}

zone::Zone* RAValidator::zone() const { return quake_->zone(); }

const RegisterSet& RAValidator::LiveRegistersAt(const HInstruction* ins) const {
  static const RegisterSet kEmpty;
  auto itr = live_at_.find(ins);
  return itr == live_at_.end() ? kEmpty : itr->second;
}

void RAValidator::CollectUsesDefs(const HInstruction* ins, RegisterSet* uses,
                                  RegisterSet* defs) {
  const bool is_parallel_copy = ins->opcode() == HOpcode::kParallelCopy;
  for (int i = 0; i < ins->opr_size(); ++i) {
    const HOperand* opr = ins->Opr(i);
    if (opr == nullptr || !opr->IsRegister()) continue;
    // kParallelCopy: even = source (use), odd = destination (def).
    if (is_parallel_copy && (i % 2 == 1)) {
      defs->Set(opr->GetRegister());
    } else {
      uses->Set(opr->GetRegister());
    }
  }

  if (ins->HasOutput() && ins->output() != nullptr &&
      ins->output()->IsRegister()) {
    defs->Set(ins->output()->GetRegister());
  }

  // Deopt side-exit checkpoint entries count as uses at this instruction
  // (conservative side-exit treatment, same policy as LivenessAnalysis).
  if (ins->HasExit()) {
    ins->exit()->ForeachEntry([&](const ir::Checkpoint::Entry& e) {
      if (e.value() == nullptr) return;
      const HOperand* opr = e.value()->lir_operand();
      if (opr != nullptr && opr->IsRegister()) {
        uses->Set(opr->GetRegister());
      }
    });
  }
}

base::Error RAValidator::CheckOperandConstraints(const HInstruction* ins) {
  const auto& c = arch::GetInstructionConstraint(ins);
  if (c.is_no_constraints_marker) return {};

  const int opr_size = ins->opr_size();

  auto check_location = [&](const ArchOperandConstraint& oc,
                            const HOperand* opr, const char* what) ->
      base::Error {
        switch (oc.kind) {
          case ArchOperandKind::kNone:
            break;
          case ArchOperandKind::kFixedRegister: {
            // Register-only check: a non-register location is discharged
            // by the translate-time materialization move (legacy-tolerated
            // shapes); a register must be the fixed PHYSICAL register —
            // size-class views (edx vs rdx) alias one physical register.
            if (opr != nullptr && opr->IsRegister() &&
                opr->GetRegister()->register_kind() !=
                    oc.fixed_register->register_kind()) {
              return base::Error::Mk(
                  "RAValidator: %s of %s must sit in fixed register %s, "
                  "found %s",
                  what, ins->opcode_name().c_str(),
                  oc.fixed_register->name().c_str(),
                  opr->GetRegister()->name().c_str());
            }
            break;
          }
          case ArchOperandKind::kAnyRegister: {
            if (opr == nullptr || !opr->IsRegister()) {
              return base::Error::Mk(
                  "RAValidator: %s of %s must sit in a register",
                  what, ins->opcode_name().c_str());
            }
            break;
          }
          case ArchOperandKind::kSameAsOperand: {
            const int j = static_cast<int>(oc.same_as_operand);
            if (opr != nullptr && opr->IsRegister() && j >= 0 &&
                j < opr_size) {
              const HOperand* other = ins->Opr(j);
              if (other != nullptr && other->IsRegister() &&
                  other->GetRegister() != opr->GetRegister()) {
                return base::Error::Mk(
                    "RAValidator: %s of %s must share the same register "
                    "as operand %d",
                    what, ins->opcode_name().c_str(), j);
              }
            }
            break;
          }
        }
        return {};
      };

  const std::size_t constrained = std::min(c.operands.size(),
                                           static_cast<std::size_t>(opr_size));
  for (std::size_t i = 0; i < constrained; ++i) {
    if (auto err = check_location(c.operands[i], ins->Opr(static_cast<int>(i)),
                                  "operand");
        err) {
      return err;
    }
  }

  if (auto err = check_location(c.output, ins->output(), "output"); err) {
    return err;
  }

  return {};
}

base::Error RAValidator::CheckClobbers(const HInstruction* ins,
                                       const RegisterSet& across,
                                       const RegisterSet& uses) {
  const auto& c = arch::GetInstructionConstraint(ins);
  if (c.clobber_gp.Empty() && c.clobber_fp.Empty() &&
      c.scratch_gp.Empty() && c.scratch_fp.Empty()) {
    return {};
  }

  // Live values the instruction is answerable for: everything live across
  // except its own inputs (staged/consumed by the instruction itself) and
  // its own output register (freshly defined here).
  RegisterSet accountable = across;
  RegisterSet uses_mutable = uses;
  uses_mutable.ForeachSet([&](std::uint32_t kind_id) {
    accountable.Unset(kind_id);
  });
  if (ins->HasOutput() && ins->output() != nullptr &&
      ins->output()->IsRegister()) {
    accountable.Unset(ins->output()->GetRegister());
  }

  auto check_disjoint = [&](const RegisterSet& declared,
                            const char* set_name) -> base::Error {
    if (accountable.IsDisjoint(declared)) return {};
    std::uint32_t hit = 0;
    bool found = false;
    accountable.ForeachSet([&](std::uint32_t kind_id) {
      if (!found && declared.Get(kind_id)) {
        hit = kind_id;
        found = true;
      }
    });
    const Register* r = RegisterFromKindId(hit);
    return base::Error::Mk(
        "RAValidator: %s %s of %s #%d holds a live value across the "
        "instruction",
        r->name().c_str(), set_name, ins->opcode_name().c_str(), ins->id());
  };

  if (auto err = check_disjoint(c.clobber_gp, "clobber_gp"); err) {
    return err;
  }
  if (auto err = check_disjoint(c.clobber_fp, "clobber_fp"); err) {
    return err;
  }
  if (auto err = check_disjoint(c.scratch_gp, "scratch_gp"); err) {
    return err;
  }
  if (auto err = check_disjoint(c.scratch_fp, "scratch_fp"); err) {
    return err;
  }
  return {};
}

base::Error RAValidator::CheckParallelCopy(HInstruction* ins) {
  if (ins->opcode() != HOpcode::kParallelCopy) return {};

  if (ins->opr_size() % 2 != 0) {
    return base::Error::Mk(
        "RAValidator: parallel copy has an odd operand count %d",
        ins->opr_size());
  }

  const int n = ins->opr_size() / 2;

  // (1) No duplicate destinations — the one-program-point invariant.
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (LocatorSlot::Of(ParallelCopyDst(ins, i)) ==
          LocatorSlot::Of(ParallelCopyDst(ins, j))) {
        return base::Error::Mk(
            "RAValidator: parallel copy has duplicate destination in pair "
            "%d and %d",
            i, j);
      }
    }
  }

  // (2) Order simulation: interpret the pair list as a sequential move
  // program over abstract slot values and require the end state to equal
  // the requested (src -> dst) mapping. A listing whose sequential
  // execution corrupts a still-pending source is a violation.
  std::vector<std::pair<LocatorSlot, int>> value_of;
  auto value_id = [&](const LocatorSlot& slot) -> int {
    for (const auto& p : value_of) {
      if (p.first == slot) return p.second;
    }
    int id = static_cast<int>(value_of.size());
    value_of.emplace_back(slot, id);
    return id;
  };

  std::vector<int> original_src(n, -1);
  for (int i = 0; i < n; ++i) {
    original_src[i] = value_id(LocatorSlot::Of(ParallelCopySrc(ins, i)));
    value_id(LocatorSlot::Of(ParallelCopyDst(ins, i)));
  }

  for (int i = 0; i < n; ++i) {
    const int v = value_id(LocatorSlot::Of(ParallelCopySrc(ins, i)));
    // Assign: overwrite the destination's value id.
    const LocatorSlot dst = LocatorSlot::Of(ParallelCopyDst(ins, i));
    for (auto& p : value_of) {
      if (p.first == dst) {
        p.second = v;
        break;
      }
    }
  }

  for (int i = 0; i < n; ++i) {
    const int cur = value_id(LocatorSlot::Of(ParallelCopyDst(ins, i)));
    if (cur != original_src[i]) {
      return base::Error::Mk(
          "RAValidator: parallel copy pairs are not in a correct order: "
          "pair %d clobbers a still-pending source",
          i);
    }
  }

  return {};
}

base::Error RAValidator::CheckExitBoundary(HInstruction* ins) {
  if (ins->opcode() == HOpcode::kParallelCopy && ins->HasExit()) {
    return base::Error::Mk(
        "RAValidator: parallel copy carries an exit (message: copy sequences "
        "never span a snapshot/deopt point)");
  }

  if (!ins->HasExit()) return {};
  auto* prev = ins->prev();
  if (prev == nullptr || prev->opcode() != HOpcode::kParallelCopy) return {};

  // The instruction is immediately preceded by a parallel copy: every
  // location the snapshot references must be a post-copy location — the
  // pair's destination or a slot untouched by the copy — never a pure
  // pre-copy source slot.
  const int n = ParallelCopyPairCount(prev);
  std::vector<LocatorSlot> srcs, dsts;
  for (int i = 0; i < n; ++i) {
    srcs.push_back(LocatorSlot::Of(ParallelCopySrc(prev, i)));
    dsts.push_back(LocatorSlot::Of(ParallelCopyDst(prev, i)));
  }
  auto is_pure_src = [&](const LocatorSlot& slot) {
    if (!slot.IsLocation()) return false;
    bool is_src = false, is_dst = false;
    for (const auto& s : srcs) {
      if (s == slot) is_src = true;
    }
    for (const auto& d : dsts) {
      if (d == slot) is_dst = true;
    }
    return is_src && !is_dst;
  };

  base::Error stale = base::Error::Mk(
      "RAValidator: snapshot location refers to a pre-copy source slot "
      "(message: MachineRefs must describe the post-copy boundary)");

  bool violated = false;
  ins->exit()->ForeachEntry([&](const ir::Checkpoint::Entry& e) {
    if (violated || e.value() == nullptr) return;
    if (is_pure_src(LocatorSlot::Of(e.value()->lir_operand()))) {
      violated = true;
    }
  });
  if (violated) return stale;

  for (int i = 0; i < ins->opr_size(); ++i) {
    if (is_pure_src(LocatorSlot::Of(ins->Opr(i)))) return stale;
  }

  return {};
}

base::Error RAValidator::Run() {
  live_at_.clear();

  cfg_.CreateCFG(as_);

  // Collect the CFG blocks and their local use/def sets over physical
  // registers.
  std::vector<BasicBlock*> blocks;
  cfg_.ForeachBasicBlock(
      [&](BasicBlock* bb) -> bool {
        blocks.push_back(bb);
        return true;
      });

  const std::uint32_t max_bid = cfg_.block_id_max();
  std::vector<RegisterSet> use_set(max_bid + 1), def_set(max_bid + 1);
  std::vector<RegisterSet> live_in(max_bid + 1), live_out(max_bid + 1);

  auto bid_of = [](const BasicBlock* bb) { return bb->block_id(); };

  for (auto* bb : blocks) {
    // gen/kill discipline: the local USE set is upward-exposed only —
    // a use preceded by a local def of the same register is not live-in
    // through the block's top. Walk in order, recording uses before the
    // register's first local def.
    RegisterSet killed;
    for (auto* ins = bb->start(); ins != bb->end(); ins = ins->next()) {
      RegisterSet uses, defs;
      CollectUsesDefs(ins, &uses, &defs);
      RegisterSet exposed;
      uses.ForeachSet([&](std::uint32_t kind_id) {
        if (!killed.Get(kind_id)) exposed.Set(kind_id);
      });
      use_set[bid_of(bb)].Union(exposed);
      def_set[bid_of(bb)].Union(defs);
      killed.Union(defs);
    }
  }

  // Backwards fixpoint: live_in[B] = use[B] U (live_out[B] - def[B]),
  // live_out[B] = U live_in[S] over successors. Small CFGs — a plain
  // iterate-to-fixpoint loop converges quickly.
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto* bb : blocks) {
      const auto bid = bid_of(bb);
      RegisterSet new_out;
      if (bb->left_bb() != nullptr) {
        new_out.Union(live_in[bid_of(bb->left_bb())]);
      }
      if (bb->right_bb() != nullptr) {
        new_out.Union(live_in[bid_of(bb->right_bb())]);
      }
      if (new_out.Count() != live_out[bid].Count()) {
        live_out[bid] = new_out;
        changed = true;
      }
      RegisterSet new_in = live_out[bid];
      new_in.Exclude(def_set[bid]);
      new_in.Union(use_set[bid]);
      if (new_in.Count() != live_in[bid].Count()) {
        live_in[bid] = new_in;
        changed = true;
      }
    }
  }

  // Per-instruction backwards walk: reconstruct the live-across set and
  // run every per-instruction check.
  for (auto* bb : blocks) {
    RegisterSet live = live_out[bid_of(bb)];
    auto* ins = bb->last_instruction();
    if (ins == nullptr) continue;
    while (true) {
      RegisterSet uses, defs;
      CollectUsesDefs(ins, &uses, &defs);

      RegisterSet across = live;
      across.Union(uses);
      live_at_.emplace(ins, across);

      if (auto err = CheckOperandConstraints(ins); err) return err;
      if (auto err = CheckClobbers(ins, across, uses); err) return err;
      if (auto err = CheckParallelCopy(ins); err) return err;
      if (auto err = CheckExitBoundary(ins); err) return err;

      live.Exclude(defs);
      live.Union(uses);

      if (ins == bb->start()) break;
      ins = ins->prev();
      // Latch-shaped blocks (the loop backedge makes the block's start
      // physically LATER than its end; RA insertions can turn the
      // post-backedge instruction into a leader) cannot be walked to their
      // start — stop at the stream boundary instead of dereferencing null.
      if (ins == nullptr) break;
    }
  }

  return {};
}

void RAValidator::RunChecked() {
  if (auto err = Run(); err) {
    LOG(FATAL) << "RAValidator: post-RA verification failed: "
               << err.GetDisplay();
  }
}

}  // namespace lir
}  // namespace quake
