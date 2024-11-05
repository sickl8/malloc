#ifndef __INDEX_H__
#define __INDEX_H__

#include <stddef.h>
#include "out.h"
#include "binary_tree/index.h"

/* ============================ #DEFINE ============================ */

#define MIN_ALLOCS_IN_ZONE				(128UL)

#define TINY_BLOCK_SIZE					((1UL << 8UL) - sizeof(char)) // 2 ** 8 - s = 256 - s
#define SMALL_BLOCK_SIZE				((1UL << 13UL) - sizeof(short)) // 2 ** 13 - s = 1024 * 8 = 8k - s
#define LARGE_BLOCK_SIZE(__alloc__)		(sizeof(large_alloc_t) + __alloc__)

/* ============================ TYPES ============================ */

typedef union	size_or_bf_u {
	// bit-field to mark each block either as free or used
	char	bf_free_blocks[MIN_ALLOCS_IN_ZONE / 8];
	// large allocation size
	size_t	size;
}				size_or_bf_t;

typedef size_or_bf_t sbf_t;

#define ____HOVER_OVER_ME sizeof(sbf_t);

typedef enum	alloc_type_e {
	TINY_ALLOC,
	SMALL_ALLOC,
	LARGE_ALLOC,
}				alloc_type_t;

typedef alloc_type_t type_t;

typedef struct	tiny_alloc_s {
	char			data[TINY_BLOCK_SIZE];
	char			size;
}				tiny_alloc_t __attribute__ ((aligned (16)));

#define ____HOVER_OVER_ME sizeof(tiny_alloc_t);

typedef struct	zone_meta_s {
	// red-black tree nodes for log2(n) lookups
	node_t	node;
	// node type, small, tiny, or large
	type_t	type;
	// union of size and bit-field describing free blocks
	sbf_t	meta;
}				zone_meta_t;

#define ____HOVER_OVER_ME sizeof(zone_meta_t);

typedef zone_meta_t meta_t;

typedef struct	tiny_zone_s {
	meta_t			meta;
	tiny_alloc_t	blocks[MIN_ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}				tiny_zone_t;

#define ____HOVER_OVER_ME sizeof(tiny_zone_t);

typedef struct	small_alloc_s {
	char			data[SMALL_BLOCK_SIZE];
	unsigned short	size;
}				small_alloc_t __attribute__ ((aligned (16)));

#define ____HOVER_OVER_ME sizeof(small_alloc_t);

typedef struct	small_zone_s {
	meta_t			meta;
	small_alloc_t	blocks[MIN_ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}				small_zone_t;

#define ____HOVER_OVER_ME sizeof(small_zone_t);

typedef struct	large_alloc_s {
	meta_t			meta;
	char			ptr[0];
}				large_alloc_t;

#define ____HOVER_OVER_ME sizeof(large_alloc_t);

typedef struct	g_data_s {
	size_t			total_allocated;
	tiny_zone_t		*tiny;
	small_zone_t	*small;
	large_alloc_t	*large;
}				g_data_t;

/* ============================ GLOBALS ============================ */

extern g_data_t global_tracker;

#endif