#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
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

void _seg_tree_check_bounds(seg_tree_node_t *root, int ran_l, int ran_r) {
	assert(ran_l < ran_r);
	assert(root->ran_l <= ran_l);
	assert(ran_r <= root->ran_r);
}

int _seg_tree_query(seg_tree_node_t *node, int que_l, int que_r, int lazy_sum) {
	node->value += lazy_sum * (node->ran_r-node->ran_l);

	int ret;
	if (node->ran_r <= que_l || que_r <= node->ran_l) {
		// query range does not intersect node range
		ret = 0;
		node->lazy += lazy_sum;

		pthread_mutex_unlock(&node->lock);
	} else if (que_l <= node->ran_l && node->ran_r <= que_r) {
		// query range fully encapsulates node range
		ret = node->value;
		node->lazy += lazy_sum;

		pthread_mutex_unlock(&node->lock);
	} else {
		// query range intersects both halves of current node range
		lazy_sum += node->lazy;
		node->lazy = 0;

		pthread_mutex_lock(&node->lc->lock);
		pthread_mutex_lock(&node->rc->lock);
		pthread_mutex_unlock(&node->lock);

		ret = _seg_tree_query(node->lc, que_l, que_r, lazy_sum) +
				_seg_tree_query(node->rc, que_l, que_r, lazy_sum);
	}
	return ret;
}

int seg_tree_query(seg_tree_t *tree, int que_l, int que_r) {
	_seg_tree_check_bounds(tree->root, que_l, que_r);
	pthread_mutex_lock(&tree->root->lock);
	return _seg_tree_query(tree->root, que_l, que_r, 0);
}

void _seg_tree_update(seg_tree_node_t *node, int ran_l, int ran_r, int inc,
		int lazy_sum) {

	node->value += lazy_sum * (node->ran_r-node->ran_l);

	if (ran_l <= node->ran_l && node->ran_r <= ran_r) {
		// query range fully encapsulates node range
		node->value += inc * (node->ran_r-node->ran_l);
		node->lazy += lazy_sum+inc;

		pthread_mutex_unlock(&node->lock);
	} else if (node->ran_r > ran_l && ran_r > node->ran_l) {
		// query range intersects node range, but does not fully encapsulate
		node->value += inc * (min(ran_r, node->ran_r) - max(ran_l, node->ran_l));
		lazy_sum += node->lazy;
		node->lazy = 0;

		pthread_mutex_lock(&node->lc->lock);
		pthread_mutex_lock(&node->rc->lock);
		pthread_mutex_unlock(&node->lock);

		_seg_tree_update(node->lc, ran_l, ran_r, inc, lazy_sum);
		_seg_tree_update(node->rc, ran_l, ran_r, inc, lazy_sum);
	} else {
		// query range does not intersect node range
		node->lazy += lazy_sum;

		pthread_mutex_unlock(&node->lock);
	}
}

void seg_tree_update(seg_tree_t *tree, int ran_l, int ran_r, int inc) {
	_seg_tree_check_bounds(tree->root, ran_l, ran_r);
	pthread_mutex_lock(&tree->root->lock);
	_seg_tree_update(tree->root, ran_l, ran_r, inc, 0);
}
