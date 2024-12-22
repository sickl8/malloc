#include "index.h"
#include "rotate.h"
#include "init.h"
#include <stddef.h>
#include "static_helpers.h"

static void transplant(node_t **root, node_t *u, node_t *v, node_t *s) {
	if (!u->parent || u->parent == s) {

		*root = v;
	} else if (u->parent->left == u) {
		u->parent->left = v;
	} else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}


static void fix_tree_after_remove(node_t **root, node_t *to_fix) {
	node_t *sibling = NULL;
	while (to_fix != *root && (to_fix && to_fix->color == BLACK)) {
		int node_dir = child_direction(to_fix);
		sibling = to_fix->parent->children[!node_dir];
		if (sibling->color == RED) {
			sibling->color = BLACK;
			to_fix->parent->color = RED;
			rotate(get_child_addr(root, to_fix->parent->parent, to_fix->parent), to_fix->parent, node_dir);
			sibling = to_fix->parent->children[!node_dir];
		}
		if ((!sibling->left || sibling->left->color == BLACK) && (!sibling->right || sibling->right->color == BLACK)) {
			sibling->color = RED;
			to_fix = to_fix->parent;
		} else {
			if (!sibling->children[!node_dir] || sibling->children[!node_dir]->color == BLACK) {
				sibling->children[node_dir]->color = BLACK;
				sibling->color = RED;
				rotate(get_child_addr(root, sibling->parent, sibling), sibling, !node_dir);
				sibling = to_fix->parent->children[!node_dir];
			}
			sibling->color = to_fix->parent->color;
			to_fix->parent->color = BLACK;
			sibling->children[!node_dir]->color = BLACK;
			rotate(get_child_addr(root, to_fix->parent->parent, to_fix->parent), to_fix->parent, node_dir);
			to_fix = *root;
		}
	}
	if (to_fix)
		to_fix->color = BLACK;
}

static node_t *leftmost_node(node_t *root) {
	while (root->left) {
		root = root->left;
	}
	return root;
}

static node_t *create_sentinel(node_t *sentinel, node_t *parent, node_dir_t child_dir) {
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

static node_t *tree_remove_node(node_t **root, node_t *node) {
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
		fix_tree_after_remove(root, to_fix);
	}
	if (to_fix && to_fix->parent && to_fix == &sentinel) {
		*get_child_addr(root, to_fix->parent, to_fix) = NULL;
	}
	if (nodeWasLonelyRoot) {
		*root = NULL;
	}
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return ret;
}
