#include "../index.h"

void list_prepend_node(node_t **root, node_t *node) {
	node->next = *root;
	node->previous = NULL;
	if (*root) {
		(*root)->previous = node;
	}
	*root = node;
}

node_t *list_pop_node(node_t **root) {
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

void list_remove_node(node_t **root, node_t *node) {
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
