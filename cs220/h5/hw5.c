/* File:       mpi_hello0.c
 *
 * Purpose:    A "hello,world" program that uses MPI
 *
 * Compile:    mpicc -g -Wall -o mpi_hello0 mpi_hello0.c
 * Run:        mpiexec -n<number of processes> ./mpi_hello0
 *
 * Input:      None
 * Output:     A greeting from each process
 *
 * Algorithm:  Each process sends a message to process 0,
 *             which prints the messages it has received,
 *             as well as its own message.
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];
   int        my_rank, p, q;

   /* Start up MPI */
   MPI_Init(NULL, NULL);

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if (my_rank == 0) {
      printf("Greetings from process %d of %d\n", my_rank, p);

      /* Receive incoming message */
      int source = (my_rank + p - 1) % p;
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, source, 
         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n",greeting);
   } else {
      /* send its message to the next higher ranked process */
      int dest = (my_rank + 1) % p;
      sprintf(greeting, "Greetings from process %d of %d", 
            my_rank, p);
      MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, dest, 0,
            MPI_COMM_WORLD);

      /* Receive incoming message */
      int source = (my_rank + p - 1) % p;
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, source, 
         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", greeting);
   }

   /* Shut down MPI */
   MPI_Finalize();
   printf("done");
   return 0;
}  /* main */
