#include "../../include/hashdb.h"
#include <stdio.h>


#define ARRAYSIZE(x) sizeof((x)) / sizeof((x)[0])

char *keys[]   = {"hello1", "hello2", "hello3", "hello4", "hello5", "hello6", "hello7", "hello8", "hello9", "hello10", "hello11"};
char *values[] = {"world1", "world2", "world3", "world4", "world5", "world6", "world7", "world8", "world9", "world10", "world11"};

void print_status(const char *msg, hashdb *db)
{
    printf("%s\n", msg);
    printf("Cap: %zu\n", hashdb_cap(db));
    printf("Len: %zu\n", hashdb_len(db));
    printf("Prc: %.2f%%\n", hashdb_percentage(db));
}

int main(int argc, const char *argv[])
{
    hashdb *db = hashdb_create();
    hashdb_fix(db, true);

    for(size_t i = 0; i < ARRAYSIZE(keys)-3; i++) {
        printf("%s\n", keys[i]);
        hashdb_add(db, keys[i], values[i]);
    }
    

    printf("%d add should fail (-1)\n", hashdb_add(db, "hello8", "world8"));
    hashdb_fix(db, false);
    printf("%d add should not fail (0)\n", hashdb_add(db, "hello8", "world8"));

    for(size_t i = ARRAYSIZE(keys)-3; i < ARRAYSIZE(keys); i++) {
        hashdb_add(db, keys[i], values[i]);
    }

    hashdb_entry entry;

    while(hashdb_next(db, &entry)) {
        printf("%s - %s\n", entry.key, (char *) entry.value);
    }
    
    printf("\n");

    while(hashdb_next(db, &entry)) {
        printf("%s - %s\n", entry.key, (char *) entry.value);
    }

    print_status("Before clear", db);
 
    hashdb_clear(db);

    print_status("After clear", db);



    hashdb_reset(db);
    print_status("After reset", db);


    hashdb_fix(db, true);

    for(size_t i = 0; i < ARRAYSIZE(keys); i++) {
        hashdb_add(db, keys[i], values[i]);
    }

    print_status("after adding again", db);


    char buffer[512];
    hashdb_to_http(db, buffer, sizeof buffer);

    printf("%s\n", buffer);


    hashdb *new = hashdb_create();
    if(new == NULL) {
        return -1;
    }

    
    if(hashdb_update(new, db) != 0) {
        printf("update failed\n");
        return -1;
    }



    printf("printing new\n");
    while(hashdb_next(new, &entry)) {
        printf("%s - %s\n", entry.key, (char *) entry.value);
    }


    print_status("after update", new);

/* 

    hashdb_reset(db);

    print_status("after reset", db); */



    hashdb_del(new, "hello1");
    hashdb_del(new, "hello2");
    hashdb_del(new, "hello3");
    hashdb_del(new, "hello4");
    hashdb_del(new, "hello5");
    hashdb_del(new, "hello6");
    hashdb_del(new, "hello7");
    char *ret = hashdb_del(new, "hello8");
    printf("%s\n", (ret == NULL) ? "null" : ret);

    print_status("After deleting 7", new);

    hashdb_destroy(db);
    hashdb_destroy(new);



}