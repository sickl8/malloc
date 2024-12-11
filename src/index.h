#ifndef __SRC_INDEX_H__
#define __SRC_INDEX_H__

#include <stddef.h>
#include "out.h"
#include "defs.h"
#include "types.h"
#include "proto.h"

/* ============================ #DEFINE ============================ */

#define SIZEOF_LARGE_ALLOC(__size__)	(sizeof(header_t) + (__size__))

/* ============================ TYPES ============================ */

typedef enum	node_color_e {
	RED,
	BLACK,
}				node_color_t;

typedef enum	node_dir_e {
	LEFT	= 0,
	RIGHT	= 1,
}				node_dir_t;

typedef enum	bool_e {
	false,
	true,
}				bool_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(meta_t);

typedef struct	tiny_alloc_s {
	char			data[TINY_ALLOC_SIZE];
	tiny_size_t		size;
}				tiny_alloc_t __attribute__ ((aligned (16)));

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(tiny_alloc_t);


typedef struct	small_alloc_s {
	char			data[SMALL_ALLOC_SIZE];
	small_size_t	size;
}				small_alloc_t __attribute__ ((aligned (16)));

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(small_alloc_t);

typedef struct	meta_s {
	union {
		struct meta_s *children[2];
		struct {
			struct meta_s	*left;
			struct meta_s	*right;
		};
	};
	struct meta_s		*parent;
	node_color_t	color;
	// data
	struct {
		#ifdef DEBUG
			int data;
		#endif
		// real allocation size returned by align_to_page_size or equivalent
		size_t	real_size;
		// node type, small, tiny, or large
		type_t	type;
		// union of size and bit-field describing free blocks
		union {

			// bit-field to mark each block either as free or used
			unsigned char	bf_free_blocks[(ALLOCS_IN_ZONE / 8)];

			// large allocation size, zero if free, non-zero otherwise
			size_t			size;
		};
		// pointer to next node with free blocks if current node is a small or tiny alloc
		struct meta_s *next;
		struct meta_s *previous;
	};
	union {
		// large alloc
		char ptr[0];
		// tiny zone blocks
		tiny_alloc_t tiny_blocks[0];
		// small zone blocks
		small_alloc_t small_blocks[0];
	} __attribute__ ((aligned (16)));
}			meta_t;

typedef meta_t node_t;

typedef struct	small_zone_s {
	meta_t			meta;
	small_alloc_t	blocks[ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}			small_zone_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(small_zone_t);

typedef struct	tiny_zone_s {
	meta_t			meta;
	tiny_alloc_t	blocks[ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}			tiny_zone_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(tiny_zone_t);

typedef meta_t header_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(header_t)

typedef header_t large_alloc_t;

typedef struct	block_s {
	// user allocation to find
	void	*ptr;
	// user allocation size
	size_t	alloc_size;
	// real allocation size
	size_t	real_size;
	// allocation index, zero for large
	int		index;
	// pointer to allocation zone
	meta_t	*meta;
}				block_t;

typedef struct	g_data_s {
	union {
		struct {
			// linked list of tiny zones with at least one free block
			tiny_zone_t		*tiny_has_free_blocks;
			// linked list of small zones with at least one free block
			small_zone_t	*small_has_free_blocks;
		};
		meta_t *has_free_blocks[2];
	};

	// empty mmap'ed zones that will not be munmmap'ed to save on calls to mmap
	meta_t			*totally_free_zone[3];

	// root of red black tree containing all zones
	meta_t			*alloc_tree;

	// mmap calls
	size_t mmap_calls;
	// munmap calls
	size_t munmap_calls;
	// malloc calls
	size_t malloc_calls;
	// realloc calls
	size_t realloc_calls;
	// free calls
	size_t free_calls;
	// total real size allocated
	size_t total_real_size;
}				g_data_t;

/* ============================ GLOBALS ============================ */

extern g_data_t global_tracker;

#endif