/* File:       hw5.c
 *
 * Purpose:    A "hello,world" program that uses MPI
 *
 * Compile:    mpicc -g -Wall -o hw5 hw5.c
 * Run:        mpiexec -n <number of processes> ./hw5
 *
 * Input:      None
 * Output:     A greeting from each process
 *
 * Algorithm:  Each process sends a message to the
 *             next higher ranked process,
 *             which prints the message it has received,
 *             as well as its own message.
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];
   int        my_rank, p;

   /* Start up MPI */
   MPI_Init(NULL, NULL);

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if (p == 1) {
      printf("Greetings from process %d of %d\n", my_rank, p);
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
   return 0;
}  /* main */
