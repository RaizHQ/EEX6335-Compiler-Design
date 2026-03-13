#ifndef AST_PRINTER_H
#define AST_PRINTER_H
#include "../Main/ast.h"

void initASTPrinter(const char* path);
void printAST(ASTNode* root);
void closeASTPrinter();

#endif