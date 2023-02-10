#ifndef _QUAKE_IR_BYTECODE_H_
#define _QUAKE_IR_BYTECODE_H_
#include "base.h"

// Directly mapping from quickjs-opcode.h file

namespace quake {
namespace ir {

enum class Bytecode : std::uint8_t {

#define DEF(id, ...) OP_##id,
#include <quickjs-opcode.h>
#undef DEF  // DEF

  OP_size_of_bytecodes
};

enum class BytecodeFormat : std::uint8_t {
#define FMT(A) kFmt_##A,
#include <quickjs-opcode.h>
#undef FMT  // FMT
  kSizeOfFormat
};

struct QJSBytecodeInfo {
  std::string name;       // name of opcode
  Bytecode opcode;        // opcode of bytecode
  BytecodeFormat format;  // format of bytecode
  std::uint32_t size;     // size of bytecode (in bytes)
  std::uint32_t n_pop;    // # of stack pop
  std::uint32_t n_push;   // # of stack push
};

const QJSBytecodeInfo& GetQJSBytecodeInfo(Bytecode);

inline bool BytecodeIsLast(Bytecode bc) {
  switch (bc) {
    case Bytecode::OP_return:
    case Bytecode::OP_return_undef:
    case Bytecode::OP_tail_call:
    case Bytecode::OP_tail_call_method:
      return true;
    default:
      return false;
  }
}

// Get next bytecode from the current bytecode specification
const void* BytecodeNext(const void*);

// Simiar as BytecodeNext, but instead returns nullptr if current position
// points to a opcode which is following
//
// 1) OP_return
// 2) OP_return_undef
// 3) OP_tail_call
// 4) OP_tail_call_method
inline const void* BytecodeNextInFunction(const void* op) {
  if (auto opc = *static_cast<const Bytecode*>(op); BytecodeIsLast(opc)) {
    return nullptr;
  } else {
    return BytecodeNext(op);
  }
}

struct BytecodeRange {
  const void* start;
  const void* end;

  BytecodeRange(const void* s, const void* e) : start(s), end(e) {}

  BytecodeRange(const BytecodeRange&) = default;
  BytecodeRange(BytecodeRange&&) = default;
  BytecodeRange& operator=(const BytecodeRange&) = default;
  BytecodeRange& operator=(BytecodeRange&&) = default;

  bool Contains(const void* ptr) const { return ptr >= start && ptr <= end; }
  bool Before(const void* ptr) const { return ptr >= end; }
  bool After(const void* ptr) const { return ptr <= start; }

  bool operator==(const BytecodeRange& range) const {
    return start == range.start && end == range.end;
  }
  bool operator!=(const BytecodeRange& range) const {
    return !(*this == range);
  }

  std::string Print() const { return base::Fmt("%p-%p", start, end); }
};

class BytecodeIterator {
 public:
  explicit BytecodeIterator(const BytecodeRange& range)
      : current_(range.start), end_(range.end) {}

  BytecodeIterator(const void* start, const void* end)
      : current_(start), end_(end) {}

  bool Has() const { return current_ < end_; }

  void Next() {
    const auto& i = info();
    current_ = static_cast<const char*>(current_) + i.size;
  }

  const QJSBytecodeInfo& info() const { return GetQJSBytecodeInfo(opcode()); }

  Bytecode opcode() const {
    CHECK(Has());
    return static_cast<Bytecode>(*static_cast<const std::uint8_t*>(current_));
  }

  const void* current() const { return current_; }
  const void* payload() const { return static_cast<const char*>(current_) + 1; }
  const void* NextPC(std::int32_t offset) const {
    return static_cast<const char*>(payload()) + offset;
  }

 public:
  std::uint32_t GetU32(std::size_t offset = 0) const;
  std::int32_t GetI32(std::size_t offset = 0) const;
  std::uint16_t GetU16(std::size_t offset = 0) const;
  std::uint8_t GetU8(std::size_t offset = 0) const;

 private:
  const void* current_;
  const void* end_;

  DISALLOW_COPY_AND_ASSIGN(BytecodeIterator);
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_BYTECODE_H_
