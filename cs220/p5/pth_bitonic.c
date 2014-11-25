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

pthread_mutex_t barrier_mutex;
pthread_cond_t cond_var;
int barrier_counter = 0;

void Usage(char* prog_name);
void* Thread_work(void* rank);
int  Compare(const void* a_p, const void* b_p);
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

void Merge_inc(long my_rank, long partner, int stage){
	printf("Stage: %d, Merge_inc > my_rank: %ld, partner: %ld\n", stage, my_rank, partner);
}

void Merge_dec(long my_rank, long partner, int stage){
	printf("Stage: %d, Merge_dec > my_rank: %ld, partner: %ld\n", stage, my_rank, partner);
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

	pthread_mutex_init(&barrier_mutex, NULL);
	pthread_cond_init(&cond_var, NULL);
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
	pthread_mutex_destroy(&barrier_mutex);
	pthread_cond_destroy(&cond_var);
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

/*-------------------------------------------------------------------
 * Function:    Compare
 * Purpose:     Compare 2 ints, return -1, 0, or 1, respectively, when
 *              the first int is less than, equal, or greater than
 *              the second.  Used by qsort.
 */
int Compare(const void* a_p, const void* b_p) {
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else /* a > b */
      return 1;
}  /* Compare */

void* Thread_work(void* rank){
	long my_rank = (long) rank;

	unsigned bitmask = (unsigned) thread_count >> 1;
	unsigned and_bit = 2;
	long partner;
	int stage = 0;
	int local_n = n/thread_count;
	int my_head = my_rank * local_n;
	int my_tail = my_head + local_n - 1;
	int* swapper = NULL;

	printf("Th %ld > start qsort, my_head = %d, my_tail = %d\n", my_rank, my_head, my_tail);

	/* Quicksort this thread's own stuff */
	qsort(good_list + my_head, local_n, sizeof(int), Compare);
	/* Barrier here */
	printf("Th %ld > local qsort done \n", my_rank);
	pthread_mutex_lock(&barrier_mutex);
	barrier_counter++;
	if (barrier_counter == thread_count){
		barrier_counter = 0;
		pthread_cond_broadcast(&cond_var);
	} else {
		while (pthread_cond_wait(&cond_var, &barrier_mutex) != 0);
	}
	pthread_mutex_unlock(&barrier_mutex);
	printf("Th %ld > exited barrier \n", my_rank);
	/* End of Barrier */

	// while (bitmask > 0){
	// 	partner = my_rank ^ bitmask;
	// 	if ((my_rank & and_bit) == 0)
	// 		Merge_inc(my_rank, partner, stage);
	// 	else
	// 		Merge_dec(my_rank, partner, stage);
	// 	bitmask >>= 1;
	// 	and_bit <<= 1;
	// 	stage++;
	// 	/* Barrier here? */
	// }

	/* Swap pointers of temp_list and good_list (in the barrier?) */

	return NULL;
}