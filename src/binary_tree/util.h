#ifndef __BINARY_TREE_UTIL_H__
#define __BINARY_TREE_UTIL_H__

#include "index.h"
#include "../index.h"
#include "../proto.h"

#define pv(x) { print_string(#x " = "); print_number(x); print_string("\n"); }

static blocks_t *find_tracker(zone_t *zone, blocks_t **root, void *alloc) {
	blocks_t **tracer = root;
	while (*tracer) {
		if (&zone->blocks[(*tracer)->offset] == alloc) {
			return *tracer;
		}
		if ((char*)alloc < &zone->blocks[(*tracer)->offset]) {
			tracer = &(*tracer)->left;
		} else {
			tracer = &(*tracer)->right;
		}
	}
	return NULL;
}

static block_t find_alloc(meta_t **root, void *alloc) {
	block_t ret;

	ft_memset(&ret, 0, sizeof(ret));
	if (!root || !alloc) {
		return (ret);
	}
	meta_t **tracer = root;
	meta_t *tmeta = NULL;
	char *ptr = (char*)alloc;
	while (tmeta = *tracer) {
		size_t num = (size_t[]){
			ALLOCS_IN_ZONE, // TINY_ALLOC
			ALLOCS_IN_ZONE, // SMALL_ALLOC
			1, // LARGE_ALLOC
		}[tmeta->type];
		size_t size_of_each = (size_t[]){
			sizeof(tiny_alloc_t), // TINY_ALLOC
			sizeof(small_alloc_t), // SMALL_ALLOC
			tmeta->size // LARGE_ALLOC
		}[tmeta->type];
		size_t total_size = (tmeta->type == LARGE_ALLOC ? sizeof(header_t) : sizeof(zone_t)) + (size_of_each * num);
		if (ptr < (char*)(*tracer)) {
			tracer = &(*tracer)->left;
		} else if (ptr > ((char*)(*tracer) + tmeta->real_size)) {
			tracer = &(*tracer)->right;
		} else { // (*tracer <= alloc && alloc <= (*tracer + total_size))
			// within range
			if (tmeta->type == LARGE_ALLOC) {
				if (ptr == tmeta->ptr) {
					ret.ptr = ptr;
					ret.alloc_size = tmeta->size;
					ret.meta = tmeta;
				}
			} else {
				blocks_t *tracker = NULL;
				if (tracker = find_tracker((zone_t*)tmeta, &tmeta->used_blocks_tree, alloc)) {
					ret.ptr = ptr;
					ret.alloc_size = tracker->size;
					ret.meta = tmeta;
					ret.tracker = tracker;
				}
			}
			return (ret);
		}
	}
	return (ret);
}

#endif