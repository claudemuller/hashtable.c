#include "hashtable-chain.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t *hash_table[TABLE_SIZE];

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
        size_t c = 0;

        if (hash_table[i] == NULL) {
            printf("| \t%zu\t|\t---\t\t|\n", i);
        } else {
            printf("| \t%zu\t|\t", i);
            node_t *tmp = hash_table[i];
            while (tmp != NULL) {
                if (c == 0) {
                    printf("%s\t\t|\n", tmp->value);
                } else {
                    printf("| \t.\t|\t%s\t\t|\n", tmp->value);
                }
                tmp = tmp->next;
                ++c;
            }
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
    node_t *n = (node_t *)malloc(sizeof(node_t));
    if (!n) {
        fprintf(stderr, "Error allocating space for node.\n");
    }
    n->value = name;
    n->next = hash_table[idx];
    hash_table[idx] = n;

    return true;
}

char *hashtable_lookup(char *name)
{
    size_t idx = hash(name);
    node_t *tmp = hash_table[idx];

    while (tmp != NULL && strncmp(tmp->value, name, MAX_STRING) != 0) {
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        return NULL;
    }

    return tmp->value;
}

char *hashtable_delete(char *name)
{
    size_t idx = hash(name);
    node_t *tmp = hash_table[idx];
    node_t *prev = NULL;

    while (tmp != NULL && strncmp(tmp->value, name, MAX_STRING) != 0) {
        prev = tmp;
        tmp = tmp->next;
    }

    // If nothing was found, do nothing
    if (tmp == NULL) {
        return NULL;
    }

    // We're deleting the head of the list
    if (prev == NULL) {
        hash_table[idx] = tmp->next;
    } else {
        prev->next = tmp->next;
    }

    char *ret = tmp->value;
    free(tmp);

    return ret;
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
