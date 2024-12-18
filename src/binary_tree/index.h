#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <assert.h>
#include <stdio.h>
#include "../types.h"
#include "../defs.h"
#include "../index.h"

int rotate_left(node_t **root, node_t *left);
int rotate_right(node_t **root, node_t *right);
int rotate(node_t **root, node_t *upper, node_dir_t dir);
void init_node(node_t *node, node_t *parent);
int	tree_insert_node(node_t **root, node_t *node);
node_t *tree_remove_node(node_t **root, node_t *node);
void fix_tree_after_remove(node_t **root, node_t *node);

int check_red_violation(node_t *node);
void print_violation(node_t *node, const char *violation);
int check_properties(node_t *node, int black_count, int *black_height);
int check_red_black_tree(node_t *root);
block_t find_alloc(node_t **root, void *alloc);

#ifdef DEBUG_ASSERT
	#define __assert(...) assert(__VA_ARGS__)
#else
	#define __assert(...)
#endif

#ifdef DEBUG
	#define __CHILD_DIR_ASSERT__		__assert(node != NULL); \
										__assert(node->parent != NULL); \
										__assert((node->parent->left == node || node->parent->right == node));
	#define __GET_CHILD_ADDR_ASSERT__	__assert(!p || p->left == n || p->right == n);
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
	return !p ? r : ((void*[]){&p->left, &p->right})[n == p->right]; \
} \
 \
static inline node_dir_t child_direction(node_t *node) { \
	__CHILD_DIR_ASSERT__ \
	return node != node->parent->left /* ? RIGHT : LEFT */; \
}

#endif