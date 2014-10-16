/****************************************************************
 *	Grade: 6/6
 *	Commets:
 *
 *
 *
 *
 ***************************************************************/
/* File:      hw6.c
 * Author:    Mark Yan
 * Purpose:   MPI tree structured broadcast function.
 * Input:     A single int
 * Output:    Value received by each process.
 *
 * Algorithm: 
 *    1. Process 0 reads in an int
 *    2. Processes call Bcast
 *    3. Each process prints value it received
 *
 * Compile:  mpicc -g -Wall -o hw6 hw6.c
 * Run:      mpiexec -n <number of processes> ./hw6
 *
 * Notes:
 *    1. This program only needs to work when p is a power of 2.
 *    2. I suggest you used this tree:
 *       0->1
 *       0->2, 1->3
 *       0->4, 1->5, 2->6, 3->7
 *       etc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Bcast(int in_val, int my_rank, int p, MPI_Comm comm);

int main(int argc, char* argv[]) {
   int p, my_rank;
   MPI_Comm comm;
   int result, in_val;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   if (my_rank == 0) {
      printf("Enter an int\n");
      scanf("%d", &in_val);
   }

   result = Bcast(in_val, my_rank, p, comm);

   printf("Proc %d > result = %d\n", my_rank, result); 

   MPI_Finalize();
   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:    Bcast
 * Purpose:     Broadcast a value from process 0 to all the other
 *              processes
 *
 * Input args:  in_val = the value to be broadcast from process 0
 *              my_rank = process's rank
 *              p = number of processes
 *              comm = communicator
 * Return val:  on each process the value broadcast by process 0
 *
 * Notes:
 *    1.  Uses tree structured communication.
 *    2.  The pairing of the processes is done using bitwise
 *        exclusive or.  Here's a table showing the rule for
 *        for bitwise exclusive or
 *           X Y X^Y
 *           0 0  0
 *           0 1  1
 *           1 0  1
 *           1 1  0
 *        Here's a table showing the process pairing with 8
 *        processes (r = my_rank, other column heads are 
 *        bitmask)
 *           r     001 010 100 
 *           -     --- --- ---
 *           0 000 001 010 100  
 *           1 001 000 011 101
 *           2 010  x  000 110
 *           3 011  x  001 111
 *           4 100  x   x  000
 *           5 101  x   x  001
 *           6 110  x   x  010
 *           7 111  x   x  111
 */
int Bcast(int in_val, int my_rank, int p, MPI_Comm comm) {
   int        partner;
   unsigned   bitmask = 1;

   while (bitmask < p) {
      if (bitmask > (my_rank >> 1)) { /* if I participate */
         partner = my_rank ^ bitmask;
         if (my_rank < partner) /* if (I send) */
            MPI_Send(&in_val, 1, MPI_INT, partner, 0, comm);
         else
            MPI_Recv(&in_val, 1, MPI_INT, partner, 0, comm, 
                  MPI_STATUS_IGNORE);
      } 
      bitmask <<= 1; /* Update bitmask */
   }
   return in_val;
}  /* Bcast */
