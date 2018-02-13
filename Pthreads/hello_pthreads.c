#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 4

//  Context  shared  by all  threads.
struct global_context {

	int num_threads;
	pthread_t threads[MAX_THREADS];
};

//  Context  private  to one  thread.
struct local_context {
	struct global_context *global;
	int id;
};
void *func(void *arg) {
	struct local_context *local = arg;
	struct global_context *global = local->global;
	printf("Hello  from  thread %d out of %d\n",
		local->id , global->num_threads );
	free(local);
	return NULL;
}
int main(int argc, char *argv []) {
	struct global_context  global = {
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
}