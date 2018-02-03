#include <stdio.h>
#include <mpi.h>

int main(void) {

	int my_rank, comm_sz, n;

	int data;
	int result;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	data = my_rank+1;

	MPI_Scan(&data , &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	
	if(my_rank == (comm_sz-1)){
		printf("%d\n", result);
	}

	MPI_Finalize();
	return 0;
}
