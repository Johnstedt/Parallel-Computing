#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double dot_product(int n, double x[], double y[]) {
	double s = 0;
	for (int i = 0; i < n; i++) {
		s += x[i] * y[i];
	}
	
	return s;
}

int main(void) {

	double scale;
	int my_rank, comm_sz, n, local_n;
	double *x, *y;
	double *local_x, *local_y;
	double *gs;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	if(my_rank == 0){

		gs = malloc(comm_sz * sizeof(*gs));
		
		printf("Give vector length\n");
		scanf("%d", &n);

		printf("Give first vectors\n");

		x = malloc(n * sizeof(*x));

		for(int i = 0; i < n; i++) { 
			scanf("%lf", &x[i]);
		}
		
		printf("Give second vectors\n");

		y = malloc(n * sizeof(*y));

		for(int i = 0; i < n; i++) { 
			scanf("%lf", &y[i]);
		}
	}	

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 

	local_n = n/comm_sz;
	local_x = malloc(local_n * sizeof(*local_x));
	local_y = malloc(local_n * sizeof(*local_y));
	MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(y, local_n, MPI_DOUBLE, local_y, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// All calculate
	double s = dot_product(local_n, local_x, local_y);

	MPI_Gather(&s, 1, MPI_DOUBLE, gs, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	free(local_x);
	free(local_y);
	
	if(my_rank == 0){

		double g_sum = 0;
		for(int i = 0; i < comm_sz; i++) { 
			g_sum += gs[i];
		}
		printf(" %f ", g_sum);
		free(x);
		free(y);
	}

	MPI_Finalize();
	return 0;
}