/* File: pth_bitonic.c
 * Author: Mark Yan
 *
 * Purpose: sort
 *
 * Compile: gcc -g -Wall -o pth_bitonic pth_bitonic.c -lpthread
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
int thread_n;
int* good_list;
int* temp_list;

pthread_mutex_t barrier_mutex;
pthread_cond_t cond_var;
int barrier_counter = 0;

void Usage(char* prog_name);
void* Thread_work(void* rank);
int  Compare(const void* a_p, const void* b_p);
void Merge_split_low(int local_A[], int temp_B[], int temp_C[], 
         int local_n);
void Merge_split_high(int local_A[], int temp_B[], int temp_C[], 
        int local_n);
void Set_args(int argc, char* argv[], int* has_g, int* has_o);
void Print_list(char* text, int list[]);
void Generate_list();
void Get_list();
void Merge_inc(long my_rank, long partner);
void Merge_dec(long my_rank, long partner);
void Parallel_qsort(long my_rank);
void Butterfly_barrier(unsigned stage_bitmask, int stage);

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
   	thread_n = n/thread_count;
   	thread_handles = malloc(thread_count*sizeof(pthread_t));
   	good_list = malloc(n*sizeof(int));
	temp_list = malloc(n*sizeof(int));

	Set_args(argc, argv, &has_g, &has_o);

	/* Create the shared list */
	if (has_g == 1)
		Generate_list();
	else
		Get_list();

	if (has_o == 1)
		Print_list("Original: ", good_list);

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
	Print_list("Result: ", good_list);

	/* Free */
	pthread_mutex_destroy(&barrier_mutex);
	pthread_cond_destroy(&cond_var);
	free(thread_handles);
	free(good_list);
	free(temp_list);

	return 0;
} /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <thread count> <number of elements> [g] [o]\n", 
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

/*-------------------------------------------------------------------
 * Function:    Merge_split_low
 * Purpose:     Merge the smallest local_n elements in local_A 
 *              and temp_B into temp_C.
 * In args:     local_n, temp_B
 * In/out args: local_A
 * Out args:    temp_C
 */
void Merge_split_low(int local_A[], int temp_B[], int temp_C[], 
        int local_n) {
   int ai, bi, ci;
   
   ai = 0;
   bi = 0;
   ci = 0;
   while (ci < local_n) {
      if (local_A[ai] <= temp_B[bi]) {
         temp_C[ci] = local_A[ai];
         ci++; ai++;
      } else {
         temp_C[ci] = temp_B[bi];
         ci++; bi++;
      }
   }
}  /* Merge_split_low */

/*-------------------------------------------------------------------
 * Function:    Merge_split_high
 * Purpose:     Merge the largest local_n elements in local_A 
 *              and temp_B into temp_C.
 * In args:     local_n, temp_B
 * In/out args: local_A
 * Out args:    temp_C
 */
void Merge_split_high(int local_A[], int temp_B[], int temp_C[], 
        int local_n) {
   int ai, bi, ci;
   
   ai = local_n-1;
   bi = local_n-1;
   ci = local_n-1;
   while (ci >= 0) {
      if (local_A[ai] >= temp_B[bi]) {
         temp_C[ci] = local_A[ai];
         ci--; ai--;
      } else {
         temp_C[ci] = temp_B[bi];
         ci--; bi--;
      }
   }
}  /* Merge_split_low */

/*-------------------------------------------------------------------
 * Function:    Set_args
 * Purpose:     Determine if the 'g' and 'o' characters are on the
 *              command line, set corresponding values
 * In args:     argc, argv
 * In/out args: has_g, has_o
 */
void Set_args(int argc, char* argv[], int* has_g, int* has_o){
	if (argc > 3){
		if (strcmp(argv[3], "g") == 0){
			*has_g = 1;
		}
		else if (strcmp(argv[3], "o") == 0)
			*has_o = 1;
		if (argc > 4){
			if (strcmp(argv[4], "g") == 0)
				*has_g = 1;
			else if (strcmp(argv[4], "o") == 0)
				*has_o = 1;
		}
	}
} /* Set_args */

/*-------------------------------------------------------------------
 * Function:    Print_list
 * Purpose:     Print a list
 * In args:     text: some text to be printed before the list
 *              list: the list to be printed
 */
void Print_list(char* text, int list[]){
	int i;
	printf("%s", text);
	for (i = 0; i < n; i++){
		printf("%d ", list[i]);
	}
	printf("\n");
} /* Print_list */

/*-------------------------------------------------------------------
 * Function:    Generate_list
 * Purpose:     Generate random integer values for the shared 
 *              list "good_list"
 */
void Generate_list(){
	srandom(1);
	int i;
	for (i = 0; i < n; i++){
		good_list[i] = random() % RMAX;
	}
} /* Generate_list */

/*-------------------------------------------------------------------
 * Function:    Get_list
 * Purpose:     Read in integer values from the user for the shared 
 *              list "good_list"
 */
void Get_list(){
	int i;
	for (i = 0; i < n; i++){
		scanf("%d", &good_list[i]);
	}
} /* Get_list */

/*-------------------------------------------------------------------
 * Function:    Merge_inc
 * Purpose:     Merge the contents of the partner's sublist into
 *              calling thread's sublist to create
 *              an increasing sublist
 * In args:     my_rank: the calling thread's rank
 *              partner: the rank of the calling thread's partner
 */
void Merge_inc(long my_rank, long partner){
	int my_head = my_rank * thread_n;
	int partner_head = partner * thread_n;
	if (my_rank < partner){
		Merge_split_low(good_list+my_head, good_list+partner_head, 
			temp_list+my_head, thread_n);
	} else {
		Merge_split_high(good_list+my_head, good_list+partner_head, 
			temp_list+my_head, thread_n);
	}
} /* Merge_inc */

/*-------------------------------------------------------------------
 * Function:    Merge_dec
 * Purpose:     Merge the contents of the partner's sublist into
 *              calling thread's sublist to create
 *              a decreasing sublist
 * In args:     my_rank: the calling thread's rank
 *              partner: the rank of the calling thread's partner
 */
void Merge_dec(long my_rank, long partner){
	int my_head = my_rank * thread_n;
	int partner_head = partner * thread_n;
	if (my_rank > partner){
		Merge_split_low(good_list+my_head, good_list+partner_head, 
			temp_list+my_head, thread_n);
	} else {
		Merge_split_high(good_list+my_head, good_list+partner_head, 
			temp_list+my_head, thread_n);
	}
} /* Merge_dec */

/*-------------------------------------------------------------------
 * Function:    Parallel_qsort
 * Purpose:     Sort the elements of the calling thread's sublist
 *              (using quicksort)
 * In args:     my_rank: the calling thread's rank
 */
void Parallel_qsort(long my_rank){
	/* Quicksort this thread's own stuff */
	int my_head = my_rank * thread_n;
	qsort(good_list + my_head, thread_n, sizeof(int), Compare);
	/* Barrier */
	pthread_mutex_lock(&barrier_mutex);
	barrier_counter++;
	if (barrier_counter == thread_count){
		barrier_counter = 0;
#		ifdef DEBUG
		Print_list("List after Quicksort: ", good_list);
#		endif
		pthread_cond_broadcast(&cond_var);
	} else {
		while (pthread_cond_wait(&cond_var, &barrier_mutex) != 0);
	}
	pthread_mutex_unlock(&barrier_mutex);
	/* End of Barrier */
} /* Parallel_qsort */

/*-------------------------------------------------------------------
 * Function:    Butterfly_barrier
 * Purpose:     Act as a barrier for the butterfly. 
 *              Swap the pointers of the shared lists
 *              good_list and temp_list before allowing the
 *              threads to continue execution.
 *
 *              If the DEBUG flag is defined, print out
 *              which butterfly is being executed,
 *              which stage of the butterfly has been completed, and
 *              the contents of the shared, sorted list (good_list)
 *
 * In args:     bitmask: the bitmask for the current butterfly
 *                       to be used to determine which butterfly
 *                       is being executed
 *              stage: the completed stage of the current butterfly
 */
void Butterfly_barrier(unsigned bitmask, int stage){
	int* swapper = NULL;
	pthread_mutex_lock(&barrier_mutex);
	barrier_counter++;
	if (barrier_counter == thread_count){
		barrier_counter = 0;
		/* swap pointers */
		swapper = good_list;
		good_list = temp_list;
		temp_list = swapper;
#		ifdef DEBUG
		printf("Butterfly: %d-thread butterfly, Stage: %d, ", 
			bitmask*2, stage);
		Print_list("List: ", good_list);
#		endif
		pthread_cond_broadcast(&cond_var);
	} else {
		while (pthread_cond_wait(&cond_var, &barrier_mutex) != 0);
	}
	pthread_mutex_unlock(&barrier_mutex);
} /* Butterfly_barrier */

void* Thread_work(void* rank){
	long my_rank = (long) rank;
	long partner;

	unsigned bitmask = (unsigned) thread_count >> 1;
	unsigned and_bit = 2;
	unsigned stage_bitmask = 1;
	int stage;

	Parallel_qsort(my_rank);

	while (stage_bitmask < thread_count){
		bitmask = stage_bitmask;
		stage = 0;
		while (bitmask > 0){
			partner = my_rank ^ bitmask;
			if ((my_rank & and_bit) == 0)
				Merge_inc(my_rank, partner);
			else
				Merge_dec(my_rank, partner);
			bitmask >>= 1;
			stage++;

			Butterfly_barrier(stage_bitmask, stage);
		}
		stage_bitmask <<= 1;
		and_bit <<= 1;
	}

	return NULL;
} /* Thread_work */