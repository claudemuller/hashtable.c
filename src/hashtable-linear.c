#include "hashtable-linear.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *hash_table[TABLE_SIZE];

static uint32_t hash(char *name);

void hashtable_init(void)
{
    if (TABLE_SIZE < 100) {
        // Slower for large arrays
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            hash_table[i] = NULL;
        }
        return;
    }

    // Faster for large arrays
    memset(hash_table, 0, TABLE_SIZE * sizeof(char *));
}

void hashtable_print(void)
{
    printf("+---------------------------------------+\n");

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        if (hash_table[i] == NULL) {
            printf("| \t%zu\t|\t---\t\t|\n", i);
        } else if (hash_table[i] == DELETED) {
            printf("| \t%zu\t|\t---<deleted>\t|\n", i);
        } else {
            printf("| \t%zu\t|\t%s\t\t|\n", i, hash_table[i]);
        }
    }

    printf("+---------------------------------------+\n");
}

// Uses open addressing with linear probing when a collision is encountered
bool hashtable_insert(char *p)
{
    if (p == NULL) {
        return false;
    }

    size_t idx = hash(p);

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        size_t try = (i + idx) % TABLE_SIZE;
        if (hash_table[try] == NULL || hash_table[try] == DELETED) {
            hash_table[try] = p;
            return true;
        }
    }

    return false;
}

char *hashtable_lookup(char *name)
{
    size_t idx = hash(name);
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        size_t try = (i + idx) % TABLE_SIZE;

        if (hash_table[try] == NULL) {
            return NULL;
        }

        if (hash_table[try] == DELETED) {
            continue;
        }

        if (strncmp(hash_table[try], name, MAX_STRING) == 0) {
            return hash_table[try];
        }
    }
    return NULL;
}

char *hashtable_delete(char *name)
{
    size_t idx = hash(name);
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        size_t try = (i + idx) % TABLE_SIZE;

        if (hash_table[try] == NULL) {
            return NULL;
        }

        if (hash_table[try] == DELETED) {
            continue;
        }

        if (strncmp(hash_table[try], name, MAX_STRING) == 0) {
            char *tmp = hash_table[try];
            hash_table[try] = (char *)DELETED;
            return tmp;
        }
    }
    return NULL;
}

static uint32_t hash(char *name)
{
    int32_t len = strnlen(name, MAX_STRING);
    uint32_t hash_val = 0;
    for (size_t i = 0; i < len; ++i) {
        hash_val += name[i];
        hash_val *= name[i];
    }

    return hash_val % TABLE_SIZE;
}
