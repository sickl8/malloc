#ifndef __LINKED_LIST_PREPEND_H__
#define __LINKED_LIST_PREPEND_H__

#include "index.h"

static void list_prepend_node(node_t **root, node_t *node) {
	node->next = *root;
	node->previous = NULL;
	if (*root) {
		(*root)->previous = node;
	}
	*root = node;
}

#endif