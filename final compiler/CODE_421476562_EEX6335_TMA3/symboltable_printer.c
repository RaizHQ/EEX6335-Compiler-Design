/* symboltable_printer.c - Logic to print the symbol table */
#include "compiler.h"

// Helper function to print indentation
static void print_indent(FILE* out, int level) {
    for (int i = 0; i < level; i++) {
        fprintf(out, "    "); // 4 spaces per level
    }
}

// Helper function to convert SymbolKind to string
static const char* kind_to_string(SymbolKind kind) {
    switch (kind) {
        case SYMBOL_KIND_VAR: return "Variable";
        case SYMBOL_KIND_FUNC: return "Function";
        case SYMBOL_KIND_CLASS: return "Class";
        default: return "Unknown";
    }
}

// Recursive function to print a table and all its children
static void print_table_recursive(SymbolTable* table, FILE* out, int indent_level) {
    if (!table) return;

    print_indent(out, indent_level);
    fprintf(out, "--- Scope Level %d ---\n", table->scope_level);

    // Iterate through all buckets in this table
    for (int i = 0; i < 1024; i++) {
        SymbolInfo* info = table->buckets[i];
        
        // Iterate through the chain in this bucket
        while (info) {
            print_indent(out, indent_level);
            fprintf(out, "  '%s'\n", info->name);
            
            print_indent(out, indent_level);
            fprintf(out, "    Kind: %s\n", kind_to_string(info->kind));
            
            print_indent(out, indent_level);
            fprintf(out, "    Type: %s\n", type_to_string(info->type));
            
            fprintf(out, "\n");

            // If this symbol (like a func/class) has its own scope,
            // recursively print it one level deeper.
            if (info->child_table) {
                print_table_recursive(info->child_table, out, indent_level + 1);
            }

            info = info->next;
        }
    }
}

/* --- Public API Function --- */

void print_symbol_table_to_file(ScopeManager* manager, const char* filename) {
    FILE* out = fopen(filename, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
        return;
    }

    fprintf(out, "===== Symbol Table Dump =====\n\n");
    
    // Start the recursive print from the global scope
    print_table_recursive(manager->global, out, 0);

    fprintf(out, "=============================\n");
    fclose(out);
}