#include <pthread.h>
#include <stdbool.h>

#include "common.h"
#include "seg-tree.h"
#include "generate-ops.h"

typedef struct _worker_args_t {
	seg_tree_t *tree;
	ops_list_t ops_list;
	bool print;
} worker_args_t;

void *worker(void *args) {
	worker_args_t *wargs = (worker_args_t*) args;
	ops_t *cur_op = wargs->ops_list.head;
	for (int i = 0; i < wargs->ops_list.num_ops; i++) {
		if (cur_op->is_query) {
			printif(wargs->print, "q %d %d %d\n",
					cur_op->ran_l, cur_op->ran_r,
					seg_tree_query(wargs->tree, cur_op->ran_l, cur_op->ran_r));
		} else {
			seg_tree_update(wargs->tree, cur_op->ran_l, cur_op->ran_r,
					cur_op->inc);
			printif(wargs->print, "u %d %d %d\n",
					cur_op->ran_l, cur_op->ran_r, cur_op->inc);
		}
		cur_op++;
	}
	return NULL;
}
