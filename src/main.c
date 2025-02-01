#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum algo {
    DUMB,
    LINEAR,
    CHAIN,
};

// #define ALGO DUMB
// #include "hashtable-dumb.c"
// #define ALGO LINEAR
// #include "hashtable-linear.c"
#define ALGO CHAIN
#include "hashtable-chain.c"

static inline uint64_t rdtsc()
{
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(void)
{
    printf("+---------------------------------------+\n");

    switch (ALGO) {
    case DUMB: {
        printf("> No collision handling\n");
    } break;

    case LINEAR: {
        printf("> Using open addressing with linear probing\n");
    } break;

    case CHAIN: {
        printf("> Using external chaining with a linked list\n");
    } break;

    default: {
        printf("> No algo selected :(\n");
        return 0;
    }
    }

    hashtable_init();

    hashtable_print();

    char *jacob = "Jacob";

    uint64_t start, end;
    start = rdtsc();
    printf("> Adding Jacob...\n");
    hashtable_insert(jacob);
    end = rdtsc();
    printf("CPU cycles: %zu\n", end - start);

    printf("> Adding Piet...\n");
    hashtable_insert("Piet");
    printf("> Adding Koos...\n");
    hashtable_insert("Koos");
    printf("> Adding Sara...\n");
    hashtable_insert("Sara");
    printf("> Adding Edna...\n");
    hashtable_insert("Edna");
    printf("> Adding Maren...\n");
    hashtable_insert("Maren");
    printf("> Adding Robert...\n");
    hashtable_insert("Robert");
    printf("> Adding Tebogo...\n");
    hashtable_insert("Tebogo");
    printf("> Adding Jane...\n");
    hashtable_insert("Jane");
    printf("> Adding Albert...\n");
    hashtable_insert("Albert");

    hashtable_print();

    char *res;

    printf("> Searching for Piet...\n");
    if ((res = hashtable_lookup("Piet")) != NULL) {
        printf("Found: %s\n", res);
    } else {
        printf("Piet not found\n");
    }

    printf("> Searching for Suzie...\n");
    if ((res = hashtable_lookup("Suzie")) != NULL) {
        printf("Found %s\n", res);
    } else {
        printf("Suzie not found\n");
    }

    printf("> Search for Albert who should be found...\n");
    if ((res = hashtable_lookup("Albert")) != NULL) {
        printf("Found %s\n", res);
    } else {
        printf("Albert not found\n");
    }

    printf("> Deleting Piet...\n");
    hashtable_delete("Piet");
    if ((res = hashtable_lookup("Piet")) != NULL) {
        printf("Found %s\n", res);
    } else {
        printf("Piet not found\n");
    }

    hashtable_print();

    return 0;
}
