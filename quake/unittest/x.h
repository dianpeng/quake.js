#ifndef _UNITTEST_X_H_
#define _UNITTEST_X_H_

#include <base.h>
#include <string>
#include <filesystem>

namespace base {

std::string CONFIG_base_log = "";

}  // namespace base

namespace x {

inline std::string RootPath() {
  auto path = std::filesystem::current_path();
  std::vector<std::string> component_set;
  for (auto itr = path.begin(); itr != path.end(); ++itr) {
    component_set.push_back(*itr);
  }
  // reversly searching the |Sparrow|
  while (!component_set.empty()) {
    if (component_set.back() != "quake") {
      component_set.pop_back();
    } else {
      break;
    }
  }

  CHECK(!component_set.empty());

  std::filesystem::path output_path(component_set.front());
  for (std::size_t i = 0; i < component_set.size(); ++i) {
    output_path /= component_set[i];
  }

  return output_path.c_str();
}

inline std::string PathAt(const char* r = "") { return RootPath() + "/" + r; }

inline std::string TestPathAt(const char* r = "") {
  return RootPath() + "/unittest/" + r;
}

}  // namespace x

#endif  // _UNITTEST_X_H_
