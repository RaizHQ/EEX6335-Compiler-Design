#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* createNode(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->metaData = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* createOpNode(char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = createNode(NODE_OP);
    node->metaData = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* createIDNode(char* name) {
    ASTNode* node = createNode(NODE_VAR);
    node->metaData = strdup(name);
    return node;
}

ASTNode* createIntNode(int val) {
    ASTNode* node = createNode(NODE_INT);
    node->intVal = val;
    return node;
}

ASTNode* createFloatNode(float val) {
    ASTNode* node = createNode(NODE_FLOAT);
    node->floatVal = val;
    return node;
}

void addSibling(ASTNode* node, ASTNode* sibling) {
    if (node == NULL) return;
    ASTNode* temp = node;
    /* Traverse to the end of the chain */
    while (temp->right != NULL) {
        temp = temp->right;
    }
    temp->right = sibling;
}