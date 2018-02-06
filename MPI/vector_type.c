#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void send_vector(int count, int blocklenght, int stride, int* matrix, int send_to);
void receive_vector(int count, int blocklenght, int stride, int* matrix, int receive_from);

int main(void) {

	int my_rank, comm_sz;
	MPI_Datatype input_mpi_t;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	int *matrix;
	matrix = malloc(36 * sizeof(*matrix));

	for(int i = 0; i < 36; i++){
			matrix[i] = 0;
		}

	if(my_rank == 0){
		for(int i = 0; i < 36; i++){
			matrix[i] = i;
		}
		send_vector(6, 3, 6, matrix, 1);

	} else if(my_rank == 1){

		receive_vector(6, 3, 6, matrix, 0);
	}

	MPI_Finalize();
	return 0;
}

void send_vector(int count, int blocklenght, int stride, int* matrix, int send_to){

	MPI_Datatype new_type;
	MPI_Type_vector(count, blocklenght, stride, MPI_INT, &new_type);
	MPI_Type_commit(&new_type);

	MPI_Send(matrix, 1, new_type, send_to, 0, MPI_COMM_WORLD);	
}

void receive_vector(int count, int blocklenght, int stride, int* matrix, int receive_from){

	MPI_Datatype new_type;
	MPI_Type_vector(count, blocklenght, stride, MPI_INT, &new_type);
	MPI_Type_commit(&new_type);

	MPI_Recv(matrix, 1, new_type, receive_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	

/* Loop through the array it was received into, printing values */
    for (int i = 0; i < 36; i++)
    {
        printf("%d\n", matrix[i]);
    }
}