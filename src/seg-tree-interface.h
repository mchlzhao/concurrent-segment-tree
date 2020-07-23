#ifndef _SEG_TREE_INTERFACE_H
#define _SEG_TREE_INTERFACE_H

typedef struct _seg_tree_node_t seg_tree_node_t;

typedef struct _seg_tree_t {
	seg_tree_node_t *root;
	int range;
} seg_tree_t;

void seg_tree_init(seg_tree_t *tree, int range);

void seg_tree_destroy(seg_tree_t *tree);

int seg_tree_query(seg_tree_t *tree, int que_l, int que_r);

void seg_tree_update(seg_tree_t *tree, int ran_l, int ran_r, int inc);

#endif
