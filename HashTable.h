#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdbool.h>
#include <stdint.h>

#define MAX_SIZE 1024
#define BITS_IN_UINT 32

typedef struct HashTable* HashTable;

/** Entry in the HashTable */
struct HTEntry {
  char *key;
  int value;
};

/** @flags: MAX_SIZE bit array represents the table entries-..
 * Usage - open addressing cleanup using bitwise flags.
 * */
struct HashTable {
  struct HTEntry entries[MAX_SIZE];
  uint32_t flags[MAX_SIZE / BITS_IN_UINT];
  int current_size;
  int removed_counter;
};

/** HashTable Init. */
HashTable HTCreate();

/** removes all memory allocated for the table. */
void HTDestroy(HashTable table);

/** Adds a new <key> - <value> pair.
 * if <key> already exists - existing <value> is replaced.
 * False if insertion fails. */
bool HTPut(HashTable table, char *string, int value);

/** deletes a <key> - <value> pair, False if key doesn't exist. */
bool HTDel(HashTable table, char *string);

/** returns a <value> for a certain <key>, else -1. */
int HTGet(HashTable table, char *string);

/** returns the current amount of keys. */
int HTSize(HashTable table);

/** increments a <value> for a given string, False if key doesn't exist. */
bool HTIncrement(HashTable table, char *string);

/** checks if a certain <key> is in the table. */
bool HTContains(HashTable table, char *string);


#endif //HASHTABLE_H
