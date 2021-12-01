
/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * hash_func.h:
 * DESCRIPION:
 *    This is the header file of the hash_func.c file
 *    including this file means all defined hash functions in this file
 *    can be used.
 *  
 * USAGE:
 *    This File can be used by including the header file hash_table.h
 *    And then calling the functions by passing the right variables.
 */

/* Example hash function with terrible performance */
unsigned long hash_too_simple(unsigned char *str);

/* Add the header for your own added hash functions here. You may search online
 * for existing solutions for hashing function, as long as you as you
 * attribute the source, meaning links to the used material.  */

/* This hash fucntion is derived from https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/
It gets each character of the string and multiplies it by some value given to p to the power of p. And then sums this total.*/
unsigned long polynomialRollingHash(unsigned char *str);