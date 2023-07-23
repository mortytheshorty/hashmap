// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "misc.h"



/**
 * @brief Returns index based on current loop index and bucketlist capacity
 * 
 * @param hash  hash of key
 * @param i     index
 * @param cap   capacity
 * @return size_t 
 */
size_t hash_probe(uint64_t hash, size_t i, size_t cap)
{

    /** this will hit every single bucket index if cap is always a prime number */
    if (i & 1)
    { // odd
        return (hash + -(i * i)) % cap;
    }
    else
    {
        return (hash + (i * i)) % cap;
    }
}

/* Definition of prime number functions */

/**
 * @brief Returns true if number is prime, false if not
 * 
 * @param n     number
 * @return true 
 * @return false 
 */
bool is_prime(size_t n)
{
    // (n & 1) checks if number is odd
    if ((n & 1) != 0)
    {
        for (size_t divisor = 3; divisor <= n / divisor; divisor += 2)
        {
            if ((n % divisor) == 0)
            {
                return 0;
            }
        }
        return n > 1;
    }
    return (n == 2);
}

/**
 * @brief Get the Next Prime object
 * 
 * @param n 
 * @param direction 
 * @return size_t 
 */
size_t get_next_prime(size_t n, int direction)
{

    // (n | 1) add 1 to even numbers or let odd number be
    // direction should be +2 or -2 to step up or down by to because prime numbers are all odd
    // if next prime is found it will be returned
    for (size_t i = (n | 1); i < LONG_MAX && i > 0; i += (long long)direction)
    {
        if (is_prime(i))
        {
            return i;
        }
    }
    return n;
}

/**
 * @brief Get the Higher 3 mod 4 Prime object
 * 
 * @param n 
 * @return size_t 
 */
size_t get_higher_prime(size_t n)
{
    return get_next_prime(n, HIGHER_PRIME);
}

/**
 * @brief Get the Lower 3 mod 4 Prime object
 * 
 * @param n 
 * @return size_t 
 */
size_t get_lower_prime(size_t n)
{
    return get_next_prime(n, LOWER_PRIME);
}

/* hashing functions */

/**
 * @brief Hashes a string or byte data
 * NOTE: it will assume a string if size is set to 0 and it will also update size
 *       if was zero
 *
 * @param data
 * @param size
 * @return uint64_t
 */
uint64_t fnv_hash(void *data, size_t *size)
{
    size_t hash = FNV_OFFSET;
    unsigned char *d = data;

    if (*size == 0)
    {
        *size = strlen(data);
    }
    size_t size_bak = *size;
    while (size_bak--)
    {
        hash = hash ^ *d++;
        hash = hash * FNV_PRIME;
    }
    return hash;
}