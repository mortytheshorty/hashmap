#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>

#define HASHMAP_DEFAULT_CAP 31

/** @struct bucket
 *  bucket datatype
 *  
 *  @var bucket::key
 *      pointer to key data
 *  @var bucket::keysz
 *      size of key
 *  @var bucket::data
 *      pointer to data from key
 */
typedef struct bucket
{
    void *key;
    size_t keysz;
    void *data;
} bucket_t;

/** @struct hashmap
 *  hashmap datatype
 * 
 * @var hashmap::count
 *      number of elements
 * @var hashmap::cap
 *      maximum number of elements
 * @var hashmap::buckets
 *      bucket list with entries
 */
typedef struct hashmap
{
    size_t count;
    size_t cap;
    bucket_t *buckets;
} hashmap_t;

/**
 * @brief Creates a hashmap
 * 
 * @return hashmap_t* 
 */
hashmap_t *hashmap_create();

/**
 * @brief Destroys a hashmap
 * 
 * @param map   pointer to hashmap
 */
void hashmap_destroy(hashmap_t *map);

/**
 * @brief Adds a key value pair to the hashmap
 * 
 * @param map   pointer to hashmap
 * @param key   key (string or bytes)
 * @param keysz key size, 0 == string, n == bytes
 * @param data  pointer to data
 * @return int 
 */
int hashmap_add(hashmap_t *map, void *key, size_t keysz, void *data);

/**
 * @brief Returns value from a given key
 * 
 * @param map   pointer to hashmap
 * @param key   key (string or bytes)
 * @param keysz key size, 0 == string, n == bytes
 * @return void* 
 */
void *hashmap_get(hashmap_t *map, void *key, size_t keysz);

/**
 * @brief Deletes key value pair from hashmap and returns its value
 * 
 * @param map   pointer to hashmap
 * @param key   key (string or bytes)
 * @param keysz key size, 0 == string, n == bytes
 * @return void* 
 */
void *hashmap_del(hashmap_t *map, void *key, size_t keysz);

/**
 * @brief Returns number of entries
 * 
 * @param map   pointer to hashmap
 * @return size_t 
 */
size_t hashmap_count(hashmap_t *map);

/**
 * @brief Returns fillage
 * 
 * @param map   pointer to hashmap
 * @return size_t 
 */
double hashmap_fillage(hashmap_t *map);

/**
 * @brief Returns info 
 * 
 * @param map   pointer to hashmap
 * @return size_t 
 */
void hashmap_info(hashmap_t *map);

//bucket_t* hashmap_iterate(hashmap_t *map);

#endif