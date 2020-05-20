#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

void * thread_function();

#define LIMIT 100000
#define RESET_LIMIT 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter;
int reset;

int main() {

	pthread_t t1, t2;
	void * res;
	int s;

	// create thread 1
	s = pthread_create(&t1, NULL, thread_function, NULL);
	if (s != 0) {
		perror("pthread_create()");
		exit(EXIT_FAILURE);
	}

	// create thread 2
	s = pthread_create(&t2, NULL, thread_function, NULL);
	if (s != 0) {
		perror("pthread_create()");
		exit(EXIT_FAILURE);
	}

	// join thread 1
	s = pthread_join(t1, &res);
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	s = pthread_join(t2, &res);
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	printf("reset: %d\n", reset);
	printf("counter: %d\n", counter);

	exit(0);
}

void * thread_function(){

	while(1){
		// acquisizione mutex
		pthread_mutex_lock(&mutex);

		if (reset > RESET_LIMIT){
			// rilascio mutex
			pthread_mutex_unlock(&mutex);
		    break;
		}

		counter++;
		if (counter > LIMIT){
		  counter = counter - LIMIT;
		  reset++;
		  printf("reset: %d\n", reset);
		}

		// rilascio mutex
		pthread_mutex_unlock(&mutex);


	}

	return NULL;
}

