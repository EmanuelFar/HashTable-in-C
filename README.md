# HashTable Implementation in C

## Table of Contents
- [Introduction](#introduction)
- [Implementation Details](#implementation-details)
- [Functions](#functions)


## Introduction

This project provides a HashTable implementation designed for handling key-value pairs in C, Key values are <char*> - < int > .
It uses double hashing for collision resolution and bitwise flags for efficient memory usage and cleanup.

## Implementation Details

- **HashTable Structure**: 
  - `struct HTentry`: Represents a key-value pair entry.
  - `struct flagLocation`: Defines the location structure for bitwise flags.
  - `struct HashTable`: Contains entries, flags, current size, and removed counter.

- **Functions**:
  - `HTCreate()`:                  Initializes a new HashTable instance.
  - `HTDestroy(table)`:            Deallocates all memory allocated for the HashTable.
  - `HTput(table, string, value)`: Inserts a new key-value pair into the HashTable.
  - `HTdel(table, string)`:        Deletes a specific key-value pair from the HashTable.
  - `HTget(table, string)`:        Retrieves the value associated with a given key.
  - `HTsize(table)`:               Returns the current number of keys in the HashTable.
  - `HTincrement(table, string)`:  Increments the value associated with a key by 1.
  - `HTcontains(table, string)`:   Checks if a key exists in the HashTable.
