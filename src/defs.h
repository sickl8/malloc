#ifndef __SRC_DEF_H__
#define __SRC_DEF_H__

#include "types.h"

#define ALLOCS_IN_ZONE			(126UL)
#define __MAX_OFFSETS_IN_ZONE	(ALLOCS_IN_ZONE * 2 + 1)
#define MAX_OFFSETS_IN_ZONE		(__MAX_OFFSETS_IN_ZONE + 0)

#define TINY_ALLOC_SIZE			(1UL << 8UL) // 2 ** 8 = 256
#define SMALL_ALLOC_SIZE		(1UL << 12UL) // 2 ** 12 = 1024 * 4 = 4k

#define sizeof_array(__arr__)	(sizeof(__arr__) / sizeof(*(__arr__)))

#ifdef DEBUG
#define PRINT_MINFLT() print_minflt(__FILE__, __LINE__)
#else
#define PRINT_MINFLT()
#endif

#endif