#include "../index.h"
#include "../binary_tree/index.h"
#include <stddef.h>
#include <sys/mman.h>

void *recreate(void *ptr, meta_t *meta, size_t size) {
	char *ret = malloc(size);
	char *old_ptr = meta->ptr;
	if (ret) {
		size_t i = 0;
		for (; i < meta->size / sizeof(size_t); i++) {
			((size_t*)ret)[i] = ((size_t*)old_ptr)[i];
		}
		i *= sizeof(size_t);
		for (; i < meta->size; i++) {
			ret[i] = old_ptr[i];
		}
	}
	free(ptr);
	return ret;
}

void *realloc(void *ptr, size_t size) {
	block_t f = find_alloc(&global_tracker.alloc_tree, ptr);
	if (f.ptr) {
		if (!size) {
			free(ptr);
			return NULL;
		}
		meta_t *meta = f.meta;
		if (meta->type == LARGE_ALLOC) {
			if (meta->real_size - sizeof(header_t) >= size) {
				meta->size = size;
				return ptr;
			} else {
				return recreate(ptr, meta, size);
			}
		} else {
			size_t block_size;
			if (meta->type == TINY_ALLOC) {
				block_size = sizeof(tiny_alloc_t);
			} else {
				block_size = sizeof(small_alloc_t);
			}
			if (size <= block_size) {
				meta->size = size;
				return ptr;
			} else {
				return recreate(ptr, meta, size);
			}
		}
	}
	return ptr;
}