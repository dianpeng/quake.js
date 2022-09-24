#ifndef _QUAKE_IR_SSA_VAR_H_
#define _QUAKE_IR_SSA_VAR_H_
#include "base.h"
#include "quake.h"

#include "ir/ssa.h"

namespace quake {
namespace ir {

class Node;

// A simple record that traces which node at what position reference the SSAVar
struct SSARef {
  Node* who;
  int index;
  SSAVar** ref;  // a pointer to where the SSAVar* is stored. Used for fast
                 // node replacement etc ...

  static SSARef Make(Node* w, int i, SSAVar** r) { return SSARef{w, i, r}; }
};

class SSARefList {
 public:
  explicit SSARefList(zone::Zone* zone) : use_list_(zone) {}

  // Find the ref record of (Node*, int)
  std::optional<SSARef> FindUse(Node*, int) const;

  // Check whether (Node*, int) uses the SSAVar or not
  bool HasUse(Node* n, int i) const { return FindUse(n, i).has_value(); }
  std::size_t HasUse(Node*) const;

  bool RemoveUse(Node*, int);
  std::size_t RemoveUse(Node*);

  bool AddUse(const SSARef&);
  bool AddUse(Node* n, int i, SSAVar** ref) {
    return AddUse(SSARef::Make(n, i, ref));
  }
  void AddUseChecked(Node* n, int i, SSAVar** ref) { CHECK(AddUse(n, i, ref)); }

  std::size_t UseSize() const { return use_list_.size(); }
  bool HasUse() const { return !use_list_.empty(); }

 private:
  // Def-Use chain, ie record a list of use position where the node uses this
  // SSAVar object
  zone::Vector<SSARef> use_list_;
};

// Each Node will use SSAVar to indirectly reference an operand or produce a
// SSAVar as output. SSAVar is a variable corresponding to the SSA name.
// We organize SSAVar in a sperate array for fast expression level access and
// optimization.
class SSAVar : public SSARefList {
 public:
  SSAVar(Quake* q, std::uint32_t index, Node* n);

 public:
  Quake* quake() const { return quake_; }
  std::uint32_t index() const { return index_; }
  SSAFlag flag() const { return flag_; }
  void set_flag(SSAFlag f) { flag_ = f; }

  void SetDead() { flag_.SetDead(); }
  bool IsDead() const { return flag_.IsDead(); }

 public:
  Node* node() const { return node_; }
  const Type& type() const { return type_; }
  bool IsInternal() const { return index_ == kInternalIndex; }

 public:
  // Lowering usage. Helper to transition from SSA => LIR

 private:
  Quake* quake_;
  std::uint32_t index_;
  SSAFlag flag_;
  Type type_;

  // The node that generates this SSAVar variable
  Node* node_;
};

}  // namespace ir
}  // namespace quake

#endif  // _QUAKE_IR_SSA_VAR_H_
