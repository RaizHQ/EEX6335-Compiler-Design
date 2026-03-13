#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward Declaration to break circular dependency
struct Scope;

typedef enum {
    NODE_PROGRAM, NODE_CLASS_DECL, NODE_IMPL_DEF, NODE_FUNC_DEF, NODE_FUNC_HEAD,
    NODE_VAR_DECL, NODE_MEMBER_DECL, NODE_BLOCK, NODE_ASSIGN, NODE_IF,
    NODE_WHILE, NODE_READ, NODE_WRITE, NODE_RETURN, NODE_FUNC_CALL,
    NODE_EXPR_BINOP, NODE_EXPR_UNOP, NODE_INDICE, NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT, NODE_TYPE, NODE_PARAM, NODE_LIST
} NodeType;

typedef struct ASTNode {
    NodeType kind;
    
    // Metadata
    char *name;         
    char *type_name;    
    char *op;           
    int int_val;        
    float float_val;
    char *visibility;   
    
    // Structure
    struct ASTNode *left;   
    struct ASTNode *right;  
    struct ASTNode *extra;  
    struct ASTNode *next;   
    
    int lineno;

    struct Scope *scope; 
} ASTNode;


ASTNode* createNode(NodeType kind);
ASTNode* createBinary(char *op, ASTNode *left, ASTNode *right);
ASTNode* createUnary(char *op, ASTNode *child);
ASTNode* createID(char *name);
ASTNode* createLiteralInt(int val);
ASTNode* createLiteralFloat(float val);
void ast_print(ASTNode *root, char *filename);

#endif