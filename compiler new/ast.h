#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================================
   Node Type Enumeration
   ========================================================================= */
typedef enum {
    NODE_CLASS_DECL,
    NODE_IMPL_DEF,
    NODE_FUNC_DEF,
    NODE_FUNC_HEAD,
    NODE_FUNC_CALL,
    NODE_VAR_DECL,
    NODE_PARAM,
    NODE_BLOCK,
    NODE_LIST,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_READ,
    NODE_WRITE,
    NODE_RETURN,
    NODE_BINARY_EXPR,
    NODE_UNARY_EXPR,
    NODE_VAR_ACCESS,
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_TYPE,
    NODE_ID
} NodeType;

/* =========================================================================
   AST Node Structure
   ========================================================================= */
typedef struct ASTNode {
    NodeType type;

    // Tree Structure
    struct ASTNode *left;       // Child: Left / Condition / LHS
    struct ASTNode *right;      // Child: Right / Body / RHS
    struct ASTNode *next;       // Sibling: Linked List (Next statement/param)
    struct ASTNode *extra;      // Child: Specific for IF-ELSE (The Else block)

    // Payload
    char *name;
    char *type_name;
    char *op;
    char *visibility;
    int int_val;
    float float_val;

} ASTNode;

// --- Factory Functions ---
ASTNode* createNode(NodeType type);
ASTNode* createBinary(char* op, ASTNode* left, ASTNode* right);
ASTNode* createUnary(char* op, ASTNode* left);
ASTNode* createLiteralInt(int val);
ASTNode* createLiteralFloat(float val);
ASTNode* createID(char* name);

// --- Visualization ---
void ast_print(ASTNode *root, char *filename);

#endif // AST_H