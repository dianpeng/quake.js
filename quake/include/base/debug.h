#ifndef _BASE_DEBUG_H_
#define _BASE_DEBUG_H_

#if defined(BASE_DEBUG)

#define BASE_DBG(...) __VA_ARGS__

#else

// When DEBUG flag is not defined, then BASE_DBG yield to nothing.
#define BASE_DBG(...)

#endif  // BASE_DEBUG

#endif  // _BASE_DEBUG_H_
