#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* --- Pre-defined global types --- */
Type *g_type_int;
Type *g_type_float;
Type *g_type_void;

/* --- Private Helper: Hash Function --- */
static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % 1024;
}

static SymbolTable* create_symbol_table(int level, SymbolTable *parent) {
    SymbolTable *table = (SymbolTable*) malloc(sizeof(SymbolTable));
    memset(table, 0, sizeof(SymbolTable));
    table->scope_level = level;
    table->parent = parent;
    return table;
}

/* --- Type System Implementation --- */

Type* create_atomic_type(AtomicType atom) {
    Type *t = (Type*) malloc(sizeof(Type));
    t->kind = TYPE_KIND_ATOM;
    t->data.atom = atom;
    return t;
}

Type* create_array_type(Type *base_type) {
    Type *t = (Type*) malloc(sizeof(Type));
    t->kind = TYPE_KIND_ARRAY;
    t->data.array.base_type = base_type;
    return t;
}

int types_are_compatible(Type *t1, Type *t2) {
    if (!t1 || !t2) return 0;
    
    // Rule: float can be assigned an int
    if (t1->kind == TYPE_KIND_ATOM && t1->data.atom == ATOM_FLOAT &&
        t2->kind == TYPE_KIND_ATOM && t2->data.atom == ATOM_INT) {
        return 1;
    }
    
    if (t1->kind != t2->kind) return 0;
    
    switch (t1->kind) {
        case TYPE_KIND_ATOM:
            return t1->data.atom == t2->data.atom;
        // ... (add other cases for array, class, etc.) ...
        default:
            return 0;
    }
}

/* --- Scope Manager Implementation --- */


// Helper to recursively free a symbol table and its contents

static void free_symbol_table_recursive(SymbolTable *table) {
    if (!table) return;
    for (int i = 0; i < 1024; i++) {
        SymbolInfo *current = table->buckets[i];
        while (current) {
            SymbolInfo *next = current->next;
            free(current->name);
            // NOTE: In a complete system, Type* would also be freed here
            
            // Recursively free child tables (e.g., function parameter scope)
            if (current->child_table) {
                 free_symbol_table_recursive(current->child_table);
            }
            free(current);
            current = next;
        }
    }
    free(table);
}


ScopeManager* create_scope_manager() {
    ScopeManager *manager = (ScopeManager*) malloc(sizeof(ScopeManager));
    manager->current_level = 0;
    manager->global = create_symbol_table(0, NULL);
    manager->current = manager->global;
    
    // Init global types
    g_type_int = create_atomic_type(ATOM_INT);
    g_type_float = create_atomic_type(ATOM_FLOAT);
    g_type_void = create_atomic_type(ATOM_VOID);
    
    return manager;
}

void destroy_scope_manager(ScopeManager *manager) {
    // ... (full implementation would free all tables and symbols) ...
    if (manager->global) {
        free_symbol_table_recursive(manager->global);
    }
    // Free the manually allocated global Type structures
    free(g_type_int);
    free(g_type_float);
    free(g_type_void);
    free(manager);
}

void scope_enter(ScopeManager *manager) {
    manager->current_level++;
    SymbolTable *new_table = create_symbol_table(manager->current_level, manager->current);
    manager->current = new_table;
}

void scope_exit(ScopeManager *manager) {
    if (manager->current == manager->global) return;
    manager->current = manager->current->parent;
    manager->current_level--;
    // Note: We don't free the table here. It's owned by the
    // function/class that created it, for its SymbolInfo->child_table
}

/* --- Symbol Management Implementation --- */

SymbolInfo* scope_insert(ScopeManager *manager, char *name, SymbolKind kind, Type *type) {
    unsigned long index = hash(name);
    SymbolTable *table = manager->current;
    
    SymbolInfo *info = (SymbolInfo*) malloc(sizeof(SymbolInfo));
    info->name = strdup(name);
    info->kind = kind;
    info->type = type;
    info->scope_level = table->scope_level;
    info->child_table = NULL;
    
    info->next = table->buckets[index];
    table->buckets[index] = info;
    
    return info;
}

SymbolInfo* scope_lookup(ScopeManager *manager, char *name) {
    unsigned long index = hash(name);
    SymbolTable *table = manager->current;
    
    while (table != NULL) {
        for (SymbolInfo *info = table->buckets[index]; info; info = info->next) {
            if (strcmp(info->name, name) == 0) {
                return info; // Found it
            }
        }
        table = table->parent; // Not in this scope, try parent
    }
    return NULL; // Not found
}

SymbolInfo* scope_lookup_current(ScopeManager *manager, char *name) {
    unsigned long index = hash(name);
    SymbolTable *table = manager->current;
    
    for (SymbolInfo *info = table->buckets[index]; info; info = info->next) {
        if (strcmp(info->name, name) == 0) {
            return info; // Found it
        }
    }
    return NULL;
}

char* type_to_string(Type *type) {
    // Static buffer to hold the string representation
    static char buffer[256]; 

    if (!type) {
        return "unknown";
    }

    switch (type->kind) {
        case TYPE_KIND_ATOM:
            switch (type->data.atom) {
                case ATOM_INT:   return "integer";
                case ATOM_FLOAT: return "float";
                case ATOM_VOID:  return "void";
                default:         return "unknown_atom";
            }
        
        case TYPE_KIND_ARRAY:
            // Recursively call to get the base type
            snprintf(buffer, 255, "array of %s", type_to_string(type->data.array.base_type));
            return buffer;

        case TYPE_KIND_CLASS:
            // Assuming data.class_ref.name is set
            snprintf(buffer, 255, "class(%s)", type->data.class_ref.name);
            return buffer;

        case TYPE_KIND_FUNC:
            // A simple representation for a function type
            snprintf(buffer, 255, "function(...) returning %s", type_to_string(type->data.func.return_type));
            return buffer;
            
        default:
            return "invalid_type";
    }
}



////////////////////////////////////////////////////////////////

// --- Global Scope Stack and Memory Allocation Counter ---
SymbolTable *symtab_stack = NULL;
static int current_offset = 0; // Tracks memory usage for current scope

// --- ADT/RDT Management (Implemented conceptually in code/descriptors) ---
// Note: RDT and ADT structures are declared in compiler.h
// RDT[R_COUNT] and ADT_map are managed externally, but we update symtab entry.

#define MAX_SYMBOLS_PER_SCOPE 128 // Max entries per hash table/list
#define MAX_SCOPES 16

// --- Helper: Find the current scope's symbol table ---
SymbolTable *current_scope() {
    return symtab_stack;
}

// --- Scope Management ---

void push_scope() {
    SymbolTable *new_scope = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!new_scope) {
        fprintf(stderr, "Error: Memory allocation failed for new scope.\n");
        exit(1);
    }
    new_scope->count = 0;
    new_scope->prev = symtab_stack;
    symtab_stack = new_scope;
    
    // Reset offset for new scope (local memory starts fresh)
    current_offset = 0;
}

void pop_scope() {
    if (symtab_stack == NULL) return;
    SymbolTable *old_scope = symtab_stack;
    symtab_stack = symtab_stack->prev;
    
    // Free entries and the table structure
    for (int i = 0; i < old_scope->count; i++) {
        free(old_scope->entries[i]->name);
        // Note: ADT/RDT state for these variables must be handled in CodeGen exit logic
        free(old_scope->entries[i]); 
    }
    free(old_scope);
}

// --- Symbol Insertion and Lookup ---

SymbolTableEntry *insert_sym(char *name, Type type, int width) {
    if (symtab_stack == NULL) { push_scope(); }
    
    // Create new entry
    SymbolTableEntry *entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    if (!entry) exit(1);

    // Initialize entry fields
    entry->name = strdup(name);
    entry->type = type;
    entry->width = width;
    
    // Calculate and store offset (stack grows to lower addresses, so we use negative offset)
    // NOTE: ADT fields are handled externally or in a more complex structure
    current_offset -= width;
    entry->offset = current_offset; 
    
    // Add to current scope (simplified: assumes name uniqueness within scope and space)
    if (symtab_stack->count < MAX_SYMBOLS_PER_SCOPE) {
        symtab_stack->entries[symtab_stack->count++] = entry;
    } else {
        fprintf(stderr, "Error: Scope full.\n");
        exit(1);
    }
    
    return entry;
}

SymbolTableEntry *lookup_sym(char *name) {
    SymbolTable *current = symtab_stack;
    while (current != NULL) {
        for (int i = 0; i < current->count; i++) {
            if (strcmp(current->entries[i]->name, name) == 0) {
                return current->entries[i]; // Found in current or outer scope
            }
        }
        current = current->prev; // Move to enclosing scope
    }
    return NULL; // Not found
}

// --- Placeholder for Assembly Emit (Used by CodeGen) ---

void emit_asm(const char* format, ...) {
    // This is a placeholder for the actual assembly output function
    // (Defined fully in codegen.c for output, but needed here for linkage)
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    va_end(args);
}

// --- End of symboltable.c --