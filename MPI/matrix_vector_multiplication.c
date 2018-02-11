#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//void createType(int n, int columns, MPI_Datatype type);
int* receive_vector(int x, int y, int rows, int columns, int* matrix, int receive_from);

int main(void) {

	int my_rank, comm_sz, local_n;
	int* local_a;
	MPI_Datatype input_mpi_t;

	int n = 10;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	int *A;
	A = malloc(n*n * sizeof(*A));

	int *v;
	v = malloc(n * sizeof(*v));

	int *w;
	w = malloc(n * sizeof(*w));

	if(my_rank == 0){
		for(int i = 0; i < n*n; i++){
			A[i] = i;
		}

		for(int j = 0; j < n; j++){
			v[j] = j;
		}
	} 

	int *local_v;
	local_v = malloc(n/comm_sz * sizeof(*local_v));

	MPI_Scatter(v, n/comm_sz, MPI_INT, local_v, n/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Datatype type, resize_type;

	int sizes[2]    = {n, n};  /* size of global array */
    int subsizes[2] = {n, n/comm_sz};  /* size of sub-region */
    int starts[2]   = {0,0};  /* let's say we're looking at region "0",
                                 which begins at index [0,0] */

    MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_INT, &type);
    //cMPI_Type_commit(&type);

	MPI_Type_create_resized(type, 0, n/comm_sz * sizeof(int), &resize_type);
	MPI_Type_commit(&resize_type);

	local_n = n/comm_sz;
	local_a = malloc(n/comm_sz * n * sizeof(*local_a));

	int *counts = malloc(n * sizeof(*counts));
	for(int i = 0; i < n; i++){
		counts[i] = 1;//n/comm_sz;
	}

	int *displs = malloc(n * sizeof(*displs));
	for(int i = 0; i < n; i++){
		displs[i] = i;
	}

	MPI_Scatterv(A, counts, displs, resize_type, local_a, n*n/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);

	int *local_w;
	local_w = calloc(n, sizeof(*local_w));
	for(int i = 0; i < n; i++){	
		for(int j = 0; j < (n/comm_sz); j++){
			
			local_w[i] += local_a[(i * n/comm_sz) + j] * local_v[j];
		}
	}

	MPI_Reduce(local_w, w, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(my_rank == 0){
		for (int i = 0; i < n; i++) {
			printf("%d, ", w[i]);
		}
	}
	

	MPI_Finalize();
	return 0;
}
