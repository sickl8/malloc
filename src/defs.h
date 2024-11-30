#ifndef __SRC_DEF_H__
#define __SRC_DEF_H__

#include "types.h"

#define MIN_ALLOCS_IN_ZONE				(128UL)

#define TINY_BLOCK_SIZE					((1UL << 8UL) - sizeof(tiny_size_t)) // 2 ** 8 - s = 256 - s
#define SMALL_BLOCK_SIZE				((1UL << 13UL) - sizeof(small_size_t)) // 2 ** 13 - s = 1024 * 8 = 8k - s

#endif