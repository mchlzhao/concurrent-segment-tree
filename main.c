#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "seg-tree.h"

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
