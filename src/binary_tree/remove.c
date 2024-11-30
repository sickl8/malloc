#include "index.h"
#include <stddef.h>

INCLUDE_STATIC_HELPER_METHODS

void transplant(node_t **root, node_t *u, node_t *v, node_t *s) {
	if (!u->parent || u->parent == s) {

		*root = v;
	} else if (u->parent->left == u) {
		u->parent->left = v;
	} else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}

void fix_tree_after_remove(node_t **root, node_t *x, node_t *s) {
	node_t *w = NULL;
	while (x != *root && (x && x->color == BLACK)) {
		int node_dir = child_direction(x);
		w = x->parent->children[!node_dir];
		if (w->color == RED) {
			w->color = BLACK;
			x->parent->color = RED;
			rotate_dir(node_dir)(get_child_addr(root, x->parent->parent, x->parent), x->parent);
			w = x->parent->children[!node_dir];
		}
		if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
			w->color = RED;
			x = x->parent;
		} else {
			if (!w->children[!node_dir] || w->children[!node_dir]->color == BLACK) {
				w->children[node_dir]->color = BLACK;
				w->color = RED;
				rotate_dir(!node_dir)(get_child_addr(root, w->parent, w), w);
				w = x->parent->children[!node_dir];
			}
			w->color = x->parent->color;
			x->parent->color = BLACK;
			w->children[!node_dir]->color = BLACK;
			rotate_dir(node_dir)(get_child_addr(root, x->parent->parent, x->parent), x->parent);
			x = *root;
		}
	}
	if (x)
		x->color = BLACK;
}


node_t *leftmost_node(node_t *root) {
	while (root->left) {
		root = root->left;
	}
	return root;
}

node_t *rightmost_node(node_t *root) {
	while (root->right) {
		root = root->right;
	}
	return root;
}

node_t *create_sentinel(node_t *sentinel, node_t *parent, node_dir_t child_dir) {
	// return parent->children[child_dir];
	node_t *ret = parent->children[child_dir];
	if (!ret) {
		ret = sentinel;
		ret->color = BLACK;
		ret->parent = parent;
		parent->children[child_dir] = ret;
	}
	return ret;
}

node_t *remove_node(node_t **root, node_t *node) {
	node_t *orig = node;
	node_color_t orig_color = orig->color;
	node_t sentinel;
	node_t *to_fix = &sentinel;
	node_t *ret = node;
	int nodeWasLonelyRoot = node == *root && !node->left && !node->right;

	init_node(to_fix, NULL);
	#ifdef DEBUG
		to_fix->data = -1;
	#endif

	if (!node->left) {
		to_fix = create_sentinel(to_fix, node, RIGHT);
		transplant(root, node, node->right, &sentinel);
	} else if (!node->right) {
		to_fix = create_sentinel(to_fix, node, LEFT);
		transplant(root, node, node->left, &sentinel);
	} else {
		orig = leftmost_node(node->right);
		orig_color = orig->color;
		to_fix = create_sentinel(to_fix, orig, RIGHT);
		if (orig->parent == node) {
			to_fix->parent = orig;
		} else {
			transplant(root, orig, orig->right, &sentinel);
			orig->right = node->right;
			orig->right->parent = orig;
		}
		transplant(root, node, orig, &sentinel);
		orig->left = node->left;
		orig->left->parent = orig;
		orig->color = node->color;
	}
	if (orig_color == BLACK) {
		fix_tree_after_remove(root, to_fix, &sentinel);
	}
	if (to_fix && to_fix->parent && to_fix == &sentinel) {
		*get_child_addr(root, to_fix->parent, to_fix) = NULL;
	}
	if (nodeWasLonelyRoot) {
		*root = NULL;
	}
	return ret;
}
