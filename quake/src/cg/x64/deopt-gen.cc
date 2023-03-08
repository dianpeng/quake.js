#include "cg/x64/deopt-gen.h"
#include "cg/x64/asm-util.h"
#include "cg/x64/abi.h"

#include "quake-rt.h"
#include "qjs.h"

#define __ (cg_->as())->

namespace quake {
namespace cg {
namespace x64 {

namespace a = asmjit;

void DeoptGen::MoveTag(a::x86::Mem dest, int tag) {
  __ mov(u::ScrD(), tag);
  __ movsxd(u::ScrQ(), u::ScrD());
  __ mov(dest, u::ScrQ());
}

void DeoptGen::BoxFromRegister(a::x86::Gp value, int tag, a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  __ mov(value_addr, value);

  MoveTag(tag_addr, tag);
}

void DeoptGen::BoxFromRegister(a::x86::Xmm value, int tag, a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  value_addr.setSize(8);
  __ movq(value_addr, value);

  MoveTag(tag_addr, tag);
}

a::x86::Mem DeoptGen::SpillAddress(int spill_slot) const {
  static constexpr std::size_t kSize = 8;

  auto base = quake()->spill_offset();
  auto offset = base + spill_slot * kSize + kSize;
  auto memptr = a::x86::ptr(a::x86::rbp, -offset);

  return memptr;
}

void DeoptGen::BoxFromSpill(int spill_slot, int tag, a::x86::Mem slot) {
  auto memptr = SpillAddress(spill_slot);
  auto size_class = JVal::GetSizeClass(tag);
  auto byte_size = SizeClassToByteSize(size_class);
  CHECK(byte_size <= 8);
  memptr.setSize(byte_size);

  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  switch (byte_size) {
    case 8:
      __ mov(u::ScrQ(), memptr);
      __ mov(value_addr, u::ScrQ());
      break;
    case 4:
      __ mov(u::ScrD(), memptr);
      __ mov(value_addr, u::ScrD());
      break;
    case 2:
      __ mov(u::ScrW(), memptr);
      __ mov(value_addr, u::ScrW());
      break;
    case 1:
      __ mov(u::ScrB(), memptr);
      __ mov(value_addr, u::ScrB());
      break;
    default:
      UNREACHABLE();
      break;
  }

  MoveTag(tag_addr, tag);
}

void DeoptGen::BoxFromInt32(std::int32_t value, int tag, a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  __ mov(u::ScrD(), value);
  __ mov(value_addr, u::ScrD());
  MoveTag(tag_addr, tag);
}

void DeoptGen::BoxFromInt64(std::int64_t value, int tag, a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  __ movabs(u::ScrQ(), value);
  __ mov(value_addr, u::ScrQ());
  MoveTag(tag_addr, tag);
}

void DeoptGen::BoxFromFloat64(double value, int tag, a::x86::Mem slot) {
  auto i64 = base::TypePuneCast<std::int64_t>(value);
  BoxFromInt64(i64, tag, slot);
}

void DeoptGen::BoxUndefined(a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  __ xor_(u::ScrQ(), u::ScrQ());
  __ mov(value_addr, u::ScrQ());
  MoveTag(tag_addr, JVal::kUndefined);
}

void DeoptGen::BoxNull(a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  __ xor_(u::ScrQ(), u::ScrQ());
  __ mov(value_addr, u::ScrQ());
  MoveTag(tag_addr, JVal::kNull);
}

void DeoptGen::BoxUninitialized(a::x86::Mem slot) {
  auto value_addr = slot.cloneAdjusted(Layout::Value::kValue);
  auto tag_addr = slot.cloneAdjusted(Layout::Value::kTag);

  __ xor_(u::ScrQ(), u::ScrQ());
  __ mov(value_addr, u::ScrQ());
  MoveTag(tag_addr, JVal::kUninitialized);
}

// ----------------------------------------------------------------------------
void DeoptGen::MaterializeTo(a::x86::Mem data, Deopt::ValueOrigin origin,
                             int index) {
  switch (origin) {
    case Deopt::kLocal:
      MaterializeToLocal(index, data);
      break;

    case Deopt::kParam:
      MaterializeToParam(index, data);
      break;

    case Deopt::kTemp:
      MaterializeToTemp(index, data);
      break;

    default:
      UNREACHABLE();
      break;
  }
}

void DeoptGen::MaterializeToSlot(int index, a::x86::Mem data,
                                 std::size_t offset) {
  __ movdqu(u::ScrX(), data);  // value store inside of xmm scratch

  // figure out the local slot, notes context is still available
  __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), offset));
  auto addr = a::x86::xmmword_ptr(u::ScrQ(), Layout::kCellSize * index);
  __ movups(addr, u::ScrX());
}

void DeoptGen::MaterializeToLocal(int index, a::x86::Mem data) {
  MaterializeToSlot(index, data, Layout::Context::kLocal);
}

void DeoptGen::MaterializeToParam(int index, a::x86::Mem data) {
  MaterializeToSlot(index, data, Layout::Context::kParam);
}

void DeoptGen::MaterializeToTemp(int index, a::x86::Mem data) {
  MaterializeToSlot(index, data, Layout::Context::kTempCur);
}

// ----------------------------------------------------------------------------
void DeoptGen::BoxValueInt32(const Deopt::Val& value) {
  auto temp = __ StackTempXmm0();

  switch (value.ref.type) {
    case Deopt::kRegister:
      BoxFromRegister(
          MapRegisterGPFromRegisterKindASMJIT(
              static_cast<RegisterKind>(value.ref.data), SizeClass::kDWord),
          value.kind, temp);
      break;

    case Deopt::kSpill:
      BoxFromSpill(value.ref.data, value.kind, temp);
      break;

    case Deopt::kImm: {
      switch (value.ref.imm.index()) {
        case Deopt::kI32:
          BoxFromInt32(base::Get<std::int32_t>(value.ref.imm), value.kind,
                       temp);
          break;
        default:
          UNREACHABLE();
          break;
      }
      break;

      default:
        UNREACHABLE();
        break;
    }
  }

  // okay, value is inside of the temp, then just move the temp to the target
  MaterializeTo(temp, value.origin, value.index);
}

void DeoptGen::BoxValueInt64(const Deopt::Val& value) {
  auto temp = __ StackTempXmm0();

  switch (value.ref.type) {
    case Deopt::kRegister:
      BoxFromRegister(
          MapRegisterGPFromRegisterKindASMJIT(
              static_cast<RegisterKind>(value.ref.data), SizeClass::kQWord),
          value.kind, temp);
      break;

    case Deopt::kSpill:
      BoxFromSpill(value.ref.data, value.kind, temp);
      break;

    case Deopt::kImm: {
      switch (value.ref.imm.index()) {
        case Deopt::kI64:
          BoxFromInt64(base::Get<std::int64_t>(value.ref.imm), value.kind,
                       temp);
          break;
        default:
          UNREACHABLE();
          break;
      }
      break;

      default:
        UNREACHABLE();
        break;
    }
  }

  MaterializeTo(temp, value.origin, value.index);
}

void DeoptGen::BoxValueFloat64(const Deopt::Val& value) {
  auto temp = __ StackTempXmm0();

  switch (value.ref.type) {
    case Deopt::kRegister:
      BoxFromRegister(MapRegisterFPFromRegisterKindASMJIT(
                          static_cast<RegisterKind>(value.ref.data)),
                      value.kind, temp);
      break;

    case Deopt::kSpill:
      BoxFromSpill(value.ref.data, value.kind, temp);
      break;

    case Deopt::kImm: {
      switch (value.ref.imm.index()) {
        case Deopt::kF64:
          BoxFromFloat64(base::Get<double>(value.ref.imm), value.kind, temp);
          break;
        default:
          UNREACHABLE();
          break;
      }
      break;

      default:
        UNREACHABLE();
        break;
    }
  }

  MaterializeTo(temp, value.origin, value.index);
}

void DeoptGen::BoxValueBoxRef(const Deopt::Val& value) {
  a::x86::Mem temp;

  switch (value.ref.type) {
    case Deopt::kRegister: {
      auto r = MapRegisterGPFromRegisterKindASMJIT(
          static_cast<RegisterKind>(value.ref.data), SizeClass::kQWord);
      temp = a::x86::xmmword_ptr(r);
    } break;

    case Deopt::kSpill: {
      auto spill_address = SpillAddress(value.ref.data);
      temp = spill_address;
    } break;

    default:
      UNREACHABLE();
      break;
  }

  MaterializeTo(temp, value.origin, value.index);
}

void DeoptGen::BoxValuePtr(const Deopt::Val& value) { BoxValueInt64(value); }

void DeoptGen::BoxValue(const Deopt::Val& value) {
  switch (value.kind) {
    case JVal::kInt:
    case JVal::kBool:
    case JVal::kCatchOffset:
    case JVal::kException:
      BoxValueInt32(value);
      break;

    case JVal::kFloat64:
      BoxValueFloat64(value);
      break;

    case JVal::kNull: {
      auto temp = __ StackTempXmm0();
      BoxNull(temp);
      MaterializeTo(temp, value.origin, value.index);
    } break;

    case JVal::kUndefined: {
      auto temp = __ StackTempXmm0();
      BoxUndefined(temp);
      MaterializeTo(temp, value.origin, value.index);
    } break;

    case JVal::kUninitialized: {
      auto temp = __ StackTempXmm0();
      BoxUninitialized(temp);
      MaterializeTo(temp, value.origin, value.index);
    } break;

    case Deopt::kBoxRef:
      BoxValueBoxRef(value);
      break;

    default:
      if (value.kind < 0) {
        BoxValuePtr(value);
      } else {
        UNREACHABLE() << "invalid type: " << value.kind;
      }
      break;
  }
}

void DeoptGen::GenerateVMStack(const Deopt& deopt, VMStackGenOption opt) {
  for (const auto& v : deopt.top_frame().value_list) {
    COMMENT(cg_, ">>> DeoptGen(%d: %s)", deopt.deopt_id(), v.Print().c_str());
    switch (opt) {
      case VMStackGenOption::kFull:
        BoxValue(v);
        break;

      case VMStackGenOption::kReturn:
        if (v.origin != Deopt::kParam) {
          BoxValue(v);
        }
        break;

      default:
        UNREACHABLE();
        break;
    }
  }
}

void DeoptGen::GenerateVMState(const Deopt& deopt) {
  auto is_happen_before = deopt.happen_before();
  const auto& top_frame = deopt.top_frame();
  auto did = deopt.deopt_id();

  // setup the correct TempCur
  {
    COMMENT(cg_, ">>> DeoptGen(%d: temp cur)", did);
    const auto temp_size = top_frame.temp_size();
    __ mov(u::ScrQ(), a::x86::qword_ptr(u::Ctx(), Layout::Context::kTemp));
    __ lea(u::ScrQ(),
           a::x86::qword_ptr(u::ScrQ(), temp_size * Layout::kCellSize));
    __ mov(a::x86::qword_ptr(u::Ctx(), Layout::Context::kTempCur), u::ScrQ());
  }

  // setup next_bc
  {
    COMMENT(cg_, ">>> DeoptGen(%d: next pc)", did);
    if (is_happen_before) {
      __ movabs(u::ScrQ(),
                base::TypePuneCast<std::int64_t>(top_frame.current_bytecode));
    } else {
      __ movabs(u::ScrQ(),
                base::TypePuneCast<std::int64_t>(top_frame.next_bytecode));
    }
    __ mov(a::x86::qword_ptr(u::Ctx(), Layout::Context::kNextPC), u::ScrQ());
  }

  // setup exit_id
  {
    COMMENT(cg_, ">>> DeoptGen(%d: exit id)", did);
    __ mov(a::x86::dword_ptr(u::Ctx(), Layout::Context::kExitId), did);
  }

  // mark inner frame to be null
  {
    COMMENT(cg_, ">>> DeoptGen(%d: inner frame)", did);
    __ xor_(u::ScrQ(), u::ScrQ());
    __ mov(a::x86::qword_ptr(u::Ctx(), Layout::Context::kInnerMostFrame),
           u::ScrQ());
  }
}

}  // namespace x64
}  // namespace cg
}  // namespace quake
