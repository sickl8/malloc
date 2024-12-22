#ifndef __BINAR_TREE_STATIC_HELPERS_H__
#define __BINAR_TREE_STATIC_HELPERS_H__

#include "index.h"

static inline node_t *get_uncle(node_t *g, node_t *p) {
	return (node_t *)(((size_t)g->left ^ (size_t)g->right) ^ (size_t)p);
}

static inline node_t **get_child_addr(node_t **r, node_t *p, node_t *n) {
	return !p ? r : (node_t**)((void *[]){&p->left, &p->right})[n == p->right];
}

static inline node_dir_t child_direction(node_t *node) {
	return (node_dir_t)(node != node->parent->left);
}

#endif