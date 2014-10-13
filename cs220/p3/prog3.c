/* prog3.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void) {
   int p, my_rank;
   MPI_Comm comm;
   int n;

   int* temp_mat[];
   int* local_mat[];
   int* row_int_city[];

   int root;
   int int_city;
   int local_int_city;
   int local_city1;
   int city2;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   /* Get and broadcast n */
   if (my_rank == 0) {
      printf("Enter n\n");
      scanf("%d", &n);
   }
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Scatter(temp_mat, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm);

   /* Print the matrix when done */
   MPI_Gather(local_mat, n*n/p, MPI_INT, temp_mat, n*n/p, MPI_INT, 0, comm);

   MPI_Finalize();
   return 0;
}  /* main */