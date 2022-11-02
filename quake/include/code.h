#ifndef _QUAKE_CODE_H_
#define _QUAKE_CODE_H_
#include "base.h"
#include "common.h"

#include <asmjit/asmjit.h>

namespace quake {

// Representing machine code
class Code {
 public:
  virtual asmjit::CodeHolder& code_holder() = 0;
  virtual const asmjit::CodeHolder& code_holder() const = 0;
  virtual JitCodeEntry entry() const = 0;

 public:
  // Very slow and it will consume lots of memory :(
  virtual std::string Disassemble() const = 0;

  virtual ~Code() {}
};

}  // namespace quake

#endif  // _QUAKE_CODE_H_
