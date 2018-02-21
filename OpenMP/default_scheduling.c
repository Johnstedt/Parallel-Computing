#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc, char* argv[]) 
{

	int thread_count = strtol(argv[1], NULL, 10);

	int arr[thread_count*2];

	#pragma omp parallel for num_threads(thread_count) 
		for(int i=0; i < thread_count*2; i++)
		{
			int my_id = omp_get_thread_num();
			arr[i] = my_id;
			//int num_threads = omp_get_num_threads();
		}
		//printf("Hello form thread %d within a team of %d\n", my_id, num_threads);

		int start, end;
	for(int j=0;j<thread_count;j++){
		start = thread_count*2+1;
		end = 0;
		for(int i=0;i<thread_count*2;i++){
			if(arr[i] == j){
				start = MIN(i,start);
				end = i;
			}
		}
		printf("Thread %d: Iteration %d -- %d\n", j, start, end);
	}
	
}