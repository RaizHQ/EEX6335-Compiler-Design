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
    int offset;
    struct Symbol *next; // Linked list for hash collisions
} Symbol;

// Scope Node
typedef struct Scope {
    int id;
    char *name;
    struct Symbol **table; 
    struct Scope *parent;  
    struct Scope *next;    
    
    // --- STACK FRAME MANAGEMENT ---
    int paramOffset;     // Counts up (arguments): +4, +6...
    int localOffset;     // Counts down (locals): -2, -4...
    int totalStackSize;  // Final size of locals to reserve (SUB SP, N)
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

void calculate_stack_offsets(ASTNode *funcNode);


#endif