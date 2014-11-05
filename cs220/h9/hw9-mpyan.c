/****************************************************************
 *	Grade: 8/8
 *	Commets:
 *
 *
 *
 *
 ***************************************************************/
/* File:    hw9.c
 * 
 * Author:  Mark Yan
 *
 * Purpose: Compute a DAXPY (Double Precision Alpha X Plus Y)
 *
 * Input:   n,     the number of elements in x and y
 *          x,     the first array
 *          y,     the second array, and also the array to store
 *                 the result
 *          alpha  the number to use as the multiplier
 *
 * Output:  The modified contents of y as the result.
 *
 * Compile: gcc -g -Wall -o hw9 hw9.c -lpthread
 * Usage:   ./hw9 <thread_count>
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables: accessible to all threads */
int thread_count;
double alpha;
double* x = NULL;
double* y = NULL;
int n;

/* Serial functions */
void Usage(char* prog_name);
void Print_array(char* text, double x[], int n);
void Read_array(char* text, double x[], int n);

/* Parallel function */
void *Daxpy(void* rank);

int main(int argc, char* argv[]){
	long thread;
	pthread_t* thread_handles = NULL;

	/* Get number of threads from command line */
   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0)
      Usage(argv[0]);

   thread_handles = malloc(thread_count*sizeof(pthread_t));

   /* Read in n */
   printf("Please enter n: ");
   scanf("%d", &n);

   /* Allocate and initialize */
   x = malloc(n*sizeof(double));
   y = malloc(n*sizeof(double));

   /* Read in x, y and alpha */
   Read_array("Enter x", x, n);
   Read_array("Enter y", y, n);
   printf("Please enter alpha: ");
   scanf("%lf", &alpha);

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL, 
         Daxpy, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

   /* Print the result */
   Print_array("Result: ", y, n);

   free(x);
   free(y);
   free(thread_handles);

   return 0;
} /* main */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "number of threads > 0\n");
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Print_array
 * Purpose:     Print an array
 * In args:     text: a string message to display before printing
 *                    the contents of the array x
 *              x:    the array to print 
 *              n:    the number of elements in the array
 */
void Print_array(char* text, double x[], int n) {
   int i;

   printf("%s\n", text);
   for (i = 0; i < n; i++)
      printf("%f ", x[i]);

   printf("\n");
}  /* Print_array */

/*------------------------------------------------------------------
 * Function:        Read_array
 * Purpose:         Read in the array x
 * In arg:          text: a prompt to display before reading
 *                  n:    the number of elements in the array
 * Out arg:         x:    the array to read in
 */
void Read_array(char* text, double x[], int n) {
   int i;

   printf("%s\n", text);
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}  /* Read_array */

/*------------------------------------------------------------------
 * Function:        Daxpy
 * Purpose:         Parallel compute DAXPY from data stored in
 *                  two shared arrays x and y, and a shared double
 *                  alpha, and store result in y
 * In arg:          rank: the thread "rank"
 */
void *Daxpy(void* rank){
   long my_rank = (long) rank;
   int local_n = n/thread_count;
   int my_first_row = my_rank*local_n;
   int my_last_row = my_first_row + local_n - 1;
   int i;

   for (i = my_first_row; i <= my_last_row; i++){
      y[i] += alpha*x[i];
   }

   return NULL;
} /* Daxpy */