
#ifndef SEMANTIC_PRINTER_H
#define SEMANTIC_PRINTER_H

void initSemanticPrinter(const char* outputPath);
void writeSemanticError(int lineNo, const char* errorMsg);
void closeSemanticPrinter();

#endif