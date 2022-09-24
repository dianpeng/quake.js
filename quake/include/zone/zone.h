#ifndef _ZONE_ZONE_H_
#define _ZONE_ZONE_H_
#include <base.h>

namespace zone {

class Zone {
 private:
  // Zone will track type T if it requires to be destructed manually, ie as
  // long as its destructor is not trivially destructible and then it will be
  // recorded inside of the dtor list
  struct CallDtor {
    virtual ~CallDtor() {}
  };

  template <typename T>
  struct CallDtorImpl : public CallDtor {
    explicit CallDtorImpl(T* x) : ptr_(x) {}
    ~CallDtorImpl() override { base::CallDtor(ptr_); }
    T* ptr_;
  };

  template <typename T,
            std::enable_if_t<std::is_trivially_destructible<T>::value, int> = 0>
  void AddDestructor(T* ptr) {
    UNUSE(ptr);
  }

  template <typename T, std::enable_if_t<
                            !std::is_trivially_destructible<T>::value, int> = 0>
  void AddDestructor(T* ptr) {
    dtor_list_.emplace_back(std::make_unique<CallDtorImpl<T>>(ptr));
  }

 public:
  // Default maximum size for the allocator. After this size, the growth
  // strategy will grow at most max(kDefaultMax, needed_size) instead of
  // exponentially
  static constexpr std::size_t kDefaultMax = 128 * base::kKB;

  // Alignment of the returned pointer. By default it is default alignment
  // on each arch.
  static constexpr std::size_t kAlignment =
      static_cast<std::size_t>(kPlatformAlignmentSize);

 public:
  inline Zone(std::size_t, std::size_t max);

  ~Zone() { Clear(); }

 public:
  inline void* Malloc(std::size_t);

  // New a piece of memory back. Notes the T's constructor is not invoked
  template <typename T>
  T* Malloc() {
    return static_cast<T*>(Malloc(sizeof(T)));
  }

  // New a list of malloc
  template <typename T>
  T* MallocArray(std::size_t size) {
    return static_cast<T*>(Malloc(sizeof(T) * size));
  }

 public:
  // Call constructor on top of the T. Notes, the type specified must not have
  // special type, otherwise it is not freed (its destructor will not be called)
  template <typename T, typename... ARGS>
  T* New(ARGS&&... args);

  // Similar as New, but will place destructor call if needed
  template <typename T, typename... ARGS>
  T* Grab(ARGS&&... args);

 public:
  // Clear all previously allocated memory, notes after the clear, all the
  // memory returned by arena allocator will become invalid.
  void Clear();

 public:
  std::size_t size() const { return size_; }
  std::size_t segment_size() const { return segment_size_; }
  std::size_t segment_count() const { return segment_count_; }
  std::size_t max_size() const { return max_size_; }

  // This returns the totally allocated size from underlying allocator, ie
  // malloc/free etc.
  std::size_t GetTotalSize() const;

 protected:
  inline Zone(std::size_t max, void*, std::size_t);

 private:
  inline void InitSSO(void*, std::size_t);

  inline void Grow(std::size_t);
  inline void GrowAtLeast(std::size_t);

  // One allocation obtained from runtime allocator. We try to allocate relative
  // large chunk and then slice them into small part for allocation purpose.
  struct Segment {
    Segment* next;
    std::size_t size;
  };

 private:
  Segment* segment_;
  void* pool_;
  std::size_t size_;
  std::size_t segment_size_;
  std::size_t segment_count_;
  const std::size_t max_size_;

  std::vector<std::unique_ptr<CallDtor>> dtor_list_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Zone);
};

// Try to allocate small chunk of pre-allocated static buffer initially and only
// move to heap allocation when needed.
template <std::size_t N>
class SSOZone : public Zone {
 public:
  explicit SSOZone(std::size_t max = Zone::kDefaultMax)
      : Zone(max, byte_, N), byte_() {}

 private:
  std::uint8_t byte_[N];
  DISALLOW_COPY_AND_ASSIGN(SSOZone);
};

}  // namespace zone

#include "zone-inl.h"

#endif  // _ZONE_ZONE_H_
