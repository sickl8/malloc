#ifndef __SRC_INDEX_H__
#define __SRC_INDEX_H__

#include <stddef.h>
#include "out.h"
#include "defs.h"
#include "types.h"
#include "binary_tree/index.h"

/* ============================ #DEFINE ============================ */

#define SIZEOF_LARGE_ALLOC(__size__)	(sizeof(header_t) + (__size__))

/* ============================ TYPES ============================ */

typedef node_t meta_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(meta_t);

typedef struct	tiny_alloc_s {
	char			data[TINY_BLOCK_SIZE];
	tiny_size_t		size;
}				tiny_alloc_t __attribute__ ((aligned (16)));

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(tiny_alloc_t);

typedef struct	tiny_zone_s {
	meta_t			meta;
	tiny_alloc_t	blocks[MIN_ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}				tiny_zone_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(tiny_zone_t);


typedef struct	small_alloc_s {
	char			data[SMALL_BLOCK_SIZE];
	small_size_t	size;
}				small_alloc_t __attribute__ ((aligned (16)));

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(small_alloc_t);

typedef struct	small_zone_s {
	meta_t			meta;
	small_alloc_t	blocks[MIN_ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}				small_zone_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(small_zone_t);

typedef struct	header_s {
	meta_t			meta;
	char			ptr[0] __attribute__ ((aligned (16)));
}				header_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(header_t)

typedef header_t large_alloc_t;

typedef struct	alloc_s {
	// user allocation to find
	void	*ptr;
	// user allocation size
	size_t	alloc_size;
	// real allocation size
	size_t	real_size;
	// pointer to allocation zone
	meta_t	*meta;
}				alloc_t;

alloc_t find_alloc(node_t **root, void *alloc);

typedef struct	g_data_s {
	// total allocated by user, without book-keeping metadata
	size_t			total_allocated;

	// linked list of tiny zones with at least one free block
	tiny_zone_t		*tiny_has_free_blocks;
	// linked list of small zones with at least one free block
	small_zone_t	*small_has_free_blocks;
	// empty mmap'ed zones that will not be munmmap'ed to save on calls to mmap
	meta_t			*totally_free_zone[3];

	// root of red black tree containing all zones
	meta_t			*alloc_tree;
}				g_data_t;

/* ============================ GLOBALS ============================ */

extern g_data_t global_tracker;

#endif