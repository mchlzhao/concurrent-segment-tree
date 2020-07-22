#ifndef _SEG_TREE_COMMON_H
#define _SEG_TREE_COMMON_H

typedef struct _seg_tree_node_t {
	int value;
	int lazy;
	int ran_l, ran_r;
	bool is_child;
	struct _seg_tree_node_t *lc, *rc;
} seg_tree_node_t;

void seg_tree_init(seg_tree_node_t **root, int range);
void _seg_tree_init(seg_tree_node_t *node, int ran_l, int ran_r);

void seg_tree_destroy(seg_tree_node_t *root);

void _seg_tree_clean_node(seg_tree_node_t *node);

void seg_tree_check_bounds(seg_tree_node_t *root, int ran_l, int ran_r);

int seg_tree_query(seg_tree_node_t *root, int que_l, int que_r);
int _seg_tree_query(seg_tree_node_t *node, int que_l, int que_r);

void seg_tree_update(seg_tree_node_t *root, int ran_l, int ran_r, int inc);
void _seg_tree_update(seg_tree_node_t *node, int ran_l, int ran_r, int inc);

#endif
