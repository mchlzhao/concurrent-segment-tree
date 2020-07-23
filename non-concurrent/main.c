#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "generate-ops.h"
#include "seg-tree.h"

#include "worker.c"

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("usage: [range] [number of operations] [number of threads]\n");
		exit(1);
	}
	srand(0);
	int range = atoi(argv[1]);
	int num_ops = atoi(argv[2]);
	int num_threads = atoi(argv[3]);
	bool print = true;

	seg_tree_t tree;
	seg_tree_init(&tree, range);

	worker_args_t wargs[num_threads];

	pthread_t p[num_threads];
	for (int i = 0; i < num_threads; i++) {
		wargs[i].tree = &tree;
		wargs[i].print = print;
		ops_list_init(&wargs[i].ops_list, num_ops, range);
		pthread_create(&p[i], NULL, worker, &wargs);
	}
	for (int i = 0; i < num_threads; i++) {
		pthread_join(p[i], NULL);
	}

	seg_tree_destroy(&tree);
	for (int i = 0; i < num_threads; i++) {
		ops_list_destroy(&wargs[i].ops_list);
	}
}
