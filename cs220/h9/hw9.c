/* File: hw9.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables: accessible to all threads */
int thread_count;
double alpha = 0;
double* x = NULL;
double* y = NULL;
int n;

void Usage(char* prog_name);

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

int main(int argc, char* argv[]){
	long thread;
	pthread_t* thread_handles = NULL;

	/* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0) Usage(argv[0]);

   /* Read in n */
   printf("Please enter n: ");
   scanf("%d", &n);

   /* Allocate and initialize */
   x = malloc(n*sizeof(double));
   y = malloc(n*sizeof(double));

   /* Read in x, y and alpha */
   // printf("Please enter alpha: ");
   // scanf("%lf", &alpha);
   // printf("got alpha");
   alpha = 1.0;

   printf("Please enter x:\n");
   int i;
   for (i = 0; i < n; i++){
      scanf("%lf", &x[i]);
   }
   printf("Please enter y:\n");
   for (i = 0; i < n; i++){
      scanf("%lf", &y[i]);
   }

   printf("done");

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL, 
         Daxpy, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

   /* Print the result */
   printf("Result: \n");
   for (i = 0; i < n; i++)
      printf("%4.1f ", y[i]);

   free(x);
   free(y);
} /* main */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "number of threads > 0\n");
   exit(0);
}  /* Usage */