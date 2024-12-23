#ifndef __LINKED_LIST_POP_H__
#define __LINKED_LIST_POP_H__

#include "index.h"

static node_t *list_pop_node(node_t **root) {
	node_t *ret = *root;
	if (ret) {
		*root = ret->next;
		if (ret->next) {
			(*root)->previous = NULL;
		}
		ret->next = NULL;
		ret->previous = NULL;
	}
	return ret;
}

#endif