#include <stdio.h>
#include <stdlib.h>
#include "symbol_table_printer.h"

static FILE* f_sym = NULL;

void initSymbolTablePrinter(const char* outputPath) {
    f_sym = fopen(outputPath, "w");
    if (!f_sym) exit(1);
    
    /* Add Scope Column */
    fprintf(f_sym, "==========================================================================\n");
    fprintf(f_sym, "%-20s | %-10s | %-10s | %-20s\n", "Symbol Name", "Kind", "Type", "Scope");
    fprintf(f_sym, "==========================================================================\n");
}

void printSymbolTableEntry(const char* name, const char* kind, const char* type, const char* scope) {
    if (f_sym) {
        fprintf(f_sym, "%-20s | %-10s | %-10s | %-20s\n", name, kind, type, scope);
    }
}

void closeSymbolTablePrinter() {
    if (f_sym) fclose(f_sym);
}