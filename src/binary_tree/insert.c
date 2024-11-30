#include "index.h"
#include <stddef.h>
/*

REQUIREMENTS:
1. Every node is either RED or BLACK
2. The root node and all NULL nodes are BLACK
3. If a node is RED, then it's children are BLACK
4. Every path from a given node to any of its descendant NULL nodes goes through the same number of BLACK nodes
5. (Conclusion) If a node N has exactly one child, the child must be RED, because if it were BLACK, its NULL
   descendants would sit at a different BLACK depth than N's NULL child, violating requirement 4

*/

INCLUDE_STATIC_HELPER_METHODS

void init_node(node_t *node, node_t *parent) {
	node->color = RED;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
}

int	insert_node(node_t **root, node_t *node) {
	if (!root || !node) {
		return (1);
	}
	node_t **tracer = root, *_parent = NULL, *gparent = NULL, *uncle = NULL;
	while (*tracer) {
		_parent = *tracer;
		if (node < (*tracer)) {
			tracer = &(*tracer)->left;
		} else {
			tracer = &(*tracer)->right;
		}
	}
	*tracer = node;
	init_node(node, _parent);

	while (node->parent && node->parent->color == RED) {
		// parent is red, means it must have a non-NULL black grandparent
		gparent = node->parent->parent;
		uncle = get_uncle(gparent, node->parent);
		if (uncle && uncle->color == RED) { // 2. node's uncle is RED
			node->parent->color = BLACK;
			uncle->color = BLACK;
			gparent->color = RED;
			node = gparent;
		} else { // uncle is black
			int parent_dir = child_direction(node->parent);
			int uncle_dir = !parent_dir;
			int node_dir = child_direction(node);
			if (uncle_dir == node_dir) { // 3. uncle is black and in same direction of node
				node = node->parent;
				rotate_dir(parent_dir)(get_child_addr(root, gparent, node), node);
			}
			node->parent->color = BLACK;
			gparent->color = RED;
			rotate_dir(uncle_dir)(get_child_addr(root, gparent->parent, gparent), gparent);
		}
	}
	(*root)->color = BLACK;
	return (0);
}
