#ifndef DEBUG
#define DEBUG
#endif
#include "index.h"

static int check_red_violation(node_t *node) {
    if ((node->left && node->left->color == RED) || (node->right && node->right->color == RED)) {
        return 0; // Red violation found
    }
    return 1; // No red violation
}

static void print_violation(node_t *node, const char *violation) {
    printf("Violation at node with data %d: %s\n", node->data, violation);
}

static int check_properties(node_t *node, int black_count, int *black_height) {
    if (node == NULL) {
        // For a NULL (leaf), ensure black height consistency
        if (*black_height == 0) {
            *black_height = black_count;
        } else if (black_count != *black_height) {
            printf("Black height inconsistency: Expected %d, got %d\n", *black_height, black_count);
            return 1;
        }
        return 0; // No violation
    }

    // If the node is red, it cannot have red children
    if (node->color == RED) {
        if (!check_red_violation(node)) {
            print_violation(node, "Red-Red violation (consecutive red nodes)");
            return 1;
        }
    }

    // Increment black count if node is black
    if (node->color == BLACK) {
        black_count++;
    }

    // Recursively check left and right subtrees
    if (check_properties(node->left, black_count, black_height)) {
        return 1;
    }
    if (check_properties(node->right, black_count, black_height)) {
        return 1;
    }

    return 0; // No violations
}

static int check_red_black_tree(node_t *root) {
    if (root == NULL) {
        return 0; // An empty tree is valid
    }

    // The root must be black
    if (root->color != BLACK) {
        print_violation(root, "Root is not black");
        return 1;
    }

    int black_height = 0;
    return check_properties(root, 0, &black_height);
}
