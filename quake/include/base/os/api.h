#ifndef _BASE_OS_API_H_
#define _BASE_OS_API_H_
#include <base/common.h>

#include "basic.h"

namespace base {
namespace os {

// ------------------------------------------------------------------------
// current director related manipulation. Notes not all APIs are provided on
// all platform if we need to do porting

// modify root file director
Error Chroot(const char*);

// cd into another directory
Error Chdir(const char*);

// get the current PWD
Result<std::string> PWD();
std::string PWD(const char*);

// ------------------------------------------------------------------------
Result<int> SignalFromString(const std::string&);
Result<const char*> GetSignalName(int);

// Process ----------------------------------------------------------------
//  This may or may not work on Linux, since it needs to hack the cli buffer
std::int32_t GetPid();
std::size_t GetPageSize();
std::optional<std::string> GetHostname();
Error Kill(std::int32_t, int);

std::optional<std::uint32_t> GetHardwareConcurrency();

// Process wise cpu time
Result<std::uint64_t> ProcessGetCpuTime();

// IO/unistd related. Whatever fd can be used here
inline bool IsFdOkay(fd_t fd) { return fd != kInvalidFd; }

// unversal file system
void CloseFd(fd_t);
void CloseFdNoBS(fd_t);

// file link
enum LinkType { kSymbolic, kHard };

Error Link(const std::string&, std::string&, LinkType);
Error Unlink(const std::string&);
// Follow the symbolic link type, hard link is at fs level and actually existed
Result<std::string> FollowLink(const std::string&);

}  // namespace os
}  // namespace base

#include "api-inl.h"

#endif  // _BASE_OS_API_H_
