#include "hashtable-dumb.h"
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

bool hashtable_insert(char *name)
{
    if (name == NULL) {
        return false;
    }

    size_t idx = hash(name);
    if (hash_table[idx] != NULL) {
        return false;
    }

    hash_table[idx] = name;

    return true;
}

char *hashtable_lookup(char *name)
{
    size_t idx = hash(name);
    if (hash_table[idx] != NULL && strncmp(hash_table[idx], name, MAX_STRING) == 0) {
        return hash_table[idx];
    }
    return NULL;
}

char *hashtable_delete(char *name)
{
    size_t idx = hash(name);
    if (hash_table[idx] != NULL && strncmp(hash_table[idx], name, MAX_STRING) == 0) {
        char *tmp = hash_table[idx];
        hash_table[idx] = NULL;
        return tmp;
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
