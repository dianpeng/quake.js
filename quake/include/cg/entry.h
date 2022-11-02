#ifndef _QUAKE_CG_ENTRY_H_
#define _QUAKE_CG_ENTRY_H_
#include "base.h"

// ----------------------------------------------------------------------------
// Function entry to JITTed code

namespace quake {
namespace cg {

// Each JITTed code will have following calling convention for now
using JitCodeEntry = void (*)(QuakeRT*);

}  // namespace cg
}  // namespace quake

#endif  // _QUAKE_CG_ENTRY_H_
