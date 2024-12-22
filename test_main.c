#ifndef __USE_MISC
#define __USE_MISC
#endif
#include "src/out.h"
#include "src/index.h"
#include "src/mmap/index.h"
#include "src/binary_tree/index.h"
#include "src/binary_tree/util.h"
#include "test.h"
#include <unistd.h>

void test_show_alloc_mem() {
	int alloc[] = { 42, 84, 3725, 48847 };
	for (int i = 0; i < sizeof_array(alloc); i++) {
		void *ptr = malloc(alloc[i]);
	}
	show_alloc_mem();
	pv(global_tracker.mmap_calls);
	pv(global_tracker.malloc_calls);
}

void test_malloc_tiny() {
	int itr = 1024 * 16;
	for (int i = 0; i < itr; i++) {
		int rd = rand();
		int r = ((double)rd / (double)RAND_MAX) * TINY_ALLOC_SIZE;
		r += !r;
		void *ptr = malloc(r);
		block_t f = find_alloc(&global_tracker.alloc_tree, ptr);
		assert(!r || f.ptr);
		assert(f.ptr == ptr);
		assert(f.alloc_size == r);
		type_t type;
		if (r > SMALL_ALLOC_SIZE) { // large alloc
			type = LARGE_ALLOC;
		} else if (r > TINY_ALLOC_SIZE) { // small alloc
			type = SMALL_ALLOC;
		} else { // tiny alloc
			type = TINY_ALLOC;
		}
		assert(!f.ptr || f.meta->type == type);
		free(f.ptr);
	}
	// show_alloc_mem();
	pv(global_tracker.mmap_calls);
	pv(global_tracker.munmap_calls);
	pv(global_tracker.malloc_calls);
	pv(global_tracker.free_calls);
}

void test_malloc_2() {
	int itr = 1024 * 16 * 1 + 10 * 0;
	pv(seed);
	for (int i = 0; i < itr; i++) {
		int rd = rand();
		int r = ((double)rd / (double)RAND_MAX) * (SMALL_ALLOC_SIZE * 2);
		r += !r;
		void *ptr = malloc(r);
		block_t f = find_alloc(&global_tracker.alloc_tree, ptr);
		assert(!r || f.ptr);
		assert(f.ptr == ptr);
		assert(f.alloc_size == r);
		type_t type;
		if (r > SMALL_ALLOC_SIZE) { // large alloc
			type = LARGE_ALLOC;
		} else if (r > TINY_ALLOC_SIZE) { // small alloc
			type = SMALL_ALLOC;
		} else { // tiny alloc
			type = TINY_ALLOC;
		}
		assert(!f.ptr || f.meta->type == type);
		free(ptr);
		f = find_alloc(&global_tracker.alloc_tree, ptr);
		assert(!f.ptr);
	}
	show_alloc_mem();
	pv(global_tracker.mmap_calls);
	pv(global_tracker.munmap_calls);
	pv(global_tracker.malloc_calls);
	pv(global_tracker.free_calls);
}


void recursive_free(zone_t *zone, blocks_t *tracker) {
	if (tracker) {
		recursive_free(zone, tracker->left);
		recursive_free(zone, tracker->right);
		free(&zone->blocks[tracker->offset]);
	}
}

void test_malloc_speed() {
	int itr = 10000000;
	pv(global_tracker.malloc_calls);
	pv(global_tracker.free_calls);
	for (int i = 0; i < itr; i++) {
		if (i % 1000 == 0) {
			// printf("i = %d\n", i);
			pv(i);
		}
		void *ptr = malloc(SMALL_ALLOC_SIZE);
	}
	for (int i = 0; i < itr; i++) {
		if (i % 1000 == 0) {
			int d = i;
			// printf("d = %d\n", i);
			pv(d);
		}
		meta_t *meta = global_tracker.alloc_tree;
		if (!meta) {
			break;
		}
		if (meta->type == LARGE_ALLOC) {
			free(meta->ptr);
		} else {
			recursive_free((zone_t*)meta, meta->used_blocks_tree);
		}
	}
	pv(global_tracker.malloc_calls);
	pv(global_tracker.free_calls);
}

int main(int ac, char **av) {
	if (av[1]) {
		seed = strtol(av[1], NULL, 10);
	} else {
		seed = get_seed();
	}
	srand(seed);
	pv(seed);
	// run_test(test_malloc_speed);
	// run_test(test_malloc_tiny);
	// run_test(test_malloc_2);
	run_test(test_show_alloc_mem);
	pv(seed);
}
