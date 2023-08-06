#include "hashmap.h"

#include <inttypes.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

/*********************************************************************************************************/
/*                                                DEFINITIONS                                            */
/*********************************************************************************************************/

/**
 * @brief Factor of fillage when to grow or shrink
 */
#define HASHMAP_REBUILD_FACTOR 0.7

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
/*                                      MISCELLANEOUS FUNCTION PROTOTYPES                                 */
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





hashmap_t *hashmap_create()
{
    hashmap_t *map = (hashmap_t *)calloc(1, sizeof *map);
    if (map == NULL)
    {
        return NULL;
    }

    map->buckets = (bucket_t *)calloc(HASHMAP_DEFAULT_CAP, sizeof *map->buckets);
    if (map->buckets == NULL)
    {
        free(map);
        return NULL;
    }

    map->cap = HASHMAP_DEFAULT_CAP;

    return map;
}

void hashmap_destroy(hashmap_t *map)
{
    free(map->buckets);
    free(map);
}

/**
 * @brief Returns the bucket for given key hash
 * 
 * @param buckets   pointer to bucket list memory
 * @param cap       capacity of bucket list
 * @param key       key to get bucket for
 * @param keysz     size of key (if not string)
 * @return bucket_t* 
 */
bucket_t *hashmap_get_bucket(bucket_t *buckets, size_t cap, void *key, size_t keysz)
{
    size_t size = keysz;
    uint64_t hash = fnv_hash(key, &size);

    for (size_t i = 0; i < cap; i++)
    {
        size_t probe = hash_probe(hash, i, cap);

        if (buckets[probe].key == NULL)
        {
            buckets[probe].keysz = size;
            return &buckets[probe];

        }

        if (buckets[probe].keysz != size)
        {
            continue;
        }

        if (memcmp(buckets[probe].key, key, size) == 0)
        {
            buckets[probe].keysz = size;
            return &buckets[probe];
        }
    }

    return NULL;
}

/**
 * @brief Calculates the new capacity (based on fillage)
 * 
 * @param map   pointer to hashmap
 * @return size_t 
 */
size_t hashmap_newcap(hashmap_t *map)
{
    size_t new_cap = 0;

    double fillage = (double) map->count / (double) map->cap;

    if (map->count == 0)
    {
        return map->cap;
    }
    else if (fillage > HASHMAP_REBUILD_FACTOR)
    {
        new_cap = get_higher_prime(map->cap * 2);
    }
    else if (fillage < (HASHMAP_REBUILD_FACTOR / 2))
    {
        new_cap = get_lower_prime(map->cap / 2);
    }
    else
    {
        new_cap = map->cap;
    }

    return new_cap;
}

/**
 * @brief 
 * 
 * @param map 
 * @return int 
 */
int hashmap_rebuild(hashmap_t *map)
{
    size_t newcap = hashmap_newcap(map);

    if (newcap == map->cap)
    {
        return 0;
    }

    bucket_t *newbuckets = calloc(newcap, sizeof *newbuckets);
    if (newbuckets == NULL)
    {
        return -1; // mem error
    }

    /* iterate over the hashmap and insert buckets into the new allocated memory */
    for (size_t i = 0; i < map->cap; i++)
    {
        if (map->buckets[i].key == NULL)
        {
            continue;
        }
        bucket_t *current = hashmap_get_bucket(newbuckets, newcap, map->buckets[i].key, map->buckets[i].keysz);
        if (current == NULL)
        {
            printf("WARNING: no enough space in new bucket list");
            continue;
        }

        current->key = map->buckets[i].key;
        current->keysz = map->buckets[i].keysz;
        current->data = map->buckets[i].data;
    }

    free(map->buckets);

    map->buckets = newbuckets;
    map->cap = newcap;

    return 0;
}

int hashmap_add(hashmap_t *map, void *key, size_t keysz, void *data)
{
    bucket_t *bucket = hashmap_get_bucket(map->buckets, map->cap, key, keysz);
    if (bucket == NULL)
    {
        return -1; // failed
    }

    bucket->key = key;
    bucket->data = data;
    map->count++;

    int ret = hashmap_rebuild(map);

    if (ret == -1)
    {
        printf("mem error\n");
    }

    return 0;
}

void *hashmap_get(hashmap_t *map, void *key, size_t keysz)
{
    bucket_t *bucket = hashmap_get_bucket(map->buckets, map->cap, key, keysz);
    if (bucket == NULL)
    {
        return NULL; // failed
    }

    return bucket->data;
}
void *hashmap_del(hashmap_t *map, void *key, size_t keysz)
{
    bucket_t *bucket = hashmap_get_bucket(map->buckets, map->cap, key, keysz);
    if (bucket == NULL)
    {
        return NULL; // failed
    }

    void *data = bucket->data;

    memset(bucket, 0, sizeof *bucket);

    map->count--;

    int ret = hashmap_rebuild(map);
    if (ret == -1)
    {
        printf("mem error\n");
    }

    return data;
}

size_t hashmap_count(hashmap_t *map)
{
    return map->count;
}

double hashmap_fillage(hashmap_t *map)
{
    return (double) map->count / (double) map->cap;
}

void hashmap_info(hashmap_t *map)
{
    printf("Hashmap:\n");
    printf("Count: %zu\n", map->count);
    printf("Cap: %zu\n", map->cap);
    printf("Fillage: %lf\n", hashmap_fillage(map));

    for (size_t i = 0; i < map->cap; i++)
    {
        if (map->buckets[i].key == NULL)
            continue;

        printf("Key: %s, KeySz: %zu, Data: %s\n", (char*) map->buckets[i].key, map->buckets[i].keysz, (char*) map->buckets[i].data);
    }
}


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

size_t get_higher_prime(size_t n)
{
    return get_next_prime(n, HIGHER_PRIME);
}

size_t get_lower_prime(size_t n)
{
    return get_next_prime(n, LOWER_PRIME);
}

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