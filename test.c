#ifndef DEBUG
#define DEBUG
#endif
#include "src/index.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

#define print_int(x) printf("%s = %d", #x, x)
#define println_int(x) printf("%s = %ld\n", #x, (long)x)

INCLUDE_STATIC_HELPER_METHODS

int max(int a, int b) {
	return a > b ? a : b;
}

int _get_depth(node_t *node, int d) {
	// printf("node = %p, NULL = %p\n", node, NULL);
	// if (node && (char*)(node) - 1) {
	// 	println_int(node->data);
	// }
	return node ? max(_get_depth(node->left, d + 1), _get_depth(node->right, d + 1)) : d;
}

int get_depth(node_t *root) {
	return _get_depth(root, 0);
}

void _recursive_table_fill(node_t *node, node_t ***data, int depth, int offset, int max_depth) {
	if (depth == max_depth + 1) return;
	// printf("++++++++++++\n");
	// println_int(depth);
	// println_int(depth + 1);
	// println_int(offset);
	// println_int(offset * 2);
	// println_int(offset * 2 + 1);
	data[depth][offset] = node;
	_recursive_table_fill(node ? node->left : NULL, data, depth + 1, offset * 2, max_depth);
	_recursive_table_fill(node ? node->right : NULL, data, depth + 1, offset * 2 + 1, max_depth);
}

void recursive_table_fill(node_t *root, node_t ***data, int depth) {
	return _recursive_table_fill(root, data, 0, 0, depth);
}

#define CORNER_LEFT "┌"
#define CORNER_RIGHT "┐"
#define MIDDLE_BAR "─"

void _print_tree(node_t *root) {
	if (!root) return;
	// println_int(root);
	println_int(root->data);
	// println_int(root->color);
	// println_int(root->left);
	// println_int(root->right);
	// println_int(root->parent);
	_print_tree(root->left);
	_print_tree(root->right);
}

void print_tree(node_t *root) {
	if (!root) return;
	
	// return;
	int depth = get_depth(root);
	node_t ***data = malloc(sizeof(*data) * (depth + 1));
	for (int i = 0; i < (depth + 1); i++) {
		data[i] = malloc(sizeof(**data) * (1U << (int)i));
	}
	recursive_table_fill(root, data, depth);
	for (int i = 0; i < depth; i++) {

	}
	// goto l;
	const char spacing[] = "  ";
	const int width_of_node_text = 2;
	const int width_of_space_between_nodes = sizeof(spacing) - 1;
	const int max_width = width_of_node_text * (1 << depth) + width_of_node_text * ((1 << depth) - 0);
	for (int i = 0; i < depth; i++) {
		int node_count = 1 << i;
		int first_space = ((1 << (depth - (i + 2))) - 1) * width_of_node_text;
		int bar_space = first_space;
		int space_between = ((1 << (depth - (i + 1))) - 1) * width_of_node_text;
		for (int k = 0; k < first_space; k++)
			printf(" ");
		for (int j = 0; j < node_count; j++) {
			node_t *node = data[i][j];
			char *left_pad = " ", *right_pad = " ", *right_corner = "  ", *left_corner = "  ";
			if (node) {
				if (node->left) {
					left_pad = MIDDLE_BAR;
					left_corner = CORNER_LEFT MIDDLE_BAR;
				}
				if (node->right) {
					right_pad = MIDDLE_BAR;
					right_corner = MIDDLE_BAR CORNER_RIGHT;
				}
			}
			if (i == depth - 1) {
				left_pad = "";
				right_pad = "";
				left_corner = "";
				right_corner = "  ";
			}
			printf("%s", left_corner);
			for (int k = 0; k < bar_space; k++) {
				printf("%s", left_pad);
			}

			if (node) {
				printf("%s%02d%s", node->color == RED ? "\x1b[41m" : "", node->data, "\x1b[0m");
			}
			else {
				printf("  ");
			}

			for (int k = 0; k < bar_space; k++) {
				printf("%s", right_pad);
			}
			printf("%s", right_corner);
			for (int k = 0; k < space_between; k++)
				printf(" ");
		}
		printf("\n");
	}
	l:
	for (int i = 0; i < (depth + 1); i++) {
		free(data[i]);
	}
	free(data);
	for (int i = 0; i < width_of_node_text * ((1 << depth) - 1); i++)
		printf("-");
	printf("\n");

	// _print_tree(root);
}

void init_node(node_t *node, node_t *parent) {
	node->color = RED;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
}

// static inline int (*rotate_dir(node_dir_t dir))(node_t **root, node_t *node) {
// 	return dir == RIGHT ? rotate_right : rotate_left;
// }

// static inline int child_direction(node_t *node) {
// 	#ifdef DEBUG
// 		assert(node && (node->parent->left == node || node->parent->right == node));
// 	#endif
// 	return node == node->parent->left ? LEFT : RIGHT;
// }
// static inline node_t *get_uncle(node_t *g, node_t *p) {
// 	// printf("g = %p, g->l = %p, g->r = %p, p = %p\n", g, g->left, g->right, p);
// 	return (node_t*)(((size_t)g->left ^ (size_t)g->right) ^ (size_t)p);
// }

// static inline node_t **get_child_addr(node_t **r, node_t *p, node_t *n) {
// 	return !p ? r : (p->left == n ? &p->left : &p->right);
// }


int	insert_node(node_t **root, node_t *node) {
	// printf("here?\n");
	// struct timeval b;
	// gettimeofday(&b, NULL);
	if (!root || !node) {
		return (1);
	}
	node_t **tracer = root, *_parent = NULL, *gparent = NULL, *uncle = NULL;
	while (*tracer) {
		_parent = *tracer;
		if (node->data < (*tracer)->data) {
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

node_t *create_node(int data) {
	node_t *ret = malloc(sizeof(node_t));
	ret->data = data;
	init_node(ret, NULL);
	// println_int(ret);
	// println_int(ret->data);
	// println_int(ret->left);
	// println_int(ret->right);
	// println_int(ret->parent);
	// println_int(ret->color);
	return ret;
}

node_t *find_node(node_t *root, int data) {
	while (root) {
		if (root->data == data) {
			return root;
		}
		if (data < root->data) {
			root = root->left;
		} else {
			root = root->right;
		}
	}
	return NULL;
}

int insert(node_t **root, int data) {
	int ret = insert_node(root, create_node(data));
	// print_tree(*root);
	// if (data % 10000 == 0 && check_red_black_tree(*root)) {
	// 	printf("VIOLATION -------------------------\n");
	// }
	// if (!find_node(*root, data)) {
	// 	printf("DID NOT INSERT %d -------------------------\n", data);
	// }
	return ret;
}

void delete(node_t **root, int data) {
	node_t *node = find_node(*root, data);
	if (node) {
		remove_node(root, node);
		// print_tree(*root);
		// if (data % 10000 == 0 && check_red_black_tree(*root)) {
		// 	printf("VIOLATION -------------------------\n");
		// }
		// if (find_node(*root, data)) {
		// 	printf("DID NOT DELETE %d -------------------------\n", data);
		// }
	}
}

void shuffle(int *data, int len) {
	// shuffle
	for (int i = 0; i < 2 * len; i++) {
		int index1 = rand() % len;
		int index2 = rand() % len;
		int a = data[index1], b = data[index2];
		data[index1] = b;
		data[index2] = a;
	}
}

node_t *g_root = NULL;

int main(int ac, char **av) {
	int itr = (int)1e6;
	// int data[itr];
	int *data = malloc(sizeof(int) * itr);
	struct timeval start, end;
	for (int i = 0; i < itr; i++) {
		data[i] = i;
	}
	gettimeofday(&start, NULL);
	size_t seed = (size_t)start.tv_sec * (long)1e6 + (size_t)start.tv_usec;
	println_int(seed);
	if (av[1]) {
		seed = atoll(av[1]);
	}
	srand(seed);
	shuffle(data, itr);
	gettimeofday(&start, NULL);
	printf("inserting...\n");
	for (int i = 0; i < itr; i++) {
		// printf("inserting %d...\n", data[i]);
		// if (!(i % 1000)) {
		// 	println_int(i);
		// }
		insert(&g_root, data[i]);
	}
	printf("done inserting...\n");
	// print_tree(g_root);
	// printf("noshufle: [");
	// for (int i = 0; i < itr; i++) {
	// 	printf("%d%s", data[i], i != itr - 1 ? ", " : "");
	// }
	// printf("]\n");
	// shuffle(data, itr);
	// // printf("shuffled: [");
	// // for (int i = 0; i < itr; i++) {
	// // 	printf("%d%s", data[i], i != itr - 1 ? ", " : "");
	// // }
	// // printf("]\n");
	// printf("deleting...\n");
	// // for (int i = 0; i < itr; i++) {
	// // 	// printf("deleting: %u\n", data[i]);
	// // 	delete(&g_root, data[i]);
	// // }
	// // print_tree(g_root);
	// for (int d = 0; d < itr; d++) {
	// 	// printf("deleting: %u\n", g_root->data);
	// 	// if (!(d % 1000)) {
	// 	// 	println_int(d);
	// 	// }
	// 	// if (data[d] == 0) {
	// 	// 	println_int(data[d]);
	// 	// 	println_int(d);
	// 	// }
	// 	delete(&g_root, data[d]);
	// 	// print_tree(g_root);
	// }
	gettimeofday(&end, NULL);
	long time = (end.tv_sec * (long)1e6 + end.tv_usec) - (start.tv_sec * (long)1e6 + start.tv_usec);
	printf("took %lus%lums\n", time / (long)1e6, (time % (long)1e6) / 1000);
	println_int(seed);
	println_int(itr);

	// print_tree(g_root);
	// insert(&g_root, 30);
	// g_root->color = RED;
	// node_t *v = create_node(30);
	// v->left = g_root;
	// v->color = RED;
	// g_root->parent = v;
	// g_root = v;
	// print_tree(g_root);
	// if (check_red_black_tree(g_root)) {
	// 	printf("main: VIOLATION -------------------------\n");
	// }
	// return (0);
	// insert(&g_root, 8);
	// insert(&g_root, 5);
	// insert(&g_root, 15);
	// insert(&g_root, 12);
	// insert(&g_root, 19);
	// insert(&g_root, 23);
	// insert(&g_root, 9);
	// insert(&g_root, 13);
	// insert(&g_root, 10);
	// delete(&g_root, 12);
	// delete(&g_root, 13);
	// insert(&g_root, 12);
	// insert(&g_root, 14);
}