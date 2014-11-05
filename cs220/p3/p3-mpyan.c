/****************************************************************
 *	Grade: 100/100
 *	Commets: 1) Accept input format: 10/10
 *           2) Correct i/o: 20/20
 *           3) Floyd Test: 35/35
 *           4) Documentation: 10/10
 *           5) Source format: 10/10
 *           6) Quality: 15/15
 ***************************************************************/
/* File:     prog3.c
 *
 * Author:   Mark Yan
 *
 * Purpose:  Implement parallelized Floyd's algorithm 
 *           for solving the all-pairs shortest
 *           path problem:  find the length of the shortest path between each
 *           pair of vertices in a directed graph.
 *
 * Input:    n, the number of vertices in the digraph
 *           mat, the adjacency matrix of the digraph
 * Output:   A matrix showing the costs of the shortest paths
 *
 * Compile:  mpicc -g -Wall -o prog3 prog3.c
 *
 * Run:      mpiexec -n <number of processes> ./prog3
 *           For large matrices, put the matrix into a file with n as
 *           the first line and run with 
 *           mpiexec -n <number of processes> ./prog3 < large_matrix
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int INFINITY = 1000000;

void Read_matrix(int mat[], int n);
void Print_matrix(int mat[], int n);
int Min(int x, int y);
void Floyd(int n, int p, int my_rank, int row_int_city[], 
   int local_mat[], int dest_mat[], MPI_Comm comm);

int main(void) {
   int p, n, my_rank;
   MPI_Comm comm;

   /* arrays */
   int* temp_mat = NULL;
   int* local_mat = NULL;
   int* row_int_city = NULL;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   /* Get and broadcast n */
   if (my_rank == 0) {
      printf("Enter n\n");
      scanf("%d", &n);
   }
   MPI_Bcast(&n, 1, MPI_INT, 0, comm);

   /* allocate memory */
   local_mat = malloc(n*n*sizeof(int));
   row_int_city = malloc(n*sizeof(int));

   /* Read in the matrix */
   if (my_rank == 0){
      printf("Enter the matrix\n");
      temp_mat = malloc(n*n*sizeof(int));
      Read_matrix(temp_mat, n);
   }
   MPI_Scatter(temp_mat, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm);

   /* Find the costs of the shortest paths, store results in temp_mat */
   Floyd(n, p, my_rank, row_int_city, local_mat, temp_mat, comm);

   /* Print the matrix showing the costs of the shortest paths */
   if (my_rank == 0) {
      printf("The solution is: \n");
      Print_matrix(temp_mat, n);
   }

   free(temp_mat);
   free(local_mat);
   free(row_int_city);

   MPI_Finalize();
   return 0;
}  /* main */  

/*-------------------------------------------------------------------
 * Function:  Read_matrix
 * Purpose:   Read in the adjacency matrix
 * In arg:    n:   the size of the matrix
 * Out arg:   mat: the matrix that was read in
 */
void Read_matrix(int mat[], int n) {
   int i, j;

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         scanf("%d", &mat[i*n+j]);
}  /* Read_matrix */

/*-------------------------------------------------------------------
 * Function:  Print_matrix
 * Purpose:   Print the contents of the matrix
 * In args:   mat: the matrix to print
 *            n:   the size of the matrix
 */
void Print_matrix(int mat[], int n) {
   int i, j;

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++)
         if (mat[i*n+j] == INFINITY)
            printf("i ");
         else
            printf("%d ", mat[i*n+j]);
      printf("\n");
   }
}  /* Print_matrix */

/*-------------------------------------------------------------------
 * Function:  Min
 * Purpose:   Find the minumum value between two integers
 * In args:   x, y: the two integers to compare
 * Return value: value of the smaller integer
 */
int Min(int x, int y){
   if (x <= y)
      return x;
   else
      return y;
}

/*-------------------------------------------------------------------
 * Function:  Floyd
 * Purpose:   Find the length of the shortest path between each
 *            pair of vertices in a directed graph
 * In args:   n:            the size of the matrix
 *            p:            the number of processes
 *            my_rank:      the process rank
 *            row_int_city: row of intermediate city values
 *            local_mat:    the local matrix of the process
 *            comm:         the communicator
 * Out arg:   dest_mat:     the matrix storing the result with the
 *                          lengths of the shortest paths between
 *                          between each pair of vertices.
 */
void Floyd(int n, int p, int my_rank, int row_int_city[], 
   int local_mat[], int dest_mat[], MPI_Comm comm){

   int root;
   int int_city;
   int local_int_city;
   int local_city1;
   int city2;

   int i = 0;
   for (int_city = 0; int_city < n; int_city++){
      root = int_city/(n/p);
      if (my_rank == root){
         local_int_city = int_city % (n/p);
         for (i = 0; i < n; i++)
            row_int_city[i] = local_mat[local_int_city*n + i];
      }
      MPI_Bcast(row_int_city, n, MPI_INT, root, comm);
      for (local_city1 = 0; local_city1 < n/p; local_city1++){
         for (city2 = 0; city2 < n; city2++)
            local_mat[local_city1*n + city2] = 
               Min(local_mat[local_city1*n + city2], 
                  local_mat[local_city1*n + int_city] 
                     + row_int_city[city2]);
      }
   }
   MPI_Gather(local_mat, n*n/p, MPI_INT, dest_mat, n*n/p, MPI_INT, 0, comm);
} /* Floyd */