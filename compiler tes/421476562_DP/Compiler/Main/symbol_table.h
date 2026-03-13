#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum { 
    TYPE_INT, TYPE_FLOAT, TYPE_VOID, TYPE_UNKNOWN 
} DataType;

/* Global Context */
extern DataType currentFunctionReturnType;

void initSymbolTable();

/* Core Operations */
void insertSymbol(char* name, DataType type, char* kind); 
DataType lookupType(char* name);

/* SCOPE MANAGEMENT */
void enterScope(char* scopeName);
void exitScope();

/* MISSING FUNCTION RESTORED HERE: */
void setGlobalReturnType(DataType type); 

/* Output */
void printSymbolTableSnapshot(); 

#endif