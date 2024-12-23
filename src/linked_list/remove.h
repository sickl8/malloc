#ifndef __LINKED_LIST_REMOVE_H__
#define __LINKED_LIST_REMOVE_H__

#include "index.h"

static void list_remove_node(node_t **root, node_t *node) {
	if (node->next) {
		node->next->previous = node->previous;
	}
	if (node->previous) {
		node->previous->next = node->next;
	} else { // node == *root
		*root = node->next;
		if (*root) { // in case only element
			(*root)->previous = NULL;
		}
	}
	node->next = NULL;
	node->previous = NULL;
}

#endif