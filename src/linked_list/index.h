#ifndef __LINKED_LIST_INDEX_H__
#define __LINKED_LIST_INDEX_H__

#include "../index.h"
#ifndef NODE_TYPE
#define NODE_TYPE meta_t
#endif
typedef NODE_TYPE node_t;

static void list_prepend_node(node_t **root, node_t *node) {
	node->next = *root;
	node->previous = NULL;
	if (*root) {
		(*root)->previous = node;
	}
	*root = node;
}

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

static void list_insert_node_after(node_t *after, node_t *node) {
	node->previous = after;
	node->next = after->next;
	if (after->next) {
		after->next->previous = node;
	}
	after->next = node;
}

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