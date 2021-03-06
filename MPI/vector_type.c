#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void send_vector(int x, int y, int rows, int columns, int* matrix, int send_to);
int* receive_vector(int x, int y, int rows, int columns, int* matrix, int receive_from);

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
		send_vector(4,4,2,2, matrix, 1);

	} else if(my_rank == 1){

		int *new_matrix = receive_vector(4,4,2,2, matrix, 0);
		for (int i = 0; i < 4; i++) {
			printf("%d, ", new_matrix[i]);

        	if((i+1) % 2 == 0 ){
        		printf("\n");
        	}
    	}
	}

	MPI_Finalize();
	return 0;
}

void send_vector(int x, int y, int rows, int columns, int* matrix, int send_to){

	int count = columns;
	int blocklenght = rows;
	int stride = 6;

	MPI_Datatype new_type;
	MPI_Type_vector(count, blocklenght, stride, MPI_INT, &new_type);
	MPI_Type_commit(&new_type);

	MPI_Send(&matrix[(x*6) + y], 1, new_type, send_to, 0, MPI_COMM_WORLD);	
}

int* receive_vector(int x, int y, int rows, int columns, int* matrix, int receive_from){

	int count = columns;
	int blocklenght = rows;
	int stride = 6;

	MPI_Datatype new_type;
	MPI_Type_vector(count, blocklenght, stride, MPI_INT, &new_type);
	MPI_Type_commit(&new_type);

	MPI_Recv(&matrix[(x*6) + y], 1, new_type, receive_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	

	int *new_matrix = malloc(x*y * sizeof(*new_matrix));
	for(int i = 0; i < columns; i++){
		for(int j = 0; j < rows; j++){
			new_matrix[(i*rows)+j] = matrix[((x+i)*6) + y + j];
	//	printf(" %d ", new_matrix[i]);
		}
	}
	return new_matrix;
}