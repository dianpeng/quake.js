#ifndef _BASE_PTR_H_
#define _BASE_PTR_H_

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "check.h"
#include "free-list.h"

namespace base {
template <typename T>
class IntrusiveRefCountUnsafeFreeList;
}  // namespace base

namespace boost {
template <typename T>
void intrusive_ptr_add_ref(base::IntrusiveRefCountUnsafeFreeList<T>*);
template <typename T>
void intrusive_ptr_release(base::IntrusiveRefCountUnsafeFreeList<T>*);
}  // namespace boost

namespace base {

// Here we just make a convinient way to allow user to choose certain types of
// smart pointers exposed not by boost. stl still lacks several types of smart
// pointer for us to choose from
template <typename T>
class IntrusiveRefCountUnsafe
    : public boost::intrusive_ref_counter<IntrusiveRefCountUnsafe<T>,
                                          boost::thread_unsafe_counter> {};

template <typename T>
class IntrusiveRefCountSafe
    : public boost::intrusive_ref_counter<IntrusiveRefCountSafe<T>,
                                          boost::thread_safe_counter> {};

// Customized deleter to suit the needs for deleting pointer inside of
// free list.
template <typename T>
class IntrusiveRefCountUnsafeFreeList {
 public:
  std::size_t count() const { return count_; }
  bool unique() const { return count_ == 1; }

 protected:
  // Constructed a unsafe free list backed intrusive pointer with targetted
  // free list pointer.
  explicit IntrusiveRefCountUnsafeFreeList(FreeList<T>* fl) : free_list_(fl) {}

  virtual ~IntrusiveRefCountUnsafeFreeList() { CHECK(count_ == 0); }

 private:
  std::size_t count_;
  FreeList<T>* free_list_;

  template <typename U>
  friend void boost::intrusive_ptr_add_ref(IntrusiveRefCountUnsafeFreeList<U>*);

  template <typename U>
  friend void boost::intrusive_ptr_release(IntrusiveRefCountUnsafeFreeList<U>*);
};

// The sad thing is we still don't have a effective way to know whether a ptr is
// valid or not by using weak_ptr like construct from intrusive_ptr. Sadly, we
// cannot do notification around intrusive pointer for now.
template <typename T>
using RefCountPtr = boost::intrusive_ptr<T>;

}  // namespace base

// Notes, we define the specialized type deleter inside of the boost namespace
// to avoid adl definition required by user defined type.
namespace boost {

template <typename T>
void intrusive_ptr_add_ref(base::IntrusiveRefCountUnsafeFreeList<T>* p) {
  p->count_++;
}

template <typename T>
void intrusive_ptr_release(base::IntrusiveRefCountUnsafeFreeList<T>* p) {
  if (--p->count_ == 0) {
    // During deletion, the free_list_ field member is invalided, or should be
    // considered as invalided
    auto free_list = p->free_list_;
    free_list->Delete(p);
  }
}

}  // namespace boost

#endif  // _BASE_PTR_H_
