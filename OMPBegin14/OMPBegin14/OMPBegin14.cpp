// Лабораторная работа Домбровской Анны 4.2
// OMPBegin14.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <stdio.h> 
#include <omp.h> 
#include <iostream>


double func(int i, double x, int N) {
	double jsum = 0;
	for (int j = 1; j <= i + N; ++j) {
		jsum += (j + pow(x + j, 1 / 4)) / (2 * i*j - 1);
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

	int K = 0;
	for (int i = 1; i <= N; ++i) {
		++K;
	}

	std::cout << K << " iterations" << std::endl;

	int K0 = K / 4;

	std::cout << K0 << " i-s" << std::endl;

	int K1 = 0, K2 = 0, K3 = 0;
	double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
	int count1 = 0, count2 = 0, count3 = 0, count4 = 0;
	double start_1_thread, end_1_thread, start_2_thread, end_2_thread, start_3_thread, end_3_thread, start_4_thread, end_4_thread;

#pragma omp parallel num_threads(4)
	{
#pragma omp sections
		{
#pragma omp section
			{
				start_1_thread = omp_get_wtime();
				for (int i = 1; count1 < K0; ++i) {
					sum1 += func(i, x, N);
					++count1;
				}
				end_1_thread = omp_get_wtime();
				K1 = K0;
			}

#pragma omp section
			{	
				//int K1 = K0;
				start_2_thread = omp_get_wtime();
				for (int i = K1 + 1; count2 < K0; ++i) {
					sum2 += func(i, x, N);
					++count2;
				}
				end_2_thread = omp_get_wtime();
				K2 = K1 + count2;
			}

#pragma omp section
			{ 
				start_3_thread = omp_get_wtime();
				for (int i = K2 + 1; count3 < K0; ++i) {
					sum3 += func(i, x, N);
					++count3;
				}
				end_3_thread = omp_get_wtime();
				K3 = K2 + count3;
			}

#pragma omp section
			{ 
				start_4_thread = omp_get_wtime();
				for (int i = K3 + 1; i <= N; ++i) {
					sum4 += func(i, x, N);
					++count4;
				}
				end_4_thread = omp_get_wtime();
			}
		}
#pragma omp single 
		{
			printf(" num_procs: %d\n", omp_get_num_procs());
			printf(" num_threads: %d\n", omp_get_num_threads());
		}
	}

	printf(" thread_num: 3 Count: %d Thread time: %lf\n", count4, end_4_thread - start_4_thread);
	printf(" thread_num: 2 Count: %d Thread time: %lf\n", count3, end_3_thread - start_3_thread);
	printf(" thread_num: 1 Count: %d Thread time: %lf\n", count2, end_2_thread - start_2_thread);
	printf(" thread_num: 0 Count: %d Thread time: %lf\n", count1, end_1_thread - start_1_thread);

	return sum1 + sum2 + sum3 + sum4;
}

int main(int argc, char *argv[])
{
	double start_time, end_time;
	int x = 2, N = 10000;

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

	printf(" Sum: %lf == %lf\n", s1, s2);

	return 0;
}
