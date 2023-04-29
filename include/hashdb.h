#ifndef _HMAP_H
#define _HMAP_H


/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

/** @typedef
 * @brief hashdb holds state
 */
typedef struct hashdb hashdb;

/** @struct hashdb_entry
 * @brief Argument for iterator function `hashdb_next()`
 * @var hashdb_entry::key     pointer to key string
 * @var hashdb_entry::value   pointer to value
 */
typedef struct hashdb_entry {
    const char *key;
    size_t keysz;
    void *value;
} hashdb_entry;


/*********************************************************************************************************/
/*                                           FUNCTION PROTOTYPES                                         */
/*********************************************************************************************************/

/**
 * @brief Adds key and data to hashdb
 * 
 * @param db[in]    hashdb pointer
 * @param key[in]   pointer to key string
 * @param value[in] pointer to value
 * @retval  0 on success
 * @retval -1 on failure if hashdb::cap == hashdb::n (probably cause because hashdb::fix is true)
 * @retval ENOMEM on failure if internal reallocation fails
 */
int hashdb_add(hashdb *db, const char *key, const void *value);

int hashdb_add(hashdb *db, const void *data, size_t sz, void *value);

/**
 * @brief Get current capacity
 * 
 * @param[in] db hashdb pointer
 * @retval  capacity
 */
size_t hashdb_cap(hashdb *db);

/**
 * @brief Clears all data but keeps capacity
 * 
 * @param[in] db
 */
void hashdb_clear(hashdb *db);

/**
 * @brief Creates a hashdb object
 * @return hashdb pointer
 */
hashdb* hashdb_create(void);

/**
 * @brief Deletes key and value from hashdb
 * 
 * @param[in] db        hashdb pointer 
 * @param[in] key       pointer to key string
 * @retval associated value from key on success
 * @retval NULL on failure
 */
void* hashdb_del(hashdb *db, const char *key);

/**
 * @brief Destroys a hashdb object
 * @param[in] db hashdb pointer
*/
void hashdb_destroy(hashdb *db);

/**
 * @brief Enables the hashdb iterator interface
 * 
 * @param[in]   db      hashdb from which to enable iterating
 * @param[in]   val     boolean value 
 * @retval   0 on succes
 * @retval  -1 on failure
 */
int hashdb_enable_iterator(hashdb *db, bool val);

/**
 * @brief Ensures memory for whished capacity and fix hashdb
 * 
 * @param[in] db            hashdb pointer
 * @param[in] new_capacity  new capacity
 * @return int 
 */
int hashdb_ensure_cap_fix(hashdb *db, size_t new_capacity);

/**
 * @brief Ensures memory for wished capacity
 * 
 * @param[in] db            hashdb pointer
 * @param[in] new_capacity  new capacity
 * @retval  0 on succes
 * @retval  ENOMEM on failure
 */
int hashdb_ensure_capacity(hashdb *db, size_t new_capacity);


/**
 * @brief fixate capacity of hashdb so no reallction is possible (affects: see. `hashdb_add()`)
 * 
 * @param[in] db hashdb pointer
 * @param[in] newval boolean falue
 */
void hashdb_fix(hashdb *db, bool newval);

int hashdb_from_http(hashdb *db, char *buffer, size_t bufsiz);


/**
 * @brief Gets value from hashdb but don't delete
 * 
 * @param[in] db    hashdb pointer
 * @param[in] key   pointer to key string
 * @retval associated value from key on success
 * @retval NULL on failure  
 */
void* hashdb_get(hashdb *db, const char *key);

/**
 * @brief Get number of inserted entries
 * 
 * @param[in] db hashdb pointer
 * @retval  n number of available entries
 */
size_t hashdb_len(hashdb *db);

/**
 * @brief Returns active memory usage by hashdb object in bytes
 * 
 * @param db pointer to hashdb object
 * @return memory usage in bytes
 */
size_t hashdb_memory_usage(hashdb *db);


/**
 * @brief Jumps to and returns next value
 * 
 * @param[in] db        hashdb pointer
 * @param[out] entry      pointer to hashdb_entry
 * @retval  1 on succes (if value available)
 * @retval  0 on failure (if end or nothing availiable) 
 */
int hashdb_next(hashdb *db, hashdb_entry *entry);


/**
 * @brief Get percentage of intserted entries
 * 
 * @param[in] db hashdb pointer
 * @return floating pointer value as percentage 
 */
double hashdb_percentage(hashdb *db);

/**
 * @brief Resets hashdb to default capacity and deletes all keys / values
 * 
 * @param[in] db 
 * @retval  0 on success
 * @retval  ENOMEM on failure
 */
int hashdb_reset(hashdb *db);

/**
 * @brief Resets the iterator
 * 
 * @param[in] db    hashdb pointer
 * @retval   0 on succes
 */
void hashdb_reset_iterator(hashdb *db);

/* int hashdb_to_json(hashdb *db, const char *path);
int hashdb_from_json(hashdb *db, const char *path); */

int hashdb_to_http(hashdb *db, char *buffer, size_t bufsiz);

/**
 * @brief Adds an hashdb to another hashdb (existing keys will be skipped)
 * 
 * @param db_dest hashdb destination
 * @param db_src  hashdb source
 * @retval  0 on succes
 * @retval  -1  if nothing to add
 * @retval  ENOMEM on failure
 */
int hashdb_merge(hashdb *db_dest, hashdb *db_src);

/**
 * @brief Adds an hashdb to another hashdb (existing keys will be overwritten)
 * 
 * @param db_dest hashdb destination
 * @param db_src  hashdb source
 * @retval  0 on succes
 * @retval  -1  if nothing to add
 * @retval  ENOMEM on failure
 */
int hashdb_update(hashdb *db_dest, hashdb *db_src);


#endif