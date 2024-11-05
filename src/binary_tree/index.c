#include "index.h"

void init_node(node_t *node, node_t *parent) {
	node->color = RED_NODE;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
}

int rotate_left(node_t **root, node_t *left) {
	node_t *right;

	if (!root || !left || !(right = left->right)) {
		return (1);
	}
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

inline int child_direction(node_t *node) {
	assert(node);
	return node->parent < node ? RIGHT : LEFT;
}

inline int (*rotate_dir(node_dir_t dir))(node_t **root, node_t *node) {
	return dir == RIGHT ? rotate_right : rotate_left;
}

int	insert_node(node_t **root, node_t *node) {
	if (!root || !node) {
		return (1);
	}

	node_t **tracer = root, *parent = NULL, *gparent = NULL, *uncle = NULL;
	node_dir_t dir = RIGHT;

	while (*tracer) {
		parent = *tracer;
		if (node < *tracer) {
			tracer = &(*tracer)->left;
		} else {
			tracer = &(*tracer)->right;
		}
	}
	init_node(node, parent);
	*tracer = node;
	if (parent == NULL) {
		return (0);
	}
	do {
		if (parent->color == BLACK_NODE) {
			return (0);
		}
		if (!(gparent = parent->parent)) {
			goto case_i4;
		}
		dir = child_direction(parent);
		uncle = dir == RIGHT ? gparent->left : gparent->right;
		if (!uncle || uncle->color == BLACK_NODE) {
			goto case_i56;
		}
		parent->color = BLACK_NODE;
		uncle->color = BLACK_NODE;
		gparent->color = RED_NODE;
		node = gparent;
	} while (parent = node->parent);
		return (0);
	case_i4:
		parent->color = BLACK_NODE;
		return (0);
	case_i56:
		if (child_direction(node) != dir) {
			rotate_dir(dir)(root, parent);
			node = parent;
			parent = dir == RIGHT ? gparent->right : gparent->left;
		}
	rotate_dir(RIGHT - dir)(root, gparent);
	parent->color = BLACK_NODE;
	gparent->color = RED_NODE;
	return (0);
}