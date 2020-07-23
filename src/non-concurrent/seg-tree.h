#ifndef _SEG_TREE_H
#define _SEG_TREE_H

void _seg_tree_init(seg_tree_node_t *node, int ran_l, int ran_r);

void _seg_tree_destroy(seg_tree_node_t *root);

void _seg_tree_clean_node(seg_tree_node_t *node);

void _seg_tree_check_bounds(seg_tree_node_t *root, int ran_l, int ran_r);

int _seg_tree_query(seg_tree_node_t *node, int que_l, int que_r);

void _seg_tree_update(seg_tree_node_t *node, int ran_l, int ran_r, int inc);

#endif
