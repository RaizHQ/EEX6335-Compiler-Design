#ifndef SYMBOL_TABLE_PRINTER_H
#define SYMBOL_TABLE_PRINTER_H

void initSymbolTablePrinter(const char* outputPath);

/* Updated to accept Scope */
void printSymbolTableEntry(const char* name, const char* kind, const char* type, const char* scope);

void closeSymbolTablePrinter();

#endif