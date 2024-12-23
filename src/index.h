#ifndef __SRC_INDEX_H__
#define __SRC_INDEX_H__

#include <stddef.h>
#include "out.h"
#include "defs.h"
#include "types.h"

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
}				tiny_alloc_t __attribute__ ((aligned (16)));

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(tiny_alloc_t);


typedef struct	small_alloc_s {
	char			data[SMALL_ALLOC_SIZE];
}				small_alloc_t __attribute__ ((aligned (16)));

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(small_alloc_t);

// typedef struct	blocks_s {
// 	u32_t		offset;		// 4
// 	u16_t		size;		// 2
// 	u8_t		parent;		// 1
// 	union {
// 		u8_t			children[2];
// 		struct {
// 			u8_t		left;
// 			u8_t		right;
// 		};
// 	}; 						// 2
// 	u8_t		previous;	// 1
// 	u8_t		next;		// 1
// 	u8_t		color;		// 1
// }				blocks_t;

typedef struct	blocks_s {
	struct blocks_s		*parent;	// 8
	union {
		struct blocks_s		*children[2];
		struct {
			struct blocks_s	*left;
			struct blocks_s	*right;
		};
	}; 								// 16
	struct blocks_s		*previous;	// 8
	struct blocks_s		*next;		// 8
	u32_t				offset;		// 4
	#ifdef DEBUG
		int data;					// 4
	#endif
	u32_t				size;		// 4
	u8_t				color;		// 1
	u8_t				type;		// 1
}				blocks_t;

#define SIZEOF_BLOCKS_T sizeof(blocks_t)

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
			// large allocation size, zero if free, non-zero otherwise
			size_t			size;
			struct {
				// tree root tracking the used zones in non-large allocation
				blocks_t	*used_blocks_tree;
				// free zones tree root
				blocks_t	*free_blocks_tree;
				// pointer to biggest free zone
				blocks_t	*free_max_size;
				// number of used zones
				u8_t	used_count;
				// number of free zones
				u8_t	free_count;
			};
		};
		// pointer to next node with free blocks if current node is a small or tiny alloc
		struct meta_s *next;
		struct meta_s *previous;
	};
	union {
		// large alloc
		char		ptr[0]; 
		// offsets
		blocks_t	trackers[0];
	} __attribute__ ((aligned (16)));
}			meta_t;

typedef struct	blocks_zone_s {
	meta_t			meta;
	blocks_t		trackers[MAX_OFFSETS_IN_ZONE] __attribute__ ((aligned (16)));
	char			blocks[0] __attribute__ ((aligned (16)));
}				blocks_zone_t;

typedef blocks_zone_t zone_t;

typedef struct	small_zone_s {
	meta_t			meta;
	blocks_t		trackers[MAX_OFFSETS_IN_ZONE] __attribute__ ((aligned (16)));
	small_alloc_t	blocks[ALLOCS_IN_ZONE] __attribute__ ((aligned (16)));
}			small_zone_t;

#ifdef ____HOVER_OVER_ME
#undef ____HOVER_OVER_ME
#endif
#define ____HOVER_OVER_ME sizeof(small_zone_t);

typedef struct	tiny_zone_s {
	meta_t			meta;
	blocks_t		trackers[MAX_OFFSETS_IN_ZONE] __attribute__ ((aligned (16)));
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
	//			user allocation to find
	void		*ptr;
	// 			user allocation size
	size_t		alloc_size;
	// 			allocation tracker, NULL for large
	blocks_t	*tracker;
	// 			pointer to allocation zone
	meta_t		*meta;
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
	// minor page faults
	size_t minflt;
}				g_data_t;

/* ============================ GLOBALS ============================ */

extern g_data_t global_tracker;


int get_index_first_free_block(unsigned char *bf);
void set_block_availability(unsigned char *bf, int index, free_block_t unavailable);
free_block_t get_block_availability(unsigned char *bf, int index);
bool_t zone_is_full(meta_t *meta);
bool_t zone_is_empty(meta_t *meta);
void ft_memset(void *ptr, int c, size_t len);
char *output_number_base(size_t num, int base, char *output);
void print_number_base(size_t num, int base);
void print_hex(size_t num);
void print_string(char *str);
void print_number(size_t num);
int critical_assert();
blocks_t *get_first_free_tracking_block(meta_t *meta);
void *allocate_zone(meta_t *meta, u16_t size);
void remove_zone(block_t *block, meta_t *meta);
void print_minflt(char *file, int line);
void reallocate_zone(meta_t *meta, blocks_t *tracker, size_t size);

#endif