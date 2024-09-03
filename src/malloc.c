#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_ALLOCS_IN_PAGE	(100UL)

#define TINY_MAX_ALLOC		(1UL << 5UL)
#define SMALL_MAX_ALLOC		(1UL << 16UL)

#define TINY_ZONE_SIZE		(MIN_ALLOCS_IN_PAGE * TINY_MAX_ALLOC)
#define SMALL_ZONE_SIZE		(MIN_ALLOCS_IN_PAGE * SMALL_MAX_ALLOC)

enum node_color {
	RED_NODE,
	BLACK_NODE,
};

typedef struct	red_black_tree_node_s {
	enum node_color					color;
	void							*data;
	struct red_black_tree_node_s	*left;
	struct red_black_tree_node_s	*right;
	struct red_black_tree_node_s	*parent;
}				red_black_tree_node_t;

typedef red_black_tree_node_t node_t;

typedef struct	allocation_data_s {
	void						*address;
	size_t						size;
	struct allocation_data_s	*next;
	struct allocation_data_s	*prev;
}				allocation_data_t;

typedef struct	zone_s {
	size_t	size;
	size_t	max_alloc;
	node_t	*allocs;
}				zone_t;

typedef struct	alloc_data_s {
	size_t page_size;
	zone_t tiny;
	zone_t small;
}				alloc_data_t;

alloc_data_t global_tracker;

typedef struct	alloc_s {

}				alloc_t;

void init_zone(zone_t *zone, size_t size, size_t max_alloc) {
	zone->max_alloc = max_alloc;
	zone->size = size;
	size_t remainder = size % global_tracker.page_size;
	if (remainder != 0) {
		/* closest multiple of page_size */
		zone->size = size - remainder + global_tracker.page_size;
	} else {
		zone->size = size;
	}
}

int init_alloc_data() {

	/* return 1 */

	global_tracker.page_size = getpagesize();
	return 0;
}

void *_malloc(size_t size) {
	if	((!size) || (!global_tracker.page_size && !init_alloc_data())) {
		return NULL;
	}
	void *ret = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (ret == ((void*)-1)) {
		return NULL;
	}
	return ret;
}
