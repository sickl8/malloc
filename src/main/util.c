#define NODE_TYPE blocks_t
#include "../index.h"
#include "../binary_tree/index.h"
#include "../binary_tree/insert.h"
#include "../binary_tree/remove.h"
#include "../linked_list/insert_after.h"
#include "../linked_list/prepend.h"
#include "../linked_list/remove.h"
#include <unistd.h>

bool_t zone_is_full(meta_t *meta) {
	return meta->used_count >= ALLOCS_IN_ZONE || !meta->free_max_size || meta->free_max_size->size < (meta->type == SMALL_ALLOC ? SMALL_ALLOC_SIZE : TINY_ALLOC_SIZE);
}

bool_t zone_is_empty(meta_t *meta) {
	return meta->used_count == 0;
}

void ft_memset(void *ptr, int c, size_t len) {
	unsigned char *p = ptr;
	for (size_t i = 0; i < len; i++) {
		p[i] = (unsigned char)c;
	}
}

char *output_number_base(size_t num, int base, char *output) {
	char chars[] = "0123456789ABCDEF";
	size_t a = 1;
	while (a <= num / base) {
		a *= base;
	}
	for (int i = 0; a; i++) {
		int char_index = num / a;
		output[i] = chars[char_index];
		num %= a;
		a /= base;
	}
	return output;
}

size_t ft_strlen(char *str) {
	size_t ret = 0;
	while (str[ret]) {
		ret++;
	}
	return ret;
}

void print_number_base(size_t num, int base) {
	char str[64 + 1];
	ft_memset(str, 0, sizeof(str));
	output_number_base(num, base, str);
	write(1, str, ft_strlen(str));
}

void print_hex(size_t num) {
	char str[16 + 2 + 1] = "0x";
	ft_memset(str + 2, 0, sizeof(str) - 2);
	output_number_base(num, 16, str + 2);
	write(1, str, ft_strlen(str));
}

void print_number(size_t num) {
	char str[21 + 1];
	ft_memset(str, 0, sizeof(str));
	output_number_base(num, 10, str);
	write(1, str, ft_strlen(str));
}

void print_string(char *str) {
	write(1, str, ft_strlen(str));
}

blocks_t	*get_first_free_tracking_block(meta_t *meta) {
	for (int i = 0; i < (int)MAX_OFFSETS_IN_ZONE; i++) {
	// for (int i = (int)MAX_OFFSETS_IN_ZONE - 1; i > -1; i++) {
		if (!meta->trackers[i].size) {
			return &meta->trackers[i];
		}
	}
	return NULL;
}

blocks_t	sixteen_align_free_zone(blocks_t *tracker) {
	blocks_t ret = *tracker;
	u8_t difference = (16 - ret.offset % 16) % 16;
	ret.offset += difference;
	ret.size -= difference;
	return ret;
}

size_t	tracker_sixteen_aligned_size(blocks_t *tracker) {
	return tracker->size - ((16 - tracker->offset % 16) % 16);
}

static inline int	used_blocks_cmp(blocks_t *a, blocks_t *b) {
	return a->offset < b->offset;
}

static inline int	free_blocks_cmp(blocks_t *a, blocks_t *b) {
	return a->size < b->size;
}

void set_free_max_size(meta_t *meta, blocks_t *fz, bool_t added) {
	if (added) {
		if (!meta->free_max_size || meta->free_max_size->size < fz->size) {
			meta->free_max_size = fz;
		}
	} else {
		if (meta->free_max_size == fz) {
			if (!meta->free_max_size->left) {
				meta->free_max_size = meta->free_max_size->parent;
			} else {
				meta->free_max_size = leftmost_node(meta->free_max_size);
			}
		}
	}
}

void	reallocate_zone(meta_t *meta, blocks_t *tracker, size_t size) {
	blocks_t *next = tracker->next;
	if (next && next->type == FREE_BLOCK) {
		tree_remove_node(&meta->free_blocks_tree, next);
		next->size += tracker->size - size;
		tree_insert_node(&meta->free_blocks_tree, next, free_blocks_cmp);
		set_free_max_size(meta, next, true);
	} else {
		blocks_t *new_free = get_first_free_tracking_block(meta);
		new_free->size = tracker->size - size;
		new_free->offset = tracker->offset + size;
		list_insert_node_after(tracker, new_free);
		set_free_max_size(meta, new_free, true);
		tree_insert_node(&meta->free_blocks_tree, new_free, free_blocks_cmp);
		meta->free_count++;
	}
	// defer
	tracker->size = size;
}

void	*allocate_zone(meta_t *meta, u16_t size) {
	blocks_t *node = meta->free_blocks_tree;
	while (node) {
		size_t aligned_size = tracker_sixteen_aligned_size(node);
		if (aligned_size >= size) {
			break;
		}
		node = node->right;
	}
	#ifdef DEBUG
		assert(node);
	#endif
	set_free_max_size(meta, node, false);
	meta->free_count--; tree_remove_node(&meta->free_blocks_tree, node);
	blocks_t aligned = sixteen_align_free_zone(node);
	blocks_t *used = get_first_free_tracking_block(meta);

	used->type = USED_BLOCK;
	used->offset = aligned.offset;
	used->size = size;
	list_insert_node_after(node, used);
	meta->used_count++; tree_insert_node(&meta->used_blocks_tree, used, used_blocks_cmp);
	if (aligned.size > size) {
		blocks_t *new_free = get_first_free_tracking_block(meta);
		new_free->type = FREE_BLOCK;
		new_free->offset = aligned.offset + size;
		new_free->size = node->size - new_free->offset;
		list_insert_node_after(used, new_free);
		meta->free_count++; tree_insert_node(&meta->free_blocks_tree, new_free, free_blocks_cmp); set_free_max_size(meta, new_free, true);
	}

	if (aligned.offset != node->offset) {
		node->size = aligned.offset - node->offset;
		meta->free_count++; tree_insert_node(&meta->free_blocks_tree, node, free_blocks_cmp); set_free_max_size(meta, node, true);
	} else {
		node->size = 0; // marking the tracking block as free by setting the size to 0
		list_remove_node(&node->previous, node);
	}
	return &((zone_t*)meta)->blocks[used->offset];
}

void remove_zone(block_t *block, meta_t *meta) {
	blocks_t *tracker = block->tracker;
	blocks_t *next = tracker->next;
	blocks_t *previous = tracker->previous;
	blocks_t _root;
	blocks_t *root = &_root;

	root->next = next;
	root->previous = previous;

	u32_t new_size = tracker->size;
	u32_t new_offset = tracker->offset;

	tracker->size = 0;
	tree_remove_node(&meta->used_blocks_tree, tracker);
	list_remove_node(&tracker->previous, tracker);
	meta->used_count--; 
	
	if (next && next->type == FREE_BLOCK) {
		new_size += next->size;
		root->next = next->next;

		next->size = 0;
		set_free_max_size(meta, next, false);
		tree_remove_node(&meta->free_blocks_tree, next);
		list_remove_node(&tracker->next, next);
		meta->free_count--;
	}
	if (previous && previous->type == FREE_BLOCK) {
		new_size += previous->size;
		new_offset = previous->offset;
		root->next = previous->previous;

		previous->size = 0;
		set_free_max_size(meta, next, false);
		tree_remove_node(&meta->free_blocks_tree, previous);
		list_remove_node(&tracker->previous, previous);
		meta->free_count--;
	}

	blocks_t *new_free = get_first_free_tracking_block(meta);

	new_free->offset = new_offset;
	new_free->size = new_size;

	set_free_max_size(meta, new_free, true);
	tree_insert_node(&meta->free_blocks_tree, new_free, free_blocks_cmp);
	if (root->previous) {
		list_insert_node_after(root->previous, new_free);
	} else {
		list_prepend_node(&root->next, new_free);
	}
	meta->free_count++;
}
