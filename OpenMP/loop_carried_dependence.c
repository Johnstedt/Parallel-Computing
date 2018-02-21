#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{

	int thread_count = strtol(argv[1], NULL, 10);

	int n = 50;
	int a[50];

	#pragma omp parallel for num_threads(thread_count)
	for(int i = 0; i < n; i++)
	{
		a[i] =  0.5*i*(i+1);
	}

	
	for (int i = 1; i < n; i++){		
		printf(" %d ", a[i]);
	}

}
