
#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef THREADS
#define THREADS 1
#endif

#define e 2.71828182846

double sum_local[THREADS];

struct integral_param {

	double start;
	double fin;
	double delta;
	int num;
};

void* integral(void* data) {

	clock_t start_time = 0, fin_time = 0;
	start_time = clock();


	struct integral_param* param = data;

	sum_local[param->num] = 0;

	for( ; param->start < param->fin; param->start += param->delta) {
		double x = param->start + param->delta / 2;

		sum_local[param->num] += pow(e, x) * param->delta;
	}


 	fin_time = clock();

	size_t time = (fin_time - start_time) / (CLOCKS_PER_SEC / 1000);

	printf("time[%u] - %ld mc\n", param->num, time);
}

int main(int argc, char** argv) {

	cpu_set_t *cpusetp[THREADS];
	size_t size[THREADS];
	int num_cpus = 1;

	for(int i = 0; i < THREADS; i++) {
		cpusetp[i] = CPU_ALLOC(1);
		if (cpusetp == NULL) {
		   perror("CPU_ALLOC");
		   exit(EXIT_FAILURE);
		}

		size[i] = CPU_ALLOC_SIZE(num_cpus);
	
		CPU_ZERO_S(size[i], cpusetp[i]);
	}
/*
	if (argc < 2) {
	   fprintf(stderr, "Usage: %s <num-cpus>\n", argv[0]);
	   exit(EXIT_FAILURE);
	}

	num_cpus = atoi(argv[1]);
*/

	for (int cpu = 0; cpu < num_cpus; cpu += 2) {//Hyperthreading

		CPU_SET_S(cpu, size[cpu], cpusetp[cpu]);
		printf("CPU_COUNT() [%d] of set:    %d\n", cpu, CPU_COUNT_S(size[cpu], cpusetp[cpu]));
	}

	

   //    int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
    ///                              const cpu_set_t *cpuset);

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

		params[i].start = global_start + i * interval;
		params[i].fin = global_start + (i + 1) * interval;
		params[i].delta = global_delta;
		params[i].num = i;
		pthread_attr_init(&thread_attrs[i]);
	}

	clock_t start_time = 0, fin_time = 0;
	double sum_global = 0;

	start_time = clock();
	for (int i = 0; i < THREADS; i++) {

		err = pthread_create(&thread_ids[i], NULL, integral, &params[i]);

		err = pthread_setaffinity_np(thread_ids[i], size[i], cpusetp[i]);
		if (err != 0) {
		    perror("CPU_ALLOC");
		    //exit(EXIT_FAILURE);
		}

	}
	for (int i = 0; i < THREADS; i++) {

		err = pthread_join(thread_ids[i], NULL);
	}
	for (int i = 0; i < THREADS; i++) {

		sum_global += sum_local[i];
	}
 	fin_time = clock();

	size_t time = (fin_time - start_time) / (CLOCKS_PER_SEC / 1000);

	printf("integral - %lf\n", sum_global);
	printf("time - %lu mc\n", time);

//	CPU_FREE(cpusetp);

}