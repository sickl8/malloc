#include "../index.h"
#include "../binary_tree/index.h"
#include <unistd.h>

#define pv(x) { print_string(#x " = "); print_number(x); print_string("\n"); }

int get_index_first_free_block(unsigned char *bf) {
	int index = 0;
	index += 8 * !(u64_t)(~((u64_t*)(bf + index))[0]);
	index += 4 * !(u32_t)(~((u32_t*)(bf + index))[0]);
	index += 2 * !(u16_t)(~((u16_t*)(bf + index))[0]);
	index += 1 * !(u8_t)(~((u8_t*)(bf + index))[0]);
	u8_t sbf = (u8_t)~bf[index];
	int bit_index = 0;
	bit_index += 4 * !((u8_t)(sbf << bit_index) & 0b11110000U);
	bit_index += 2 * !((u8_t)(sbf << bit_index) & 0b11000000U);
	bit_index += 1 * !((u8_t)(sbf << bit_index) & 0b10000000U);
	return index * 8 + bit_index;
}

void set_block_availability(unsigned char *bf, int index, free_block_t unavailable) {
	int bf_index = index / 8;
	int bf_bit_index = index % 8;
	if (!unavailable) {
		bf[bf_index] = (u8_t)(bf[bf_index] ^ (u8_t)(1U << (7 - bf_bit_index)));
	} else {
		bf[bf_index] = (u8_t)(bf[bf_index] | (u8_t)(1U << (7 - bf_bit_index)));
	}
}

free_block_t get_block_availability(unsigned char *bf, int index) {
	int bf_index = index / 8;
	int bf_bit_index = index % 8;
	return (bf[bf_index] & (1 << (7 - bf_bit_index))) >> (7 - bf_bit_index);
}

int zone_is_full(unsigned char *bf) {
	size_t *st = (size_t*)bf;
	return !((u64_t)~st[0]) && !((u64_t)~st[1]);
}

int zone_is_empty(unsigned char *bf) {
	size_t *st = (size_t*)bf;
	return !st[0] && !st[1];
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

bool_t tree_contains_node(meta_t *node) {
	meta_t *root = global_tracker.alloc_tree;
	while (root) {
		if (root == node) {
			return true;
		}
		if (node < root) {
			root = root->left;
		} else {
			root = root->right;
		}
	}
	return false;
}

bool_t list_contains_node(meta_t *node) {
	if (node->type == LARGE_ALLOC) return false;
	meta_t *ptr = global_tracker.has_free_blocks[node->type];
	while (ptr) {
		if (ptr == node) {
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

typedef enum	zone_is_in_e {
	ARRAY = 1,
	LIST = 2,
	TREE = 4,
}				places_t;

void violation(meta_t *ptr, int bf_is_in, int bf_should_be_in, int bf_should_NOT_be_in) {
	print_string("-------------- FATAL ERROR --------------\n");
	print_string("ZONE: ");
	print_hex((size_t)ptr);
	print_string(" TYPE: ");
	print_number(ptr->type);
	print_string("\n");
	#define IS_IN(thing, in) if (thing & in) { print_string(#in " "); }
	print_string("ZONE IS IN: ");
	IS_IN(bf_is_in, ARRAY);
	IS_IN(bf_is_in, LIST);
	IS_IN(bf_is_in, TREE);
	print_string("\nZONE SHOULD BE IN: ");
	IS_IN(bf_should_be_in, ARRAY);
	IS_IN(bf_should_be_in, LIST);
	IS_IN(bf_should_be_in, TREE);
	print_string("\nZONE SHOULD NOT BE IN: ");
	IS_IN(bf_should_NOT_be_in, ARRAY);
	IS_IN(bf_should_NOT_be_in, LIST);
	IS_IN(bf_should_NOT_be_in, TREE);
	print_string("\n");
}

int critical_assert() {
	for (int i = 0; i < sizeof_array(global_tracker.totally_free_zone); i++) {
		meta_t *ptr = global_tracker.totally_free_zone[i];
		if (ptr) {
			int should_NOT_be_in = 0;
			if (tree_contains_node(ptr)) {
				should_NOT_be_in |= TREE;
			}
			if (list_contains_node(ptr)) {
				should_NOT_be_in |= LIST;
			}
			if (should_NOT_be_in) {
				violation(ptr, ARRAY | should_NOT_be_in, ARRAY, should_NOT_be_in);
				return (1);
			}
		}
	}
	for (int type = 0; type < 2; type++) {
		meta_t *ptr = global_tracker.has_free_blocks[type];
		while (ptr) {
			if (!tree_contains_node(ptr)) {
				violation(ptr, LIST, LIST | TREE, ARRAY);
				return (1);
			}
			ptr = ptr->next;
		}
	}
	return (0);
}
