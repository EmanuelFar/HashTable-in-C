#include "HashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** HashTable initialization */
HashTable HTCreate() {
    HashTable table =
      (HashTable)malloc(sizeof(struct HashTable));
  if (table == NULL) {
    return NULL;
  }
    // Initialize the entries array
    for (int i = 0; i < MAX_SIZE; i++) {
        table->entries[i].key = NULL;
        table->entries[i].value = 0;
    }
   memset(table->flags,0,MAX_SIZE/BITS_IN_UINT * sizeof(uint32_t));
  table->current_size = 0;
  table->removed_counter = 0;
  return table;
}

/** Function to free memory allocated for HashTable */
void HTDestroy(HashTable table) {
  if (table == NULL) {
    return;
  }
  for (int entry = 0; entry < MAX_SIZE; entry++) {
      if (table->entries[entry].key != NULL) {
          free(table->entries[entry].key);  // Free the allocated key
      }
  }
  free(table);
}

/** Simple hash function using prime number 7 and multiplier 13 */
int hash(char *string) {
  int sum = 7;
  for (char *c = string; *c != '\0'; c++) {
    sum = (sum * 13) + *c;
  }
  return sum % MAX_SIZE;
}

/** Double hashing function */
int doubleHash(int hashed_value, int i) {
  /** Choose a prime number close to MAX_SIZE for double hashing */
  int double_hash_func = 1 + (hashed_value % 829);
  return (hashed_value + (i * double_hash_func)) % MAX_SIZE;
}

/** Function to set the occupied flag in the flags array */
void setOccupied(HashTable table, int index) {
    int jump = index / BITS_IN_UINT;
    int offset = index % BITS_IN_UINT;

    uint32_t mask = 1 << offset;
    table->flags[jump] |= mask;
}

/** Function to check if a slot is occupied in the HashTable */
bool isOccupied(HashTable table, int index) {
    int jump = index / BITS_IN_UINT;
    int offset = index % BITS_IN_UINT;

  uint32_t mask = 1 << offset;
  return table->flags[jump] & mask;
}

/** Function to find an index using double hashing */
int findIndex(HashTable table, char *string) {
    int i = 0;
    int hashed = hash(string);
  while (i < MAX_SIZE) {
    int index = doubleHash(hashed, i);

    if (!isOccupied(table, index))
      return index;
    else
      i++;
  }
  return -1; /** Unable to find a suitable index */
}

/** Function to find an entry by key in the HashTable */
struct HTEntry* findEntry(HashTable table, char *string) {
  int i = 0;
  int hashed = hash(string);

  while (i < MAX_SIZE) {
    int index = doubleHash(hashed, i);

    /** Check if the slot at 'index' is occupied */
    if (isOccupied(table, index)) {

      /** Check if the key matches at the current index */
      if (table->entries[index].key != NULL &&
          strcmp(table->entries[index].key, string) == 0) {
        return &table->entries[index]; /** Key found, return the entry */
      }
      i++;
    } else {
      /** If the slot is not occupied, key is not present */
      return NULL;
    }
  }
  /** If we exhaust all slots and key is not found */
  return NULL;
}

/** Function to perform cleanup of flag bits after every 20 deletions */
void cleanup(struct HashTable *table) {
    for (int index = 0; index < MAX_SIZE; index++) {
        if (!isOccupied(table, index)) continue;
        if (table->entries[index].key == NULL) {
            int flag_index = index / BITS_IN_UINT;
            int bit_offset = index % BITS_IN_UINT;
            table->flags[flag_index] &= ~(1 << bit_offset);
        }
    }
}

/** Function to insert a key-value pair into the HashTable */
bool HTPut(HashTable table, char *string, int value) {
  /** length of the string is blocked by 30 for safety */
  if (string == NULL || string[0] == '\0' || strlen(string) > 30){
    return false;
  }
  /** Check if the key already exists, then update its value */
  struct HTEntry *entry = findEntry(table, string);
  if (entry != NULL) {
    entry->value = value;
    return true;
  }
  /** Find a suitable index using double hashing */
  int index = findIndex(table, string);
  if (index == -1)
    return false;

  int string_length = (int)strlen(string);
  /** Allocate memory for the key and perform a deep copy */
  table->entries[index].key =
      (char *)malloc(sizeof(char) * (string_length + 1));
  if (table->entries[index].key == NULL) {
    return false;
  }

  strcpy(table->entries[index].key, string);
  table->entries[index].key[string_length] = '\0';
  table->entries[index].value = value;

  table->current_size++;
  /** Mark the bit flag as occupied */
  setOccupied(table, index);
  return true;
}

/** Function to retrieve the value associated with a key from the HashTable */
int HTGet(HashTable table, char *string) {
  struct HTEntry *entry = findEntry(table, string);
  if (entry == NULL) {
    return -1; /** Key not found */
  }
  return entry->value;
}

/** Function to delete a key-value pair from the HashTable */
bool HTDel(HashTable table, char *string) {
  struct HTEntry *entry = findEntry(table, string);
  if (entry == NULL) {
    return false; /** Key not found */
  }

  /** Free the memory allocated for the key */
  if (entry->key != NULL) {
    free(entry->key);
    entry->key = NULL;
    table->current_size--;
    table->removed_counter++;
  }

  /** Perform cleanup if necessary */
  if (table->removed_counter == 20) {
    cleanup(table);
    table->removed_counter = 0;
  }
  entry->value = 0;

  return true;
}

/** Function to return the current size of the HashTable */
int HTSize(HashTable table) { return table->current_size; }

/** Function to increment the value associated with a key in the HashTable */
bool HTIncrement(HashTable table, char *string) {
  struct HTEntry *entry = findEntry(table, string);
  if (entry == NULL) {
    return false; /** Key not found */
  }
  entry->value++;
  return true;
}

/** Function to check if a key exists in the HashTable */
bool HTContains(HashTable table, char *string) {
  struct HTEntry *entry = findEntry(table, string);
  return (entry != NULL);
}