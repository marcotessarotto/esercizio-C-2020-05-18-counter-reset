#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>

#define LIMIT 100000
#define RESET_LIMIT 10

int counter;
int reset;

sem_t * mutex;

#define DEBUG

#define CHECK_ERR(a, msg){if(a!=0) {perror(msg); exit(EXIT_FAILURE); } }

void * thread_function1(){

	void * retval = NULL;

	while(1){

	if (sem_wait(mutex) == -1) {
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
   counter++;

#ifdef DEBUG
   	   	   	   printf("%d\n", counter);
#endif

   if (sem_post(mutex) == -1) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	}

   if (counter > LIMIT) {
	  counter = counter - LIMIT;

	  if (sem_wait(mutex) == -1) {
	  		perror("sem_wait");
	  		exit(EXIT_FAILURE);
	  	}
	  reset++;


	  if (sem_post(mutex) == -1) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	  	}

	 }

   if (reset > RESET_LIMIT){
	   pthread_exit(retval);
	   return retval;

   }
}

	 // thread termina
}


void * thread_function2(){

	void * retval = NULL;

	while(1){

	if (sem_wait(mutex) == -1) {
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
   counter++;

#ifdef DEBUG
   	   	   	   printf("%d\n", counter);
#endif

   if (sem_post(mutex) == -1) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	}

   if (counter > LIMIT) {
	  counter = counter - LIMIT;

	  if (sem_wait(mutex) == -1) {
	  		perror("sem_wait");
	  		exit(EXIT_FAILURE);
	  	}
	  reset++;
#ifdef DEBUG
   	   	   	   printf("%d\n", reset);
#endif

	  if (sem_post(mutex) == -1) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	  	}

	 }

   if (reset > RESET_LIMIT){
	   pthread_exit(retval);
	   return retval;

   }

	 // thread termina
	}
}



int main() {
	int res;

	mutex = malloc(sizeof(sem_t));

	res = sem_init(mutex,
			0, // 1 => il semaforo è condiviso tra processi,
			   // 0 => il semaforo è condiviso tra threads del processo
			1 // valore iniziale del semaforo (se mettiamo 0 che succede?)
		  );

	pthread_t thread1;
	pthread_t thread2;

	res = pthread_create(&thread1, NULL, thread_function1, NULL);
	CHECK_ERR(res, "pthread_create()");

	res = pthread_create(&thread2, NULL, thread_function2, NULL);
	CHECK_ERR(res, "pthread_create()");


	res = pthread_join(thread1, NULL);
	CHECK_ERR(res, "pthread_join()");

	res = pthread_join(thread2, NULL);
	CHECK_ERR(res, "pthread_join()");

	printf("final value of shared counter = %d\n", counter);

	return 0;
}
