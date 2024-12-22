#define NODE_TYPE blocks_t
#include "../index.h"
#include "../binary_tree/index.h"
#include "../binary_tree/remove.h"
#include "../binary_tree/util.h"
#include "../linked_list/index.h"
#include <stddef.h>
#include <sys/mman.h>

void *recreate(void *ptr, block_t *f, size_t size) {
	char *ret = malloc(size);
	char *old_ptr = f->ptr;
	if (ret) {
		size_t i = 0;
		for (; i < f->alloc_size / sizeof(size_t); i++) {
			((size_t*)ret)[i] = ((size_t*)old_ptr)[i];
		}
		i *= sizeof(size_t);
		for (; i < f->alloc_size; i++) {
			ret[i] = old_ptr[i];
		}
	}
	free(ptr);
	return ret;
}

void *realloc(void *ptr, size_t size) {
	block_t f = find_alloc(&global_tracker.alloc_tree, ptr);
	if (f.ptr) {
		meta_t *meta = f.meta;
		if (!size) {
			free(ptr);
			return NULL;
		} else if (f.alloc_size >= size) {
			if (meta->type == LARGE_ALLOC) {
				meta->size = size;
				return ptr;
			}
			blocks_t *tracker = f.tracker;
			blocks_t *next = tracker->next;
			if (meta->used_count < ALLOCS_IN_ZONE - 2) {

			}
		}
		if (meta->type == LARGE_ALLOC) {
			if (meta->real_size - sizeof(header_t) >= size) {
				meta->size = size;
				return ptr;
			} else {
				return recreate(ptr, &f, size);
			}
		} else {
			blocks_t *tracker = f.tracker;
			blocks_t *next = tracker->next;
			if (((meta->type == TINY_ALLOC && size <= TINY_ALLOC_SIZE) ||
				(meta->type == SMALL_ALLOC && size <= SMALL_ALLOC_SIZE)) &&
				(next && next->type == FREE_BLOCK && tracker->size + next->size >= size))
			{
				next->size -= size - tracker->size;
				tracker->size = size;
				if (!next->size) {
					meta->free_blocks_tree--;
					list_remove_node(&tracker, next);
					tree_remove_node(&meta->free_blocks_tree, next);
				}
				return ptr;
			} else {
				return recreate(ptr, &f, size);
			}
		}
	}
	return ptr;
}