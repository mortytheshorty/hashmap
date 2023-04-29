// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

#include "hashmap_internal.h"


/** @brief Default capacity for hashmap object
 */
#define hashmap_DEFAULT_CAP 7

/** @typedef
 * @brief placeholder definition of hashmap_enty
 */

int hashmap_add(hashmap *db, const char *key, void *value)
{

    /* check if it should grow */
    if(hashmap_grow(db) != 0) {
        return ENOMEM;
    }

    if(db->n == db->cap) {
        return -1; /* full only happens if hashmap::*/
    }

    if(bucket_list_insert(db->map, db->cap, key, value)) {
        return -1; /* internal error */
    }

    db->n++;

   

    return 0;
}

size_t hashmap_cap(hashmap *db)
{
    return db->cap;
}

void hashmap_clear(hashmap *db)
{
    size_t sz = sizeof *db->map * db->cap;
    memset(db->map, 0, sz);
    db->n = 0;
}

hashmap* hashmap_create()
{
    hashmap *db = calloc(1, sizeof *db);
    if(db == NULL) {
        return NULL;
    }

    db->cap = hashmap_DEFAULT_CAP;
    db->map = calloc(hashmap_DEFAULT_CAP, sizeof *db->map);
    if(db->map == NULL) {
        free(db);
        return NULL;
    }

    return db;
}

void* hashmap_del(hashmap *db, const char *key)
{
    bucket *item = bucket_list_return(db->map, db->cap, key);
    if(item == NULL) {
        return item;
    }

    void *value = item->value;
    //printf("deleted key: %s by given key %s\n", item->key, key);
    item->key = NULL;
    item->value = NULL;
    db->n--;
    if(hashmap_shrink(db) != 0) {
        return NULL;
    }


    return value;
}

void hashmap_destroy(hashmap *db)
{
    free(db->map);
    free(db);
}


int hashmap_ensure_capfix(hashmap *db, size_t new_capacity)
{
    size_t new_cap = GetHigherPrime(new_capacity);

    if(hashmap_realloc(db, new_cap) != 0) {
        return ENOMEM;
    }

    db->fixed = true;

    return 0;
}

int hashmap_ensure_capacity(hashmap *db, size_t new_capacity)
{
    size_t new_cap = GetHigherPrime(new_capacity);

    if(hashmap_realloc(db, new_cap) != 0) {
        return ENOMEM;
    }

    return 0;
}

void hashmap_fix(hashmap *db, bool newval)
{
    db->fixed = newval;
}

void* hashmap_get(hashmap *db, const char *key)
{
    bucket *item = bucket_list_return(db->map, db->cap, key);
    if(item == NULL) {
        return item; /* not found */
    }

    return item->value;
}

size_t hashmap_len(hashmap *db)
{
    return db->n;
}

size_t hashmap_memory_usage(hashmap *db) {
    return db->cap * sizeof *db->map;
}

int hashmap_next(hashmap *db, bucket *buckent)
{
    size_t i = db->iterator;
    bucket *entry = (bucket*) buckent;
    entry->key    = NULL;
    entry->value  = NULL;

    if(db->iterator == db->cap) {
        db->iterator = 0;
        return 0;    // need to reset
    }

    // loop over map and set buckent to key/value
    while(i < db->cap) {
        if(db->map[i].key != NULL)
        {
            entry->key = db->map[i].key;
            entry->value = db->map[i].value;
            db->iterator = ++i;
            return 1;
        }

        i++;
    }
    db->iterator = 0;
    return 0;
}

double hashmap_percentage(hashmap *db)
{
    return (double) (double) db->n / (double) db->cap * (double) 100;
}

int hashmap_reset(hashmap *db)
{
    //printf("resetting\n");
    free(db->map);
    bucket *new_map = calloc(hashmap_DEFAULT_CAP, sizeof *new_map);
    if(new_map == NULL) {
        return ENOMEM;
    }
    //printf("worked\n");
    db->map = new_map;
    db->cap = hashmap_DEFAULT_CAP;
    db->n   = 0;

    return 0;
}

void hashmap_reset_iterator(hashmap *db)
{
    db->iterator = 0;
}

int hashmap_to_http(hashmap *db, char *buffer, size_t bufsz)
{
    bucket pair;

    while(hashmap_next(db, &pair) && bufsz) {
        const char *key   = pair.key;
        const char *value = pair.value;

        while(*key!= '\0' && bufsz) {
            *buffer++ = *key++;
            bufsz--;
        }
        if(bufsz) {
            *buffer++ = ':'; bufsz--;
        }

        if(bufsz) {
            *buffer++ = ' '; bufsz--;
        }

        while(*value != '\0' && bufsz) {
            *buffer++ = *value++;
            bufsz--;
        }

        if(bufsz) {
            *buffer++ = '\n';
            bufsz--;
        }
        
    }

    if(db->iterator != 0) {
        *buffer = '\0';
        return -1;
    }

    if(bufsz) {
        *buffer = '\0';
    }

    return 0;
}

int hashmap_merge(hashmap *db_dest, hashmap *db_src)
{
    bucket pair = { 0 };

    while(hashmap_next(db_src, &pair)) {

        if(hashmap_grow(db_dest) != 0) {
            return ENOMEM;
        }

        if(bucket_list_compare_insert(db_dest->map, db_dest->cap, pair.key, pair.value) == 0) {
            db_dest->n++;
        }

    }

    return 0;
}

int hashmap_update(hashmap *db_dest, hashmap *db_src)
{
    bucket pair = { 0 };

    while(hashmap_next(db_src, &pair)) {

        if(hashmap_grow(db_dest) != 0) {
            return ENOMEM;
        }

        if(bucket_list_probe(bucket_overwrite_insert, db_dest->map, db_dest->cap, pair.key, pair.value) == 0) {
            db_dest->n++;
        }
    }

    return 0;
}