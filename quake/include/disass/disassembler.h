#ifndef _QUAKE_DISASS_DISASSEMBLER_H_
#define _QUAKE_DISASS_DISASSEMBLER_H_
#include "base.h"

namespace quake {
namespace disass {

class Disassembler {
 public:
  virtual bool HasNext() const = 0;
  virtual std::optional<std::string> Print() const = 0;
  virtual std::size_t Cursor() const = 0;
  virtual void Next() = 0;
  virtual ~Disassembler() {}
};

std::unique_ptr<Disassembler> CreateDisassembler(const void*, std::size_t);

}  // namespace disass
}  // namespace quake

#endif  // _QUAKE_DISASS_DISASSEMBLER_H_
