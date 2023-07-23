// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef _HASHDB_MISC_H
#define _HASHDB_MISC_H

/*********************************************************************************************************/
/*                                               DEPENDANICIES                                           */
/*********************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

/*********************************************************************************************************/
/*                                                DEFINITIONS                                            */
/*********************************************************************************************************/

/** @brief Offset for FVN-Hashing
 */
#define FNV_OFFSET 0xCBF29CE484222325

/** @brief Prime for xor overation on character
 */
#define FNV_PRIME 0x1099511628211

/** @brief next higher direction for GetNextPrime
 */
#define HIGHER_PRIME 2
/** @brief next lower direction for GetNextPrime
 */
#define LOWER_PRIME -2

/**********************************************************************************************************/
/*                                            FUNCTION PROTOTYPES                                         */
/**********************************************************************************************************/

/** @brief Returns hash index based on quadratic probing
 * @param[in]   hash uint64 hash
 * @param[in]   i index of loop
 * @param[in]   cap capacity of bucket list
 */
size_t hash_probe(uint64_t hash, size_t i, size_t cap);

/** @brief Test if number is prime function
 * @param[in]     n   number to test
 * @retval    true
 * @retval    false
 */
bool is_prime(size_t n);

/**
 * @brief Get the Next 3 mod 4 Prime number
 *
 * @param direction direction to go to
 * @return size_t
 */
size_t get_next_prime(size_t n, int direction);

/**
 * @brief Get the next higher 3mod 4 congruent Prime number
 *
 * @param n number to get the next heigher prime from
 * @retval  next higher 3mod4 congruent prime number
 */
size_t get_higher_prime(size_t n);

/**
 * @brief Get the next lower 3mod 4 congruent Prime number
 *
 * @param n number to get the next lower prime from
 * @retval  next higher 3mod4 congruent prime number
 */
size_t get_lower_prime(size_t n);

/** @brief Get FVN-Hash by bytedata
 @param[in]     data    bytes to hash
 @param[in]     size    size of bytes to hash
 @retval    uint64_t hash
*/
uint64_t fnv_hash(void *data, size_t *size);

#endif