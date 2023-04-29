#include <stdio.h>
#include <string.h>

#include "../misc.h"

size_t hash_probe(unsigned long hash, size_t i, size_t cap)
{

    size_t idx = 0;

    if(i & 1) { // odd
        idx = (hash + -(i*i)) % cap;
    } else {
        idx = (hash + (i*i)) % cap;
    }
    return idx;
}

#define TEST_PRIME 7

int main(void)
{   
    
    printf("%zu\n", GetNextHigher3mod4Prime(7 * 2));

    return 0;
}