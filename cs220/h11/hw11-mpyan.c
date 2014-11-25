/****************************************************************
 *	Grade: 4/4
 *	Commets:
 *
 *
 *
 *
 ***************************************************************/
/* File:
 *    hw11.c
 *
 * Author: Mark Yan
 *
 * Purpose:
 *    Generate random numbers with function that is threadsafe.
 *
 * Compile:
 *    gcc -g -Wall -o hw11 hw11.c -lpthread
 * Usage:
 *    ./hw11 <thread_count> <number of random numbers per thread>
 *
 * Input:
 *    None
 * Output:
 *    Random numbers from each thread
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MR_MULTIPLIER 279470273 
#define MR_INCREMENT 0
#define MR_MODULUS 4294967291U

int thread_count;
int n;

void Usage(char* prog_name);
unsigned My_random(unsigned seed, unsigned* saveptr);
void *Thread_work(void* rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long        thread;
   pthread_t* thread_handles; 

   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);

   thread_handles = malloc(thread_count*sizeof(pthread_t));

   printf("Enter text\n");
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL);

   free(thread_handles);
   return 0;
}  /* main */


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
 * Function:    Thread_work
 * Purpose:     Each thread calls a function and prints a message
 * In arg:      rank
 * Global vars: thread_count (in) 
 * Return val:  Ignored
 */
void *Thread_work(void* rank) {
   long my_rank = (long) rank;
   int i;
   unsigned saveptr;

   My_random(my_rank + 1, &saveptr);  /* "Seed" Random number generator */
   for (i = 0; i < n; i++) 
      printf("Th %ld > %u\n", my_rank, My_random(0, &saveptr));

   return NULL;
}  /* Thread_work */


/*-------------------------------------------------------------------
 * Function:    My_random
 * Purpose:     Generate random numbers
 * In arg:      seed  (ignored if = 0)
 * In/out arg:  
 *     saveptr: a pointer to an unsigned int variable that is used
 *              internally by My_random() in order to maintain 
 *              context between successive calls to the function.
 *
 * Ret val:     A "random" unsigned int
 *
 * Note:        The first time the function is called, the random
 *              number should be "seeded" with a nonzero argument
 *              for the 1st argument "seed",
 *              and the return value ignored.  Subsequent calls 
 *              should have a zero argument for the 1st argument
 *              and the return values
 *              will form a "pseudo-random" sequence.
 *
 *              All calls to My_random() by the same thread must
 *              have a pointer to the same unsigned integer
 *              in the 2nd argument "saveptr"
 */
unsigned My_random(unsigned seed, unsigned* saveptr) {
   unsigned long tmp;

   if (seed != 0) *saveptr = seed;
   
   tmp = (*saveptr) * (MR_MULTIPLIER); 
   *saveptr = tmp % MR_MODULUS;
   return *saveptr;
}  /* My_random */
