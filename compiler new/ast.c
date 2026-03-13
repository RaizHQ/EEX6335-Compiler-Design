#include "ast.h"

/* =========================================================================
   Node Creation Factories (Standard)
   ========================================================================= */

ASTNode* createNode(NodeType type) {
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(1);
    }
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->next = NULL;
    newNode->extra = NULL;
    newNode->name = NULL;
    newNode->type_name = NULL;
    newNode->op = NULL;
    newNode->visibility = NULL;
    newNode->int_val = 0;
    newNode->float_val = 0.0f;
    return newNode;
}

ASTNode* createBinary(char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = createNode(NODE_BINARY_EXPR);
    node->op = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* createUnary(char* op, ASTNode* left) {
    ASTNode* node = createNode(NODE_UNARY_EXPR);
    node->op = strdup(op);
    node->left = left;
    return node;
}

ASTNode* createLiteralInt(int val) {
    ASTNode* node = createNode(NODE_LITERAL_INT);
    node->int_val = val;
    return node;
}

ASTNode* createLiteralFloat(float val) {
    ASTNode* node = createNode(NODE_LITERAL_FLOAT);
    node->float_val = val;
    return node;
}

ASTNode* createID(char* name) {
    ASTNode* node = createNode(NODE_ID);
    node->name = strdup(name);
    return node;
}

/* =========================================================================
   AST Visualization Logic
   ========================================================================= */

const char* nodeTypeToString(NodeType type) {
    switch(type) {
        case NODE_CLASS_DECL:   return "ClassDecl";
        case NODE_IMPL_DEF:     return "ImplDef";
        case NODE_FUNC_DEF:     return "FuncDef";
        case NODE_FUNC_HEAD:    return "FuncHead";
        case NODE_FUNC_CALL:    return "FuncCall";
        case NODE_VAR_DECL:     return "VarDecl";
        case NODE_PARAM:        return "Param";
        case NODE_BLOCK:        return "Block";
        case NODE_LIST:         return "List";
        case NODE_ASSIGN:       return "Assign";
        case NODE_IF:           return "If";
        case NODE_WHILE:        return "While";
        case NODE_READ:         return "Read";
        case NODE_WRITE:        return "Write";
        case NODE_RETURN:       return "Return";
        case NODE_BINARY_EXPR:  return "BinaryExpr";
        case NODE_UNARY_EXPR:   return "UnaryExpr";
        case NODE_VAR_ACCESS:   return "VarAccess";
        case NODE_LITERAL_INT:  return "LiteralInt";
        case NODE_LITERAL_FLOAT:return "LiteralFloat";
        case NODE_TYPE:         return "Type";
        case NODE_ID:           return "ID";
        default:                return "Unknown";
    }
}

/**
 * Recursive printer with Tree Branch Tagging.
 * * fp:    File pointer
 * node:  Current node
 * depth: Indentation level
 * tag:   A string indicating the relationship to the parent (L, R, Next, etc.)
 */
void print_ast_recursive(FILE *fp, ASTNode *node, char *prefix, char *tag) {
    if (node == NULL) return;

    // 1. Print the Prefix (The vertical flow lines)
    //    Then the Connector (|--)
    //    Then the Tag and Type
    fprintf(fp, "%s|-- %-6s %s", prefix, tag, nodeTypeToString(node->type));

    // 2. Print Data Payload
    if (node->name)         fprintf(fp, " [Name: %s]", node->name);
    if (node->type_name)    fprintf(fp, " [Type: %s]", node->type_name);
    if (node->op)           fprintf(fp, " [Op: %s]", node->op);
    if (node->visibility)   fprintf(fp, " [Vis: %s]", node->visibility);
    if (node->type == NODE_LITERAL_INT) fprintf(fp, " [Val: %d]", node->int_val);
    if (node->type == NODE_LITERAL_FLOAT) fprintf(fp, " [Val: %f]", node->float_val);

    fprintf(fp, "\n");

    // 3. Prepare Prefix for CHILDREN
    //    Children are indented deeper. We add "|   " to the prefix.
    //    Note: We allocate a new buffer on the stack for the next level.
    char childPrefix[1024];
    // Safety check to prevent buffer overflow on extremely deep trees
    if (strlen(prefix) < 1000) {
        sprintf(childPrefix, "%s|   ", prefix); 
    } else {
        sprintf(childPrefix, "%s", prefix); // Stop indenting if too deep
    }

    // 4. Recurse Children
    if (node->left)  print_ast_recursive(fp, node->left, childPrefix, "[L]");
    if (node->right) print_ast_recursive(fp, node->right, childPrefix, "[R]");
    if (node->extra) print_ast_recursive(fp, node->extra, childPrefix, "[Else]");

    // 5. Recurse SIBLINGS (Nodes in a Linked List)
    //    CRITICAL: Siblings use the *CURRENT* prefix, not the childPrefix.
    //    This visually places them at the same level as the current node.
    if (node->next) {
        print_ast_recursive(fp, node->next, prefix, "[Next]");
    }
}

void ast_print(ASTNode *root, char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing AST.\n", filename);
        return;
    }

    fprintf(fp, "=== Abstract Syntax Tree (Visual) ===\n");
    // Initial prefix is empty
    print_ast_recursive(fp, root, "", "[Root]");
    
    fclose(fp);
    printf("AST successfully written to %s\n", filename);
}