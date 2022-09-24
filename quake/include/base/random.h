#ifndef _BASE_RANDOM_H_
#define _BASE_RANDOM_H_
#include "common.h"

namespace base {

std::int64_t RandInt64(std::int64_t, std::int64_t);
std::uint64_t RandUInt64(std::uint64_t, std::uint64_t);

inline std::int64_t RandInt64();
inline std::uint64_t RandUInt64();
inline double RandDouble();

std::uint8_t RandByte();
void RandByte(std::uint8_t*, std::size_t);
void RandString(std::string*, std::size_t);
inline std::string RandString(std::size_t);

}  // namespace base

#include "random-inl.h"

#endif  // _BASE_RANDOM_H_
