/* File:     solve_heat_eqn.c
 * Author:   Mark Yan
 *
 * Purpose:  Compute approximate solution to the one-dimensional heat equation 
 *           on [0,1]x[0,1].
 *
 * Compile:  gcc -g -Wall -o solve_heat_eqn solve_heat_eqn.c -lm
 * Run:      ./solve_heat_eqn
 *
 *           (or)
 *
 *           ./solve_heat_eqn < data
 *
 * Input:    m, the number of grid points in the x-direction is m+1
 *           n, the number of grid points in the t-direction is n+1
 *           followed by the initial temperatures
 *
 * Output:   For each time t = 0, 1/n, 2/n, . . . , (n-1)/n, 1, 
 *           t and u(x,t), for x = 0, 1/m, 2/m, . . . , (m-1)/m, 1.
 */
 
#include <stdio.h>
#include <math.h>

const int MAX = 101; /* Maximum array size */

void print_array(double the_array[], int size);
void copy_array(double the_old[], double the_new[], int size);

int main(void){
    /* new_u stores the temperature currently being computed */
    /* old_u stores the temperatures corresponding to the previous time */
    double new_u[MAX], old_u[MAX];
    
    int m; /* The number of bar segments */
    int n; /* The number of time intervals */
    double h; /* The length of each segment in meters*/
    double d; /* The length of each interval in seconds */
    double t_j; /* Time t_j */
    int i; /* Iterator */
    int j; /* Second Iterator */
    
    t_j = 0.0; /* Initialize t_j */
    
    /* Read the initial input */
    printf("Enter m\n");
    scanf("%d", &m);
    printf("Enter n\n"); 
    scanf("%d", &n);
    printf("Enter the initial conditions\n");
    for (i = 0; i < m; i++){
        scanf("%lf", &old_u[i]);
    } 
    
    /* compute h and d */
    h = 1.0/(double)m;
    d = 1.0/(double)n;
    
    /* Print the initial data */
    printf("%0.3f ", t_j);
    print_array(old_u, m);
    
    /* Compute and print the times and temperatures */
    for (j = 1; j <= n; j++){
        t_j = j*d;
        new_u[0] = new_u[m] = 0.0;
        for (i = 1; i < m; i++){           
            new_u[i] = old_u[i] + d/(h*h)*(old_u[i-1] - 2*old_u[i] + old_u[i+1]);
        }
        printf("%0.3f ", t_j);
        print_array(new_u, m); /* Print new_u values */
        copy_array(old_u, new_u, m); /* Copy new_u into old_u for next pass */
    }
    
    return 0;
} /* main */

/*-------------------------------------------------------------------*/
/* Function:    print_array
 * Purpose:     Print the contents of an array (plus a newline)
 * Input args:  the_array: the array to print
 *              size: the number of elements in of the array
 *
 * Note:        Contents are printed to 3 decimal places
 */
void print_array(double the_array[], int size){
    int i;
    for (i = 0; i <= size; i++){
        printf("%.3f ", the_array[i]);
    }
    printf("\n");
} /* print_array */

/*-------------------------------------------------------------------*/
/* Function:    copy_array
 * Purpose:     Copy the contents of one array to another
 * Input args:  dest: the array to copy TO
 *              orig: the array to copy FROM
 *              size: the number of elements in the arrays
 */
void copy_array(double dest[], double orig[], int size){
    int i;
    for (i = 0; i < size; i++){
        dest[i] = orig[i];
    }
} /* copy_array */

