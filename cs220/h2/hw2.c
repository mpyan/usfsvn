/* File:     hw2.c
 * Author:   Mark Yan
 *
 * Purpose:  Read in an array of ints, print out the input array
 *           Reverse the order of the elements in the array
 *           And print the updated array
 *
 * Compile:  gcc -g -Wall -o hw2 hw2.c
 * Run:      ./hw2
 *
 * Input:    n, the number of integers in the array
 *           the contents of the array
 *           
 * Output:   The original contents of the array
 *           The contents of the reversed array
 *
 */
#include <stdio.h>
void reverse_array(int the_array[], int n);

int main(void){
    int n; /* number of elements in the array */
    int i;
    
    printf("Input the number of integers: ");
    scanf("%d", &n);
    printf("Input %d integers: ", n);
    
    int int_array[n];
    
    for (i = 0; i < n; i++){
        scanf("%d", &int_array[i]);
    }
    
    printf("Original contents of the array:\n");
    
    for (i = 0; i < n; i++){
        printf("%d ", int_array[i]);
    }
    
    reverse_array(int_array, n);
    
    printf("\nContents of reversed array:\n");
    
    for (i = 0; i < n; i++){
        printf("%d ", int_array[i]);
    }
    
    printf("\n");

    return 0;
} /* main */

/*------------------------------------------------------------------
 * Function:     reverse_array
 * Purpose:      Reverse the contents of a given array
 * Input args:   the_array: the array to reverse
 *               n: the number of elements in "the_array"
 */
void reverse_array(int the_array[], int n){
    int i;
    int temp;
    int last = n - 1;

    for (i = 0; i < n/2; i++){
        temp = the_array[i];
        the_array[i] = the_array[last];
        the_array[last] = temp;
        last--;
    }
} /* reverse_array */

