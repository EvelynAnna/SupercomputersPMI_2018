// Лабораторная работа Домбровской Анны 4.2
// OMPBegin22.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <stdio.h>
#include <omp.h> 
#include <iostream>

double func(int i, double x, int N) {
	double jsum = 0;
	for (int j = 1; j <= i + N; ++j) {
		jsum += (j + pow(x + j, 1/5)) / (2 * i*j - 1);
	}
	return 1 / jsum;
}

double F(double x, int N) {
	double sum = 0;
	for (int i = 1; i <= N; ++i) {
		sum += func(i, x, N);
	}
	return sum;
}

double parallelF(double x, int N) {
	double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
	int count1 = 0, count2 = 0, count3 = 0, count4 = 0;
	double total_time_1 = 0, total_time_2 = 0, total_time_3 = 0, total_time_4 = 0;
#pragma omp parallel num_threads(4)
	{
#pragma omp for
		for (int i = 1; i <= N; ++i) {
			if (omp_get_thread_num() == 0) {

				double start_1_thread = omp_get_wtime();

				sum1 += func(i, x, N);
				++count1;

				double end_1_thread = omp_get_wtime();
				total_time_1 += end_1_thread - start_1_thread;
			}
			if (omp_get_thread_num() == 1) {

				double start_2_thread = omp_get_wtime();

				sum2 += func(i, x, N);
				++count2;

				double end_2_thread = omp_get_wtime();
				total_time_2 += end_2_thread - start_2_thread;
			}
			if (omp_get_thread_num() == 2) {

				double start_3_thread = omp_get_wtime();

				sum3 += func(i, x, N);
				++count3;

				double end_3_thread = omp_get_wtime();
				total_time_3 += end_3_thread - start_3_thread;
			}
			if (omp_get_thread_num() == 3) {

				double start_4_thread = omp_get_wtime();

				sum4 += func(i, x, N);
				++count4;

				double end_4_thread = omp_get_wtime();
				total_time_4 += end_4_thread - start_4_thread;
			}
		}
#pragma omp single 
		{
			printf(" num_procs: %d\n", omp_get_num_procs());
			printf(" num_threads: %d\n", omp_get_num_threads());
		}
	}
	printf(" thread_num: 3 Count: %d Thread time: %lf\n", count4, total_time_4);
	printf(" thread_num: 2 Count: %d Thread time: %lf\n", count3, total_time_3);
	printf(" thread_num: 1 Count: %d Thread time: %lf\n", count2, total_time_2);
	printf(" thread_num: 0 Count: %d Thread time: %lf\n", count1, total_time_1);
	return sum1 + sum2 + sum3 + sum4;
}

int main(int argc, char *argv[])
{
	double start_time, end_time;
	double x = 2;
	int N = 10000;

	start_time = omp_get_wtime();
	double s1 = F(x, N);
	end_time = omp_get_wtime();
	printf(" Non-parallel time: %lf\n", end_time - start_time);

	double start_parallel_time, end_parallel_time;

	start_parallel_time = omp_get_wtime();

	double s2 = parallelF(x, N);

	end_parallel_time = omp_get_wtime();

	printf(" Total parallel time: %lf\n", end_parallel_time - start_parallel_time);

	printf(" Rate: %lf\n", (end_time - start_time) / (end_parallel_time - start_parallel_time));

	printf(" Sum: %d == %d\n", s1, s2);

	return 0;
}