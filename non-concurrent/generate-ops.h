#ifndef _GENERATE_OPS_H
#define _GENERATE_OPS_H

#define INC_MIN -5
#define INC_MAX 5

typedef struct _ops_t {
	bool is_query;
	int ran_l;
	int ran_r;
	int inc;
} ops_t;

typedef struct _ops_list_t {
	ops_t *head;
	int num_ops;
} ops_list_t;

void ops_list_init(ops_list_t *ops_list, int num_ops, int range);
void ops_list_print(ops_list_t *ops_list);
void ops_list_destroy(ops_list_t *ops_list);

#endif
