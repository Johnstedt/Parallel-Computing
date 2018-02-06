#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void) {

	int my_rank, comm_sz, n;

	int data;
	int result;
	
	int series[comm_sz];	

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	data = my_rank + 1;

	MPI_Scan(&data , &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	
	/*if(my_rank == (comm_sz-1)){
		printf("%d\n", result);
	}*/
	
	//series = malloc(comm_sz * sizeof(*series));

	MPI_Gather(&result, 1, MPI_INT, series, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(my_rank == 0){
		for(int i = 0; i < comm_sz; i++) { 
			printf(" %d ", series[i]);
		}
	}

	MPI_Finalize();
	return 0;
}