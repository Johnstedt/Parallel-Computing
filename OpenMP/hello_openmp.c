#include <omp.h>
#include <stdio.h>

int main(void) 
{

	#pragma omp parallel 
	{

		int num_threads = omp_get_num_threads();
		int my_id = omp_get_thread_num();
		printf("Hello form thread %d within a team of %d\n", my_id, num_threads);

	}
}