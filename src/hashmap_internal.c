// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)


/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include "hashmap_internal.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/



/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/


/* hashing function interface for double support */
Hash hash1 = fnv_hash_string;
Hash hash2 = djb2;

// size_t hash_probe(uint64_t hash, size_t i, size_t cap)
// {
//     //return (hash + ( (-1) * (i * i + 1) * (i / 2) * (i / 2))) % cap;
//     return (size_t) (hash + (-1) * (i*i)) % cap;
// }

size_t hash_probe(uint64_t hash, size_t i, size_t cap)
{

    if(i & 1) { // odd
        return (hash + -(i*i)) % cap;
    } else {
        return (hash + (i*i)) % cap;
    }
}

int bucket_insert(bucket *bucket, const char *key, void *value)
{
    bucket->key = key;
    bucket->value = value;
    
    return 1;
}

int bucket_ifnull_insert(bucket *bucket, const char *key, void *value)
{
    if(bucket->key == NULL) {
        bucket->key = key;
        bucket->value = value;
        return 1;
    }
    
    return 0;
}

int bucket_compare_insert(bucket *bucket, const char *key, void *value)
{
    if(bucket_ifnull_insert(bucket, key, value)) {
        return 1;
    }

    if(strcmp(bucket->key, key) == 0) {
        return 1;
    }

    return 0;
}

int bucket_overwrite_insert(bucket *bucket, const char *key, void *value)
{
    if(bucket_ifnull_insert(bucket, key, value)) {
        return 1;
    }

    if(strcmp(bucket->key, key) == 0) {
        return bucket_insert(bucket, key, value);
    }

    return 0;
}

int bucket_list_probe(int (*bucket_handler_func)(bucket *, const char *key, void *data), bucket *list, size_t capacity, const char *key, void *data)
{
    size_t i = 0;
    size_t hash = strhash(key);
    size_t probe = 0;


    for(i = 0; i < capacity; i++) {
        probe = hash_probe(hash, i, capacity);

        if(bucket_handler_func(&list[probe], key, data)) {
            return 0;
        }

    }

    return -1;

}

int bucket_list_insert(bucket *list, size_t capacity, const char *key, void *value)
{
    size_t i = 0;
    size_t hash = strhash(key);
    size_t probe = 0;
    //printf("hashing string: %s\n", key);

    /* starting at 1 because 1 and 0 would get the same probe so we can safe that step */
    for(i = 1; i <= capacity; i++) {

        probe = hash_probe(hash, i, capacity);
        //printf("probe: %zu\n", probe);
        if(list[probe].key == NULL)
        {
            list[probe].key   = key;
            list[probe].value = value;
            return 0; 
        }
    }

    //printf("hashing end\n");

    return -1;    
}

int bucket_list_compare_insert(bucket *list, size_t capacity, const char *key, void *value)
{
    size_t i = 0;
    size_t hash = strhash(key);
    size_t probe = 0;


    for(i = 1; i <= capacity; i++) {

        probe = hash_probe(hash, i, capacity);

        if(list[probe].key == NULL)
        {
            list[probe].key   = key;
            list[probe].value = value;
            return 0;
        }

        if(strcmp(list[probe].key, key) == 0) {
            return 0;
        }
    }

    return -1;

}

int bucket_list_overwrite_insert(bucket *list, size_t capacity, const char *key, void *value)
{
        size_t i = 0;
    size_t hash = strhash(key);
    size_t probe = 0;


    for(i = 1; i <= capacity; i++) {

        probe = hash_probe(hash, i, capacity);

        if(list[probe].key == NULL)
        {
            list[probe].key   = key;
            list[probe].value = value;
            return 0;
        }

        if(strcmp(list[probe].key, key) == 0) {
            list[probe].key   = key;
            list[probe].value = value;
            return 0;
        }
    }

    return -1;
}

bucket* bucket_list_return(bucket *list, size_t capacity, const char *key)
{

    size_t i = 0;
    size_t probe = 0;
    size_t hash = strhash(key);

    for(i = 1; i <= capacity; i++) {

        probe = hash_probe(hash, i, capacity);

        if(list[probe].key != NULL)
        {
            if(strcmp(key, list[probe].key) == 0) {
                return &list[probe];
            }
        }
    }
    
    return NULL;
}

void hashhmap_rehash(bucket *old_list, size_t old_cap, bucket *new_list, size_t new_cap)
{
    for(size_t i = 0; i < old_cap; i++) {
        if(old_list[i].key != NULL) {
            //bucket_list_probe(bucket_ifnull_insert, new_list, new_cap, old_list[i].key, old_list[i].value);
            bucket_list_insert(new_list, new_cap, old_list[i].key, old_list[i].value);
        }
    }

}

int hashhmap_realloc(hashmap *hmap, size_t new_cap)
{
    if(new_cap < HASHMAP_DEFAULT_CAPACITY) {
        return 0;
    }

    bucket *new_list = calloc(new_cap, sizeof *hmap->map);
    if(new_list == NULL) {
        return ENOMEM;
    }

    hashhmap_rehash(hmap->map, hmap->cap, new_list, new_cap);
    free(hmap->map);

    hmap->map = new_list;
    hmap->cap = new_cap;

    return 0;
}

// doubles the capacitiy if conditions are met
int hashhmap_grow(hashmap *hmap)
{
    // if number of elements in hashhmap lower than (hmap->cap * load factor) or hashhmap is fixed -> return 0 */
    if(hmap->n < (size_t) ((double) hmap->cap * HASHMAP_DEFAULT_LOADFACTOR) || hmap->fixed) {
        return 0;
    }
    //size_t new_cap = GetHigherPrime(hmap->cap * 2);
    size_t new_cap = GetHigher3mod4Prime(hmap->cap * 2);

    if(hashhmap_realloc(hmap, new_cap) != 0) {
        return ENOMEM;
    }

    return 0;
}

// bisects capacity if conditions are met
int hashhmap_shrink(hashmap *hmap)
{

    // we only want to shrink if 
    // if(((hmap->n > (size_t) ((double) (hmap->cap) / 2 * (1.0 - HASHhmap_DEFAULT_LOADFACTOR))) )|| hmap->fixed) {
    //     return 0;
    // }

    if(((hmap->n > (size_t) ((double) (hmap->cap) / 4 * (1.0 - HASHMAP_DEFAULT_LOADFACTOR))) )|| hmap->fixed) {
        return 0;
    }

    size_t new_cap = GetLower3mod4Prime(hmap->cap / 2);
    
    //printf("hmap->n = %zu loadfactor value: %zu cap is: %zu\n", hmap->n, (size_t) ((double) (hmap->cap / 4) * (1.0 - HASHhmap_DEFAULT_LOADFACTOR)), hmap->cap);
    
    //size_t new_cap = GetLowerPrime(hmap->cap / 2);
    //size_t new_cap = GetLower3mod4Prime(hmap->cap / 2); // removed to get next prime number before testing (see. above)

    if(hashhmap_realloc(hmap, new_cap) != 0) {
        return ENOMEM;
    }

    return 0;
}

/* general hash function to hash a string based on Hash hash1 and Hash hash2 (see. above) */
size_t strhash(const char *str)
{
    size_t hash;

    if(hash1 != NULL && hash2 != NULL) {
        hash = hash1(str) + hash2(str);
    }
    else {
        hash = hash1(str);
    }

    return hash;
}