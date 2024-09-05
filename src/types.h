#ifndef __TYPES_H__
#define __TYPES_H__

#include <stddef.h>

enum node_color {
	RED_NODE,
	BLACK_NODE,
};

typedef struct	red_black_tree_node_s {
	enum node_color					color;
	void							*data;
	struct red_black_tree_node_s	*left;
	struct red_black_tree_node_s	*right;
	struct red_black_tree_node_s	*parent;
}				red_black_tree_node_t;

typedef red_black_tree_node_t node_t;

typedef struct	allocation_data_s {
	void						*address;
	size_t						size;
	struct allocation_data_s	*next;
	struct allocation_data_s	*prev;
}				allocation_data_t;

typedef struct	zone_s {
	size_t	size;
	size_t	max_alloc;
	node_t	*allocs;
}				zone_t;

typedef struct	alloc_data_s {
	size_t page_size;
	zone_t tiny;
	zone_t small;
}				alloc_data_t;

typedef struct	alloc_s {

}				alloc_t;

#endif /*__TYPES_H__*/