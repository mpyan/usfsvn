# usfsvn
**A collection of C programs using MPI and Pthreads.**

*Programs written as coursework for "CS220: Introduction to Parallel Computing" at the University of San Francisco.
(Cloned from student Subversion repository)*

## Highlights
###[Multi-Threaded Bitonic Sort](https://github.com/mpyan/usfsvn/blob/master/cs220/p5/pth_bitonic.c)###
Sort a list of integer values using parallel bitonic sort with **Pthread**s.

### [Parallelized Floyd's Algorithm](https://github.com/mpyan/usfsvn/blob/master/cs220/p3/prog3.c)
Implement parallelized Floyd's algorithm for solving the all-pairs shortestpath problem: find the length of the shortest path between each pair of vertices in a directed graph. Uses **MPI**.

### [Primes](https://github.com/mpyan/usfsvn/blob/master/cs220/p4/prog4.c)
Find all primes less than or equal to an input value, using multiple processes with **MPI**.

### [Sorted Linked List](https://github.com/mpyan/usfsvn/blob/master/cs220/p2/prog2.c)
Implement an sorted linked list with ops insert, print, member, delete, free_list. Items in the list are stored in alphabetical order. (Serial).
