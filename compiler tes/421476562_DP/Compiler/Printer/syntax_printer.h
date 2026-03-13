#ifndef SYNTAX_PRINTER_H
#define SYNTAX_PRINTER_H

void initSyntaxPrinter(const char* outputPath);
void writeSyntaxError(int lineNo, const char* errorMsg, const char* offendingToken);
void finalizeSyntaxPrinter();

#endif