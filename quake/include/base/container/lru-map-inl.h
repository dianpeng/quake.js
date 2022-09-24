#ifndef _BASE_CONTAINER_LRU_MAP_INL_H_
#define _BASE_CONTAINER_LRU_MAP_INL_H_

namespace base {

template <typename K, typename V>
LRUMap<K, V>::LRUMap(std::size_t me, bool r)
    : max_elements_(me), map_(), list_() {
  DCHECK(me);
  if (r) {
    map_.reserve(me);
  }
}

template <typename K, typename V>
void LRUMap<K, V>::DoEvict() {
  if (list_.empty()) return;

  // The least recent used element is at the back of the std::list, just pop it
  // up and do the erase. Notes, we still need to find it from the map and erase
  // it from the OOL index map and then do the list pop up. The order is of
  // greate importance since the map's entry is stored inside of the list,
  // including its key
  auto& back = list_.back();

  // Find the index inside of the map and erase the map first
  {
    auto itr = map_.find(&(back.first));
    DCHECK(itr != map_.end());
    map_.erase(itr);
  }

  // Lastly pop the element out of the list
  list_.pop_back();
}

template <typename K, typename V>
std::size_t LRUMap<K, V>::Evict(std::size_t num) {
  auto old = num;
  for (; !list_.empty() && num; --num) {
    DoEvict();
  }
  return old - num;
}

template <typename K, typename V>
V* LRUMap<K, V>::Put(K&& key, V&& value) {
  // (0) Check whether need to do the eviction, if so just do it
  if (size() == max_elements_) {
    DoEvict();
  }
  DCHECK(size() < max_elements_);

  // (1) Insert the element into the storage list first, notes, at the front
  //     since the newly added element is been treated as most recenetly used
  auto pos = list_.insert(list_.begin(),
                          std::make_pair(std::move(key), std::move(value)));

  // (2) Now start to insert the entry into the OOL map/index
  auto iret = map_.insert(std::make_pair(&(pos->first), pos));

  // (3) The map insertion |CAN| failed due to the name collision
  if (!iret.second) {
    // Now remove the old entry and then do the insertion again.
    // NOTES(dpeng): Add policy configuration ?
    auto list_itr = iret.first->second;
    map_.erase(iret.first);
    list_.erase(list_itr);

    iret = map_.insert(std::make_pair(&(pos->first), pos));
    DCHECK(iret.second);
  }

  return &(pos->second);
}

template <typename K, typename V>
V* LRUMap<K, V>::Get(const K& key) {
  auto itr = map_.find(&key);
  if (itr == map_.end()) {
    return nullptr;
  }

  // Once it is been get, we need to mutate its position inside of the list,
  // ie just using splice to insert it back to the front. This simply does
  // pointer linking and no need to bother heap allocation which gonna break
  // the iterator validity
  list_.splice(list_.begin(), list_, itr->second);
  return &((itr->second)->second);
}

template <typename K, typename V>
bool LRUMap<K, V>::Remove(const K& key) {
  auto itr = map_.find(&key);
  if (itr == map_.end()) {
    return false;
  }

  auto list_itr = itr->second;

  // remove from index
  map_.erase(itr);

  // remove from list
  list_.erase(list_itr);

  return true;
}

template <typename K, typename V>
bool LRUMap<K, V>::Visit(
    std::function<bool(const K&, const V&)>&& visitor) const {
  for (const auto& x : list_) {
    if (!visitor(x.first, x.second)) {
      return false;
    }
  }
  return true;
}

}  // namespace base

#endif  // _BASE_CONTAINER_LRU_MAP_INL_H_
