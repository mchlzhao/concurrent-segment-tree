#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef struct _seg_tree_node_t {
	int value;
	int lazy;
	int ran_l, ran_r;
	bool is_child;
	// ??? can we get rid of _ here
	struct _seg_tree_node_t *lc, *rc;
} seg_tree_node_t;

// recursively initialises segment tree nodes
void _seg_tree_init(seg_tree_node_t *node, int ran_l, int ran_r) {
	node->value = node->lazy = 0;
	node->ran_l = ran_l;
	node->ran_r = ran_r;
	node->is_child = false;
	if (ran_l== ran_r-1) {
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

int _seg_tree_query(seg_tree_node_t *node, int que_l, int que_r) {
	_seg_tree_clean_node(node);
	int ret;
	if (que_l <= node->ran_l && node->ran_r <= que_r) {
		// query range fully encapsulates node range
		ret = node->value;
	} else {
		int ran_m = (node->ran_l+node->ran_r)/2;
		if (que_l < ran_m && ran_m < que_r) {
			// query range intersects both halves of current node range
			ret = max(_seg_tree_query(node->lc, que_l, que_r),
					_seg_tree_query(node->rc, que_l, que_r));
		} else if (que_r <= ran_m) {
			// query range falls within left child's range
			ret = _seg_tree_query(node->lc, que_l, que_r);
		} else {
			// query range falls within right child's range
			ret = _seg_tree_query(node->rc, que_l, que_r);
		}
	}
	return ret;
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

void _seg_tree_update(seg_tree_node_t *node, int ran_l, int ran_r, int inc) {
	_seg_tree_clean_node(node);
	if (ran_l <= node->ran_l && node->ran_r <= ran_r) {
		// query range fully encapsulates node range
		node->lazy += inc;
		_seg_tree_clean_node(node);
	} else {
		int ran_m = (node->ran_l+node->ran_r)/2;
		if (ran_l < ran_m) {
			// query range intersects left child's range
			_seg_tree_update(node->lc, ran_l, ran_r, inc);
		}
		if (ran_m < ran_r) {
			// query range intersects right child's range
			_seg_tree_update(node->rc, ran_l, ran_r, inc);
		}
		node->value = max(node->lc->value, node->rc->value);
	}
}

void seg_tree_update(seg_tree_node_t *root, int ran_l, int ran_r, int inc) {
	seg_tree_check_bounds(root, ran_l, ran_r);
	_seg_tree_update(root, ran_l, ran_r, inc);
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("usage: [range] [number of operations] [number of threads]\n");
		exit(1);
	}
	srand(0);
	int range = atoi(argv[1]);
	int num_ops = atoi(argv[2]);
	int num_threads = atoi(argv[3]);

	seg_tree_node_t *root;
	seg_tree_init(&root, range);

	for (int i = 0; i < num_ops; i++) {
		bool is_query = rand()%2;
		int ran_l = -1, ran_r = -1;
		while (ran_l >= ran_r) {
			ran_l = rand()%range;
			ran_r = rand()%range;
		}
		if (is_query) {
			printf("queried %d %d = %d\n", ran_l, ran_r, seg_tree_query(root, ran_l, ran_r));
		} else {
			int inc = rand()%11 - 5;
			seg_tree_update(root, ran_l, ran_r, inc);
			printf("updated %d %d by %d\n", ran_l, ran_r, inc);
		}
	}

	seg_tree_destroy(root);
}
