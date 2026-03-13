#include <stdio.h>
#include <stdlib.h>
#include "ast_printer.h"

static FILE* f_ast = NULL;

void initASTPrinter(const char* path) {
    f_ast = fopen(path, "w");
    if (!f_ast) { perror("AST File Error"); exit(1); }
}

const char* getTypeString(NodeType type) {
    switch(type) {
        case NODE_PROG: return "PROGRAM";
        case NODE_CLASS: return "CLASS";
        case NODE_FUNC: return "FUNC";
        case NODE_BLOCK: return "BLOCK";
        case NODE_ASSIGN: return "ASSIGN";
        case NODE_IF: return "IF";
        case NODE_WHILE: return "WHILE";
        case NODE_OP: return "OP";
        case NODE_VAR: return "ID";
        case NODE_INT: return "INT";
        default: return "NODE";
    }
}

/* Recursive Helper */
void printRecursive(ASTNode* node, int level, char* tag) {
    if (node == NULL) return;

    /* Indentation */
    for (int i = 0; i < level; i++) fprintf(f_ast, "  | ");

    /* Print Tag (Root, L, R) */
    fprintf(f_ast, "%s ", tag);

    /* Print Node Details */
    fprintf(f_ast, "[%s]", getTypeString(node->type));
    if (node->metaData) fprintf(f_ast, " Val: %s", node->metaData);
    if (node->type == NODE_INT) fprintf(f_ast, " Val: %d", node->intVal);
    fprintf(f_ast, "\n");

    /* Recurse */
    /* If it's a list (siblings), we treat 'right' as next sibling, 
       but if it's an operator, 'right' is right child. */
    
    if (node->type == NODE_OP || node->type == NODE_ASSIGN || node->type == NODE_IF) {
        // Binary/Tree structure
        printRecursive(node->left, level + 1, "(L)->");
        printRecursive(node->right, level + 1, "(R)->");
    } else {
        // List structure (Left is child, Right is Sibling)
        if (node->left) printRecursive(node->left, level + 1, "(Child)->");
        if (node->right) printRecursive(node->right, level, "(Next)->");
    }
}

void printAST(ASTNode* root) {
    if (!f_ast) return;
    fprintf(f_ast, "--- Abstract Syntax Tree ---\n");
    printRecursive(root, 0, "(ROOT)");
}

void closeASTPrinter() {
    if (f_ast) fclose(f_ast);
}