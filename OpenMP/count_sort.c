#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void count_sort_inner(int n, int in[], int out[], int threads);
void count_sort_outer(int n, int in[], int out[], int threads);
timeval_diff(struct timeval *difference,
             struct timeval *end_time,
             struct timeval *start_time
            )
{
  struct timeval temp_diff;

  if(difference==NULL)
  {
    difference=&temp_diff;
  }

  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }

  return 1000000LL*difference->tv_sec+
                   difference->tv_usec;

} /* timeval_diff() */

int main(int argc, char* argv[]) 
{


	int threads = strtol(argv[1], NULL, 10);
	int work = strtol(argv[2], NULL, 10);

	struct timeval resultsInner[threads+1][work+1];
	struct timeval resultsOuter[threads+1][work+1];

	for(int thread_count = 1; thread_count <= threads; thread_count++){
		for(int n = 10000; n < work*10000+1; n+=10000){
			int in[n];

			for(int i = 0; i < n; i++){
				in[i] = rand();
			}
			int out[n];

			struct timeval start, stop, diff;

			
			gettimeofday(&start, NULL);

			count_sort_inner(n,in,out, thread_count);

			gettimeofday(&stop, NULL);

			//diff = stop.tv_usec - start.tv_usec;
			timeval_diff(&diff,&stop,&start);

			for(int i = 0; i < n; i++){
		//		printf(" %d \n", out[i]);
			}

			
	//		printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
			resultsInner[thread_count][n/10000] = diff;

			gettimeofday(&start, NULL);

			count_sort_outer(n,in,out, thread_count);

			gettimeofday(&stop, NULL);

			timeval_diff(&diff,&stop,&start);

			for(int i = 0; i < n; i++){
			//	printf(" %d \n", out[i]);
			}

	//		printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
			resultsOuter[thread_count][n/10000] = diff;
		}
		printf("Did Done it for %d threads\n", thread_count);
	}

	printf("THIS IS INNER LOOP\n");
	for(int thread_count = 1; thread_count <= threads; thread_count++){
		printf("%d", thread_count);
		for(int n = 10000; n < work*10000+1; n+=10000){
			printf(" & %ld.%ld ", resultsInner[thread_count][n/10000].tv_sec, resultsInner[thread_count][n/10000].tv_usec/1000%1000);
		}
			printf("\n");
	}
	printf("THIS IS OUTER LOOP\n");
	for(int thread_count = 1; thread_count <= threads; thread_count++){
		printf("%d", thread_count);
		for(int n = 10000; n < work*10000+1; n+=10000){
			printf(" & %ld.%ld ", resultsOuter[thread_count][n/10000].tv_sec, resultsOuter[thread_count][n/10000].tv_usec/1000%1000);

		}
		printf("\n");
	}

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

