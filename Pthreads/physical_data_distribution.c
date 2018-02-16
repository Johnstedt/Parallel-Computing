#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 4
#define MATRIX_SIZE 8 


void generate_dummy_matrix(int size, int* matrix, int seed);
void generate_dummy_vector(int size, int* vector);
void print_res(int sources, int size, int** array);

//  Context  shared  by all  threads.
struct global_context {

	int num_threads;
	pthread_t threads[MAX_THREADS];
	int* pointer_to_rows[MAX_THREADS];
	int* pointer_to_blocks[MAX_THREADS];
	int* pointer_to_res[MAX_THREADS];
};

//  Context  private  to one  thread.
struct local_context {
	struct global_context *global;
	int id;
};

void *func(void *arg) {
	struct local_context *local = arg;
	struct global_context *global = local->global;
	
	int *local_matrix = calloc(MATRIX_SIZE*MATRIX_SIZE/MAX_THREADS, sizeof(*local_matrix));
	int *local_vector = calloc(MATRIX_SIZE/MAX_THREADS, sizeof(*local_vector));
	int *local_res = calloc(MATRIX_SIZE/MAX_THREADS, sizeof(*local_res));

	generate_dummy_matrix(MATRIX_SIZE*MATRIX_SIZE/MAX_THREADS, local_matrix, local->id);
	generate_dummy_vector(MATRIX_SIZE, local_vector);

	for(int i = 0; i < MATRIX_SIZE/MAX_THREADS; i++){
		for(int j = 0; j < MATRIX_SIZE; j++){
			local_res[i] = local_matrix[(i*MATRIX_SIZE)+j]*local_vector[j];
		}
	}

	//int** pointer_to_row = global->pointer_to_rows;
	global->pointer_to_rows[local->id] = local_matrix;
	global->pointer_to_blocks[local->id] = local_vector;
	global->pointer_to_res[local->id] = local_res;

	//printf("Hello  from  thread %d out of %d\n",
	//	local->id , global->num_threads );
	free(local);
	return NULL;
}
int main(int argc, char *argv []) {
	struct global_context global = {
		.num_threads = MAX_THREADS
	};
	for(int i = 0; i < global.num_threads; i++) {
		struct local_context *local = malloc(sizeof *local);
		local->global = &global;
		local->id = i;
		pthread_create(&global.threads[i], NULL , func, local);
	}
	for(int i = 0; i < global.num_threads; i++)
		pthread_join(global.threads[i], NULL);

	print_res(MAX_THREADS, MATRIX_SIZE/MAX_THREADS, global.pointer_to_res);
}

void generate_dummy_matrix(int size, int* matrix, int seed){

	for(int i = 0; i < size; i++){
		matrix[i] = (i+1)*(seed+1);
	}
}

void generate_dummy_vector(int size, int* vector){

	for(int i = 0; i < size; i++){
		vector[i] = i+1;
	}
}

void print_res(int sources, int size, int** array){
	for(int i = 0; i < sources; i++){
		int* source =  array[i];
		for(int j = 0; j < size; j++){
			printf(" %d ", source[j]);
		}
	}
}