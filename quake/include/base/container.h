#ifndef _BASE_CONTAINER_H_
#define _BASE_CONTAINER_H_
#include <boost/container/small_vector.hpp>

#include "container/cached-list.h"
#include "container/intrusive-list.h"
#include "container/lru-map.h"
#include "container/ring-buffer.h"

namespace base {

using boost::container::small_vector;

}  // namespace base

#endif  // _BASE_CONTAINER_H_
