#ifndef __LINKED_LIST_INSERT_AFTER_H__
#define __LINKED_LIST_INSERT_AFTER_H__

#include "index.h"

static void list_insert_node_after(node_t *after, node_t *node) {
	node->previous = after;
	node->next = after->next;
	if (after->next) {
		after->next->previous = node;
	}
	after->next = node;
}

#endif