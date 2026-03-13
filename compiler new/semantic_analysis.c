#include "semantic_analysis.h"
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>

/* =========================================================================
   COMPILER ARCHITECT: SEMANTIC ANALYZER IMPLEMENTATION
   ========================================================================= */

// Global context for the current function being analyzed
// This allows 'return' statements to check what they are returning against.
static char *currentFunctionReturnType = NULL;
static int errorCount = 0;

void report_error(int lineno, const char *msg, const char *detail) {
    fprintf(stderr, "Line %d: Semantic Error: %s %s\n", lineno, msg, detail ? detail : "");
    errorCount++;
}

// --- Type Inference Helper ---
// Guesses the type of an expression node
char* infer_type(ASTNode *node) {
    if (!node) return "void";

    switch(node->type) {
        case NODE_LITERAL_INT:   return "integer";
        case NODE_LITERAL_FLOAT: return "float";
        
        case NODE_VAR_ACCESS:
        case NODE_ID:
            if (strcmp(node->name, "self") == 0) return "class_instance";
            // Check symbol table
            return sym_lookup_type(node->name) ? sym_lookup_type(node->name) : "unknown";

        case NODE_BINARY_EXPR:
            // Logic ops always return integer (boolean)
            if (strcmp(node->op, "==") == 0 || strcmp(node->op, "<") == 0 || 
                strcmp(node->op, ">") == 0  || strcmp(node->op, "and") == 0 ||
                strcmp(node->op, "or") == 0) {
                return "integer";
            }
            // Arith ops return type of left operand (simplified)
            return infer_type(node->left);

        case NODE_FUNC_CALL: {
            char *ret = sym_lookup_type(node->name);
            return ret ? ret : "unknown";
        }

        default: return "unknown";
    }
}

// --- Recursive Traversal ---

void check_node(ASTNode *node) {
    if (!node) return;

    // 1. Handle Scope Creation and Symbol Insertion
    int isScopeBoundary = 0;

    switch(node->type) {
        case NODE_CLASS_DECL:
            if (sym_lookup_local(node->name)) {
                report_error(0, "Duplicate class declaration:", node->name);
            } else {
                sym_insert(node->name, "class", KIND_CLASS);
            }
            sym_enter_scope(node->name);
            isScopeBoundary = 1;
            break;

        case NODE_FUNC_DEF:
            if (sym_lookup_local(node->name)) {
                report_error(0, "Duplicate function declaration:", node->name);
            } else {
                // The return type is stored in the FuncHead (left child) -> right child
                char *retType = "void";
                if (node->left && node->left->right && node->left->right->name) {
                    retType = node->left->right->name;
                }
                sym_insert(node->name, retType, KIND_FUNC);
            }
            
            // CONTEXT SWITCH: Set current expected return type
            // Save previous (in case of nested functions, though unlikely in this grammar)
            char *prevReturnType = currentFunctionReturnType;
            if (node->left && node->left->right && node->left->right->name) {
                currentFunctionReturnType = node->left->right->name;
            } else {
                currentFunctionReturnType = "void";
            }

            sym_enter_scope(node->name);
            
            // Recurse specifically for FUNC_DEF to handle context restore
            if (node->left) check_node(node->left);   // Head (params)
            if (node->right) check_node(node->right); // Body (stmts)
            
            sym_exit_scope();
            
            // Restore context
            currentFunctionReturnType = prevReturnType;
            return; // Done with this node

        case NODE_VAR_DECL:
            if (sym_lookup_local(node->name)) {
                report_error(0, "Duplicate variable declaration:", node->name);
            } else {
                // node->type_name comes from the parser $$->type_name assignment
                sym_insert(node->name, node->type_name, KIND_VAR);
            }
            break;

        case NODE_PARAM:
            if (sym_lookup_local(node->name)) {
                report_error(0, "Duplicate parameter name:", node->name);
            } else {
                sym_insert(node->name, node->type_name, KIND_PARAM);
            }
            break;
            
        case NODE_ASSIGN: {
             char *lhs = infer_type(node->left);
             char *rhs = infer_type(node->right);
             if (strcmp(lhs, "unknown") != 0 && strcmp(rhs, "unknown") != 0) {
                 if (strcmp(lhs, rhs) != 0) {
                     // Strict typing: Int cannot equal Float without cast
                     if (!(strcmp(lhs, "float") == 0 && strcmp(rhs, "integer") == 0)) {
                         char buff[100];
                         sprintf(buff, "Cannot assign '%s' to '%s'", rhs, lhs);
                         report_error(0, "Type Mismatch:", buff);
                     }
                 }
             }
             break;
        }

        // --- CRITICAL: RETURN TYPE CHECKING ---
        case NODE_RETURN: {
            char *actualType = infer_type(node->left); // Infer type of expression returned
            
            if (currentFunctionReturnType == NULL) {
                // Should not happen if parser is correct
                report_error(0, "Return statement outside of function scope.", NULL);
            } else {
                if (strcmp(currentFunctionReturnType, "void") == 0) {
                     if (strcmp(actualType, "void") != 0) {
                         report_error(0, "Void function cannot return a value.", NULL);
                     }
                } 
                else if (strcmp(actualType, currentFunctionReturnType) != 0) {
                     // Check specific allowed coercion (returning int in float func)
                     if (!(strcmp(currentFunctionReturnType, "float") == 0 && strcmp(actualType, "integer") == 0)) {
                         char buff[100];
                         sprintf(buff, "Expected '%s' but returning '%s'", currentFunctionReturnType, actualType);
                         report_error(0, "Return Type Mismatch:", buff);
                     }
                }
            }
            break;
        }

        case NODE_VAR_ACCESS:
        case NODE_FUNC_CALL:
             if (strcmp(node->name, "self") != 0 && !sym_lookup(node->name)) {
                 report_error(0, "Undeclared Identifier:", node->name);
             }
             break;
             
        default:
            break;
    }

    // 2. Generic Recursion (if not handled specifically above)
    if (node->left)  check_node(node->left);
    if (node->right) check_node(node->right);
    if (node->extra) check_node(node->extra); // Else block
    if (node->next)  check_node(node->next);  // Siblings

    // 3. Close Scope
    if (isScopeBoundary) {
        sym_exit_scope();
    }
}

void semantic_analysis(ASTNode *root) {
    printf("Starting Semantic Analysis...\n");
    errorCount = 0;
    sym_init();
    
    check_node(root);
    
    if (errorCount > 0) {
        printf("Semantic Analysis Failed with %d errors.\n", errorCount);
    } else {
        printf("Semantic Analysis Passed.\n");
    }
}