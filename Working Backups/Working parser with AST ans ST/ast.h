#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_PROGRAM,
    NODE_CLASS_DECL,
    NODE_IMPL_DEF,
    NODE_FUNC_DEF,
    NODE_FUNC_HEAD,
    NODE_VAR_DECL,
    NODE_MEMBER_DECL, // For class attributes/methods with visibility
    NODE_BLOCK,       // { ... }
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_READ,
    NODE_WRITE,
    NODE_RETURN,
    NODE_FUNC_CALL,
    NODE_EXPR_BINOP,
    NODE_EXPR_UNOP,
    NODE_VAR_ACCESS,  // ID, self, or ID.ID
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_TYPE,
    NODE_PARAM,       // Function parameter
    NODE_LIST         // Generic list container
} NodeType;

typedef struct ASTNode {
    NodeType kind;
    
    // Metadata
    char *name;         // IDs, Class Names, Func Names
    char *type_name;    // "integer", "float", or class ID
    char *op;           // Operators
    int int_val;        // Literals, Array sizes
    float float_val;
    char *visibility;   // "public", "private"
    
    // Structure
    struct ASTNode *left;   // Child 1 (e.g., params, expr left)
    struct ASTNode *right;  // Child 2 (e.g., body, expr right)
    struct ASTNode *extra;  // Child 3 (e.g., else block, inheritance list)
    struct ASTNode *next;   // For lists (next stmt, next decl)
    
    int lineno;
} ASTNode;

// Core Constructors
ASTNode* createNode(NodeType kind);
ASTNode* createBinary(char *op, ASTNode *left, ASTNode *right);
ASTNode* createUnary(char *op, ASTNode *child);
ASTNode* createID(char *name);
ASTNode* createLiteralInt(int val);
ASTNode* createLiteralFloat(float val);

// Print Function
void ast_print(ASTNode *root, char *filename);

#endif