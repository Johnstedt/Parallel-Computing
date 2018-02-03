#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void vector_scale(int n, double scale, double x[]) {
	for (int i = 0; i < n; i++) {
		x[i] *= scale;
	}
}

int main(void) {

	double scale;
	int my_rank, comm_sz, n, local_n;
	double *x;
	double *local_x;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	if(my_rank == 0){
		printf("Give scalar\n");
		scanf("%lf", &scale);
		
		printf("Give vector length\n");
		scanf("%d", &n);

		printf("Give vectors\n");

		x = malloc(n * sizeof(*x));

		for(int i = 0; i < n; i++) { 
			scanf("%lf", &x[i]);
		}
	}	

	MPI_Bcast(&scale, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 

	local_n = n/comm_sz;
	local_x = malloc(local_n * sizeof(*local_x));
	MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// All calculate
	vector_scale(local_n, scale, local_x);

	MPI_Gather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	free(local_x);
	
	if(my_rank == 0){
		for(int i = 0; i < n; i++) { 
			printf(" %f ", x[i]);
		}
		free(x);
	}

	MPI_Finalize();
	return 0;
}