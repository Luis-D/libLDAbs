/**
 * @file
 * @brief A compilation of "tricks" and "hacks" to make programming easy and lazy.
 */

//A compilation of "tricks" and "hacks" to make programming easy and lazy.
//  Luis Delgado. 2018
//  Luis Delgado. 2019
//  Luis Delgado. 2020

#ifndef __LD_TRICKS_H_
#define __LD_TRICKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/** @name Iterations on array utilities 
 * @brief Utilities to iterate over an array based on pointers
 * @param type Data Type of the iterator variable
 * @param Item Name of the iterator variable
 * @param from Pointer from where the iteration will start. The first element.
 * @param to Pointer where the iteration will end. The last element.
 * */
///@{
#define array_range(type,Item,from,to) for (type * Item = from; Item<=to; (Item)++)
#define array_range_reverse(type,Item,from,to) for (type * Item = from; Item>=to; (Item)--)
#define array_range_omni(type,Item,from,to) for (type * Item = from; Item!=to+(from<=to); (Item)+=((from<=to) - (!(from<=to))))
///@}

/** @name 'foreach' implementation
 * @brief Utilities to iterate over an array based on the data type size and the number of objects in it.
 * @param type Data Type of the iterator variable
 * @param Item Name of the iterator variable
 * @param Array Pointer to an array to iterate over
 * @param Length Number of objects in the Array. Length of the Array.
 */
///@{
#define array_foreach(type,Item,Array,Length) for(type * Item = Array; Item <(Array+Length);(Item)++)
#define array_foreach_reverse(type,Item,Array,Length) for(type * Item = (Array+(Length-1));Item>=Array;(Item)--)
///@}


/** @name Printing utilities */
///@{
#define printn printf("\n");
///@}

/** @name Algebraic prints
 * @brief Utilities to print many numbers. \b V refers to vectors and \b M refers to matrices.
 * @param x  Pointer to a single floating point number, a vector or matrix.
 * */
///@{
#define printV4(x) \
    printf("(%f,%f,%f,%f)", \
	    ((float*)(x))[0],((float*)(x))[1],((float*)(x))[2],((float*)(x))[3])
#define printV3(x) \
    printf("(%f,%f,%f)", \
	    ((float*)(x))[0],((float*)(x))[1],((float*)(x))[2])
#define printV2(x) \
    printf("(%f,%f)", \
	    ((float*)(x))[0],((float*)(x))[1])
#define printV1(x) \
    printf("(%f)", \
	    ((float*)(x))[0])
#define printM4(x) \
    printf("(%f,%f,%f,%f)",((float*)(x))[0],((float*)(x))[1],((float*)(x))[2],((float*)(x))[3]); \
    printf("(%f,%f,%f,%f)",((float*)(x))[4],((float*)(x))[5],((float*)(x))[6],((float*)(x))[7]); \
    printf("(%f,%f,%f,%f)",((float*)(x))[8],((float*)(x))[9],((float*)(x))[10],((float*)(x))[11]); \
    printf("(%f,%f,%f,%f)",((float*)(x))[12],((float*)(x))[13],((float*)(x))[14],((float*)(x))[15]) 
#define printM2(x) \
    printf("(%f,%f)",((float*)(x))[0],((float*)(x))[1])	\
    printf("(%f,%f)",((float*)(x))[2],((float*)(x))[3])
///@}


//some arcane and esoteric:
#define _repeat(counter,times) for(uintptr_t counter = times;i--;)
#define _swap(a,b) a = a ^ b; b = a ^ b; a = a ^ b
#define _test(var,bitmask) ((var & bitmask) == bitmask)
#define _checkbits(var,bitmask) _test(var,bitmask)

#define _swap32(a,b) _swap(*(uint32_t*)(&(a)),*(uint32_t*)(&(b)));
#define _swap64(a,b) _swap(*(uint64_t*)(&(a)),*(uint64_t*)(&(b)));

#endif
