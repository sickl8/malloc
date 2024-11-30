#include "index.h"

INCLUDE_STATIC_HELPER_METHODS

int rotate_left(node_t **root, node_t *left) {
	node_t *right;

	if (!root || !left || !(right = left->right)) {
		return (1);
	}
	#ifdef DEBUG
		// printf("ROTATE LEFT\n");
	#endif
	root = get_child_addr(root, left->parent, left);
	left->right = right->left;
	if (left->right) {
		left->right->parent = left;
	}
	right->left = left;
	right->parent = left->parent;
	*root = right;
	left->parent = right;
	return (0);
}

int rotate_right(node_t **root, node_t *right) {
	node_t *left;

	if (!root || !right || !(left = right->left)) {
		return (1);
	}
	#ifdef DEBUG
		// printf("ROTATE RIGHT\n");
	#endif
	root = get_child_addr(root, right->parent, right);
	right->left = left->right;
	if (right->left) {
		right->left->parent = right;
	}
	left->right = right;
	left->parent = right->parent;
	*root = left;
	right->parent = left;
	return (0);
}
