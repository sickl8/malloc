#include "index.h"
#include "../index.h"

#define pv(x) { print_string(#x " = "); print_number(x); print_string("\n"); }

block_t find_alloc(node_t **root, void *alloc) {
	block_t ret;

	ret.meta = NULL;
	ret.alloc_size = 0;
	ret.real_size = 0;
	ret.index = 0;
	ret.ptr = NULL;
	if (!root || !alloc) {
		return (ret);
	}
	node_t **tracer = root;
	meta_t *tmeta = NULL;
	char *ptr = alloc;
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
		size_t total_size = sizeof(header_t) + size_of_each * num;
		if (ptr < (char*)(*tracer)) {
			tracer = &(*tracer)->left;
		} else if (ptr > ((char*)(*tracer) + total_size)) {
			tracer = &(*tracer)->right;
		} else { // (*tracer <= alloc && alloc <= (*tracer + total_size))
			// within range
			char *tptr = ((large_alloc_t*)tmeta)->ptr;
			if (ptr >= tptr && ptr <= (tptr + size_of_each * (num - 1)) && ((ptr - tptr) % size_of_each) == 0) {
				size_t real_size = (size_t[]){
					TINY_ALLOC_SIZE, // TINY_ALLOC
					SMALL_ALLOC_SIZE, // SMALL_ALLOC
					tmeta->real_size - sizeof(header_t) // LARGE_ALLOC
				}[tmeta->type];
				if (tmeta->type != LARGE_ALLOC && get_block_availability(tmeta->bf_free_blocks, (ptr - tptr) / size_of_each) == FREE_BLOCK) {
					return (ret);
				}
				ret.ptr = ptr;
				ret.index = (ptr - tptr) / size_of_each;
				ret.real_size = real_size;
				(tmeta->type);
				ret.alloc_size = tmeta->type == LARGE_ALLOC ? tmeta->size : (tmeta->type == SMALL_ALLOC ? ((small_alloc_t*)ptr)->size : ((tiny_alloc_t*)ptr)->size);
				ret.meta = tmeta;
				return (ret);
			}
			return (ret);
		}
	}
	return (ret);
}
