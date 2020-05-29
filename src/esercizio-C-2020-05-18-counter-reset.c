// Diaa Nehme  IN0500345  esercizio20200422

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>

#include<errno.h>
#include<pthread.h>

#define LIMIT 100000
#define RESET_LIMIT 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int counter;
int reset;

void * thread_function(void * arg){
	int s;
	int op1_counter = 0;
	int op2_counter = 0;

	while(1){
		s = pthread_mutex_lock(&mutex);
		if(s != 0){
			perror("pthread_mutex_lock()");
			exit(EXIT_FAILURE);
		}
		counter++;
		op1_counter++;

		if(counter > LIMIT){
			counter = counter - LIMIT;
			reset++;
			op2_counter++;
		}

		s = pthread_mutex_unlock(&mutex);
		if(s != 0){
			perror("pthread_mutex_unlock()");
			exit(EXIT_FAILURE);
		}

		if(reset > RESET_LIMIT){
			printf("op1_counter = %d, op2_counter = %d\n", op1_counter, op2_counter);
			return NULL;
		}
	}
}

int main(){
	pthread_t t1;
	pthread_t t2;
	void * res; 

	int s;

	s = pthread_create(&t1, NULL, thread_function, NULL);
	if(s != 0){
		perror("pthread_create()");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&t2, NULL, thread_function, NULL);
	if(s != 0){
		perror("pthread_create()");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t1, &res);
	if(s != 0){
		perror("pthread_join()");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t2, &res);
	if(s != 0){
		perror("pthread_join()");
		exit(EXIT_FAILURE);
	}

	printf("Final value of counter = %d, reset = %d\n", counter, reset);
	printf("BYE!");

	return 0;
}
