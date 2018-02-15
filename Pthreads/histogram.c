#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 4

//  Context  shared  by all  threads.
struct global_context {
	int num_threads;
	pthread_t threads[MAX_THREADS];
	pthread_mutex_t lock;
	int histogram[256];
	int giganticReadArray[5000];
};

//  Context  private  to one  thread.
struct local_context {
	struct global_context *global;
	int id;
	int length;
};
void *func(void *arg) {

	struct local_context *local = arg;
	struct global_context *global = local->global;

	int* local_hist = calloc(local->length, sizeof(*local_hist));
	
	int work = 5000 / global->num_threads;
	int start = local->id * work;
	int last = start + work;

	for(int i = start; i < last; i++){
		local_hist[global->giganticReadArray[i]] += 1; 
	}

	pthread_mutex_lock(&global->lock);
		for(int i = 0; i < 256; i++){
			global->histogram[i] += local_hist[i];
		}
	pthread_mutex_unlock(&global->lock);

	free(local);
	return NULL;
}
int main(int argc, char *argv []) {

	struct global_context global = {
		.num_threads = MAX_THREADS
	};

	for(int j = 0; j < 19; j++){
		for(int i = 0; i < 256; i++){
			global.giganticReadArray[(j*256)+i] = i;
		}
	}

	//init lock
	pthread_mutex_init(&global.lock, NULL);

	for(int i = 0; i < global.num_threads; i++) {
		struct local_context *local = malloc(sizeof *local);
		local->global = &global;
		local->id = i;
		local->length = 256;
		pthread_create(&global.threads[i], NULL , func, local);
	}
	for(int i = 0; i < global.num_threads; i++)
		pthread_join(global.threads[i], NULL);

	for(int i = 0; i < 256; i++)
		printf("%d\n", global.histogram[i]);
}