#ifndef _BASE___CHROMIUM_ERASE_IF_H_
#define _BASE___CHROMIUM_ERASE_IF_H_

// The following code is taken from Chromium's source code with small
// modification.
namespace base {
namespace _ {

// Calls erase on iterators of matching elements and returns the number of
// removed elements.
template <typename Container, typename Predicate>
size_t IterateAndEraseIf(Container& container, Predicate pred) {
  size_t old_size = container.size();
  for (auto it = container.begin(), last = container.end(); it != last;) {
    if (pred(*it))
      it = container.erase(it);
    else
      ++it;
  }
  return old_size - container.size();
}

}  // namespace _

template <typename CharT, typename Traits, typename Allocator, typename Value>
size_t Erase(std::basic_string<CharT, Traits, Allocator>& container,
             const Value& value) {
  auto it = std::remove(container.begin(), container.end(), value);
  size_t removed = std::distance(it, container.end());
  container.erase(it, container.end());
  return removed;
}

template <typename CharT, typename Traits, typename Allocator, class Predicate>
size_t EraseIf(std::basic_string<CharT, Traits, Allocator>& container,
               Predicate pred) {
  auto it = std::remove_if(container.begin(), container.end(), pred);
  size_t removed = std::distance(it, container.end());
  container.erase(it, container.end());
  return removed;
}

template <class T, class Allocator, class Value>
size_t Erase(std::deque<T, Allocator>& container, const Value& value) {
  auto it = std::remove(container.begin(), container.end(), value);
  size_t removed = std::distance(it, container.end());
  container.erase(it, container.end());
  return removed;
}

template <class T, class Allocator, class Predicate>
size_t EraseIf(std::deque<T, Allocator>& container, Predicate pred) {
  auto it = std::remove_if(container.begin(), container.end(), pred);
  size_t removed = std::distance(it, container.end());
  container.erase(it, container.end());
  return removed;
}

template <class T, class Allocator, class Value>
size_t Erase(std::vector<T, Allocator>& container, const Value& value) {
  auto it = std::remove(container.begin(), container.end(), value);
  size_t removed = std::distance(it, container.end());
  container.erase(it, container.end());
  return removed;
}

template <class T, class Allocator, class Predicate>
size_t EraseIf(std::vector<T, Allocator>& container, Predicate pred) {
  auto it = std::remove_if(container.begin(), container.end(), pred);
  size_t removed = std::distance(it, container.end());
  container.erase(it, container.end());
  return removed;
}

template <class T, class Allocator, class Predicate>
size_t EraseIf(std::forward_list<T, Allocator>& container, Predicate pred) {
  // Note: std::forward_list does not have a size() API, thus we need to use the
  // O(n) std::distance work-around. However, given that EraseIf is O(n)
  // already, this should not make a big difference.
  size_t old_size = std::distance(container.begin(), container.end());
  container.remove_if(pred);
  return old_size - std::distance(container.begin(), container.end());
}

template <class T, class Allocator, class Predicate>
size_t EraseIf(std::list<T, Allocator>& container, Predicate pred) {
  size_t old_size = container.size();
  container.remove_if(pred);
  return old_size - container.size();
}

template <class Key, class T, class Compare, class Allocator, class Predicate>
size_t EraseIf(std::map<Key, T, Compare, Allocator>& container,
               Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class T, class Compare, class Allocator, class Predicate>
size_t EraseIf(std::multimap<Key, T, Compare, Allocator>& container,
               Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class Compare, class Allocator, class Predicate>
size_t EraseIf(std::set<Key, Compare, Allocator>& container, Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class Compare, class Allocator, class Predicate>
size_t EraseIf(std::multiset<Key, Compare, Allocator>& container,
               Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class T, class Hash, class KeyEqual, class Allocator,
          class Predicate>
size_t EraseIf(std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& container,
               Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class T, class Hash, class KeyEqual, class Allocator,
          class Predicate>
size_t EraseIf(
    std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>& container,
    Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class Hash, class KeyEqual, class Allocator,
          class Predicate>
size_t EraseIf(std::unordered_set<Key, Hash, KeyEqual, Allocator>& container,
               Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class Key, class Hash, class KeyEqual, class Allocator,
          class Predicate>
size_t EraseIf(
    std::unordered_multiset<Key, Hash, KeyEqual, Allocator>& container,
    Predicate pred) {
  return _::IterateAndEraseIf(container, pred);
}

template <class T, class Allocator, class Value>
size_t Erase(std::forward_list<T, Allocator>& container, const Value& value) {
  // Unlike std::forward_list::remove, this function template accepts
  // heterogeneous types and does not force a conversion to the container's
  // value type before invoking the == operator.
  return EraseIf(container, [&](const T& cur) { return cur == value; });
}

template <class T, class Allocator, class Value>
size_t Erase(std::list<T, Allocator>& container, const Value& value) {
  // Unlike std::list::remove, this function template accepts heterogeneous
  // types and does not force a conversion to the container's value type before
  // invoking the == operator.
  return EraseIf(container, [&](const T& cur) { return cur == value; });
}

}  // namespace base

#endif  // _BASE__CHROMIUM_ERASE_IF_H_
