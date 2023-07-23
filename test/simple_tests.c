#include <stdio.h>

#include "../hashmap.h"

const char *safestr(const char *str)
{
    if (str == NULL)
        return "(NULL)";
    else
        return str;
}

int main(void)
{

    hashmap_t *map = hashmap_create();
    if (map == NULL)
    {
        printf("error: create\n");
        return 0;
    }

    int ret = hashmap_add(map, "Flo", 0, "Giest");
    ret = hashmap_add(map, "Jan", 0, "Peter");
    ret = hashmap_add(map, "Tobias", 0, "Muke");
    ret = hashmap_add(map, "Emsland", 0, "Meppen");
    ret = hashmap_add(map, "Jan", 0, "Meyer");
    ret = hashmap_add(map, "Hei ", 0, "Meyer");
    ret = hashmap_add(map, "JAse", 0, "Meyer");
    ret = hashmap_add(map, "Mann", 0, "Meyer");
    ret = hashmap_add(map, "Jonas", 0, "Meyer");

    int test = 40;

    ret = hashmap_add(map, &test, sizeof(test), "fourty");

    hashmap_info(map);

    printf("Del: %s\n", safestr(hashmap_del(map, "Jan", 0)));
    printf("Del: %s\n", safestr(hashmap_del(map, "Flo", 0)));
    printf("Del: %s\n", safestr(hashmap_del(map, "Tobias", 0)));
    printf("Del: %s\n", safestr(hashmap_del(map, "Mann", 0)));
    // printf("Del: %s\n", safestr(hashmap_del(map, "Jonas", 0)));

    hashmap_info(map);

    printf("%s\n", safestr(hashmap_get(map, "Jan", 0)));
    printf("%s\n", safestr(hashmap_get(map, "Flo", 0)));
    printf("%s\n", safestr(hashmap_get(map, &test, sizeof(test))));


    hashmap_destroy(map);

    return 0;
}
