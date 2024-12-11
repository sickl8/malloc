#ifndef __LINKED_LIST_INDEX_H__
#define __LINKED_LIST_INDEX_H__

#include "../index.h"

void list_prepend_node(node_t **root, node_t *node);
node_t *list_pop_node(node_t **root);
void list_remove_node(node_t **root, node_t *node);

#endif