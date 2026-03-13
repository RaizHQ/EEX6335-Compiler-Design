#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "ast.h" // Requires AST node definitions for offsets/types

/* =========================================================================
   COMPILER ARCHITECT: SYMBOL TABLE INTERFACE
   =========================================================================
   This module manages the storage of symbols (variables, functions, classes).
   It implements a "Stack of Hash Tables" approach to handle scoping.
   ========================================================================= */

// Symbol Kinds: What is this identifier?
typedef enum {
    KIND_VAR,
    KIND_FUNC,
    KIND_CLASS,
    KIND_PARAM
} SymbolKind;

// Symbol Node: A single entry in the table
typedef struct Symbol {
    char *name;          // The identifier string
    char *type;          // The type string ("integer", "float", "void")
    SymbolKind kind;     // VAR, FUNC, etc.
    int offset;          // Stack offset (e.g., BP-4)
    struct Symbol *next; // Linked list for Hash Collisions
} Symbol;

// Scope Node: Represents a block (Global, Function, Class)
typedef struct Scope {
    int id;                 // Unique ID for debugging/visualization
    char *name;             // Name of the scope (e.g., "calculate", "Global")
    struct Symbol **table;  // The Hash Table (Array of Symbol pointers)
    struct Scope *parent;   // Pointer to enclosing scope (up the stack)
    struct Scope *next;     // Global list for printing purposes only
    
    // Stack Frame Management (8086 Model)
    int paramOffset;        // Offset for parameters (positive relative to BP)
    int localOffset;        // Offset for locals (negative relative to BP)
    int totalStackSize;     // Total bytes required for locals
} Scope;

/* =========================================================================
   Public API
   ========================================================================= */

// Initialization
void sym_init();

// Scope Management
void sym_enter_scope(char *name);
void sym_exit_scope();

// CRUD Operations
void sym_insert(char *name, char *type, SymbolKind kind);
Symbol* sym_lookup(char *name);                 // Search current + parents
Symbol* sym_lookup_local(char *name);           // Search current only
char* sym_lookup_type(char *name);              // Helper to get type string

// Visualization
void sym_print(char *filename);                 // Prints tabular format to file

#endif // SYMBOL_TABLE_H