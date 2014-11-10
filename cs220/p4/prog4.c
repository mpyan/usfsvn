/* File:      prog4.c
 * Purpose:   Find all primes less than or equal to an input value.
 *
 * Compile:   mpicc -g -Wall -o prog4 prog4.c -lm
 * 
 *            or to debug,
 *            mpicc -g -Wall -o prog4 prog4.c -lm -DDEBUG
 * Run:       mpiexec -n <number of processes> ./prog4 <n>
 *
 *            n: any positive integer
 *
 * Input:     
 * Output:    
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

/* Ints in the lists will be between 0 and 100 */
const int STRING_MAX = 10000;

void Print_list(char* text, int list[], int n, int my_rank);
int Is_prime(int i);
void Update_counts(int* counts, int n, unsigned bitmask);
void Merge(int** A_p, int* asize, int B[], int bsize, int** C_p);
void Get_max_primes_recv(int* counts_arr, int* max_primes, int* max_recv,
   int n, int my_rank);

void Dist_merge_sort(int** prime_arr_p, int* recv_arr, int** temp_arr_p, 
   int* prime_count, int* local_n_p, int p, int my_rank, MPI_Comm comm);

void Primes(int** primes, int* n, int p, int my_rank, MPI_Comm comm);

int main(int argc, char* argv[]) {
   int my_rank, n, p, i;
   MPI_Comm comm;
   int* prime_arr = NULL; /* Array to store primes */
   
   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;

   MPI_Comm_rank(comm, &my_rank);
   MPI_Comm_size(comm, &p);

   /* Get and broadcast n */
   if (my_rank == 0)
      n = strtol(argv[1], NULL, 10);
   MPI_Bcast(&n, 1, MPI_INT, 0, comm);

   Primes(&prime_arr, &n, p, my_rank, comm);

   /* Print out the result */
   if(my_rank == 0) {
      for (i = 0; i < n; i++){
         printf("%d ", prime_arr[i]);
      }
      printf("\n");
   }

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
void Print_list(char* text, int list[], int n, int my_rank) {
   char string[STRING_MAX];
   char* s_p;
   int i;

   sprintf(string, "Proc %d > %s", my_rank, text);
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

void Update_counts(int* counts, int n, unsigned bitmask){
   int i_rank;
   int partner;
   for (i_rank = 0; i_rank < n; i_rank++){
      partner = i_rank ^ bitmask;
      if (i_rank < partner){
         if (i_rank < n && partner < n){
            counts[i_rank] += counts[partner];
         }
      } else {
         counts[i_rank] = 0; /* done */
      }
   }
} /* Update_counts */

void Get_max_primes_recv(int* counts_arr, int* max_primes, int* max_recv,
   int n, int my_rank){

   int partner;
   int done = 0;
   unsigned bitmask = (unsigned) 1;
   *max_primes = counts_arr[my_rank];
   *max_recv = 0;
   int* counts = malloc(n*sizeof(int));

   int i;
   for (i = 0; i < n; i++){
      counts[i] = counts_arr[i];
   }

   while ((!done) && (bitmask < n)){
      partner = my_rank ^ bitmask;
      if (my_rank < partner){
         if (partner < n){
            *max_primes += counts[partner];
            if (counts[partner] > *max_recv)
               *max_recv = counts[partner];
         }
         Update_counts(counts, n, bitmask);
         bitmask <<= 1;
      } else {
         done = 1;
      }
   }
   free(counts);

#  ifdef DEBUG
   /* print max storage and max receive */
   printf("Proc %d > Max primes = %d, max receive = %d\n",
      my_rank, *max_primes, *max_recv);
#  endif
}

/*-------------------------------------------------------------------
 * Function:   Merge
 * Purpose:    Merge the contents of the arrays A and B into array C
 * Input args:
 *    asize:  the number of elements in A
 *    bsize:  the number of elements in B
 *    csize:  the number of elements in C (= asize + bsize)
 *    A, B:  the arrays
 *
 * Output arg:
 *    C:  result array
 */
void Merge(int** A_p, int* asize, int B[], int bsize, int** C_p) {
   int ai, bi, ci;
   int* A = *A_p;
   int* C = *C_p;
   int csize = *asize + bsize;
   
   ai = bi = ci = 0;
   while (ai < *asize && bi < bsize) {
      if (A[ai] <= B[bi]) {
         C[ci] = A[ai];
         ci++; ai++;
      } else {
         C[ci] = B[bi];
         ci++; bi++;
      }
   }

   if (ai >= *asize)
      for (; ci < csize; ci++, bi++)
         C[ci] = B[bi];
   else
      for (; ci < csize; ci++, ai++)
         C[ci] = A[ai];

   /* Swap pointers */
   int* swapper = *A_p;
   *A_p = *C_p;
   *C_p = swapper;
   *asize = csize;
}  /* Merge */

void Dist_merge_sort(int** prime_arr_p, int* recv_arr, int** temp_arr_p, 
   int* prime_count, int* local_n_p, int p, int my_rank, MPI_Comm comm){
   int partner;
   int done = 0;
   unsigned bitmask = (unsigned) 1;
#  ifdef DEBUG
   int my_pass = -1;
#  endif

   while (!done && bitmask < p){
      partner = my_rank ^ bitmask;
#     ifdef DEBUG
      my_pass++;
#     endif
      if (my_rank < partner){
         if (partner < p){
            /* receive */
            MPI_Recv(recv_arr, prime_count[partner], MPI_INT, partner, 
               0, comm, MPI_STATUS_IGNORE);

            /* Merge */
            Merge(prime_arr_p, local_n_p, recv_arr, prime_count[partner], temp_arr_p);

#           ifdef DEBUG
            char message[STRING_MAX];
            sprintf(message, "After Phase %d, primes are ", my_pass);
            Print_list(message, *prime_arr_p, *local_n_p, my_rank);
#           endif
         }
         Update_counts(prime_count, p, bitmask);
         bitmask <<= 1;
      } else {
         /* send */
         MPI_Send(*prime_arr_p, *local_n_p, MPI_INT, partner, 0, comm);
         done = 1;
      }
   }
} /* Dist_merge_sort */

void Primes(int** primes, int* n, int p, int my_rank, MPI_Comm comm){
   int* prime_arr = NULL;
   int local_n, i, max_primes, max_recv;
   int* recv_arr = NULL; /* received primes */
   int* temp_arr = NULL; /* temporary storage */

   int* prime_count = NULL; /* Store each process's number of primes */

   local_n = 0;
   int* init_prime_arr = malloc((((*n)/(2*p))+2)*sizeof(int));
   if (my_rank == 0){
      init_prime_arr[0] = 2;
      local_n = 1;
   }

   /* Cyclic distribution and checking of values */
   for (i = 2*my_rank + 3; i < *n; i+=2*p){
      if (Is_prime(i)){
         init_prime_arr[local_n] = i;
         local_n++;
      }
   }

#  ifdef DEBUG
   Print_list("After search primes are ", init_prime_arr, local_n, my_rank);
#  endif

   /* Allgather */
   prime_count = malloc(p*sizeof(int));
   MPI_Allgather(&local_n, 1, MPI_INT, prime_count, 1, MPI_INT, comm);
   Get_max_primes_recv(prime_count, &max_primes, &max_recv, p, my_rank);

   /* Allocate arrays */
   recv_arr = malloc(max_recv*sizeof(int));
   temp_arr = malloc(max_primes*sizeof(int));
   prime_arr = malloc(max_primes*sizeof(int));
   for (i = 0; i < local_n; i++){
      prime_arr[i] = init_prime_arr[i];
   }
   free(init_prime_arr);

   /* Distributed Mergesort */
   Dist_merge_sort(&prime_arr, recv_arr, &temp_arr, prime_count, &local_n,
      p, my_rank, comm);

   *n = local_n;
   *primes = prime_arr;
   free(prime_count);
   free(recv_arr);
   free(temp_arr);
} /* Primes */