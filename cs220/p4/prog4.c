/* File:      prog4.c
 * Purpose:   Find all primes less than or equal to an input value.
 *
 * Compile:   mpicc -g -Wall -o prog4 prog4.c
 * Run:       
 *
 * Input:     
 * Output:    
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

/* Ints in the lists will be between 0 and RMAX */
const int RMAX = 100;
const int STRING_MAX = 10000;

void Print_list(int list[], int n, int my_rank);
int Is_prime(int i);

int main(int argc, char* argv[]) {
   int my_rank, n, i, j, p;
   MPI_Comm comm;

   /* arrays */
   int* prime_arr = NULL; /* primes */
   int* recv_arr = NULL; /* received primes */
   int* temp_arr = NULL; /* temporary storage */

   int* prime_count = NULL; /* Store each process's number of primes */
   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;

   MPI_Comm_rank(comm, &my_rank);
   MPI_Comm_size(comm, &p);

   /* Get and broadcast n */
   n = strtol(argv[1], NULL, 10);
   MPI_Bcast(&n, 1, MPI_INT, 0, comm);

   /* Start doing stuff */
   j = 0;
	prime_arr = malloc(((n/(2*p))+2)*sizeof(int));
	if (my_rank == 0){
		prime_arr[0] = 2;
		j = 1;
	}
	/* Cyclic distribution of values to check */
	for (i = 2*my_rank + 3; i < n; i+=2*p){
		prime_arr[j] = i;
		j++;
	}

	/* Print debug info for Search */
	Print_list(prime_arr, j, my_rank);

	free(prime_arr);
	MPI_Finalize();
   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:  Print_list
 * Purpose:   Convert a list of ints to a single string before
 *            printing.  This should make it less likely that the
 *            output is interrupted by another process.  This is
 *            mainly intended for debugging purposes.
 * In args:   list:  the ints to be printed
 *            n:  the number of ints
 *            my_rank:  the usual MPI variable
 */
void Print_list(int list[], int n, int my_rank) {
   char string[STRING_MAX];
   char* s_p;
   int i;

   sprintf(string, "Proc %d > ", my_rank);
   // Pointer arithmetic:  make s_p point to the character strlen(string)
   // into string; i.e., make it point at the `\0'
   s_p = string + strlen(string);
   for (i = 0; i < n; i++) {
      sprintf(s_p, "%d ", list[i]);
      s_p = string + strlen(string);
   }

   printf("%s\n", string);
   fflush(stdout);
}  /* Print_list */

/*-------------------------------------------------------------------
 * Function:   Is_prime
 * Purpose:    Determine whether the argument is prime
 * Input arg:  i
 * Return val: true (nonzero) if arg is prime, false (zero) otherwise
 */
int Is_prime(int i) {
   int j;

   for (j = 2; j <= sqrt(i); j++)
      if (i % j == 0)
         return 0;
   return 1;
}  /* Is_prime */