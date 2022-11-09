#include "stringify.h"

namespace quake {

std::string GraphvizNode::EscapeLabelString(const std::string_view& label) {
  std::string out;
  out.reserve(label.size());

  for (auto c : label) {
    switch (c) {
      case '\n':
        out.push_back('\\');
        out.push_back('n');
        break;

      case '\v':
        out.push_back('\\');
        out.push_back('v');
        break;

      case '\b':
        out.push_back('\\');
        out.push_back('b');
        break;

      case '\t':
        out.push_back('\\');
        out.push_back('t');
        break;

      case '\r':
        out.push_back('\\');
        out.push_back('r');
        break;

      case '\\':
        out.push_back('\\');
        out.push_back('\\');
        break;

      case '"':
        out.push_back('\\');
        out.push_back('"');
        break;

      default:
        out.push_back(c);
        break;
    }
  }

  return out;
}

}  // namespace quake
