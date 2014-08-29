// File:     Trap.java
// Purpose:  Calculate area using trapezoidal rule
//
// Input:    a:  left endpoint of interval
//           b:  right endpoint of interval
//           n:  number of trapezoids
// Output:   estimate of area between the x-axis, x = a, x = b, and
//           the graph of f(x) using n trapezoids
//
// Compile:  javac Trap.java
// Usage:    java Trap
//
// Notes:    f(x) is hardwired as a private static member.

import java.util.Scanner;

public class Trap {

   public static void main(String args[]) {
      double   area;      // Store result in area
      double   a, b;      // Left and right endpoints
      int      n;         // Number of trapezoids
      double   h;         // Trapezoid base width

      Scanner sc = new Scanner(System.in);

      System.out.println("Enter a, b, and n");
      a = sc.nextDouble();
      b = sc.nextDouble();
      n = sc.nextInt();

      h = (b-a)/n;
      area = trap(a, b, n, h);

      System.out.println("With n = " + n + " trapezoids, our estimate");
      System.out.print("of the area from " + a + " to " + b);
      System.out.println(" = " + area);
      
   }  // main

   // Method:       trap
   // Purpose:      Estimate area using the trapezoidal rule
   // Input args:   a: left endpoint
   //               b: right endpoint
   //               n: number of trapezoids
   //               h: stepsize = length of base of trapezoids
   // Return val:   Trapezoidal rule estimate of area between x-axis,
   //               x = a, x = b, and graph of f(x)
   // Note:         Need *static* so trap can be called from main without 
   //               instantiating an object
   static double trap(double a, double b, int n, double h) {
       double area = 0.0;   // Store result in area
       
       area = 0.5 * (f(a) + f(b));    // area
       for (int i = 1; i < n; i++) {
          double x = a + i * h;
          area += f(x);
       }
       area *= h;
       
       return area;
   } //  trap  


   // Method:     f
   // Purpose:    Compute value of function being integrated
   // Input arg:  x
   // Note:       Need *static* so f can be called from trap without 
   //             instantiating an object
   static double f(double x) {
      double return_val = 0.0;  
      
      return_val = Math.pow(x, 2) + 1;
      
      return return_val;
   }  // f

}  // class Trap

