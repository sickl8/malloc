#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "types.h"

#define MIN_ALLOCS_IN_PAGE	(256UL)

#define TINY_MAX_ALLOC		(1UL << 5UL)
#define SMALL_MAX_ALLOC		(1UL << 16UL)

#define TINY_ZONE_SIZE		(MIN_ALLOCS_IN_PAGE * TINY_MAX_ALLOC)
#define SMALL_ZONE_SIZE		(MIN_ALLOCS_IN_PAGE * SMALL_MAX_ALLOC)

alloc_data_t global_tracker;

int red_black_rotate_left(node_t **root, node_t *target) {
	/*
		*root == target
	*/
	node_t *lvl_1;
	node_t *lvl_2;

	if (!root || !(lvl_1 = target->left) || !(lvl_2 = lvl_1->right)) {
		return (1);
	}

	lvl_1->right = lvl_2->left;
	lvl_2->left = lvl_1;
	target->left = lvl_2;
	return (0);
}

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
