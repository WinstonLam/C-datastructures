/* Name: Winston Lam
 * Student ID: 11844078
 * Course: Datastructuren
 * Institution: University of Amsterdam
 *
 * hash_func.c:
 * DESCRIPION:
 *    In this file all the internal functions of 
 *    the hash func are defined. This file contains the
 *    different hash function which can be used to hash a given string.
 * USAGE:
 *    This File can be used by including the header file hash_func.h
 *    And then calling the functions by passing the right variables.
 */
#include <string.h>
/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(unsigned char *str) {
    return (unsigned long) *str;
}

/* This hash fucntion is derived from https://www.geeksforgeeks.org/string-hashing-using-polynomial-rolling-hash-function/
It gets each character of the string and multiplies it by some value given to p to the power of p. And then sums this total.*/
unsigned long polynomialRollingHash(unsigned char *str)
{   
    // P and M
    int p = 31;
    int m = (int)(1e9 + 9);
    long power_of_p = 1;
    long hash_val = 0;

    // Loop to calculate the hash value
    // by iterating over the elements of String
    for(size_t i = 0; i < strlen((char*)str); i++)
    {
        hash_val = (hash_val + (str[i] -
                    'a' + 1) * power_of_p) % m;
        power_of_p = (power_of_p * p) % m;
    }
    return (unsigned long) hash_val;
}
