#ifndef _QUAKE_LIR_CFG_H_
#define _QUAKE_LIR_CFG_H_
#include "base.h"
#include "quake.h"
#include "stringify.h"
#include "lir/instruction.h"

namespace quake {
namespace lir {

class HInstruction;
class HAssembler;

struct BasicBlockLink {
  BasicBlock* bb = nullptr;
  BasicBlockLink* prev = nullptr;
  BasicBlockLink* next = nullptr;

  explicit BasicBlockLink(BasicBlock* b) : bb(b) {}

  BasicBlock* NextBlock() const { return next ? next->bb : nullptr; }
  BasicBlock* PrevBlock() const { return prev ? prev->bb : nullptr; }

  inline void LinkAfter(BasicBlockLink*);
  inline void LinkBefore(BasicBlockLink*);
  inline void Remove();

  bool IsLinked() const { return prev != nullptr && next != nullptr; }
};

// ----------------------------------------------------------------------------
// CFG graph of LIR. Notes, after lowering and transforming the LIR is no longer
// valid as just a 2 BB graph, but can have multiple BB due to internal jumpping
// In order to allow optimization to work, we need to generate valid CFG of the
// LIR for optimization
class BasicBlock : public Stringify, public GraphvizNode {
 public:
  BasicBlock(Quake* quake, std::uint32_t bid)
      : quake_(quake),
        block_id_(bid),
        prev_(quake->zone()),
        dom_(quake->zone()),
        domtree_children_(quake->zone()) {}

  HInstruction* start() { return start_; }
  const HInstruction* start() const { return start_; }

  HInstruction* end() { return end_; }
  const HInstruction* end() const { return end_; }

  const HInstruction* last_instruction() const { return end_->prev(); }
  HInstruction* last_instruction() { return end_->prev(); }

  std::size_t GetInstructionSize() const;
  const HInstruction* GetInstruction(std::size_t idx) const;
  HInstruction* GetInstruction(std::size_t idx);

  bool ForeachInstruction(std::function<bool(HInstruction*)>&&);
  bool ForeachInstruction(std::function<bool(const HInstruction*)>&&) const;

 public:
  BasicBlock* left_bb() const { return left_; }
  BasicBlock* right_bb() const { return right_; }
  std::uint32_t block_id() const { return block_id_; }

  // Whether the backedge jump is a back edge. Notes, require DOM creation
  inline bool IsLeftBackEdge() const;
  inline bool IsRightBackEdge() const;

 public:
  struct Edge {
    BasicBlock* prev = nullptr;
    explicit Edge(BasicBlock* bb) : prev(bb) {}
  };

  // The specified BasicBlock will jump to the current BasicBlock
  inline void LinkLeft(BasicBlock*);
  inline void LinkRight(BasicBlock*);

  void AddEdge(BasicBlock* bb) { prev_.emplace_back(bb); }
  inline void RemoveEdge(BasicBlock* bb);
  inline bool HasEdge(BasicBlock* bb) const;

  // Foreach predecessors
  bool ForeachEdge(std::function<bool(const Edge&)>&&) const;

 public:
  void EmptyBlock(HAssembler*);

  std::string Print() const override;
  std::string PrintGraphName() const override;
  std::string PrintGraphNode() const override;
  std::string PrintGraphLink() const override;

 public:
  enum BlockType { kBlock, kStart, kEnd, kDeoptBlock };
  static const char* GetBlockTypeName(int);

  void set_block_type(BlockType bt) { block_type_ = bt; }
  BlockType block_type() const { return block_type_; }
  const char* block_type_name() const { return GetBlockTypeName(block_type()); }

 public:
  BasicBlock* idom() const { return idom_; }
  const zone::Vector<BasicBlock*>& domtree_children() const {
    return domtree_children_;
  }

  const zone::Vector<BasicBlock*>& dom() const { return dom_; }

 private:
  void set_end(HInstruction*);
  void set_start(HInstruction*);
  void set_idom(BasicBlock*);

 private:
  Quake* quake_;                   // quake
  std::uint32_t block_id_ = 0;     // block_id_, unique controlled by CFG
  BlockType block_type_ = kBlock;  // block type, mostly just normal block
  BasicBlock* left_ = nullptr;
  BasicBlock* right_ = nullptr;
  zone::Vector<Edge> prev_;
  HInstruction* start_ = nullptr;  // starting instruction of basic block
  HInstruction* end_ =
      nullptr;  // ending instruction (exclusive) of basic block

  // --------------------------------------------------------------------------
  BasicBlock* rpo_next_ = nullptr;  // After RPO sorting, the basic block will
                                    // be chained in RPO order which allows us
                                    // to iterate them in RPO way accordingly

  // --------------------------------------------------------------------------
  // Dominator tree orgnization, requires DOM analysis
  zone::Vector<BasicBlock*> dom_;               // dominator set
  BasicBlock* idom_ = nullptr;                  // immediate dominator
  zone::Vector<BasicBlock*> domtree_children_;  // children of domtree

  // notes for now we just record DFS timestamp, it is trivial to translate this
  // into RPO number which forms RPO order for future optimization optimal
  // convergence issue
  std::uint32_t dfs_start_ =
      std::numeric_limits<std::uint32_t>::max();  // DFS number
  std::uint32_t dfs_end_ =
      std::numeric_limits<std::uint32_t>::max();  // DFS number

  DISALLOW_COPY_AND_ASSIGN(BasicBlock);

  friend class CFG;
};

// ----------------------------------------------------------------------------
// CFG, holds all known BB and are supposed to own the BB orginization. Notes
// in theory, if CFG is not longer valid, all the BB is no longer valid anymore,
// including REF inside of the HInstruction. BB will take care of these in its
// dtor, but zone may not allow dtor to be none-trivial ...
class CFG : public Stringify {
 public:
  explicit CFG(Quake* quake) : quake_(quake) {}

  // Create CFG, after this function call all the start, end, deopt block will
  // be populated and user is capable of using BasicBlock as CFG
  void CreateCFG(HAssembler*);

  // Create Dominator Tree relationship. After the invocation of this function,
  // the dominator tree, immediate dominator relationship will be setup for each
  // BasicBlock
  void CreateDom();

 public:
  BasicBlock* start() const { return start_; }
  BasicBlock* end() const { return end_; }
  Quake* quake() const { return quake_; }
  std::uint32_t block_id_max() const { return block_id_; }

 public:
  // No order requirement (no guarantee to be Pre/Post/In/RPO etc ...)
  bool ForeachBasicBlock(std::function<bool(BasicBlock*)>&&);
  bool ForeachBasicBlock(std::function<bool(const BasicBlock*)>&&) const;

 public:
  // Visit the CFG as pre-order of DOM tree visiting order. This is useful
  // for certain pass which requires DOM relationship, ie code motion, move
  // move elimination etc ...
  bool ForeachDomTree(std::function<bool(const BasicBlock*)>&&) const;
  bool ForeachDomTree(std::function<bool(BasicBlock*)>&&);

 public:
  std::string Print() const override;
  std::string PrintGraph() const;
  std::string PrintDom() const;

 private:
  BasicBlock* AddBB(HInstruction*);
  BasicBlock* AddBB(HInstruction*, HInstruction*);
  BasicBlock* GetBB(HInstruction*);
  bool HasBB(HInstruction*) const;
  BasicBlock* NewBB();

  void FindBB(HAssembler*);
  void MaterializeBB(HAssembler*);

 private:
  void RunDFSNumber();
  void RunDom();

 private:
  template <typename T>
  bool ForeachBB(T&&);
  template <typename T>
  bool ForeachDom(T&&);

  using BBMap = std::map<HInstruction*, BasicBlock*>;

  Quake* quake_;
  BasicBlock* start_ = nullptr;
  BasicBlock* end_ = nullptr;
  BasicBlock* deopt_ = nullptr;
  std::uint32_t block_id_ = 0;
  BBMap bbmap_;

  DISALLOW_COPY_AND_ASSIGN(CFG);
};

inline void BasicBlock::LinkLeft(BasicBlock* bb) {
  if (left_) left_->RemoveEdge(this);

  left_ = bb;
  bb->AddEdge(this);
}

inline void BasicBlock::LinkRight(BasicBlock* bb) {
  if (right_) right_->RemoveEdge(this);
  right_ = bb;
  bb->AddEdge(this);
}

inline void BasicBlock::RemoveEdge(BasicBlock* bb) {
  if (auto itr = std::find_if(
          prev_.begin(), prev_.end(),
          [&](const auto prev_bb) -> bool { return prev_bb.prev == bb; });
      itr != prev_.end()) {
    prev_.erase(itr);
  }
}

inline bool BasicBlock::HasEdge(BasicBlock* bb) const {
  return std::find_if(prev_.begin(), prev_.end(),
                      [&](const auto prev_bb) -> bool {
                        return prev_bb.prev == bb;
                      }) != prev_.end();
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_CFG_H_
