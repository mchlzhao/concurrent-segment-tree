#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "seg-tree-common.h"

// pointer to root node to be stored in root
void seg_tree_init(seg_tree_node_t **root, int range) {
	*root = (seg_tree_node_t*) malloc(sizeof(seg_tree_node_t));
	_seg_tree_init(*root, 0, range);
}

// recursively destroys segment tree nodes
void seg_tree_destroy(seg_tree_node_t *root) {
	// nodes are guaranteed to have either zero or two children
	if (!root->is_child) {
		seg_tree_destroy(root->lc);
		seg_tree_destroy(root->rc);
	}
	free(root);
}

void _seg_tree_clean_node(seg_tree_node_t *node) {
	node->value += node->lazy;
	if (!node->is_child) {
		node->lc->lazy += node->lazy;
		node->rc->lazy += node->lazy;
	}
	node->lazy = 0;
}

void seg_tree_check_bounds(seg_tree_node_t *root, int ran_l, int ran_r) {
	assert(ran_l < ran_r);
	assert(root->ran_l <= ran_l);
	assert(ran_r <= root->ran_r);
}

int seg_tree_query(seg_tree_node_t *root, int que_l, int que_r) {
	seg_tree_check_bounds(root, que_l, que_r);
	return _seg_tree_query(root, que_l, que_r);
}

void seg_tree_update(seg_tree_node_t *root, int ran_l, int ran_r, int inc) {
	seg_tree_check_bounds(root, ran_l, ran_r);
	_seg_tree_update(root, ran_l, ran_r, inc);
}
