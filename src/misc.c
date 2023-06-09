// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "misc.h"

#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>


/* Definition of prime number functions */

bool IsPrime(size_t n)
{
    // (n & 1) checks if number is odd
    if((n & 1) != 0) {
        for(size_t divisor = 3; divisor <= n / divisor; divisor += 2) {
            if((n % divisor) == 0) {
                return 0;
            }
        }
        return n > 1;
    }
    return (n == 2);
}
/**
 * @brief Get the Next 3 mod 4 Prime number
 * 
 * @param direction direction to go to
 * @return size_t 
 */
size_t GetNext3mod4Prime(size_t n, int direction)
{
    for(size_t i = (n | 1); i < LONG_MAX && i >= 0; i += (long long) direction) {
        if(IsPrime(i) && (i & 3) == 3) {
            return i;
        }
    }
    return n;
}

size_t GetHigher3mod4Prime(size_t n) {
    return GetNext3mod4Prime(n, HIGHER_PRIME);
}

size_t GetLower3mod4Prime(size_t n) {
    return GetNext3mod4Prime(n, LOWER_PRIME);
}


size_t GetNextPrime(size_t n, int direction) {

    // (n | 1) add 1 to even numbers or let odd number be
    // direction should be +2 or -2 to step up or down by to because prime numbers are all odd
    // if next prime is found it will be returned
    for(size_t i = (n | 1); i < LONG_MAX && i > 0; i += (long long) direction) {
        if(IsPrime(i)) {
            return i;
        }
    }
    return n;
}

size_t GetHigherPrime(size_t n) {
    return GetNextPrime(n, HIGHER_PRIME);
}

size_t GetLowerPrime(size_t n) {
    return GetNextPrime(n, LOWER_PRIME);
}


/* hashing functions */
uint64_t fnv_hash_string(const char *str)
{
    size_t hash = FNV_OFFSET;

    while(*str) {
        hash = hash ^ (size_t) *str;
        hash = hash * FNV_PRIME;
        str++;
    }
    return hash;
}

uint64_t fnv_hash(unsigned char *data, size_t size)
{
    size_t hash = FNV_OFFSET;

    while(size--) {
        hash = hash ^ *data;
        hash = hash * FNV_PRIME;
        data++;
    }
    return hash;
}

size_t djb2(const char *str)
{
    /* djb2 offset 5381 */
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + (size_t) c; /* hash * 33 + c */

    return hash;
}


