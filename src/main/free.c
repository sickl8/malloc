#include "../index.h"
#include "../linked_list/index.h"
#include "../binary_tree/index.h"
#include <stddef.h>
#include <sys/mman.h>
#include <pthread.h>

int save_new_free_zone(meta_t *ptr) {
	for (int i = 0; i < sizeof_array(global_tracker.totally_free_zone); i++) {
		if (!global_tracker.totally_free_zone[i]) {
			global_tracker.totally_free_zone[i] = ptr;
			return i;
		}
	}
	return -1;
}

void free(void *ptr) {
	if (!ptr) {
		return;
	}
	block_t block = find_alloc(&global_tracker.alloc_tree, ptr);
	if (block.ptr) { // found
		global_tracker.free_calls++;
		bool_t should_be_unmapped = false;
		bool_t remove_from_list = false;
		bool_t remove_from_tree = false;
		meta_t *meta = block.meta;
		switch (meta->type) {
			case LARGE_ALLOC: {
				meta->size = 0UL;
				remove_from_tree = true;
				if (save_new_free_zone(meta) == -1) { // already have 3 totally free zones, unmapping
					should_be_unmapped = true;
				}
				break;
			}
			default: { // TINY_ALLOC || SMALL_ALLOC
				if (zone_is_full(meta->bf_free_blocks)) { // zone will have a free block, add it to the appropriate linked list
					list_prepend_node((node_t**)&global_tracker.has_free_blocks[meta->type], meta);
				}
				set_block_availability(meta->bf_free_blocks, block.index, FREE_BLOCK);
				if (meta->type == TINY_ALLOC) {
					meta->tiny_blocks[block.index].size = 0UL;
				} else { // meta->type == SMALL_ALLOC
					meta->small_blocks[block.index].size = 0UL;
				}
				if (zone_is_empty(meta->bf_free_blocks)) {
					remove_from_tree = true;
					if (save_new_free_zone(meta) == -1) { // zone is empty and 3 free blocks are already available, unmap
						should_be_unmapped = true;
					} else { // if it's totally empty and in the totally_free_zones, it should be removed from the list
						remove_from_list = true;
					}
				}
				break;
			}
		}
		if (should_be_unmapped || remove_from_tree) {
			tree_remove_node((node_t**)&global_tracker.alloc_tree, meta);
		}
		if ((should_be_unmapped || remove_from_list) && meta->type != LARGE_ALLOC) {
			list_remove_node(&global_tracker.has_free_blocks[meta->type], meta);
		}
		if (should_be_unmapped) {
			global_tracker.munmap_calls++;
			munmap(meta, meta->real_size);
		}
	}
}
