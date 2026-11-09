#ifndef _QUAKE_LIR_ARCH_CONSTRAINT_H_
#define _QUAKE_LIR_ARCH_CONSTRAINT_H_
#include "base.h"
#include "base/container.h"

// ----------------------------------------------------------------------------
// Declarative arch-constraint query surface
//
// One interface, generic declarations here in include/lir; the target arch
// supplies the data behind it (x64: src/cg/x64/arch-constraint.cc). The
// generic RA code queries *this* header unconditionally and must never
// include anything from cg/x64 directly — the ifdef below is the ONE
// sanctioned arch-selection point (same pattern as include/cg/abi.h).
//
// Scope note: this is interface + relocate, NOT a tree-wide
// ifdef purge — pre-existing arch ifdefs in the register model
// (include/lir/register.h, src/lir/register.cc, src/lir/instruction.cc) are
// explicitly grandfathered. Relocation only: behavior is identical to
// before.
// ----------------------------------------------------------------------------

#ifdef QUAKE_TARGET_X64
// The x64 provider (src/cg/x64/arch-constraint.cc) supplies the definitions
// of the lir::arch queries below.
#else
#error "unsupported arch: the lir::arch constraint interface has no provider"
#endif  // QUAKE_TARGET_X64

#include "lir/register.h"
#include "lir/opcode.h"

namespace quake {
namespace lir {

class HInstruction;

namespace arch {

// ----------------------------------------------------------------------------
// Pure-data constraint surface
//
// Every query below is a const lookup over per-opcode static tables
// (x64: src/cg/x64/arch-constraint.cc) with no allocator-state argument. The
// generic RA interprets this data uniformly; a future backend is a new table,
// not new solver code
//
// Semantics of the per-operand/output entries:
//   kNone          — no constraint: any materialized location is acceptable
//                    (register, spill slot, immediate).
//   kFixedRegister — the value MUST sit in fixed_register.
//   kAnyRegister   — the value MUST sit in some physical register (never a
//                    spill slot).
//   kSameAsOperand — the value MUST share its register with the operand at
//                    index same_as_operand (the declarative form of the x64
//                    two-operand rule; a register-equality relation, NOT an
//                    action on allocator state). Register-register pairs are
//                    checked for equality; when either side is a non-register
//                    location the relation is discharged by the translate-time
//                    materialization move and the validator treats it as
//                    satisfied.
//
// The clobber_* sets are the registers the instruction destroys beyond its
// own output; the scratch_* sets are registers the instruction temporarily
// borrows (both must be free of live values across the instruction). is_call
// marks instructions that perform an ABI call and therefore carry the full
// caller-saved clobber sets.
//
// Every HOpcode has an explicit table entry OR the explicit no-constraints
// marker (is_no_constraints_marker) — there is no silent default
// The completeness presence test
// (unittest/lir/arch-constraint-test.cc) walks the whole enum and pins this.
//
// Notes, the operand list uses base::static_vector (not zone::Vector): the
// table entries must be statically constructible, and zone::Vector requires
// a Zone at construction. The capacity bounds the per-instruction fixed
// operand requirements; the x64 table builder CHECKs against overflow.
// ----------------------------------------------------------------------------

enum class ArchOperandKind : std::uint8_t {
  kNone = 0,
  kFixedRegister,
  kAnyRegister,
  kSameAsOperand,
};

struct ArchOperandConstraint {
  ArchOperandKind kind = ArchOperandKind::kNone;
  const Register* fixed_register = nullptr;
  std::int32_t same_as_operand = -1;

  bool IsNone() const { return kind == ArchOperandKind::kNone; }

  static ArchOperandConstraint None() { return ArchOperandConstraint{}; }

  static ArchOperandConstraint Fixed(const Register* r) {
    ArchOperandConstraint c;
    c.kind = ArchOperandKind::kFixedRegister;
    c.fixed_register = r;
    return c;
  }

  static ArchOperandConstraint AnyRegister() {
    ArchOperandConstraint c;
    c.kind = ArchOperandKind::kAnyRegister;
    return c;
  }

  static ArchOperandConstraint SameAs(std::int32_t operand_index) {
    ArchOperandConstraint c;
    c.kind = ArchOperandKind::kSameAsOperand;
    c.same_as_operand = operand_index;
    return c;
  }
};

// Maximum number of per-operand constraint entries the static arch tables
// carry for a single instruction.
static constexpr std::size_t kMaxArchOperandConstraint = 8;

struct InstructionConstraint {
  // Explicit marker: this opcode exhibits no register behavior the RA needs
  // to know about
  bool is_no_constraints_marker = false;

  // Per-operand constraints, index-aligned with HInstruction::Opr(i).
  base::static_vector<ArchOperandConstraint, kMaxArchOperandConstraint>
      operands;

  // Output constraint (HInstruction::output()).
  ArchOperandConstraint output;

  RegisterSet clobber_gp;
  RegisterSet clobber_fp;
  RegisterSet scratch_gp;
  RegisterSet scratch_fp;

  // True when the instruction performs an ABI call (caller-saved clobbers).
  bool is_call = false;
};

// Per-opcode constraint data, audited line-by-line against
// src/cg/x64/lir-translate.cc. Also the fetch path used by the
// completeness presence test (no instruction construction needed).
const InstructionConstraint& GetInstructionConstraint(HOpcode);

// Instruction-keyed fetch (delegates to the per-opcode table).
const InstructionConstraint& GetInstructionConstraint(const HInstruction*);

// Call ABI data: the arg register for the arg_index'th GP/FP call
// argument, or nullptr when the ABI has no register for that index (the
// value must be stack-passed — currently unreachable for our calls).
const Register* GetCallArgRegister(std::size_t arg_index, bool fp);

// The caller-saved clobber sets an is_call instruction carries.
RegisterSet GetCallClobberGP();
RegisterSet GetCallClobberFP();

}  // namespace arch
}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_ARCH_CONSTRAINT_H_
