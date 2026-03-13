#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"

// Initialize the ASM file with 8086 headers
void code_gen_init(FILE *fp);

// Generate code for a node
int code_gen(ASTNode *node, FILE *fp);

// Finalize (add print utils)
void code_gen_finalize(FILE *fp);

#endif