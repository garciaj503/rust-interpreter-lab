/*
 * symbol_map.h
 *
 * Header file for a simple symbol map implementation in C.
 * This file defines structures and functions to create, manipulate,
 * and destroy a map with keys of type 'const char *' and values of type 'struct expr *'.
 *
 * Author: ChatGPT
 * Date: November 16, 2023
 *
 * Structures:
 * - struct expr: Represents a symbol with fields for its value and other relevant information.
 * - struct Node: Represents a node in the linked list used for the symbol map implementation.
 * - struct SymbolMap: Represents the symbol map itself.
 *
 * Functions:
 * - createSymbolMap: Creates a new symbol map and initializes it.
 * - insertSymbol: Inserts a key-value pair into the symbol map.
 * - getSymbol: Retrieves the value associated with a given key from the symbol map.
 * - deleteSymbol: Removes a key-value pair from the symbol map.
 * - destroySymbolMap: Frees memory allocated for the symbol map and its nodes.
 *
 * Usage:
 * 1. Include this header file in your C program: #include "symbol_map.h"
 * 2. Implement the functions in a corresponding C file.
 * 3. Use the functions to manage symbols in your program.
 *
 * Note: This implementation assumes each key in the map is unique.
 * If needed, explore other data structures for more complex use cases.
 */
#ifndef SYMBOL_MAP_H
#define SYMBOL_MAP_H

// Define the symbol structure
#include "expr.h"

// Define the node structure for the linked list
struct Node {
    const char *key;
    struct expr *value;
    struct Node *next;
};

// Define the symbol map structure
struct SymbolMap {
    struct Node *head;
};

// Function declarations
struct SymbolMap *createSymbolMap();
void insertSymbol(struct SymbolMap *map, const char *key, struct expr *value);
struct expr *getSymbol(struct SymbolMap *map, const char *key);
void deleteSymbol(struct SymbolMap *map, const char *key);
void destroySymbolMap(struct SymbolMap *map);

#endif // SYMBOL_MAP_H
