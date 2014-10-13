/* prog3.c
 * Compile:  mpicc -g -Wall -o prog3 prog3.c
 * Run:      mpiexec -n <number of processes> ./prog3
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int INFINITY = 1000000;

void Read_matrix(int mat[], int n);
void Print_matrix(int mat[], int n);

void Floyd(int mat[], int n){

} /* Floyd */

int main(void) {
   int p, my_rank;
   MPI_Comm comm;
   int n;

   /* arrays */
   int* temp_mat;
   int* local_mat;
   int* row_int_city;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   /* Get and broadcast n */
   if (my_rank == 0) {
      printf("Enter n\n");
      scanf("%d", &n);
   }
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

   /* allocate memory */
   temp_mat = malloc(n*n*sizeof(int));
   local_mat = malloc(n*n*sizeof(int));
   row_int_city = malloc(n*sizeof(int));

   if (my_rank == 0){
   	/* Read in the matrix */
   	printf("Enter the matrix\n");
      Read_matrix(temp_mat, n);
   }

   MPI_Scatter(temp_mat, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm);

   /* Floyd start */
   int root;
   int int_city;
   int local_int_city;
   int local_city1;
   int city2;

   for (int_city = 0; int_city < n; int_city++){
      root = int_city/(n/p);
      if (my_rank == root){
         local_int_city = int_city % (n/p);
         for (j = 0; j < n; j++)
            row_int_city[j] = local_mat[local_int_city*n + j];
      }
      MPI_Bcast(row_int_city, n, MPI_INT, root, MPI_COMM_WORLD);
      for (local_city1 = 0; local_city1 < n/p; local_city1++){
         for (city2 = 0; city2 < n; city2++)
            local_mat[local_city1*n + city2] = min(local_mat[local_city1*n + city2], local_mat[local_city1*n + int_city] + row_int_city[city2]);
      }
   }
   /* Floyd end */

   /* Print the matrix when done */
   MPI_Gather(local_mat, n*n/p, MPI_INT, temp_mat, n*n/p, MPI_INT, 0, comm);
   if (my_rank == 0) {
   	printf("The solution is: \n");
   	Print_matrix(temp_mat, n);
   }

   MPI_Finalize();
   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:  Read_matrix
 * Purpose:   Read in the adjacency matrix
 * In arg:    n
 * Out arg:   mat
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
 * In args:   mat, n
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