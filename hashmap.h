#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>

#define HASHMAP_DEFAULT_CAP 3

typedef struct bucket
{
    void *key;
    size_t keysz;
    void *data;
} bucket_t;

typedef struct hashmap
{
    size_t count;
    size_t cap;
    bucket_t *buckets;
} hashmap_t;

hashmap_t *hashmap_create();

void hashmap_destroy(hashmap_t *map);

int hashmap_add(hashmap_t *map, void *key, size_t keysz, void *data);
void *hashmap_get(hashmap_t *map, void *key, size_t keysz);
void *hashmap_del(hashmap_t *map, void *key, size_t keysz);

size_t hashmap_count(hashmap_t *map);
double hashmap_fillage(hashmap_t *map);
void hashmap_info(hashmap_t *map);

#endif