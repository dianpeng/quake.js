#ifndef _QUAKE_LIR_TRANSLATOR_H_
#define _QUAKE_LIR_TRANSLATOR_H_
#include "base.h"

namespace quake {
class Quake;

namespace ir {
class Graph;
class Node;
class Checkpoint;
class SSAVar;
}  // namespace ir

namespace lir {
class HAssembler;

// Translator, ie lowering IR to LIR via HAssembler object
class Translator {
 public:
  Translator(Quake*, HAssembler*);

  void Run(ir::Graph*);

 private:
 private:
  Quake* quake_;
  HAssembler* as_;

  DISALLOW_COPY_AND_ASSIGN(Translator);
};

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_TRANSLATOR_H_

