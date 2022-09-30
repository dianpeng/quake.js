#ifndef _QUAKE_INTERP_INFO_H_
#define _QUAKE_INTERP_INFO_H_
#include "base.h"
#include "quake-api.h"
#include "quake.h"
#include "qjs.h"

#include "ir/bytecode.h"
#include "ir/ssa.h"

namespace quake {
namespace ir {

// Ease the pain for clone the Node
class BytecodeInfoProvider {
 public:
  virtual BytecodeInfo bytecode_info() const = 0;
  virtual ~BytecodeInfoProvider() {}
};

class InterpInfo : public BytecodeInfoProvider {
 public:
  inline InterpInfo(Quake*, QInterpInfo*);

 public:
  JSContext* qjs_context() const { return info_->qjs_context; }
  JSRuntime* qjs_runtime() const { return info_->qjs_runtime; }

 public:
  // Notes, we purposely do not inline these decoder function since we will
  // use quickjs's cutil.h file which should be included as hidden symbols
  std::uint32_t GetU32();
  std::uint8_t GetU8();
  std::uint16_t GetU16();

 public:
  bool IsModeStrict() const {
    return qjs()->api.bytecode_get_mode(qjs_context(), info_->bc.bc_object) ==
           QJS_MODE_STRICT;
  }

 public:
  inline JVal GetParam(std::uint32_t) const;
  inline JVal GetLocal(std::uint32_t) const;
  inline JVal GetTemp(std::uint32_t) const;

  inline std::uint32_t local_size() const;
  inline std::uint32_t param_size() const;
  inline std::uint32_t temp_size() const;

  inline JVal GetSlot(std::uint32_t) const;
  inline std::uint32_t slot_size() const;

 public:
  Bytecode op() const { return pc_op_; }
  const std::string& op_name() const { return GetQJSBytecodeInfo(pc_op_).name; }

  // opcode (PC) related

  // Start PC represents the position that just contain the current bytecode,
  // ie full bytecode, including the opcode part and its optional argument
  const void* start_pc() const {
    return static_cast<const std::uint8_t*>(interp_pc_) - 1;
  }

  // The interpreter info's PC address
  const void* interp_pc() const { return interp_pc_; }

  // Next pc represents the bytecode that just after the current bytecode
  const void* next_pc() const {
    return static_cast<const std::uint8_t*>(interp_pc_) +
           GetQJSBytecodeInfo(pc_op_).size;
  }

  // Anchor at current position
  const void* PCNext(std::ptrdiff_t offset) {
    return static_cast<const std::uint8_t*>(interp_pc_) + offset;
  }

  BytecodeInfo bytecode_info() const override {
    return BytecodeInfo{pc_op_, start_pc()};
  }

 public:
  // Get a value from constant pool
  inline JVal GetConst(std::uint32_t) const;

  // Get a value from atom pool
  inline JVal GetAtom(std::uint32_t, bool) const;

 public:
  // Current JavaScript frame's internal object
  inline JVal This() const;
  inline JVal Function() const;
  inline JVal Target() const;
  inline JVal Global() const;

 public:
  QJS* qjs() const { return quake_->qjs(); }

 private:
  Quake* quake_;
  QInterpInfo* info_;

  // Bytecode(PC) related -----------------------------------------------------
  const std::uint8_t* const interp_pc_;
  const std::uint8_t* pc_;
  Bytecode pc_op_;
};

}  // namespace ir
}  // namespace quake

#include "interp-info-inl.h"

#endif  // _QUAKE_INTERP_INFO_H_
