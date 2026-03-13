#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "../Printer/symbol_table_printer.h" 

typedef struct Symbol {
    char* name;
    DataType type;
    char* kind;
    char* scope; /* NEW: Stores "Global", "main", etc. */
    struct Symbol* next;
} Symbol;

static Symbol* head = NULL;
DataType currentFunctionReturnType = TYPE_UNKNOWN;

/* Track where we are. Default is "Global" */
static char* currentScopeName = NULL; 

/* Helper for Enum to String */
const char* dataTypeToString(DataType type) {
    switch (type) {
        case TYPE_INT:      return "integer";
        case TYPE_FLOAT:    return "float";
        case TYPE_VOID:     return "void";
        default:            return "unknown";
    }
}

void initSymbolTable() {
    head = NULL;
    currentFunctionReturnType = TYPE_UNKNOWN;
    currentScopeName = strdup("Global"); /* Start in Global Scope */
}

/* NEW: Scope Helpers */
void enterScope(char* scopeName) {
    /* When entering a function, update the tracker */
    if (currentScopeName) free(currentScopeName);
    currentScopeName = strdup(scopeName);
}

void exitScope() {
    /* When leaving a function, go back to Global */
    if (currentScopeName) free(currentScopeName);
    currentScopeName = strdup("Global");
}

void insertSymbol(char* name, DataType type, char* kind) {
    Symbol* newSym = (Symbol*)malloc(sizeof(Symbol));
    newSym->name = strdup(name);
    newSym->type = type;
    newSym->kind = strdup(kind);
    
    /* CAPTURE CURRENT SCOPE */
    newSym->scope = strdup(currentScopeName); 
    
    newSym->next = head;
    head = newSym;
}

DataType lookupType(char* name) {
    Symbol* current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->type;
        }
        current = current->next;
    }
    return TYPE_UNKNOWN;
}

void setGlobalReturnType(DataType type) {
    currentFunctionReturnType = type;
}

void printSymbolTableSnapshot() {
    Symbol* current = head;
    while (current != NULL) {
        /* Pass the Scope to the printer */
        printSymbolTableEntry(
            current->name, 
            current->kind, 
            dataTypeToString(current->type),
            current->scope 
        );
        current = current->next;
    }
}