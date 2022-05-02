
#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sysinfo.h>

#ifndef THREADS
#define THREADS 1
#endif

#define e 2.71828182846
#define PAGE_SIZE 4096

double sum_local[THREADS][PAGE_SIZE / sizeof(double)];

struct integral_param {

	pthread_t thread_id;
	double start;
	double fin;
	double delta;
	int num;
};

void* integral(void* data) {

	struct integral_param* param = data;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(param->num * 2, &cpuset);

	int err = pthread_setaffinity_np(param->thread_id, sizeof(cpuset), &cpuset);
	if (err != 0) {
		printf("Err is %d in %d\n", err, param->num);
	   perror("CPU_ALLOC");
	    //exit(EXIT_FAILURE);
	}

	sum_local[param->num][0] = 0;

	for( ; param->start < param->fin; param->start += param->delta) {

		double x = param->start + param->delta / 2;
		sum_local[param->num][0] += (pow(e, x) + pow(e, -x)) * param->delta;
	}

	//CPU_FREE(&cpuset);
}

int main(int argc, char** argv) {

	printf("You have %ld proc - %d\n", sysconf(_SC_NPROCESSORS_ONLN), get_nprocs());

/*
	if (argc < 2) {
	   fprintf(stderr, "Usage: %s <num-cpus>\n", argv[0]);
	   exit(EXIT_FAILURE);
	}

	num_cpus = atoi(argv[1]);
*/	

	printf("Threads - %d\n", THREADS);

	pthread_t thread_ids[THREADS];
	pthread_attr_t thread_attrs[THREADS];
	struct integral_param params[THREADS];

	double global_start = -10;
	double global_fin = 10;
	double global_delta = 0.00000005;

	double interval = (global_fin - global_start) / THREADS;

	int err = 0;

	for (int i = 0; i < THREADS; i++) {

		// params[i].thread_id ?
		params[i].start = global_start + i * interval;
		params[i].fin = global_start + (i + 1) * interval;
		params[i].delta = global_delta;
		params[i].num = i;
		pthread_attr_init(&thread_attrs[i]);
	}

	double sum_global = 0;

	for (int i = 0; i < THREADS; i++) {

		err = pthread_create(&(params[i].thread_id), NULL, integral, &params[i]);
	}
	for (int i = 0; i < THREADS; i++) {

		err = pthread_join(params[i].thread_id, NULL);
		sum_global += sum_local[i][0];
	}

	printf("integral - %lf\n", sum_global);
}