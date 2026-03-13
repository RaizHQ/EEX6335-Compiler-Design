/* =========================================================================
   COMPILER ARCHITECT: SYMBOL TABLE IMPLEMENTATION
   =========================================================================
   Implements a Scoped Symbol Table using a "Stack of Hash Tables".
   Features:
   - O(1) Lookup (Average case)
   - Scope Management (Enter/Exit)
   - Tabular Visualization (Fixed Alignment)
   ========================================================================= */

#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 211  // Prime number for hash distribution

// ===================== GLOBAL STATE =====================

Scope *currentScope = NULL;   // Points to the active scope (Top of Stack)
Scope *scopeListHead = NULL;  // Points to the newest created scope (Linked List Head)
int scopeCount = 0;           // Unique ID counter

// ===================== HASHING =====================

// djb2 algorithm: Reliable and fast for symbol tables
unsigned int hash(char *s) {
    unsigned int h = 5381;
    int c;
    if (!s) return 0;
    while ((c = *s++)) h = ((h << 5) + h) + c;
    return h % HASH_SIZE;
}

// ===================== LIFECYCLE =====================

void sym_init() {
    scopeListHead = NULL;
    currentScope = NULL;
    scopeCount = 0;
    sym_enter_scope("Global"); // Initialize the root scope
}

void sym_enter_scope(char *name) {
    Scope *s = (Scope*)malloc(sizeof(Scope));
    if (!s) {
        fprintf(stderr, "Fatal: Memory allocation failed for new scope.\n");
        exit(1);
    }

    s->id = scopeCount++;
    s->name = strdup(name ? name : "unnamed");
    s->table = (Symbol**)calloc(HASH_SIZE, sizeof(Symbol*));
    
    // Link Hierarchy (Parent/Child relationship for Lookups)
    s->parent = currentScope;

    // Link History (Linear list for Printing)
    // We add to the head (LIFO), but we will print recursively to show Global first.
    s->next = scopeListHead;
    scopeListHead = s;

    // Stack Frame Initialization (8086 Model)
    s->paramOffset = 4;   // BP+4 (Skip RetAddr, OldBP)
    s->localOffset = -2;  // BP-2 (First local)
    s->totalStackSize = 0;

    currentScope = s;
}

void sym_exit_scope() {
    if (currentScope) {
        // Compute total local size (absolute value of negative offset)
        // Example: if localOffset is -6 (reserved -2, -4), size is 4 bytes.
        currentScope->totalStackSize = abs(currentScope->localOffset) - 2;
        currentScope = currentScope->parent;
    }
}

// ===================== OPERATIONS =====================

void sym_insert(char *name, char *type, SymbolKind kind) {
    if (!currentScope || !name) return;
    
    unsigned int h = hash(name);
    
    // Create Symbol
    Symbol *sym = (Symbol*)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type ? strdup(type) : strdup("void");
    sym->kind = kind;
    
    // Calculate Offset
    if (kind == KIND_PARAM) {
        sym->offset = currentScope->paramOffset;
        currentScope->paramOffset += 2; // Assume 2-byte words
    } else if (kind == KIND_VAR) {
        sym->offset = currentScope->localOffset;
        currentScope->localOffset -= 2;
    } else {
        sym->offset = 0; // Classes/Functions have no stack offset
    }

    // Insert (Chaining for collisions)
    sym->next = currentScope->table[h];
    currentScope->table[h] = sym;
}

Symbol* sym_lookup(char *name) {
    if (!name) return NULL;
    Scope *s = currentScope;
    while (s) {
        unsigned int h = hash(name);
        Symbol *sym = s->table[h];
        while (sym) {
            if (strcmp(sym->name, name) == 0) return sym;
            sym = sym->next;
        }
        s = s->parent;
    }
    return NULL;
}

Symbol* sym_lookup_local(char *name) {
    if (!currentScope || !name) return NULL;
    unsigned int h = hash(name);
    Symbol *sym = currentScope->table[h];
    while (sym) {
        if (strcmp(sym->name, name) == 0) return sym;
        sym = sym->next;
    }
    return NULL;
}

char* sym_lookup_type(char *name) {
    Symbol *s = sym_lookup(name);
    return s ? s->type : NULL;
}

// ===================== VISUALIZATION =====================

const char* kindToString(SymbolKind kind) {
    switch(kind) {
        case KIND_VAR:   return "Var";
        case KIND_FUNC:  return "Func";
        case KIND_CLASS: return "Class";
        case KIND_PARAM: return "Param";
        default:         return "Unknown";
    }
}

/**
 * Helper: Recursive printer to ensure Global scope prints FIRST.
 * The scopeListHead is LIFO (newest first). Recursion unwinds it.
 */
void print_scope_recursive(FILE *fp, Scope *s) {
    if (s == NULL) return;

    // 1. Go to the end of the list (Global) first
    print_scope_recursive(fp, s->next);

    // 2. Print current scope details
    fprintf(fp, "\n");
    fprintf(fp, "======================================================================================\n");
    fprintf(fp, " SCOPE: %-15s | ID: %-3d | PARENT: %-3d | SIZE: %d bytes\n", 
            s->name, s->id, s->parent ? s->parent->id : -1, s->totalStackSize);
    fprintf(fp, "======================================================================================\n");
    
    // Table Header
    // Widths: Name(25), Kind(10), Type(15), Offset(8)
    fprintf(fp, "| %-25s | %-10s | %-15s | %-8s |\n", "SYMBOL", "KIND", "TYPE", "OFFSET");
    fprintf(fp, "|---------------------------|------------|-----------------|----------|\n");

    // Table Rows
    int empty = 1;
    for (int i = 0; i < HASH_SIZE; i++) {
        Symbol *sym = s->table[i];
        while (sym) {
            empty = 0;
            // Use %.25s to truncate if too long, preventing table break
            fprintf(fp, "| %-25.25s | %-10s | %-15.15s | %-8d |\n", 
                    sym->name, kindToString(sym->kind), sym->type, sym->offset);
            sym = sym->next;
        }
    }

    if (empty) {
        fprintf(fp, "| %-64s |\n", "(No symbols in this scope)");
    }
    fprintf(fp, "--------------------------------------------------------------------------------------\n");
}

void sym_print(char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening symbol table file");
        return;
    }

    fprintf(fp, "######################################################################################\n");
    fprintf(fp, "#                                  SYMBOL TABLE DUMP                                 #\n");
    fprintf(fp, "######################################################################################\n");

    // Trigger recursive print starting from the head (newest)
    // The recursion will flip it to print Global first.
    print_scope_recursive(fp, scopeListHead);

    fclose(fp);
    printf("Symbol Table written to '%s' (Tabular Format).\n", filename);
}