// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#include "../../include/hashdb.h"

#include "timebot.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ROCKYOU_PATH "../src/test/data/rockyou.txt"
#define ROCKYOU_LINES (size_t) 14344392  /* number of lines in rockyou.txt */

#define HASHDB_START_GET_FROM 121512    /* start point for  hashdb_test_get_1000_keys and hashdb_test_get_key*/

char **test_keys = NULL;

/* initialize keys from rockyou.txt */
int init_test_keys(const char *path)
{
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        perror("open file");
        return -1;
    }

    test_keys = calloc(ROCKYOU_LINES, sizeof *test_keys);
    if(test_keys == NULL) {
        perror("init_test_keys");
        return -1;
    }

    char key_buf[128];
    for(size_t i = 0; i < ROCKYOU_LINES; i++) {
        fgets(key_buf, sizeof(key_buf), fp);
        key_buf[strlen(key_buf)-1] = '\0';
        test_keys[i] = strdup(key_buf);
    }

    fclose(fp);
    return 0;
}

void destroy_keys()
{
    for(size_t i = 0; i < ROCKYOU_LINES; i++) {
        free(test_keys[i]);
    }
    free(test_keys);
}

void hashdb_test_fullfill(hashdb *db)
{
    for(size_t i = 0; i < hashdb_cap(db); i++) {
        //printf("%s\n", test_keys[i]);
        if(hashdb_add(db, test_keys[i], test_keys[i]) != 0) {
            fprintf(stderr, "error: hashdb_add failed\n");
            abort();
        }
        //printf("adding Key: %s - %s\n", test_keys[i], test_keys[i]);
    }

    char *res = NULL;
    for(size_t i = 0; i < hashdb_cap(db); i++) {
        res = hashdb_get(db, test_keys[i]);
        if(res == NULL) {
            fprintf(stderr, "error: hashdb_get failed\n");
            abort();
        }
        //printf("getting Key: %s - %s\n", test_keys[i], res);
    }

}

void test_not_found_cap_rockyou(hashdb *db) {
    char *res = hashdb_get(db, "definitely not inside");
    if(res != NULL) {
        fprintf(stderr, "definitely not inside - found (failure/must not happen)\n");
        abort();
    }
    //printf("definitely not inside - not found (success) rockyou\n");
}

void test_not_found_default_cap(hashdb *db)
{
    char *res = hashdb_get(db, "definitely not inside");
    if(res != NULL) {
        fprintf(stderr, "definitely not inside - found (failure/must not happen)\n");
        abort();
    }
    //printf("definitely not inside - not found (success) default\n");
}

/* test append 14344392 keys */
void hashdb_test_append_rockyou(hashdb *db)
{
    size_t x = ROCKYOU_LINES-1;
    size_t i;
    for(i = 0; i < ROCKYOU_LINES; i++, x--) {
        if(hashdb_add(db, test_keys[i], test_keys[x]) != 0) {
            fprintf(stderr, "error: hashdb_test_append_rockyou\n");
            abort();
        }
    }

}

/* test get 1000 keys */
void hashdb_test_get_1000_keys(hashdb *db) {

    for(size_t i = HASHDB_START_GET_FROM; i < HASHDB_START_GET_FROM + 1000; i++) {
        const char *res = hashdb_get(db, test_keys[i]);
        if(res == NULL) {
            fprintf(stderr, "error: hashdb_test_get_1000_keys");
            abort();
        }
    }

}

/* test get single keys */
void hashdb_test_get_key(hashdb *db)
{
    const char *res = hashdb_get(db, test_keys[HASHDB_START_GET_FROM]);
    if(res == NULL) {
            fprintf(stderr, "error: hashdb_test_get_key");
            abort();
    }
}

/* test get all keys */
void hashdb_test_get_rockyou(hashdb *db)
{
    for(size_t i = 0; i < ROCKYOU_LINES; i++) {
        const char *res = hashdb_get(db, test_keys[i]);
        if(res == NULL) {
            fprintf(stderr, "error: hashdb_test_get_rockyou");
            abort();
        }
    }
}

void hashdb_test_del_rockyou(hashdb *db)
{
    for(size_t i = 0; i < ROCKYOU_LINES; i++) {
        const char *res = hashdb_del(db, test_keys[i]);
        if(res == NULL) {
            fprintf(stderr, "error: hashdb_test_del_rockyou");
            abort();
        }

    }
}

void hashdb_test_del_default(hashdb *db)
{
    for(size_t i = 0; i < 7; i++) {
        const char *res = hashdb_del(db, test_keys[i]);
        if(res == NULL) {
            fprintf(stderr, "error: hashdb_test_del_rockyou");
            abort();
        }

    }
}


int main(void)
{
    timebot *bot = timebot_create(20);

    if(init_test_keys(ROCKYOU_PATH) != 0) {
        return EXIT_FAILURE;
    }

    hashdb *db = hashdb_create();


    // /* enable constant so it don't execute the grow function */
    // hashdb_fix(db, true);
    // timebot_standalone(bot, hashdb_test_fullfill, db, "test if hashdb is filled completely if growing is disabled");
    // timebot_standalone(bot, test_not_found_default_cap, db, "test if not found works with default capacity");
    // timebot_standalone(bot, hashdb_test_del_default, db, "delete HASHDB_DEFAULT_CAP entries");
    // hashdb_fix(db, false);
    // /* disable constant so it executes the grow function */

    // printf("after creation and adding 7 (HASHDB_DEFAULT_CAPACITY)\n");
    // printf("\ncap:    %1lu   (should be 7)\n", hashdb_cap(db));
    // printf("len:    %1lu   (should be 0)\n", hashdb_len(db));
    // printf("filled: %2.1f (should be 0.0)\n", hashdb_percentage(db));

    // // ensure cap
    hashdb_ensure_capacity(db, ROCKYOU_LINES * 2);
   

    timebot_append(bot, hashdb_test_append_rockyou, db, "adding 14.344.392 keys to hashdb");
    timebot_append(bot, hashdb_test_get_key, db, "get one key");
    timebot_append(bot, hashdb_test_get_1000_keys, db, "get 1000 keys");
    timebot_append(bot, hashdb_test_get_rockyou, db, "get all keys");
    timebot_append(bot, test_not_found_cap_rockyou, db, "key is not found with cap of 14.3 million entries");
    timebot_append(bot, hashdb_test_del_rockyou, db, "deleting all rockyou keys");
    
    timebot_run(bot);

    hashdb_destroy(db);

    destroy_keys();
    timebot_destroy(bot);
}