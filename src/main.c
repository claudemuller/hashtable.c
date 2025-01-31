#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME 256
#define TABLE_SIZE 10
#define DELETED (person_t *)(0xFFFFFFFFFFFFFFFFUL)

typedef struct person {
    char name[MAX_NAME];
    int age;
    struct person *next;
} person_t;

person_t *hash_table[TABLE_SIZE];

typedef void (*hashtable_print_fn)(void);
typedef bool (*hashtable_insert_fn)(person_t *);
typedef person_t *(*hashtable_lookup_fn)(char *);
typedef person_t *(*hashtable_delete_fn)(char *);

uint32_t hash(char *name)
{
    int32_t len = strnlen(name, MAX_NAME);
    uint32_t hash_val = 0;
    for (size_t i = 0; i < len; ++i) {
        hash_val += name[i];
        hash_val *= name[i];
    }

    return hash_val % TABLE_SIZE;
}

void hashtable_init(void)
{
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        hash_table[i] = NULL;
    }
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
            printf("| \t%zu\t|\t%s\t\t|\n", i, hash_table[i]->name);
        }
    }

    printf("+---------------------------------------+\n");
}

void hashtable_print_ch(void)
{
    printf("+---------------------------------------+\n");

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        size_t c = 0;

        if (hash_table[i] == NULL) {
            printf("| \t%zu\t|\t---\t\t|\n", i);
        } else {
            printf("| \t%zu\t|\t", i);
            person_t *tmp = hash_table[i];
            while (tmp != NULL) {
                if (c == 0) {
                    printf("%s\t\t|\n", tmp->name);
                } else {
                    printf("| \t.\t|\t%s\t\t|\n", tmp->name);
                }
                tmp = tmp->next;
                ++c;
            }
        }
    }

    printf("+---------------------------------------+\n");
}

// Does not handle collisions
bool hashtable_insert(person_t *p)
{
    if (p == NULL) {
        return false;
    }

    size_t idx = hash(p->name);
    if (hash_table[idx] != NULL) {
        return false;
    }

    hash_table[idx] = p;

    return true;
}

// Uses open addressing with linear probing when a collision is encountered
bool hashtable_insert_lin(person_t *p)
{

    if (p == NULL) {
        return false;
    }

    size_t idx = hash(p->name);

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        size_t try = (i + idx) % TABLE_SIZE;
        if (hash_table[try] == NULL || hash_table[try] == DELETED) {
            hash_table[try] = p;
            return true;
        }
    }

    return false;
}

// Uses external chaining with linked lists when collisions are encountered
bool hashtable_insert_ch(person_t *p)
{
    if (p == NULL) {
        return false;
    }

    size_t idx = hash(p->name);
    p->next = hash_table[idx];
    hash_table[idx] = p;

    return true;
}

person_t *hashtable_lookup(char *name)
{
    size_t idx = hash(name);
    if (hash_table[idx] != NULL && strncmp(hash_table[idx]->name, name, MAX_NAME) == 0) {
        return hash_table[idx];
    }
    return NULL;
}

person_t *hashtable_lookup_lin(char *name)
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

        if (strncmp(hash_table[try]->name, name, MAX_NAME) == 0) {
            return hash_table[try];
        }
    }
    return NULL;
}

person_t *hashtable_lookup_ch(char *name)
{
    size_t idx = hash(name);
    person_t *tmp = hash_table[idx];

    while (tmp != NULL && strncmp(tmp->name, name, MAX_NAME) != 0) {
        tmp = tmp->next;
    }

    return tmp;
}

person_t *hashtable_delete(char *name)
{
    size_t idx = hash(name);
    if (hash_table[idx] != NULL && strncmp(hash_table[idx]->name, name, MAX_NAME) == 0) {
        person_t *tmp = hash_table[idx];
        hash_table[idx] = NULL;
        return tmp;
    }
    return NULL;
}

person_t *hashtable_delete_lin(char *name)
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

        if (strncmp(hash_table[try]->name, name, MAX_NAME) == 0) {
            person_t *tmp = hash_table[try];
            hash_table[try] = (person_t *)DELETED;
            return tmp;
        }
    }
    return NULL;
}

person_t *hashtable_delete_ch(char *name)
{
    size_t idx = hash(name);
    person_t *tmp = hash_table[idx];
    person_t *prev = NULL;

    while (tmp != NULL && strncmp(tmp->name, name, MAX_NAME) != 0) {
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

    return tmp;
}

static inline uint64_t rdtsc()
{
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(void)
{
    printf("+---------------------------------------+\n");

    // printf("> No collision handling\n");
    // hashtable_print_fn print = hashtable_print;
    // hashtable_insert_fn insert = hashtable_insert;
    // hashtable_lookup_fn lookup = hashtable_lookup;
    // hashtable_delete_fn delete = hashtable_delete;

    // printf("> Using open addressing with linear probing\n");
    // hashtable_insert_fn insert = hashtable_insert_lin;
    // hashtable_lookup_fn lookup = hashtable_lookup_lin;
    // hashtable_delete_fn delete = hashtable_delete_lin;

    printf("> Using external chaining with a linked list\n");
    hashtable_print_fn print = hashtable_print_ch;
    hashtable_insert_fn insert = hashtable_insert_ch;
    hashtable_lookup_fn lookup = hashtable_lookup_ch;
    hashtable_delete_fn delete = hashtable_delete_ch;

    // Slower for large arrays
    hashtable_init();
    // Faster for large arrays
    // memset(hash_table, 0, TABLE_SIZE * sizeof(person_t *));

    print();

    person_t jacob = {.name = "Jacob", .age = 99};

    uint64_t start, end;
    start = rdtsc();
    insert(&jacob);
    end = rdtsc();
    printf("CPU cycles: %llu\n", end - start);

    person_t piet = {.name = "Piet", .age = 14};
    insert(&piet);
    insert(&(person_t){.name = "Koos", .age = 73});
    insert(&(person_t){.name = "Sara", .age = 62});
    insert(&(person_t){.name = "Edna", .age = 3});
    insert(&(person_t){.name = "Maren", .age = 88});
    insert(&(person_t){.name = "Robert", .age = 16});
    insert(&(person_t){.name = "Tebogo", .age = 31});
    insert(&(person_t){.name = "Jane", .age = 62});
    insert(&(person_t){.name = "Albert", .age = 2});

    print();

    person_t *res;

    printf("> Searching for Piet...\n");
    if ((res = lookup("Piet")) != NULL) {
        printf("Found: %s\n", res->name);
    } else {
        printf("Piet not found\n");
    }

    printf("> Searching for Suzie...\n");
    if ((res = lookup("Suzie")) != NULL) {
        printf("Found %s\n", res->name);
    } else {
        printf("Suzie not found\n");
    }

    printf("> Deleting Piet...\n");
    delete ("Piet");
    if ((res = lookup("Piet")) != NULL) {
        printf("Found %s\n", res->name);
    } else {
        printf("Piet not found\n");
    }

    print();

    return 0;
}
