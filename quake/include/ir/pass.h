#ifndef _QUAKE_IR_PASS_H_
#define _QUAKE_IR_PASS_H_

#include "base.h"
#include "ir/ssa-var.h"

namespace quake {
class Quake;

namespace ir {
class Node;
class Graph;

// Represent an IR level optimization pass. The driver will call the pass in a
// data flow fashion until convergence
class Pass {
 public:
  struct Result {
    enum {
      // Nothing changed for the specified node
      kNoChange,

      // The node should be replaced with specified replacement
      kReplacement,

      // Remove the node
      kRemove
    };

   public:
    int state;
    Node* node = nullptr;
    Node* origin = nullptr;

    explicit Result(int s) : state(s), node(nullptr) {}
    Result(int s, Node* n) : state(s), node(n) {}

    bool IsNoChange() const { return state == kNoChange; }
    bool IsReplacement() const { return state == kReplacement; }
    bool IsRemove() const { return state == kRemove; }
  };

 public:
  static Result NoChange() { return Result{Result::kNoChange}; }
  static Result Replace(Node* node) {
    return Result{Result::kReplacement, node};
  }
  static Result Replace(SSAVar* var) { return Replace(var->node_checked()); }

  static Result Remove() { return Result{Result::kRemove}; }
  static Result Remove(Node* node) { return Result{Result::kRemove, node}; }
  static Result Remove(SSAVar* var) { return Remove(var->node_checked()); }

 public:
  // Invoked when we visit a certain node
  virtual Result On(Graph*, Node*) = 0;

  // Invoked when the whole pass is done
  virtual void Finalize() {}

  virtual ~Pass() {}

  // Get the name of the pass
  virtual std::string GetName() const = 0;

 public:
  // Run a single pass on the specified ir::Graph
  static void Run(Graph*, Pass*, std::vector<Result>*);
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_PASS_H_
