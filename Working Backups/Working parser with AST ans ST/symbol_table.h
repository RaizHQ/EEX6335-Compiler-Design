#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef enum { KIND_VAR, KIND_FUNC, KIND_CLASS, KIND_PARAM } SymbolKind;

typedef struct Symbol {
    char *name;
    char *type;
    SymbolKind kind;
    struct Symbol *next; // Hash collision chain
} Symbol;

typedef struct Scope {
    int id;
    char *name;           // "Global", "Class X", "Func Y"
    struct Symbol **table; // Hash table
    struct Scope *parent;  // Parent scope
    struct Scope *next;    // For keeping a list of ALL scopes to print
} Scope;

void sym_init();
void sym_enter_scope(char *name);
void sym_exit_scope();
void sym_insert(char *name, char *type, SymbolKind kind);
void sym_print(char *filename);

// This is the core semantic analyzer entry point
void build_symbol_table(ASTNode *node);

#endif