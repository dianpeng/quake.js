#ifndef _BASE_VALUE_H_
#define _BASE_VALUE_H_
#include "check.h"
#include "common.h"
#include "container.h"
#include "result.h"

namespace base {

class List;
class Dictionary;

enum class ValueToJSON { kCompact, kPrettyPrint, kVerbosePrettyPrint };

// A value can be used to hold arbitary value, it can be a primitive value or
// a compound value. It is generally used to hold value from external resource
// like JSON/XML/Config file.
class Value {
 public:
  struct Null {};

  // Notes(dpeng): Order matters.
  //   the |index()| call must match the Type enum definition
  using Storage =
      std::variant<Null, std::int64_t, double, bool, std::string,
                   std::shared_ptr<List>, std::shared_ptr<Dictionary>, Duration,
                   StorageSize>;

  // Notes(dpeng): For simplicity, we don't have an unsigned interface which
  //   makes us not capable of storing uint64_t type. It is up to user's
  //   responsiblity to decide how to store the value.
  enum Type : int {
    kNull = 0,
    kInt,
    kReal,
    kBoolean,
    kString,
    kList,
    kDictionary,
    kDuration,
    kStorageSize
  };

  static inline const char* GetTypeName(Type);

 public:
  inline Value();
  inline explicit Value(std::uint8_t);
  inline explicit Value(std::int8_t);
  inline explicit Value(std::uint16_t);
  inline explicit Value(std::int16_t);
  inline explicit Value(std::uint32_t);
  inline explicit Value(std::int32_t);
  inline explicit Value(std::int64_t);
  inline explicit Value(std::size_t);
  inline explicit Value(double);
  inline explicit Value(float);
  inline explicit Value(bool);

  inline explicit Value(const char*);
  inline explicit Value(const std::string&);
  inline explicit Value(const std::string_view&);
  inline explicit Value(std::string&&);

  inline explicit Value(std::shared_ptr<List>&&);
  inline explicit Value(std::shared_ptr<Dictionary>&&);
  inline explicit Value(const std::shared_ptr<List>&);
  inline explicit Value(const std::shared_ptr<Dictionary>&);

  inline explicit Value(Duration&&);
  inline explicit Value(const Duration&);
  inline explicit Value(StorageSize&&);
  inline explicit Value(const StorageSize&);

  template <typename R, typename P>
  inline explicit Value(const std::chrono::duration<R, P>&);

 public:
  Value(const Value&) = default;
  Value(Value&&) = default;

  Value& operator=(const Value&) = default;
  Value& operator=(Value&&) = default;

  inline Value& operator=(std::uint8_t);
  inline Value& operator=(std::uint16_t);
  inline Value& operator=(std::uint32_t);
  inline Value& operator=(std::int8_t);
  inline Value& operator=(std::int16_t);
  inline Value& operator=(std::int32_t);
  inline Value& operator=(std::int64_t);
  inline Value& operator=(std::size_t);
  inline Value& operator=(double);
  inline Value& operator=(float);
  inline Value& operator=(bool);

  inline Value& operator=(const std::string_view&);
  inline Value& operator=(const char*);
  inline Value& operator=(const std::string&);
  inline Value& operator=(std::string&&);

  inline Value& operator=(std::shared_ptr<List>&&);
  inline Value& operator=(const std::shared_ptr<List>&);
  inline Value& operator=(std::shared_ptr<Dictionary>&&);
  inline Value& operator=(const std::shared_ptr<Dictionary>&);

  inline Value& operator=(Duration&&);
  inline Value& operator=(const Duration&);
  inline Value& operator=(StorageSize&&);
  inline Value& operator=(const StorageSize&);

  template <typename R, typename P>
  inline Value& operator=(const std::chrono::duration<R, P>&);

 public:
  inline Type type() const;
  const char* type_name() const { return GetTypeName(type()); }

  inline bool IsNull() const;
  inline bool IsInt() const;
  inline bool IsReal() const;
  inline bool IsNumber() const;
  inline bool IsString() const;
  inline bool IsBoolean() const;
  inline bool IsList() const;
  inline bool IsDictionary() const;
  inline bool IsDuration() const;
  inline bool IsStorageSize() const;
  inline bool IsScalar() const;
  bool IsCompound() const { return !IsScalar(); }

  inline std::int64_t ToInt() const;
  inline double ToReal() const;
  inline bool ToBoolean() const;
  inline const std::string& ToString() const;

  inline std::shared_ptr<const List> ToList() const;
  inline std::shared_ptr<const Dictionary> ToDictionary() const;

  inline std::shared_ptr<List> ToList();
  inline std::shared_ptr<Dictionary> ToDictionary();

  inline const List* ToListPtr() const;
  inline const Dictionary* ToDictionaryPtr() const;

  inline List* ToListPtr();
  inline Dictionary* ToDictionaryPtr();

  inline const Duration& ToDuration() const;
  inline const StorageSize& ToStorageSize() const;

  operator bool() const { return !IsNull(); }

 public:
  inline void SetNull();
  inline void SetInt(std::int64_t);
  inline void SetReal(double);
  inline void SetBoolean(bool);

  inline void SetString(const char*);
  inline void SetString(std::string&&);
  inline void SetString(const std::string&);
  inline void SetString(const std::string_view&);

  inline void SetList(std::shared_ptr<List>&&);
  inline std::shared_ptr<List> SetList();
  inline void SetDictionary(std::shared_ptr<Dictionary>&&);
  inline std::shared_ptr<Dictionary> SetDictionary();

  inline void SetDuration(Duration&&);
  inline void SetDuration(const Duration&);

  inline void SetStorageSize(StorageSize&&);
  inline void SetStorageSize(const StorageSize&);

  void Clear() { SetNull(); }

 public:
  std::string Print() const;

  // Deep copy of value
  Value Clone() const;

  inline std::string ToJSON(ValueToJSON opt = ValueToJSON::kCompact) const;
  void ToJSON(std::ostream&, ValueToJSON opt = ValueToJSON::kCompact) const;

 public:
  // -------------------------------------------------------------------------
  //                              Search Value
  //
  //  A search is a special magic functions that allow user to write a compact
  //  and declarative way to search a value inside of the value heircahies. The
  //  grammar here is very similar to JSONPath, but strips out some fancy parts
  //  to keep it simple and intuitive.
  //
  //  The basic usage is described as following :
  //
  //   1.  .a.b -> search value when |Root| is an object which has component |a|
  //       and it is a object. Recursively it also has an component called |b|
  //
  //   2.  .a[1] -> Returns Root's component |a|, assert it as an array and
  //       return its 2nd element. Index starts from 0
  //
  //   3.  .a[1:5] -> Returns a Value with |new list| sliced from old lists
  //       range [1, 5), notes, if the old list size is less than 5, it will
  //       be truncated automatically.
  //
  //   4.  type assertion should be suffix for the |LAST| component in path,
  //       since mostly the component that is not leaf of the path already
  //       implicitly render its type so no need to assert type in the middle of
  //       the path, though you can
  //
  //       .a%N -> |%N| represents |a| is null
  //       .a%o -> |%o| represents |a| is an object
  //       .a%i -> |%i| represents |a| is an integer
  //       .a%u -> |%u| represents |a| is an unsigned integer
  //       .a%d -> |%d| represents |a| is a real
  //       .a%n -> |%n| represents |a| is a number, ie integer or real
  //       .a%b -> |%b| represents |a| is a boolean
  //       .a%s -> |%s| represents |a| is a string
  //       .a%a -> |%s| represents |a| is an array
  //       .a%D -> |%D| represents |a| is a duration objecct
  //       .a%Q -> |%Q| represents |a| is a storage size object
  //
  //       .a   -> just return a if it existed, notes, it doesn't assert type
  //
  //   5.  Array element type assertion is also applicable, ie via following
  //       grammar :
  //         .a[1:5%o]  -> this means the slice array's element must |ALL| be
  //                       object.
  //
  //         .a[1:5%(?)o]  -> this means the slice array's needs to filter out
  //                          all object element, notes the returned array may
  //                          not be of size 4
  //
  //         .a[1:5%(!)o] -> this is negative filter, ie filter out all none
  //                          object element.
  //
  //   6.  additionally, user can group multiple alternative search path to
  //       gether without needing to run this multiple times. just using the
  //       *|* operator, example like :
  //
  //         .a.b.c[] | .a.e.c[]
  //
  //  The search function will just return the first matched one from left to
  //  right
  // -------------------------------------------------------------------------

  // Index pointer allow user to know which rule takes effect, the rule can be
  // groupped by | operator. See above example
  Result<std::optional<Value>> Search(const std::string&,
                                      std::size_t* index = nullptr) const;

  // For repeated search, it is performant to just compile the whole pattern
  // into internal representation and later on using the search pattern to do
  // the search. Notes, it is faster to compile them done.
  class SearchPattern {
   public:
    // Print debugable information for the pattern been compiled internally.
    // Mostly user should not need this information at all
    virtual std::string DebugPrint() const = 0;

    // Perform the search
    virtual std::optional<Value> Search(const Value&,
                                        std::size_t* = nullptr) const = 0;

    virtual ~SearchPattern() {}
  };
  static Result<std::unique_ptr<SearchPattern>> CompileSearchPattern(
      const std::string&);

  std::optional<Value> Search(const SearchPattern&,
                              std::size_t* index = nullptr) const;

 private:
  Storage storage_;
};

class List {
  static constexpr std::size_t kSize = 4;
  using Storage = small_vector<Value, kSize>;

 public:
  List() = default;
  List(const List&) = default;
  List(List&&) = default;

  List(std::initializer_list<Value> v) : storage_(v) {}

 public:
  void Add(Value&& v) { storage_.push_back(std::move(v)); }
  void Add(const Value& v) { storage_.push_back(std::move(v)); }
  void Push(Value&& v) { Add(std::move(v)); }
  void Push(const Value& v) { Add(v); }

  void Extend(const List& list) {
    storage_.insert(storage_.end(), list.storage_.begin(), list.storage_.end());
  }

  void Extend(List&& list) {
    storage_.insert(storage_.end(), list.storage_.begin(), list.storage_.end());
  }

  void Swap(List* l) { storage_.swap(l->storage_); }
  std::size_t GetSize() const { return storage_.size(); }
  bool IsEmpty() const { return storage_.empty(); }

  Value& operator[](std::size_t i) { return storage_[i]; }
  const Value& operator[](std::size_t i) const { return storage_[i]; }
  Value& Get(std::size_t i) { return storage_[i]; }
  const Value& Get(std::size_t i) const { return storage_[i]; }

  // boost::small_vector doesn't support reserve, if we switch to std::vector
  //   then this function is usable
  void Reserve(std::size_t v) { UNUSE(v); }

  void Clear() { storage_.clear(); }

 public:
  std::shared_ptr<List> Slice(std::size_t, std::size_t) const;
  void Slice(List*, std::size_t, std::size_t) const;

  std::shared_ptr<List> Clone() const;
  void Clone(List*) const;

 public:
  // STL iterators, suitable for range-for
  using iterator = Storage::iterator;
  using const_iterator = Storage::const_iterator;

  iterator begin() { return storage_.begin(); }
  iterator end() { return storage_.end(); }
  const_iterator begin() const { return storage_.begin(); }
  const_iterator end() const { return storage_.end(); }

 public:
  std::string Print() const;
  inline std::string ToJSON(ValueToJSON opt = ValueToJSON::kCompact) const;
  void ToJSON(std::ostream&, ValueToJSON opt = ValueToJSON::kCompact) const;

 private:
  Storage storage_;
};

class Dictionary {
  // Need to work with std::string_view, so the map must be transparently
  using Storage = std::map<std::string, Value, std::less<>>;

 public:
  Dictionary() = default;
  Dictionary(const Dictionary&) = default;
  Dictionary(Dictionary&&) = default;
  Dictionary(std::initializer_list<Storage::value_type> v) : storage_(v) {}

 public:
  inline bool AddIfNotExist(const std::string_view&, const Value&);
  inline bool AddIfNotExist(const std::string_view&, Value&&);

  inline void Add(const std::string_view&, const Value&);
  inline void Add(const std::string_view&, Value&&);

  inline const Value* Get(const std::string_view&) const;
  inline Value* Get(const std::string_view&);

  inline bool Has(const std::string_view&) const;
  inline bool Remove(const std::string_view&);

  void Clear() { storage_.clear(); }
  void Swap(Dictionary* d) { storage_.swap(d->storage_); }
  void Reserve(std::size_t v) { UNUSE(v); }

  std::size_t GetSize() const { return storage_.size(); }
  bool IsEmpty() const { return storage_.empty(); }

  std::vector<std::string> GetKeyList() const;

 public:
  using iterator = Storage::iterator;
  using const_iterator = Storage::const_iterator;

  iterator begin() { return storage_.begin(); }
  iterator end() { return storage_.end(); }
  const_iterator begin() const { return storage_.begin(); }
  const_iterator end() const { return storage_.end(); }

 public:
  std::string Print() const;
  inline std::string ToJSON(ValueToJSON opt = ValueToJSON::kCompact) const;
  void ToJSON(std::ostream&, ValueToJSON opt = ValueToJSON::kCompact) const;

  void Extend(const Dictionary& d) { storage_.insert(d.begin(), d.end()); }
  void Extend(Dictionary&& d) { storage_.merge(d.storage_); }

 public:
  std::shared_ptr<Dictionary> Clone() const;
  void Clone(Dictionary*) const;

 private:
  Storage storage_;
};

// Helper function to break the circular dependency of Error and Value
inline Value ErrorToValue(const Error&);

namespace _ {

template <typename T, bool IsConst>
struct ValueGetResult {
  using ValueType = T;
};

// Only specialize for std::shared_ptr type for now
template <typename U>
struct ValueGetResult<std::shared_ptr<U>, true> {
  using ValueType = std::shared_ptr<const U>;
};

template <typename T, bool Const>
using ValueGetReturn =
    std::optional<typename ValueGetResult<T, Const>::ValueType>;

}  // namespace _

// Magic Get, which convers a |Value| to any type that is feasible. Notes, this
// function will do copy.
template <typename T>
_::ValueGetReturn<T, true> Get(const Value&);
template <typename T>
_::ValueGetReturn<T, false> Get(Value*);

template <typename T>
std::optional<Value> ToValue(T&&);

inline std::ostream& operator<<(std::ostream&, const Value&);
inline std::ostream& operator<<(std::ostream&, const List&);
inline std::ostream& operator<<(std::ostream&, const Dictionary&);

}  // namespace base

#include "value-inl.h"

#endif  // _BASE_VALUE_H_
