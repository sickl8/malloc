#include <stddef.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <sys/mman.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../index.h"
#include "../linked_list/index.h"
#include "../binary_tree/index.h"
#include "../mmap/index.h"
#include <string.h>

#define pv(x) { print_string(#x " = "); print_number(x); print_string("\n"); }

#define get_set_block_data(index) \
	if (type == TINY_ALLOC) { \
		ret = &meta->tiny_blocks[index].data; \
		meta->tiny_blocks[index].size = size; \
	} else { \
		ret = &meta->small_blocks[index].data; \
		meta->small_blocks[index].size = size; \
	}

meta_t *get_free_zone(size_t *size) {
	meta_t *alloc = NULL;
	for (int i = 0; i < sizeof_array(global_tracker.totally_free_zone); i++) {
		if (global_tracker.totally_free_zone[i] != NULL && global_tracker.totally_free_zone[i]->real_size >= *size) {
			alloc = global_tracker.totally_free_zone[i];
			*size = alloc->real_size;
			global_tracker.totally_free_zone[i] = NULL;
			return alloc;
		}
	}
	return NULL;
}

static inline void *create_alloc(size_t size, alloc_type_t type) {
	void *ret = NULL;
	size_t minimum_zone_size = type == TINY_ALLOC ? sizeof(tiny_zone_t) : sizeof(small_zone_t);
	size_t real_zone_size = minimum_zone_size;
	meta_t *meta = NULL;
	int index = 0;
	if (global_tracker.has_free_blocks[type]) { // tiny zone available with one free block at least
		meta = global_tracker.has_free_blocks[type];
		if (zone_is_empty(meta->bf_free_blocks)) { // totally empty, will not be in the tree, readding it
			tree_insert_node((node_t**)&global_tracker.alloc_tree, (node_t*)meta);
		}
		index = get_index_first_free_block(meta->bf_free_blocks);
		set_block_availability(meta->bf_free_blocks, index, USED_BLOCK);
		get_set_block_data(index);
		if (zone_is_full(meta->bf_free_blocks)) {
			list_pop_node((node_t**)&global_tracker.has_free_blocks[type]);
		}
	} else { // new zone created or resurected
		meta = get_free_zone(&real_zone_size);
		if (!meta) {
			meta = cmap(&real_zone_size);
			if (meta == MAP_FAILED) {
				return NULL;
			}
		}
		list_prepend_node((node_t**)&global_tracker.has_free_blocks[type], meta);
		meta->real_size = real_zone_size;
		meta->type = type;
		set_block_availability(meta->bf_free_blocks, 0, USED_BLOCK);
		get_set_block_data(0);
		tree_insert_node((node_t**)&global_tracker.alloc_tree, (node_t*)meta);
	}
	return ret;
}

static inline void *create_large_alloc(size_t size) {
	void *ret = NULL;
	meta_t *alloc = NULL;
	size_t minimum_zone_size = SIZEOF_LARGE_ALLOC(size);
	size_t real_zone_size = minimum_zone_size;
	// check for integer overflow
	if (minimum_zone_size < size) {
		return NULL;
	}
	alloc = get_free_zone(&real_zone_size);
	if (!alloc) {
		// no availbe zone already allocated, we call mmap
		alloc = cmap(&real_zone_size);
		if (alloc == MAP_FAILED) {
			return NULL;
		}
	}
	alloc->real_size = real_zone_size;
	alloc->size = size;
	alloc->type = LARGE_ALLOC;
	tree_insert_node((node_t**)&global_tracker.alloc_tree, (node_t*)alloc);
	ret = alloc->ptr;
	return ret;
}

void *malloc(size_t size) {
	if	(!size) {
		return NULL;
	}
	void *ret = NULL;
	if (size > SMALL_ALLOC_SIZE) { // (size >= SMALL_ALLOC_SIZE + 1) large allocation
		ret = create_large_alloc(size);
	} else if (size > TINY_ALLOC_SIZE) { // (TINY_ALLOC_SIZE < size <= SMALL_ALLOC_SIZE) small allocation
		ret = create_alloc(size, SMALL_ALLOC);
	} else { // (0 < size <= TINY_ALLOC_SIZE) tiny allocation
		ret = create_alloc(size, TINY_ALLOC);
	}
	global_tracker.malloc_calls += !!ret;
	return ret;
}
