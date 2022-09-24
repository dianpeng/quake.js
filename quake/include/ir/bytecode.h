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

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_BYTECODE_H_
