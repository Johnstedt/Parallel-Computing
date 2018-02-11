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

	int pack_buf[100];
	int position = 0;
	for(int yi = 0; yi < rows; yi++){
		for(int xi = 0; xi < columns; xi++){
			MPI_Pack(&matrix[(x+xi)*6 + y+yi], 1, MPI_INT, pack_buf, 100, &position, MPI_COMM_WORLD);
		}
	}

	MPI_Send(pack_buf, 100, MPI_PACKED, send_to, 0, MPI_COMM_WORLD);	
}

int* receive_vector(int x, int y, int rows, int columns, int* matrix, int receive_from){

	int pack_buf[100];

	MPI_Recv(pack_buf, 100, MPI_PACKED, receive_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	

	int position = 0;
	for(int yi = 0; yi < rows; yi++){
		for(int xi = 0; xi < columns; xi++){
			MPI_Unpack(pack_buf, 100, &position, &matrix[(x+xi)*6 + y+yi], 1, MPI_INT, MPI_COMM_WORLD);
		}
	}
	int *new_matrix = malloc(rows*columns * sizeof(*new_matrix));
	for(int i = 0; i < columns; i++){
		for(int j = 0; j < rows; j++){
			new_matrix[(i*rows)+j] = matrix[((x+i)*6) + y + j];
		}
	}
	return new_matrix;
}