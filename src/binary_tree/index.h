#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include "../index.h"

typedef enum	node_color_e {
	RED_NODE,
	BLACK_NODE,
}				node_color_t;

typedef enum	node_dir_e {
	LEFT	= 0,
	RIGHT	= 1,
}				node_dir_t;

typedef struct	red_black_tree_node_s {
	struct red_black_tree_node_s	*left;
	struct red_black_tree_node_s	*right;
	struct red_black_tree_node_s	*parent;
	node_color_t					color;
}				red_black_tree_node_t;

typedef red_black_tree_node_t node_t;

#endif