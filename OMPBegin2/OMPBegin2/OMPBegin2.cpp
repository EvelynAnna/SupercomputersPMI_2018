// Лабораторная работа Дамбровской Анны 4.2 
// OMPBegin2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <stdio.h>
#include <omp.h> 
#include <iostream>


double F(double x, int N) {
	double sum = 0;
	for (int i = 1; i <= N; ++i) {
		double jsum = 0;
		for (int j = 1; j <= i + N; ++j) {
			jsum += (j + pow(x + j, 1 / 3)) / (2 * i*j - 1);
		}
		sum += 1 / jsum;
	}
	return sum;
}

double parallelF(double x, int N) {
	double sum1 = 0, sum2 = 0;
	int count1 = 0, count2 = 0;
	double start_1_thread, end_1_thread, start_2_thread, end_2_thread;
#pragma omp parallel num_threads(2)
	{
		if (omp_get_thread_num() == 0) {
			start_1_thread = omp_get_wtime();
			for (int i = 2; i <= N; i += 2) {
				double jsum = 0;
				for (int j = 1; j <= i + N; ++j) {
					jsum += (j + pow(x + j, 1 / 3)) / (2 * i*j - 1);
				}

				sum1 += 1 / jsum;
				++count1;
			}
			end_1_thread = omp_get_wtime();
		}

		if (omp_get_thread_num() == 1) {
			start_2_thread = omp_get_wtime();
			for (int i = 1; i <= N; i += 2) {
				double jsum = 0;
				for (int j = 1; j <= i + N; ++j) {
					jsum += (j + pow(x + j, 1 / 3)) / (2 * i*j - 1);
				}

				sum2 += 1 / jsum;
				++count2;
			}
			end_2_thread = omp_get_wtime();
		}

#pragma omp single 
		{
			printf(" num_procs: %d\n", omp_get_num_procs());
			printf(" num_threads: %d\n", omp_get_num_threads());
		}
	}
	printf(" thread_num: 1 Count: %d Thread time: %lf\n", count2, end_2_thread - start_2_thread);
	printf(" thread_num: 0 Count: %d Thread time: %lf\n", count1, end_1_thread - start_1_thread);
	return sum1 + sum2;
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

	printf(" Rate: %lf\n", (end_time - start_time)/(end_parallel_time - start_parallel_time));

	printf(" Sum: %d == %d\n", s1, s2);

	return 0;
}