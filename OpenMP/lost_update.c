#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void create_histogram(int *arr);

int tmp_count;

int main(int argc, char* argv[]) 
{

	int thread_count = strtol(argv[1], NULL, 10);

	int global_count[5000];

	for(int j = 0; j < 5000; j++) {

		tmp_count = 0;

		#pragma omp parallel num_threads(thread_count)
		{

			int num_threads = omp_get_num_threads();
			int my_id = omp_get_thread_num();

			for(int i = 0; i < 1000000; i++){
				tmp_count += 1;
			}

			//printf("Hello form thread %d within a team of %d\n", my_id, num_threads);
		}
		printf("%d\n", j);
		global_count[j] = tmp_count;

	}

	//printf("Updating global variable from multiple threads gives %d, should be threads * M \n", global_count[0]);
	create_histogram(global_count);

}

void create_histogram(int *arr){

	int hist[800];
	for(int k=0; k < 800; k++){ 
		hist[k] = 0;
	}

	for(int i = 0; i < 800; i++){
		for(int j = 0; j < 5000; j++){
			if( i*10000 < arr[j] && (i+1)*10000 >= arr[j]){
				hist[i]++;
			}
		}
		printf("(%d, %d) \n", i, hist[i]);
	}

}

