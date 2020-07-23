#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "common.h"
#include "generate-ops.h"
#include "seg-tree-interface.h"

#include "worker.c"

void usage_error(char *prog) {
	fprintf(stderr, "usage: %s [-r range] [-n number of operations] [-t number of threads] [-v (for verbose output)] [-c (for clock)]\n", prog);
	exit(1);
}

int main(int argc, char *argv[]) {
	srand(0);

	int range = -1;
	int num_ops = -1;
	int num_threads = -1;
	bool do_print = false;
	bool do_clock = false;

	int opt;
	while ((opt = getopt(argc, argv, "r:n:t:vc")) != -1) {
		switch (opt) {
			case 'r':
				range = atoi(optarg);
				break;
			case 'n':
				num_ops = atoi(optarg);
				break;
			case 't':
				num_threads = atoi(optarg);
				break;
			case 'v':
				do_print = true;
				break;
			case 'c':
				do_clock = true;
				break;
			default:
				usage_error(argv[0]);
		}
	}
	if (range < 0 || num_ops < 0 || num_threads < 0) {
		usage_error(argv[0]);
	}

	seg_tree_t tree;
	seg_tree_init(&tree, range);

	worker_args_t wargs[num_threads];
	pthread_t p[num_threads];

	for (int i = 0; i < num_threads; i++) {
		wargs[i].tree = &tree;
		wargs[i].do_print = do_print;
		ops_list_init(&wargs[i].ops_list, num_ops, range);
	}

	struct timeval t1, t2;
	assert(gettimeofday(&t1, NULL) == 0);

	for (int i = 0; i < num_threads; i++) {
		pthread_create(&p[i], NULL, worker, &wargs[i]);
	}
	for (int i = 0; i < num_threads; i++) {
		pthread_join(p[i], NULL);
	}

	assert(gettimeofday(&t2, NULL) == 0);

	if (do_clock) {
		printf("total time: %.3f\n", t2.tv_sec-t1.tv_sec +
				(t2.tv_usec-t1.tv_usec) / 1e6);
	}

	seg_tree_destroy(&tree);
	for (int i = 0; i < num_threads; i++) {
		ops_list_destroy(&wargs[i].ops_list);
	}
}
