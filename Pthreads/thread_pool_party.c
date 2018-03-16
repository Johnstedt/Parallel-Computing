#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_THREADS 4

typedef enum { false, true } bool;

//Function declarations
void thread_init(int p);
void tp_stop(void);
void tp_execute(void (*work)(void*), void *arg);
void *func(void *arg);

//  Context  shared  by all  threads.
struct global_context {
	int num_threads;
	pthread_t threads[MAX_THREADS];
	bool quit;
	void (*function)(void*);
	void* arguments;
};

struct global_context global;

pthread_cond_t cond;
pthread_mutex_t condLock;

pthread_mutex_t thread;
pthread_mutex_t thwrite;

sem_t semaphore;

//First dummy function to test implementation
void doWork(void *arg) {
	int *local = arg;

	int hej = 0;
	for(int i = 0; i < 100000; i++){
		hej = i * i + i; 
	}

	printf("first work done with arg = %d\n", *local);
}

//Second dummy function to test implementation
void doOtherWork(void *arg) {
	int *local = arg;

	int hej = 0;
	for(int i = 0; i < 100000; i++){
		hej = i * i + i; 
	}

	printf("second work done with arg= %d\n", *local);
}

// Quick main to test implementation
int main(int argc, char *argv []) {

	thread_init(MAX_THREADS);
	int theSuperArgument = 5;
	int secondSuperArgument = 9;
	
	tp_execute(doWork, &theSuperArgument);
	tp_execute(doOtherWork, &secondSuperArgument);
	tp_execute(doOtherWork, &secondSuperArgument);
	tp_execute(doOtherWork, &theSuperArgument);
	tp_execute(doWork, &theSuperArgument);
	tp_execute(doWork, &theSuperArgument);
	tp_execute(doWork, &secondSuperArgument);

	// Not necesary but might quit before all work is executed
	sleep(1);
	tp_stop();
}

//Start the thread pool with p worker threads
void thread_init(int p){

	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&thread, NULL);
	pthread_mutex_init(&thwrite, NULL);
	sem_init(&semaphore, 1, 0);

	global.num_threads = p;
	global.quit = false;	

	for(int i = 0; i < p; i++) {
		pthread_create(&global.threads[i], NULL , func, NULL);
	}
}

void *func(void *arg) {

	void (*funcToExec)(void*);
	void* args;

	printf("thread started\n");	
	while(!global.quit) {

		pthread_mutex_lock(&condLock);
		sem_post(&semaphore);
		pthread_cond_wait(&cond, &condLock);
		pthread_mutex_unlock(&condLock);
		
		if(global.quit){
			printf("thread quit\n");
			pthread_exit(NULL);
		}
		printf("woke up thread!\n");

		pthread_mutex_lock(&thread);
		funcToExec = global.function;
		args = global.arguments;
		pthread_mutex_unlock(&thwrite);
		
		funcToExec(args);
	}
	printf("thread quit\n");
	pthread_exit(NULL);
}

//Stop all worker threads
void tp_stop(void){

	global.quit = true;
	pthread_cond_broadcast(&cond);

	for(int i = 0; i < global.num_threads; i++)
		pthread_join(global.threads[i], NULL);
}

//Execute work on some worker
void tp_execute(void (*work)(void*), void *arg){
	
	
	sem_wait(&semaphore);
	pthread_mutex_lock(&thwrite);
	global.function = work;
	global.arguments = arg;
	pthread_mutex_unlock(&thread);
	
	pthread_mutex_lock(&condLock);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&condLock);
}