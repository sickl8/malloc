#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <assert.h>
#include <stdio.h>
#include "../types.h"
#include "../defs.h"
typedef enum	node_color_e {
	RED,
	BLACK,
}				node_color_t;

typedef enum	node_dir_e {
	LEFT	= 0,
	RIGHT	= 1,
}				node_dir_t;


typedef struct	red_black_tree_node_s {
	union {
		struct red_black_tree_node_s *children[2];
		struct {
			struct red_black_tree_node_s	*left;
			struct red_black_tree_node_s	*right;
		};
	};
	struct red_black_tree_node_s		*parent;
	node_color_t	color;
	// data
	struct {
		#ifdef DEBUG
			int data;
		#endif
		// real allocation size returned by psm_mmap
		size_t	real_size;
		// node type, small, tiny, or large
		type_t	type;
		// union of size and bit-field describing free blocks
		union {
			// bit-field to mark each block either as free or used, last byte is the count of used blocks
			unsigned char	bf_free_blocks[(MIN_ALLOCS_IN_ZONE / 8) + 1];
			// large allocation size, zero if free, non-zero otherwise
			size_t			size;
		};
	};
}			red_black_tree_node_t;

typedef red_black_tree_node_t node_t;

int rotate_left(node_t **root, node_t *left);
int rotate_right(node_t **root, node_t *right);
void init_node(node_t *node, node_t *parent);
int	insert_node(node_t **root, node_t *node);
void fix_tree_after_remove(node_t **root, node_t *node, node_t *sentinel);
node_t *remove_node(node_t **root, node_t *node);

int check_red_violation(node_t *node);
void print_violation(node_t *node, const char *violation);
int check_properties(node_t *node, int black_count, int *black_height);
int check_red_black_tree(node_t *root);
#ifdef DEBUG
	#define __CHILD_DIR_ASSERT__		assert(node != NULL); \
										assert(node->parent != NULL); \
										assert((node->parent->left == node || node->parent->right == node));
	#define __GET_CHILD_ADDR_ASSERT__	assert(!p || p->left == n || p->right == n);
#else
	#define __CHILD_DIR_ASSERT__
	#define __GET_CHILD_ADDR_ASSERT__
#endif

#define INCLUDE_STATIC_HELPER_METHODS \
static inline node_t *get_uncle(node_t *g, node_t *p) { \
	return (node_t*)(((size_t)g->left ^ (size_t)g->right) ^ (size_t)p); \
} \
 \
static inline node_t **get_child_addr(node_t **r, node_t *p, node_t *n) { \
	__GET_CHILD_ADDR_ASSERT__ \
	return !p ? r : (p->left == n ? &p->left : &p->right); \
} \
 \
static inline int (*rotate_dir(node_dir_t dir))(node_t **root, node_t *node) { \
	return ((void*[]){ rotate_left, rotate_right })[dir]; \
} \
 \
static inline int child_direction(node_t *node) { \
	__CHILD_DIR_ASSERT__ \
	return node == node->parent->left ? LEFT : RIGHT; \
} \
static inline node_t *get_dir_child(node_t *node, node_dir_t dir) { \
	return dir == LEFT ? node->left : node->right; \
}

#endif