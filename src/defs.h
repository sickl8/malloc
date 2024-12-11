#ifndef __SRC_DEF_H__
#define __SRC_DEF_H__

#include "types.h"

#define ALLOCS_IN_ZONE			(128UL)

#define TINY_ALLOC_SIZE			((1UL << 8UL) - sizeof(tiny_size_t)) // 2 ** 8 - s = 256 - s
#define SMALL_ALLOC_SIZE		((1UL << 10UL)) // 2 ** 13 - s = 1024 * 8 = 8k - s

#define sizeof_array(__arr__)	(sizeof(__arr__) / sizeof(*(__arr__)))

#endif