#ifndef _BASE_LOCATION_H_
#define _BASE_LOCATION_H_

#include "common.h"

namespace base {

// Location provides basic info where of an object was constructed, or was
// significantly brought to life.
class Location {
 public:
  Location() = default;
  Location(const char* filename, int line_number)
      : filename_(filename), line_number_(line_number) {}

  bool HasInfo() const { return filename_ != nullptr; }
  operator bool() const { return HasInfo(); }

  const char* filename() const { return filename_; }
  int line_number() const { return line_number_; }

  inline std::string ToString() const;

  static Location CreateFromHere(const char* filename, int line_number) {
    return Location{filename, line_number};
  }

 private:
  const char* filename_ = nullptr;
  int line_number_ = -1;
};

inline std::string Location::ToString() const {
  return Fmt("@(%s:%d)", (filename_ ? filename_ : "N/A"), line_number_);
}

// Get the current location. til c++20, we have std::source_location, sigh :(
#define CURRENT() ::base::Location::CreateFromHere(__FILE__, __LINE__)

}  // namespace base

#endif  // _BASE_LOCATION_H_
