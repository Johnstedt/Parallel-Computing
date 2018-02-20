#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{

	int thread_count = strtol(argv[1], NULL, 10);

	#pragma omp parallel num_threads(thread_count)
	{

		int num_threads = omp_get_num_threads();
		int my_id = omp_get_thread_num();
		printf("Hello form thread %d within a team of %d\n", my_id, num_threads);

	}
}