/* File:    hw1.c
 * Purpose: Calculate area using trapezoidal rule.
 *
 * Input:   a, b, n
 * Output:  estimate of area between x-axis, x = a, x = b, and graph of f(x)
 *          using the trapezoidal rule with n trapezoids or using
 *          Simpson's rule with n subintervals.
 *
 * Compile: gcc -g -Wall -o hw1 hw1.c
 * Run:     ./hw1
 *
 * Note:    The function f(x) is hardwired.
 */

#include <stdio.h>
#include <math.h>

double Trap(double  a, double  b, int n, double h);
double Simpson(double a, double b, int n);
double f(double x);    /* Function we're integrating */

int main(void) {
   double  area;       /* Store result in area       */
   double  a, b;       /* Left and right endpoints   */
   int     n;          /* Number of trapezoids       */
   double  h;          /* Trapezoid base width       */
   double  area_s;     /* Store result of integration
                        * using Simpson's rule       */
   double  difference; /* Difference between areas   */

   printf("Enter a, b, and n\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);
   
   h = (b-a)/n;
   area = Trap(a, b, n, h);
   
   area_s = Simpson(a, b, n);
   difference = fabs(area - area_s);

   printf("Using Trapezoidal Rule:\n");
   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the area from %f to %f = %.15f\n",
      a, b, area);
      
   printf("\nUsing Simpson's Rule:\n");
   printf("With n = %d subintervals, our estimate\n", n);
   printf("of the area from %f to %f = %.15f\n",
      a, b, area_s);

   printf("\nThe absolute value of the difference\n"); 
   printf("between the two approximations = %.15f\n", difference);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:     Trap
 * Purpose:      Estimate area using the trapezoidal rule
 * Input args:   a: left endpoint
 *               b: right endpoint
 *               n: number of trapezoids
 *               h: stepsize = length of base of trapezoids
 * Return val:   Trapezoidal rule estimate of area between x-axis,
 *               x = a, x = b, and graph of f(x)
 */
double Trap(double a, double b, int n, double h) {
    double area;   /* Store result in area  */
    double x;
    int i;

    area = (f(a) + f(b))/2.0;
    for (i = 1; i <= n-1; i++) {
        x = a + i*h;
        area = area + f(x);
    }
    area = area*h;

    return area;
} /*  Trap  */

/*------------------------------------------------------------------
 * Function:     Simpson
 * Purpose:      Estimate area using Simpson's rule
 * Input args:   a: left endpoint
 *               b: right endpoint
 *               n: number of subintervals
 * Return val:   Simpson's rule estimate of area between x-axis,
 *               x = a, x = b, and graph of f(x)
 */
double Simpson(double a, double b, int n){
    double area; /* Store result in area */
    double x;
    double h;
    int i;
    
    h = (b-a)/(n-1);
    area = (1.0 / 3.0) * (f(a) + f(b));
    
    for (i = 1; i <= n-1; i = i + 2){
        x = a + i*h;
        area = area + (4.0 / 3.0) * f(x); 
    }
    
    for (i = 2; i < n-1; i = i + 2){
        x = a + i*h;
        area = area + (2.0 / 3.0) * f(x);
    }
    
    area = area * h;
    
    return area;
} /* Simpson */

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   double return_val;

   return_val = x*x + 1;
   return return_val;
}  /* f */

