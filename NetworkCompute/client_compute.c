
#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <string.h>

#include "structures.h"

#define e 2.71828182846
#define PAGE_SIZE 4096

int num_proc;

int max_int(int first, int second) {
	if (first > second) return first;
	else return second;
}

struct integral_param {

	pthread_t thread_id;
	double start;
	double fin;
	double delta;
	int num;
	double sum_local[PAGE_SIZE / sizeof(double)];
};

void* integral(void* data) {

	struct integral_param* param = data;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
 	
	CPU_SET(param->num, &cpuset);

	int err = pthread_setaffinity_np(param->thread_id, sizeof(cpuset), &cpuset);
	if (err != 0) {
		printf("Err is %d in %d\n", err, param->num);
	    perror("CPU_ALLOC");
	}

	param->sum_local[0] = 0;

	for( ; param->start < param->fin; param->start += param->delta) {

		double x = param->start + param->delta / 2;
		param->sum_local[0] += (pow(e, x) + pow(e, -x)) * param->delta;
	}
}

double compute_integral(int num_threads, struct boards_info general_boards) {

	num_proc = get_nprocs_conf(); // get_nprocs();

	int threads = num_threads;

	if (threads < 1) {
		printf("minimal threads num - 1\n");
	}
	if (threads > num_proc) {
		threads = num_proc;
	}

	pthread_t thread_ids[num_proc];
	struct integral_param params[num_proc];

	double global_start = general_boards.left;
	double global_fin = general_boards.right;
	double global_delta = general_boards.delta;

	double interval = (global_fin - global_start) / threads;

	int err = 0;

	for (int i = 0; i < threads; i++) {

		params[i].start = global_start + i * interval;
		params[i].fin = global_start + (i + 1) * interval;
		params[i].delta = global_delta;
		params[i].num = i;
	}
	for (int i = threads; i < num_proc; i++) {
		
		params[i].start = global_start;
		params[i].fin = global_start + interval;
		params[i].delta = global_delta;
		params[i].num = i;
	}

	double sum_global = 0;

	for (int i = 0; i < num_proc; i++) {

		err = pthread_create(&(params[i].thread_id), NULL, integral, &params[i]);
	}
	for (int i = 0; i < num_proc; i++) {

		err = pthread_join(params[i].thread_id, NULL);
		if (i < threads) sum_global += params[i].sum_local[0];
	}

	return sum_global;
}