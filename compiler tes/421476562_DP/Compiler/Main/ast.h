#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROG, NODE_CLASS, NODE_FUNC, NODE_BLOCK,
    NODE_VAR_DECL, NODE_ASSIGN, NODE_IF, NODE_WHILE,
    NODE_RETURN, NODE_READ, NODE_WRITE, NODE_FUNC_CALL,
    NODE_OP, NODE_VAR, NODE_INT, NODE_FLOAT, NODE_TYPE,
    NODE_PARAM
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* metaData;     /* For storing operators (+, -) or ID names */
    int intVal;         /* For integer literals */
    float floatVal;     /* For float literals */
    
    struct ASTNode* left;   /* Left Child (or First Child) */
    struct ASTNode* right;  /* Right Child (or Next Sibling/Statement) */
} ASTNode;

/* Constructors */
ASTNode* createNode(NodeType type);
ASTNode* createOpNode(char* op, ASTNode* left, ASTNode* right);
ASTNode* createIDNode(char* name);
ASTNode* createIntNode(int val);
ASTNode* createFloatNode(float val);

/* Linker: Connects a node to its sibling (used for lists of stmts) */
void addSibling(ASTNode* node, ASTNode* sibling);

#endif