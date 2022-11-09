#ifndef _QUAKE_STRINGIFY_H_
#define _QUAKE_STRINGIFY_H_

#include "base.h"

// Stringify of HIR/LIR/CG part of the code. Used to help us to perform
// verification, visulization, serdes etc ...

namespace quake {

class Stringify {
 public:
  virtual ~Stringify() {}

  // Print a compact information out for the node (IR/LIR) etc ...
  virtual std::string Print() const = 0;
};

class GraphvizNode {
 public:
  // A graph unique name, used as identifier to allow link to link to it
  virtual std::string PrintGraphName() const = 0;

  // A graph node description, ie a single element inside of the graphviz
  // description
  virtual std::string PrintGraphNode() const = 0;

  // A graph link description.
  virtual std::string PrintGraphLink() const = 0;

  virtual ~GraphvizNode() {}

 public:
  // Helper to format a string into printtable label string
  static std::string EscapeLabelString(const std::string_view&);
};

class Graphviz {
 public:
  virtual std::string PrintGraphviz() const = 0;
  virtual ~Graphviz() {}
};

}  // namespace quake

#endif  // _QUAKE_STRINGIFY_H_
