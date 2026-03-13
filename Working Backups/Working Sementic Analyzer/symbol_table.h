#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// Symbol Kinds
typedef enum {
    KIND_VAR,
    KIND_FUNC,
    KIND_CLASS,
    KIND_PARAM
} SymbolKind;

// Symbol Node
typedef struct Symbol {
    char *name;
    char *type; // "integer", "float", "MyClass", etc.
    SymbolKind kind;
    struct Symbol *next; // Linked list for hash collisions
} Symbol;

// Scope Node
typedef struct Scope {
    int id;
    char *name;
    struct Symbol **table; // Hash table
    struct Scope *parent;  // Pointer to enclosing scope
    struct Scope *next;    // For global list (printing)
} Scope;

// Core Functions
void sym_init();
void sym_enter_scope(char *name);
void sym_exit_scope();
void sym_insert(char *name, char *type, SymbolKind kind);
Symbol* sym_lookup(char *name);
char* sym_lookup_type(char *name);
void sym_print(char *filename);

// The Merged "Build & Check" Function
void semantic_analysis(ASTNode *root);

#endif