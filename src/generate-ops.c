#include <stdio.h>
#include <stdlib.h>

#include "generate-ops.h"

void ops_list_init(ops_list_t *ops_list, int num_ops, int range) {
	ops_list->head = (ops_t*) malloc(num_ops * sizeof(ops_t));
	ops_list->num_ops = num_ops;

	ops_t *cur_op = ops_list->head;
	for (int i = 0; i < num_ops; i++) {
		cur_op->is_query = rand()%2;
		cur_op->ran_l = cur_op->ran_r = cur_op->inc = 0;
		while (cur_op->ran_l >= cur_op->ran_r) {
			cur_op->ran_l = rand()%range;
			cur_op->ran_r = rand()%range+1;
		}
		if (!cur_op->is_query) {
			cur_op->inc = rand()%(INC_MAX-INC_MIN+1) + INC_MIN;
		}
		cur_op++;
	}
}

void ops_list_destroy(ops_list_t *ops_list) {
	free(ops_list->head);
}

void ops_list_print(ops_list_t *ops_list) {
	ops_t *cur_op = ops_list->head;
	for (int i = 0; i < ops_list->num_ops; i++) {
		if (cur_op->is_query) {
			printf("query %d %d\n", cur_op->ran_l, cur_op->ran_r);
		} else {
			printf("update %d %d by %d\n", cur_op->ran_l,
					cur_op->ran_r, cur_op->inc);
		}
		cur_op++;
	}
}
