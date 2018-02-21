#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void count_sort_inner(int n, int in[], int out[], int threads);
void count_sort_outer(int n, int in[], int out[], int threads);

int main(int argc, char* argv[]) 
{

	int thread_count = strtol(argv[1], NULL, 10);

	int in[] = {1,3,4,2,2,19,15,899,7623,2,111,5432,13,7,788,56};
	int out[16];
	int n = 16;


	clock_t start = clock(), diff;

	count_sort_inner(n,in,out, thread_count);

	diff = clock() - start;

	for(int i = 0; i < n; i++){
		printf(" %d \n", out[i]);
	}

	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds %d \n", msec/1000, msec%1000, diff);

	start = clock();

	count_sort_outer(n,in,out, thread_count);

	diff = clock() - start;

	for(int i = 0; i < n; i++){
		printf(" %d \n", out[i]);
	}

	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds %d \n", msec/1000, msec%1000, diff);

}

void count_sort_inner(int n, int in[], int out[], int threads)
{

	for (int i = 0; i < n; i++) {
		int x = in[i];
		int count = 0;
		#pragma omp parallel for num_threads(threads) \
			reduction(+: count)
		for (int j = 0; j < n; j++) {
			if (in[j] < x || (in[j] == x && j < i)){
				count += 1;
			}
		}
		out[count] = x;
	}
}

void count_sort_outer(int n, int in[], int out[], int threads)
{

	#pragma omp parallel for num_threads(threads) 
	for (int i = 0; i < n; i++) {
		int x = in[i];
		int count = 0;
		for (int j = 0; j < n; j++) {
			if (in[j] < x || (in[j] == x && j < i)){
				count += 1;
			}
		}
		out[count] = x;
	}
}