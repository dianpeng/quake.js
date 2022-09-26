#ifndef _BASE_STL_UTIL_H_
#define _BASE_STL_UTIL_H_
#include <iterator>

#include "common.h"

namespace base {

template <typename T>
inline void VectorAppend(std::vector<T>* vec, const T* data, std::size_t len) {
  static_assert(std::is_trivially_copyable<T>::value);
  auto old_size = vec->size();
  vec->resize(old_size + len);
  auto start = vec->data() + old_size;
  std::memcpy(start, data, len * sizeof(T));
}

template <typename T>
const typename T::container_type& GetInternalContainer(const T& adapter) {
  struct Hack : T {
    using T::c;
  };
  return adapter.*&Hack::c;
}

template <typename T>
typename T::container_type* GetInternalContainer(T* adapter) {
  struct Hack : T {
    using T::c;
  };
  return &((*adapter).*&Hack::c);
}

// Workaround the stupid std::priority_queue's |TOP| function, ie it returns a
// const reference instead of immutable one. Motherfucker, I get your stupid
// reason to keep a min-heap you should make priority immutable, BUT pls just
// make that priority immutable instead of sacrifising the usability of this
// container to what'ever your imaginary design/techinique issue. Get the shit
// work for as many scenarios as possible, this is the point of a freaking
// library !
//
// The following function does a hack, ie it const cast the TOS and then move
// it to a temporary variable. Lastly it pop the queue and return the stuff
// on the stack.

template <typename C, typename RT = typename C::value_type>
RT TopAndPop(C* q) {
  // Must be priority_queue specialization type
  static_assert(IsSpecialization<C, std::priority_queue>::kValue);

  DEFER([q]() { q->pop(); });
  return std::move(const_cast<RT&>(q->top()));
}

// --------------------------------------------------------------------------
// Reclaim make sure that the container will not hold any memory even if the
// container is still there. Notes, after the Reclaim the container will be
// cleared. ie std::size(T) == 0 && std::empty(T) == true

template <typename T>
void Reclaim(std::vector<T>* vec) {
  vec->clear();
  vec->shrink_to_fit();
}

inline void Reclaim(std::string* str) {
  str->clear();
  str->shrink_to_fit();
}

template <typename T>
void Reclaim(std::deque<T>* vec) {
  vec->clear();
  vec->shrink_to_fit();
}

namespace _ {

template <typename T>
struct ReclaimableContainer : FalseType {};

template <typename T>
struct ReclaimableContainer<std::vector<T>> : TrueType {};

template <typename T>
struct ReclaimableContainer<std::deque<T>> : TrueType {};

}  // namespace _

template <typename T, typename _P = std::priority_queue<T>,
          std::enable_if_t<
              _::ReclaimableContainer<typename _P::conatiner_type>::value, int>>
void Reclaim(std::priority_queue<T>* vec) {
  Reclaim(*GetInternalContainer(vec));
}

template <typename T, typename _P = std::queue<T>,
          std::enable_if_t<
              _::ReclaimableContainer<typename _P::conatiner_type>::value, int>>
void Reclaim(std::queue<T>* vec) {
  Reclaim(*GetInternalContainer(vec));
}

template <typename T, typename _P = std::stack<T>,
          std::enable_if_t<
              _::ReclaimableContainer<typename _P::conatiner_type>::value, int>>
void Reclaim(std::stack<T>* vec) {
  Reclaim(*GetInternalContainer(vec));
}

// Used to shrink the std::string in front and optionally mutate its underlying
// memory internally
inline void ShrinkFront(std::string* data, std::size_t len) {
  static const double kThreshold = 0.5;
  static const std::size_t kMinThreshold = 16;  // aovid SSO
  DCHECK(data->size() >= len);
  auto rest = (data->size() - len);

  // Notes(dpeng): Here we have a division of double which is really slow, and
  //   in theory since one part of the expression is a static number, so
  //   compiler should optimize it as a floating point mulitply operation but
  //   better optimize it away.
  if ((data->size() > kMinThreshold) &&
      (static_cast<double>(rest) / data->capacity()) < kThreshold) {
    std::string temp;
    temp.resize(rest);
    temp.assign(data->data() + len, rest);

    // Just do a swap with temp
    data->swap(temp);
  } else {
    data->erase(0, len);
  }
}

inline void EraseFront(std::string* data, std::size_t length) {
  DCHECK(length <= data->size());
  auto beg = data->begin();
  std::advance(beg, length);
  data->erase(data->begin(), beg);
}

template <typename T>
inline void EraseFront(std::vector<T>* vec, std::size_t length) {
  DCHECK(length <= vec->size());
  auto beg = vec->begin();
  std::advance(beg, length);
  vec->erase(vec->begin(), beg);
}

template <typename T>
inline void EraseIndex(T* set, std::size_t idx) {
  auto beg = std::begin(*set);
  std::advance(beg, idx);
  set->erase(beg);
}

// Allow iteration against a list of element and permit delete current element.
// Notes, this is nearly always in possible to use against std::vector/dequeue
// any deleteion of std::vector/std::deque will invalidate any iterators. Unless
// using a index based method.
//
// TODO(dpeng): Add traits static check against using vector/deque for this
// function.
template <typename F, typename C>
inline void ForeachMaybeDeleteCurrent(F&& h, C* c) {
  auto beg = c->begin();
  auto end = c->end();

  for (; beg != end;) {
    auto& current = *beg;
    ++beg;
    if (!h(current)) break;
  }
}

template <typename T>
class Cyclic;

// Cyclic iterator, ie forms the number as a ring for iteration purpose.
//   Usage as following :
//
//   for (const auto& v : Cyclic<int>(2, 5)) {
//     std::cout << v << std::endl;
//   }
//
//   2 3 4 5 0 1
//
// Notes, 0 is a valid number/element inside of the sequence
template <typename T>
class CyclicIterator
    : public std::iterator<std::forward_iterator_tag, T, std::size_t> {
  using Base = std::iterator<std::forward_iterator_tag, T, std::size_t>;

 public:
  using typename Base::difference_type;
  using typename Base::iterator_category;
  using typename Base::pointer;
  using typename Base::reference;
  using typename Base::value_type;

 public:
  CyclicIterator(T start, T end, T rollback = T{});
  CyclicIterator<T>& operator++();
  CyclicIterator<T> operator++(int);
  bool operator==(const CyclicIterator&) const;
  bool operator!=(const CyclicIterator&) const;
  T& operator*() { return current_; }
  const T& operator*() const { return current_; }

 private:
  CyclicIterator(T start, T end, T rollback, T count)
      : current_(0),
        start_(start),
        end_(end),
        rollback_(rollback),
        count_(count) {}

  T current_;
  T start_;
  T end_;
  T rollback_;
  std::size_t count_;

  friend class Cyclic<T>;
};

template <typename T>
class Cyclic {
 public:
  using iterator = CyclicIterator<T>;

  Cyclic(T start, T end, T rollback = T{})
      : start_(start), end_(end), rollback_(rollback) {}

  iterator begin() { return iterator{start_, end_, rollback_}; }

  iterator end() {
    auto count = (end_ - start_) + (start_ - rollback_);
    return iterator{start_, end_, rollback_, count};
  }

 private:
  T start_;
  T end_;
  T rollback_;
};

template <typename T>
CyclicIterator<T>::CyclicIterator(T start, T end, T rollback)
    : current_(start),
      start_(start),
      end_(end),
      rollback_(rollback),
      count_(0) {
  DCHECK(start <= end);
  DCHECK(rollback <= start);
  if (current_ == end_) {
    current_ = rollback_;
  }
}

template <typename T>
CyclicIterator<T>& CyclicIterator<T>::operator++() {
  ++current_;
  if (current_ == end_) {
    current_ = rollback_;
    DCHECK(current_ <= end_);
  }
  ++count_;
  return *this;
}

template <typename T>
CyclicIterator<T> CyclicIterator<T>::operator++(int) {
  auto temp{*this};
  ++current_;
  if (current_ == end_) {
    current_ = rollback_;
    DCHECK(current_ <= end_);
  }
  ++count_;
  return temp;
}

template <typename T>
bool CyclicIterator<T>::operator==(const CyclicIterator& itr) const {
  return start_ == itr.start_ && end_ == itr.end_ &&
         rollback_ == itr.rollback_ && count_ == itr.count_;
}

template <typename T>
bool CyclicIterator<T>::operator!=(const CyclicIterator& itr) const {
  return !(*this == itr);
}

}  // namespace base

#include "_/chromium-erase-if.h"

#endif  // _BASE_STL_UTIL_H_
