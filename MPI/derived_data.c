#include <stdio.h>
#include <mpi.h>

void Build_mpi_type(
	int* a_p /* in */,
	int* b_p /* in */,
	int* n_p /* in */,
	MPI_Datatype* input_mpi_t_p /* out */);

int main(void) {

	int my_rank, comm_sz;
	MPI_Datatype input_mpi_t;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	int m;
	int n;
	int matrix[16];
	Build_mpi_type(&m, &n, matrix, &input_mpi_t);

	if(my_rank == 0){
	
		int m = 4; 
		int n = 4; 
		for(int i = 0; i < 16; i++){
			matrix[i] = 23;
		}
		MPI_Send(&m, 1, input_mpi_t, 1, 0, MPI_COMM_WORLD);

	} else if(my_rank == 1){

		MPI_Recv(&m, 1, input_mpi_t, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d", matrix[4]);
	}

	MPI_Finalize();
	return 0;
}

void Build_mpi_type(
	int* a_p /* in */,
	int* b_p /* in */,
	int* n_p /* in */,
	MPI_Datatype* input_mpi_t_p /* out */) {

	int array_of_blocklengths[3] = {1, 1, 16};
	MPI_Datatype array_of_types[3] = {MPI_INT, MPI_INT, MPI_INT};

	MPI_Aint a_addr, b_addr, n_addr;
	MPI_Aint array_of_displacements[3] = {0};
	MPI_Get_address(a_p, &a_addr);
	MPI_Get_address(b_p, &b_addr);
	MPI_Get_address(n_p, &n_addr);

	array_of_displacements[1] = b_addr - a_addr;
	array_of_displacements[2] = n_addr - a_addr;

	MPI_Type_create_struct(3, array_of_blocklengths,
		array_of_displacements, array_of_types,
		input_mpi_t_p);
	MPI_Type_commit(input_mpi_t_p);
}