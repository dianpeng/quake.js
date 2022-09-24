#ifndef _BASE_CONTAINER_INTRUSIVE_LIST_H_
#define _BASE_CONTAINER_INTRUSIVE_LIST_H_

#include <boost/intrusive/list.hpp>

// Notes, this wrapper choose to use auto_unlink which means a list's object
// that spreaded cross different thread is incorrect. User should make sure
// that the intrusive list can only be used in a single thread.

namespace base {

namespace _ {
struct DefaultIntrusiveListTag {};

template <typename Tag>
using IntrusiveListHookBase = boost::intrusive::list_base_hook<
    boost::intrusive::tag<Tag>,
    boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
}  // namespace _

template <typename T, typename Tag>
using IntrusiveList = boost::intrusive::list<
    T, boost::intrusive::constant_time_size<false>, boost::intrusive::tag<Tag>,
    boost::intrusive::base_hook<_::IntrusiveListHookBase<Tag>>>;

// This is mostly a boost.intrusive wrapper around intrusive list. We prefer to
// using the base/inheritance method for setting up the hook point and also
// provide simpler wrapper around the usage of it.
template <typename TagType>
struct IntrusiveListBase : _::IntrusiveListHookBase<TagType> {
 private:
  using Base = _::IntrusiveListHookBase<TagType>;

 protected:
  // This constructor automatically insert |THIS| into intrusive pointer
  // list specified as input
  template <typename U>
  explicit IntrusiveListBase(IntrusiveList<U, TagType>* l) : Base() {
    l->push_back(static_cast<U&>(*this));
  }

  IntrusiveListBase() = default;

  // all these functions are purposely defined as protected. User needs to make
  // them public by exposing them explicitly
  bool IsLinked() const { return Base::is_linked(); }

  void Unlink() { Base::unlink(); }
};

using IntrusiveListBaseDefault = IntrusiveListBase<_::DefaultIntrusiveListTag>;

template <typename T>
using IntrusiveListDefault = IntrusiveList<T, _::DefaultIntrusiveListTag>;

// This method allow user to iterate intrusive list and also safely perform
// method call which can lead to object been destructed. Iteration the list
// while trying to delete any element inside of the intrusive list is undefined
// behavior.
// Example :
//
//   IntrusiveList<MyObject> my_object_list;
//
//   // Assume MyObject::Terminate function may delete itself if invoked
//
//   IntrusiveListDeleteIf(my_object_list,
//     [](const MyObject& obj) -> bool {
//       return obj.ShouldObject();
//     },
//     [](MyObject* obj) -> void {
//       obj->Terminate()
//     },
//     max_allowed);
//
// Internally, this function splits the processing into 2 loops and use an
// extra std::vector to track object needs to be deleted. The first loop just
// scan the object on the intrusive list with predicator provided by user and
// then move all these deletion, candidate into an extra std::vector. Lastly
// iterate this std::vector to perform the deletion. Notes, if any single delete
// operation invalidate another object in the list, no only itself, then the
// behavior is still undefined. If that's case, prefer using observer list which
// can do randomly deletion, ie regardless whether the deletion impact itself
// or any other object in the list.
template <typename U, typename Pred, typename Deleter, typename Tag>
std::size_t IntrusiveListDeleteIf(IntrusiveList<U, Tag>* list,
                                  Pred&& predication, Deleter&& deleter,
                                  std::size_t max_allowed) {
  std::vector<U*> action_list;

  // Just an educated guess to avoid memory allocation
  action_list.reserve(list->size() / 2);

  // loop 1, scan for candidates
  for (auto& e : *list) {
    if (predication(static_cast<const U&>(e))) {
      action_list.emplace_back(&e);
    }
    if (action_list.size() >= max_allowed) break;
  }

  // loop 2, perform the delete actions
  for (auto e : action_list) {
    deleter(e);
  }

  return action_list.size();
}

}  // namespace base

#endif  // _BASE_CONTAINER_INTRUSIVE_LIST_H_
