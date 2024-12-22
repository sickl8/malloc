#ifndef __BINARY_TREE_INIT_H__
#define __BINARY_TREE_INIT_H__

#include "index.h"

static void init_node(node_t *node, node_t *parent)
{
	node->color = RED;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
}

#endif