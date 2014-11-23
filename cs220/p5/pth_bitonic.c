/* File: pth_bitonic.c
 * Author: Mark Yan
 *
 * Purpose: sort
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"

int thread_count;

void Usage(char* prog_name);

int main(int argc, char* argv[]) {
	int has_g = 0;
	int has_o = 0;

	/* Check input args */
	if (argc < 3) Usage(argv[0]);
	if (argc > 3){
		if (strcmp(argv[3], "g") == 0){
			has_g = 1;
		}
		else if (strcmp(argv[3], "o") == 0)
			has_o = 1;
		if (argc > 4){
			if (strcmp(argv[4], "g") == 0)
				has_g = 1;
			else if (strcmp(argv[4], "o") == 0)
				has_o = 1;
		}
	}

	return 0;
}

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <thread count> <number of random vals per thread>\n", 
         prog_name);
   exit(0);
}  /* Usage */