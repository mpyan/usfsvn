/* File:     solve_heat_eqn.c
 * Author:   Mark Yan
 *
 * Purpose:  Compute approximate solution to the one-dimensional heat equation 
 *           on [0,1]x[0,1].
 *
 * Compile:  gcc -g -Wall -o solve_heat_eqn solve_heat_eqn.c -lm
 * Run:      ./solve_heat_eqn
 *
 * Input:    m, the number of grid points in the x-direction is m+1
 *           n, the number of grid points in the t-direction is n+1
 * Output:   For each time t = 0, 1/n, 2/n, . . . , (n-1)/n, 1, 
 *           t and u(x,t), for x = 0, 1/m, 2/m, . . . , (m-1)/m, 1.
 */
 
#include <stdio.h>
#include <math.h>

const int MAX = 101;

void print_array(double the_array[], int size){
    int i;
    for (i = 0; i <= size; i++){
        printf("%.3f ", the_array[i]);
    }
    printf("\n");
} /* print_array */

void copy_array(double the_old[], double the_new[], int size){
    int i;
    for (i = 0; i < size; i++){
        the_old[i] = the_new[i];
    }
} /* copy_array */

int main(void){
    double new_u[MAX], old_u[MAX];
    
    int m; /* The number of segments into which the bar is divided */
    int n; /* The number of intervals into which the time between 0 and 1 is divided */
    double h; /* The length of each segment in meters*/
    double d; /* The length of each interval in seconds */
    double t_j; /* Time t_j */
    int i; /* Iterator */
    int j; /* Second Iterator */
    
    /* Read the initial input */
    scanf("%d", &m); 
    scanf("%d", &n);
    for (i = 0; i < m; i++){
        scanf("%lf", &old_u[i]);
    } 
    
    /* compute h and d */
    h = 1.0/(double)m;
    d = 1.0/(double)n;
    
    printf("0.000 ");
    print_array(old_u, m);
    
    /* Loop */
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

