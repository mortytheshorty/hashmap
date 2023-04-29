// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef _hashmap_INTERNAL_H
#define _HASHBD_INTERNAL_H


/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include "misc.h"

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

/* definitions */
#define HASHMAP_DEFAULT_CAPACITY 19           /* prime number */
#define HASHMAP_DEFAULT_LOADFACTOR 0.5     /*  */

typedef struct hashmap_entry hashmap_entry;


/** @struct bucket
 * @brief bucket containing key and value pointers
 * @var bucket::key     key string
 * @var bucket::value   value pointer
 */
typedef struct bucket {
    const char *key;
    void *value;
} bucket;


/** @struct hashmap
 * @brief structure holding state of hashmap
 * 
 * @var hashmap::cap         capacity
 * @var hashmap::n           number of elements inserted
 * @var hashmap::map         pointer to bucket list    
 * @var hashmap::fixed       boolean value for set hashmap to fixed capacity
 * @var hashmap::iterator    pointer to hashmap iterator
 */
typedef struct hashmap {
    size_t cap;
    size_t n;
    bucket *map;
    bool fixed;
    size_t iterator;
} hashmap;

/*********************************************************************************************************/
/*                                           FUNCTION PROTOTYPES                                         */
/*********************************************************************************************************/

/* get index by this ... formular */
size_t hash_probe(uint64_t hash, size_t i, size_t cap);

int bucket_list_probe(int (*bucket_handler_func)(bucket *, const char *key, void *data), bucket *list, size_t capacity, const char *key, void *data);
int bucket_insert(bucket *bucket, const char *key, void *value);
int bucket_ifnull_insert(bucket *bucket, const char *key, void *value);
int bucket_compare_insert(bucket *bucket, const char *key, void *value);
int bucket_overwrite_insert(bucket *bucket, const char *key, void *value);

/* hash interface */
/**
 * @brief Hash function interface
 * 
 */
typedef size_t (*Hash)(const char *str);

/** @public
 * @brief Insert key/value pair into bucket list by capacity
 * 
 * @param[in] list      pointer to bucket list
 * @param[in] capacity  capacity of bucket list
 * @param[in] key       key
 * @param[in] value     value
 * @retval          0 on succes
 * @retval          -1 on failure 
 */

int bucket_list_insert(bucket *list, size_t capacity, const char *key, void *value);

/**
 * @brief Insert key/value pair into bucket list by capacity and compare (if match then no insert is done)
 * 
 * @param[in] list 
 * @param[in] capacity 
 * @param[in] key 
 * @param[in] value 
 * @retval  0 on success
 * @retval -1 on failure
 */
int bucket_list_compare_insert(bucket *list, size_t capacity, const char *key, void *value);

/**
 * @brief return bucket pointer from bucket list by capacity
 * 
 * @param[in] list      poitner to bucket list
 * @param[in] capacity  capacity of bucket list
 * @param[in] key       key to find value of
 * @retval          pointer to bucket on succes
 * @retval          NULL pointer on failure
 */

bucket* bucket_list_return(bucket *list, size_t capacity, const char *key);

/** @public
 * @brief Checks if bucket list needs reallocations base on load factor and number of added pairs
 * 
 * @param[in] hmap        pointer to hashmap
 * @retval          0 on succes or if no reallocation needed
 * @retval          ENONMEM if realloction fails
 */
int hashmap_grow(hashmap *hmap);

/** @public
 * @brief Reallocate the bucket list and also rehashing it
 * 
 * @param[in] hmap        pointer to hashmap
 * @param[in] new_cap   new capacity
 * @retval          0 on succes 
 * @retval          ENOMEM on failure (errno.h)
 */
int hashmap_realloc(hashmap *hmap, size_t new_cap);

/** @public
 * @brief Rehashing of old bucket list into new bucket list (called by hashmap_realloc)
 * 
 * @param[in] old_list  pointer to old bucket list
 * @param[in] old_cap   capacity of old bucket list
 * @param[out] new_list  pointer to new bucket list
 * @param[out] new_cap   capacity of new bucket list
 */
void hashmap_rehash(bucket *old_list, size_t old_cap, bucket *new_list, size_t new_cap);

/**
 * @brief Checks if bucket list needs reallocations base on (1 - load factor) and number of added pairs
 * 
 * @param[in] hmap        pointer to hashmap
 * @retval          0 on succes or if no reallocation needed
 * @retval          ENONMEM if realloction fails     
 */
int hashmap_shrink(hashmap *hmap);

/**
 * @brief Hashes a string based on hash1 and hash2
 *  if hash2 == NULL hash1 only is used
 *  if hash2 != NULL double hashing is used
 * 
 * @param[in] str       string to hash
 * @retval      uint64_t hash value
 */
uint64_t strhash(const char *str);


#endif