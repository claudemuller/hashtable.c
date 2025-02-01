#ifndef HASHTABLE_LINEAR_H_
#define HASHTABLE_LINEAR_H_

#include <stdbool.h>
#include <stddef.h>

#define TABLE_SIZE 10
#define MAX_STRING 256
#define DELETED (char *)(0xFFFFFFFFFFFFFFFFUL)

void hashtable_init(void);
void hashtable_print(void);
bool hashtable_insert(char *name);
char *hashtable_lookup(char *name);
char *hashtable_delete(char *name);

#endif // !HASHTABLE_LINEAR_H_
