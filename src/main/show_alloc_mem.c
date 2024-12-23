#include "../index.h"
#include <unistd.h>

void print_bytes(size_t bytes) {
	print_number(bytes);
	print_string(bytes - 1 ? " bytes" : " byte");
}

void print_block(size_t size, char *data) {
	print_hex((size_t)data);
	print_string(" - ");
	print_hex((size_t)(data + size));
	print_string(" : ");
	print_bytes(size);
	print_string("\n");
}

void recursive_print_zones(zone_t *zone, blocks_t *tracker, size_t *size) {
	if (tracker) {
		recursive_print_zones(zone, tracker->left, size);
		*size += tracker->size;
		print_block(tracker->size, &zone->blocks[tracker->offset]);
		recursive_print_zones(zone, tracker->right, size);
	}
}

size_t print_alloc(meta_t *alloc) {
	size_t ret = 0;
	print_string(alloc->type == TINY_ALLOC ? "TINY : " : "SMALL : ");
	print_hex((size_t)alloc);
	print_string("\n");
	recursive_print_zones((zone_t*)alloc, alloc->used_blocks_tree, &ret);
	return ret;
}

size_t print_large_alloc(meta_t *alloc) {
	large_alloc_t *zone = (large_alloc_t*)alloc;
	print_string("LARGE : ");
	print_hex((size_t)alloc);
	print_string("\n");

	print_hex((size_t)zone->ptr);
	print_string(" - ");
	print_hex((size_t)(zone->ptr + zone->size));
	print_string(" : ");
	print_bytes(zone->size);
	print_string("\n");
	return zone->size;
}


void recursive_print(meta_t *alloc, size_t *total) {
	if (alloc) {
		recursive_print(alloc->left, total);
		switch (alloc->type) {
			case LARGE_ALLOC: {
				*total += print_large_alloc(alloc);
				break;
			}
			default: {
				*total += print_alloc(alloc);
				break;
			}
		}
		recursive_print(alloc->right, total);
	}
}

void show_alloc_mem() {
	size_t total = 0;
	recursive_print(global_tracker.alloc_tree, &total);
	print_string("Total : ");
	print_bytes(total);
	print_string("\n");
}