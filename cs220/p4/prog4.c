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
#include <mpi.h>

/* Ints in the lists will be between 0 and RMAX */
const int RMAX = 100;
const int STRING_MAX = 10000;

void Print_list(int list[], int n, int my_rank);

int main(int argc, char* argv[]) {
   int my_rank, n;
   MPI_Comm comm;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_rank(comm, &my_rank);

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
