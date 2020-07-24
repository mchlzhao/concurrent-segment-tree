#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common.h"
#include "../seg-tree-interface.h"
#include "seg-tree.h"

typedef struct _seg_tree_node_t {
	int value;
	int lazy;
	int ran_l, ran_r;
	bool is_child;
	struct _seg_tree_node_t *lc, *rc;
	pthread_mutex_t lock;
} seg_tree_node_t;

// recursively initialises segment tree nodes
void _seg_tree_init(seg_tree_node_t *node, int ran_l, int ran_r) {
	node->value = node->lazy = 0;
	node->ran_l = ran_l;
	node->ran_r = ran_r;
	node->is_child = false;
	pthread_mutex_init(&node->lock, NULL);
	if (ran_l == ran_r-1) {
		node->lc = node->rc = NULL;
		node->is_child = true;
	} else {
		node->lc = (seg_tree_node_t*) malloc(sizeof(seg_tree_node_t));
		node->rc = (seg_tree_node_t*) malloc(sizeof(seg_tree_node_t));
		int ran_m = (ran_l+ran_r)/2;
		_seg_tree_init(node->lc, ran_l, ran_m);
		_seg_tree_init(node->rc, ran_m, ran_r);
	}
}

// pointer to root node to be stored in root
void seg_tree_init(seg_tree_t *tree, int range) {
	tree->root = (seg_tree_node_t*) malloc(sizeof(seg_tree_node_t));
	tree->range = range;
	_seg_tree_init(tree->root, 0, range);
}

// recursively destroys segment tree nodes
void _seg_tree_destroy(seg_tree_node_t *node) {
	// nodes are guaranteed to have either zero or two children
	if (!node->is_child) {
		_seg_tree_destroy(node->lc);
		_seg_tree_destroy(node->rc);
	}
	pthread_mutex_destroy(&node->lock);
	free(node);
}

void seg_tree_destroy(seg_tree_t *tree) {
	_seg_tree_destroy(tree->root);
}

void _seg_tree_clean_node(seg_tree_node_t *node) {
	if (node->lazy == 0) return;
	node->value += node->lazy;
	if (!node->is_child) {
		node->lc->lazy += node->lazy;
		node->rc->lazy += node->lazy;
	}
	node->lazy = 0;
}

void _seg_tree_check_bounds(seg_tree_node_t *root, int ran_l, int ran_r) {
	assert(ran_l < ran_r);
	assert(root->ran_l <= ran_l);
	assert(ran_r <= root->ran_r);
}

int _seg_tree_query(seg_tree_node_t *node, int que_l, int que_r) {
	_seg_tree_clean_node(node);
	int ret;
	if (node->ran_r <= que_l || que_r <= node->ran_l) {
		// query range does not intersect node range
		ret = 0;
		pthread_mutex_unlock(&node->lock);
	} else if (que_l <= node->ran_l && node->ran_r <= que_r) {
		// query range fully encapsulates node range
		ret = node->value;
		pthread_mutex_unlock(&node->lock);
	} else {
		// query range intersects both halves of current node range
		pthread_mutex_lock(&node->lc->lock);
		pthread_mutex_lock(&node->rc->lock);
		pthread_mutex_unlock(&node->lock);

		ret = _seg_tree_query(node->lc, que_l, que_r) +
				_seg_tree_query(node->rc, que_l, que_r);
	}
	return ret;
}

int seg_tree_query(seg_tree_t *tree, int que_l, int que_r) {
	_seg_tree_check_bounds(tree->root, que_l, que_r);
	pthread_mutex_lock(&tree->root->lock);
	int ret = _seg_tree_query(tree->root, que_l, que_r);
	// printf("q %d %d %d\n", que_l, que_r, ret);
	return ret;
}

void _seg_tree_update(seg_tree_node_t *node, int ran_l, int ran_r, int inc) {
	_seg_tree_clean_node(node);
	if (ran_l <= node->ran_l && node->ran_r <= ran_r) {
		// query range fully encapsulates node range
		node->lazy += inc;
		_seg_tree_clean_node(node);

		pthread_mutex_unlock(&node->lock);
	} else if (node->ran_r > ran_l && ran_r > node->ran_l) {
		// query range intersects node range, but does not fully encapsulate
		node->value += inc * (min(ran_r, node->ran_r) - max(ran_l, node->ran_l));

		pthread_mutex_lock(&node->lc->lock);
		pthread_mutex_lock(&node->rc->lock);
		pthread_mutex_unlock(&node->lock);

		_seg_tree_update(node->lc, ran_l, ran_r, inc);
		_seg_tree_update(node->rc, ran_l, ran_r, inc);
	} else {
		pthread_mutex_unlock(&node->lock);
	}
}

void seg_tree_update(seg_tree_t *tree, int ran_l, int ran_r, int inc) {
	_seg_tree_check_bounds(tree->root, ran_l, ran_r);
	pthread_mutex_lock(&tree->root->lock);
	_seg_tree_update(tree->root, ran_l, ran_r, inc);
	// printf("u %d %d %d\n", ran_l, ran_r, inc);
}
