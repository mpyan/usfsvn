/* File: pth_bitonic.c
 * Author: Mark Yan
 *
 * Purpose: sort
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"

const int RMAX = 999999;

int thread_count;
int n;
int* good_list;
int* temp_list;

void Usage(char* prog_name);
void* Thread_work(void* rank);
void Print_list(char* text, int list[]){
	int i;
	printf("%s", text);
	for (i = 0; i < n; i++){
		printf("%d ", list[i]);
	}
	printf("\n");
}
void Generate_list(){
	srandom(1);
	int i;
	for (i = 0; i < n; i++){
		good_list[i] = random() % RMAX;
	}
}
void Get_list(){
	int i;
	for (i = 0; i < n; i++){
		scanf("%d", &good_list[i]);
	}
}

int main(int argc, char* argv[]) {
	long thread;
	pthread_t* thread_handles;
	int has_g = 0;
	int has_o = 0;
	double start, finish;

	/* Check input args */
	if (argc < 3) Usage(argv[0]);
	thread_count = strtol(argv[1], NULL, 10);
   	n = strtol(argv[2], NULL, 10);
   	thread_handles = malloc(thread_count*sizeof(pthread_t));
   	good_list = malloc(n*sizeof(int));
	temp_list = malloc(n*sizeof(int));

	if (argc > 3){
		if (strcmp(argv[3], "g") == 0){
			has_g = 1;
		}
		else if (strcmp(argv[3], "o") == 0)
			has_o = 1;
		if (argc > 4){
			if (strcmp(argv[4], "g") == 0)
				has_g = 1;
			else if (strcmp(argv[4], "o") == 0)
				has_o = 1;
		}
	}

	/* Create the shared list */
	if (has_g == 1){
		/* generate list */
		Generate_list();
	} else {
		/* read in the list from stdin */
		Get_list();
	}
	if (has_o == 1) /* maybe use memcpy here, move this code elsewhere? */
		Print_list("original: ", good_list);

	/* Start work */
	GET_TIME(start);
	for (thread = 0; thread < thread_count; thread++)
      	pthread_create(&thread_handles[thread], NULL,
          	Thread_work, (void*) thread);

   	for (thread = 0; thread < thread_count; thread++) 
      	pthread_join(thread_handles[thread], NULL);
	GET_TIME(finish);
	/* End work */
	printf("Elapsed time = %e seconds\n", finish-start);

	/* Print the list */
	Print_list("result: ", good_list);

	/* Free */
	free(thread_handles);

	return 0;
}

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <thread count> <number of random vals per thread>\n", 
         prog_name);
   exit(0);
}  /* Usage */

void* Thread_work(void* rank){
	long my_rank = (long) rank;
	return NULL;
}