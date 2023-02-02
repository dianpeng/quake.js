#ifndef _QUAKE_INTERP_INFO_H_
#define _QUAKE_INTERP_INFO_H_
#include "base.h"
#include "quake-C.h"
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

// This object has been fully abstracted out to enable simpler mock testing
// in the future. Ie, user does not need to craft any code that is relying
// on the QuickJS to test the IR module
class InterpInfo : public BytecodeInfoProvider {
 public:
  virtual ~InterpInfo() {}
  virtual JSContext* qjs_context() const = 0;
  virtual JSRuntime* qjs_runtime() const = 0;
  virtual QJS* qjs() const = 0;

 public:
  // Notes, we purposely do not inline these decoder function since we will
  // use quickjs's cutil.h file which should be included as hidden symbols
  virtual std::uint32_t GetU32() = 0;
  virtual std::uint8_t GetU8() = 0;
  virtual std::uint16_t GetU16() = 0;
  virtual std::int32_t GetI32() = 0;

  std::uint32_t PeekU32();
  std::int32_t PeekI32();
  std::uint16_t PeekU16();
  std::uint8_t PeekU8();

 public:
  virtual bool IsModeStrict() const = 0;

 public:
  virtual JVal GetParam(std::uint32_t) const = 0;
  virtual JVal GetLocal(std::uint32_t) const = 0;
  virtual JVal GetTemp(std::uint32_t) const = 0;
  JVal GetTOS() const {
    CHECK(temp_size() != 0);
    return GetTemp(temp_size() - 1);
  }
  JVal GetTempVal(int idx) const {
    idx = idx < 0 ? -idx : idx;
    CHECK(idx <= static_cast<int>(temp_size()));
    return GetTemp(temp_size() - idx);
  }

  virtual std::uint32_t local_size() const = 0;
  virtual std::uint32_t param_size() const = 0;
  virtual std::uint32_t temp_size() const = 0;

  std::uint32_t slot_size() const {
    return local_size() + param_size() + temp_size();
  }

  virtual JVal GetVarRef(std::uint32_t) const = 0;
  virtual std::uint32_t var_ref_size() const = 0;

 public:
  virtual Bytecode op() const = 0;
  const std::string& op_name() const { return GetQJSBytecodeInfo(op()).name; }

  // opcode (PC) related

  // Start PC represents the position that just contain the current bytecode,
  // ie full bytecode, including the opcode part and its optional argument
  virtual const void* start_pc() const = 0;

  // The interpreter info's PC address
  virtual const void* interp_pc() const = 0;

  // Next pc represents the bytecode that just after the current bytecode
  virtual const void* next_pc() const = 0;

  // Anchor at current position
  virtual const void* PCNext(std::ptrdiff_t offset) = 0;

 public:
  // Get a value from constant pool
  virtual JVal GetConst(std::uint32_t) const = 0;

  // Get a value from atom pool
  virtual JVal GetAtom(std::uint32_t, bool) const = 0;

 public:
  // Current JavaScript frame's internal object
  virtual JVal This() const = 0;
  virtual JVal Function() const = 0;
  virtual JVal Target() const = 0;
  virtual JVal Global() const = 0;
};

class InterpInfoQJS : public InterpInfo {
 public:
  inline InterpInfoQJS(Quake*, QInterpInfo*);
  ~InterpInfoQJS() override;

 public:
  JSContext* qjs_context() const override { return info_->qjs_context; }
  JSRuntime* qjs_runtime() const override { return info_->qjs_runtime; }

 public:
  // Notes, we purposely do not inline these decoder function since we will
  // use quickjs's cutil.h file which should be included as hidden symbols
  std::uint32_t GetU32() override;
  std::uint8_t GetU8() override;
  std::uint16_t GetU16() override;
  std::int32_t GetI32() override;

 public:
  bool IsModeStrict() const override {
    return qjs()->api.bytecode_get_mode(qjs_context(), info_->bc.bc_object) ==
           QJS_MODE_STRICT;
  }

 public:
  inline JVal GetParam(std::uint32_t) const override;
  inline JVal GetLocal(std::uint32_t) const override;
  inline JVal GetTemp(std::uint32_t) const override;

  inline std::uint32_t local_size() const override;
  inline std::uint32_t param_size() const override;
  inline std::uint32_t temp_size() const override;

  inline JVal GetVarRef(std::uint32_t) const override;
  inline std::uint32_t var_ref_size() const override;

 public:
  Bytecode op() const override { return pc_op_; }

  const std::string& op_name() const { return GetQJSBytecodeInfo(pc_op_).name; }

  // opcode (PC) related

  // Start PC represents the position that just contain the current bytecode,
  // ie full bytecode, including the opcode part and its optional argument
  const void* start_pc() const override {
    return static_cast<const std::uint8_t*>(interp_pc_) - 1;
  }

  // The interpreter info's PC address
  const void* interp_pc() const override { return interp_pc_; }

  // Next pc represents the bytecode that just after the current bytecode
  const void* next_pc() const override {
    return static_cast<const std::uint8_t*>(start_pc()) +
           GetQJSBytecodeInfo(pc_op_).size;
  }

  // Anchor at current position
  const void* PCNext(std::ptrdiff_t offset) override {
    return static_cast<const std::uint8_t*>(interp_pc_) + offset;
  }

  BytecodeInfo bytecode_info() const override {
    return BytecodeInfo{pc_op_, start_pc(), next_pc()};
  }

 public:
  // Get a value from constant pool
  inline JVal GetConst(std::uint32_t) const override;

  // Get a value from atom pool
  inline JVal GetAtom(std::uint32_t, bool) const override;

 public:
  // Current JavaScript frame's internal object
  inline JVal This() const override;
  inline JVal Function() const override;
  inline JVal Target() const override;
  inline JVal Global() const override;

 public:
  QJS* qjs() const override { return quake_->qjs(); }

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
