#include "hashtable-linear.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *hash_table[TABLE_SIZE];

static uint64_t hash(const char *key);

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

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Uses the Fowler–Noll–Vo hash function that returns a 64bit FNV-1a hash for key.
static uint64_t hash(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash % TABLE_SIZE;
}
