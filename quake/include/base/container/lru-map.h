#ifndef _BASE_CONTAINER_LRU_MAP_H_
#define _BASE_CONTAINER_LRU_MAP_H_
#include <base/check.h>
#include <base/common.h>

namespace base {

// A very simple LRUMap. Each Get/Put will impact the relative position inside
// of the LRUMap later to be used by |Evict|. Internally, max_elements will be
// specified and it constraints the upper bound of the LRUMap which gonna
// trigger the LRUMap eviction automatically when size bypass the threshold
template <typename K, typename V>
class LRUMap {
 public:
  explicit LRUMap(std::size_t max_elements, bool reserve = false);

  LRUMap(LRUMap&&) = default;
  LRUMap(const LRUMap&) = default;
  LRUMap& operator=(const LRUMap&) = default;
  LRUMap& operator=(LRUMap&&) = default;

  std::size_t max_elements() const { return max_elements_; }
  std::size_t size() const { return map_.size(); }
  bool empty() const { return map_.empty(); }

  std::size_t GetSize() const { return size(); }
  bool IsEmpty() const { return map_.empty(); }

  bool Visit(std::function<bool(const K&, const V&)>&&) const;

 public:
  V* Put(K&&, V&&);
  V* Put(const K& key, const V& val) { return Put(K{key}, V{val}); }

  V* Get(const K&);
  const V* Get(const K& key) const {
    return const_cast<LRUMap*>(this)->Get(key);
  }
  bool Remove(const K&);
  void Clear() {
    list_.clear();
    map_.clear();
  }

 public:
  // Evict specified number of entry out based on LRU policy
  std::size_t Evict(std::size_t);

 private:
  using Entry = std::pair<K, V>;

  // Notes, we use std::list purposely since the std::list will maintain its
  // iterator validity when erase element, the one that is been erased is not
  // included. This is critical for us to align the storage as following :
  //
  //   1. store the concrete part of data into the std::list
  //   2. use map to set up a OOL map/index pointing back to the list
  //   3. Notes, the map gonna store |iterator| as value for obvious reason
  //
  // And then we can manipulate the element order inside of the std::list
  // without mutating the iterater from std::list from APIs like splice.
  //
  // So the relative LRU order is matinained inside of the std::list and map
  // is existed sololy for fast lookup
  using List = std::list<Entry>;
  using Iterator = typename List::iterator;
  using ConstIterator = typename List::const_iterator;

  // The hash wrapper used for unordered set
  struct EntryHash final {
    std::size_t operator()(const K* const& e) const noexcept {
      return std::hash<std::string>()(*e);
    }
  };
  struct EntryEqual final {
    bool operator()(const K* const& lhs, const K* const& rhs) const noexcept {
      return *lhs == *rhs;
    }
  };
  using Map = std::unordered_map<const K*, Iterator, EntryHash, EntryEqual>;

  void DoEvict();

 private:
  std::size_t max_elements_;

  // NOTES: Currently I just use std::list, if there're performance issue,
  //   like we have very large LRU map size, then it is better to use free
  //   list based list to avoid too frequent memory allocation.
  Map map_;
  mutable List list_;
};

}  // namespace base

#include "lru-map-inl.h"

#endif  // _BASE_CONTAINER_LRU_MAP_H_
