#ifndef _QUAKE_DEOPT_H_
#define _QUAKE_DEOPT_H_
#include "base.h"
#include "frame.h"
#include "quick-js-inc.h"

#include "lir/register.h"

namespace quake {
class QuakeRT;

namespace lir {
class Snapshot;
}  // namespace lir

enum class DeoptReason : std::uint8_t { kGuard };

enum class DeoptNext : std::uint8_t { kContinueInterp, kErrorOut };

using DeoptId = std::uint32_t;  // deoptimization id, used to look up

// invalid deoptimization id
constexpr DeoptId kInvalidDeoptId = std::numeric_limits<DeoptId>::max();

// ----------------------------------------------------------------------------
// A deoptimization entry resulted from Checkpoint. Notes, we do not store
// Checkpoint and all the IR/LIR are on zone and will be reclaimed after
// JIT
class Deopt {
 public:
  enum ValueOrigin : std::uint8_t { kLocal, kParam, kTemp, kFrame };

  struct Null {};
  struct Undefined {};
  struct Uninit {};

  using MachineImm =
      std::variant<std::int32_t, std::int64_t, double, Null, Undefined, Uninit>;

  enum MachineImmType { kI32, kI64, kF64, kNull, kUndefined };

  enum MachineRefType { kRegister, kSpill, kImm };

  // --------------------------------------------------------------------------
  // Used to indicate a reference inside of machine representation. We can
  // get the value related from the DeoptData object, which is been implemented
  // inside of each arch
  struct MachineRef {
    MachineRefType type;  // type of machine ref
    MachineImm imm;       // imm number
    std::uint32_t data;   // index/register kind, depending on the type

    MachineRef(MachineRefType t, MachineImm&& i, std::uint32_t d)
        : type(t), imm(std::move(i)), data(d) {}

    MachineRef(MachineRefType t, std::uint32_t d) : type(t), imm(), data(d) {}

    MachineRef() = default;

    // Assert it is a register save point and return the register kind
    lir::RegisterKind register_kind() const {
      CHECK(type == kRegister);
      return static_cast<lir::RegisterKind>(data);
    }

    bool IsRegister() const { return type == kRegister; }
    bool IsFP() const {
      CHECK(IsRegister());
      return lir::RegisterKindIsFP(static_cast<lir::RegisterKind>(data));
    }
    bool IsGP() const { return !IsFP(); }
    bool IsSpill() const { return type == kSpill; }
    bool IsImm() const { return type == kImm; }
    std::uint32_t Slot() const {
      CHECK(!IsRegister());
      return data;
    }
  };

  // --------------------------------------------------------------------------
  // Live value existed when exit from a certain deoptimization point
  struct Val {
    ValueOrigin origin;  // origin of the value, ie how to interpret the index
    int index;           // index used to store the value back
    int kind;            // same as JVal::Tag, used to box the value back
    MachineRef ref;      // reference into machine resource/slot, register/stack
  };

  // --------------------------------------------------------------------------
  // All the function frame
  struct Frame {
    std::vector<Val> value_list;

    /** added based on quickjs.c's frame logic **/
    JitFrameType type;
    const void* bytecode_object = nullptr;
    const void* next_bytecode = nullptr;

    /** frame recreation information **/
    std::size_t slot_size = 0;
    std::size_t param_size = 0;
    std::size_t local_size = 0;

    Frame() = default;

    Frame(JitFrameType t, const void* cbc, const void* nbc, std::size_t ss,
          std::size_t ps, std::size_t ls)
        : type(t),
          bytecode_object(cbc),
          next_bytecode(nbc),
          slot_size(ss),
          param_size(ps),
          local_size(ls) {}
  };

 public:
  static DeoptNext DeoptFromJit(const void*, const void*);

 public:
  Deopt(DeoptId id, DeoptReason reason, DeoptNext next)
      : deopt_id_(id), reason_(reason), next_(next) {}

  DeoptId deopt_id() const { return deopt_id_; }
  DeoptReason reason() const { return reason_; }
  DeoptNext next() const { return next_; }
  const std::vector<Frame>& frame() const { return frame_; }
  void ForeachFrameReversely(std::function<void(const Frame&)>&&) const;

  void From(lir::Snapshot&&);

 private:
  DeoptId deopt_id_;
  DeoptReason reason_;
  DeoptNext next_;

  //  list of (inlined) frame
  std::vector<Frame> frame_;
};

// ----------------------------------------------------------------------------
// A vritual data structure been used to help Deopt function to learn the actual
// way of how data been layout on each arch
class DeoptData {
 public:
  using RawValue = std::uint64_t;

  // Get a spilled value with its Stack::Slot. Notes the return valued is a
  // RawValue and its interpretation is been left to the caller to learn it
  virtual RawValue GetSpill(std::int32_t) const = 0;

  // The spilled count currently been represented by the DeoptData
  virtual std::size_t GetSpillCount() const = 0;

  // Get the register with its regiter kind.
  virtual RawValue GetRegister(lir::RegisterKind) const = 0;

  // Get deoptimization id
  virtual DeoptId GetDeoptId() const = 0;

  // Get context
  virtual QuakeRT* GetQuakeRT() const = 0;

  // TODO(dpeng): Add frame information

  // Printing out debugging information
  virtual std::string Print() const;

 public:
  virtual ~DeoptData() {}
};

}  // namespace quake

#endif  // _QUAKE_DEOPT_H_
