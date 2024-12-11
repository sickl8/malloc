#include "index.h"

int rotate(node_t **root, node_t *upper, node_dir_t dir) {
	node_t *lower;

	if (!root || !upper || !(lower = upper->children[!dir])) {
		return (1);
	}
	#ifdef DEBUG
		// printf("ROTATE LEFT\n");
	#endif
	upper->children[!dir] = lower->children[dir];
	if (upper->children[!dir]) {
		upper->children[!dir]->parent = upper;
	}
	lower->children[dir] = upper;
	lower->parent = upper->parent;
	*root = lower;
	upper->parent = lower;
	return (0);
}
