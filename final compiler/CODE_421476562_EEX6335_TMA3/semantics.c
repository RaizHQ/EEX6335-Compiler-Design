#include "compiler.h"

static ScopeManager* g_manager;

// Forward declaration for mutual recursion
static void check_node(ASTNode *node);

static void check_stmt_list(ASTNode *node) {
    while (node) {
        check_node(node);
        node = node->next_sibling;
    }
}

// This function walks the tree and *returns* the type of the expression
static Type* check_expr(ASTNode *node) {
    if (!node) return NULL;

    switch (node->type) {
        case NODE_INTLIT:
            node->expr_type = g_type_int; // Annotate
            return g_type_int;
            
        case NODE_FLOATLIT:
            node->expr_type = g_type_float; // Annotate
            return g_type_float;

        case NODE_ID: {
            SymbolInfo* info = scope_lookup(g_manager, node->sval);
            if (!info) {
                record_error(node->lineno, ERROR_SEMANTIC, "Undeclared identifier '%s'", node->sval);
                return NULL;
            }
            if (info->kind != SYMBOL_KIND_VAR) {
                record_error(node->lineno, ERROR_SEMANTIC, "'%s' is not a variable", node->sval);
                return NULL;
            }
            node->symbol = info; // Annotate
            node->expr_type = info->type; // Annotate
            return info->type;
        }
        
        case NODE_BINARY_OP: {
            Type* lhs = check_expr(node->left);
            Type* rhs = check_expr(node->right);
            
            if (!lhs || !rhs) return NULL; // Error propagated

            TokenType op = (TokenType)node->ival;

            /////////////////////////////////////////
            
            if (op == TOK_PLUS || op == TOK_MINUS || op == TOK_MULTIPLY || op == TOK_DIVIDE) {
                // Numeric operations: Implement implicit widening (float promotion)
                if (!types_are_compatible(lhs, rhs)) {
                    // Explicitly check for float promotion for C-like compatibility
                    if (lhs == g_type_float || rhs == g_type_float) {
                        node->expr_type = g_type_float;
                        return g_type_float;
                    }
                    record_error(node->lineno, ERROR_SEMANTIC, "Arithmetic operators require compatible numeric types");
                    return NULL;
                }
                // If compatible and not float, must be int
                node->expr_type = lhs; // Preserve the highest compatible type (int/float)
                return lhs;
            }
    
            if (op >= TOK_EQ && op <= TOK_GE) {
                // Relational operators: Require compatible types, result is standard int/Bool
                if (types_are_compatible(lhs, rhs)) {
                    node->expr_type = g_type_int; // Standard practice for boolean result
                    return g_type_int;
                }
                record_error(node->lineno, ERROR_SEMANTIC, "Incompatible types for relational operator");
                return NULL;
            }
            /////////////////////////////////////////////

            return NULL; // Default
        }
        
        default:
            check_node(node);
            return node->expr_type;
    }
}

static void check_node(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_STMT_LIST:
            check_stmt_list(node->left);
            break;
            
        case NODE_LOCAL_VAR_DECL: {
            // e.g., local varDecl
            // varDecl -> id : type ...
            // node->left = ID node, node->right = type node
            char* name = node->left->sval;
            if (scope_lookup_current(g_manager, name)) {
                record_error(node->lineno, ERROR_SEMANTIC, "Redeclaration of '%s'", name);
            } else {
                // ... (logic to get Type* from type node) ...
                //ASTNode* type_node = node->left->right;
                Type* var_type = g_type_int; // STUB

                if (var_type) {
                    SymbolInfo* info = scope_insert(g_manager, name, SYMBOL_KIND_VAR, var_type);
                    node->symbol = info;
                }
            }
            break;
        }

        case NODE_ASSIGN_STMT: {
            // node->left = variable, node->right = expression
            Type* lhs_type = check_expr(node->left);
            Type* rhs_type = check_expr(node->right);
            
            if (lhs_type && rhs_type) {
                if (!types_are_compatible(lhs_type, rhs_type)) {
                    record_error(node->lineno, ERROR_SEMANTIC, "Type mismatch in assignment");
                }
            }
            break;
        }

        // ... (other cases for IF, WHILE, FUNC_DEF, etc.) ...
        
        default:
            // Default action: just traverse
            check_node(node->left);
            check_node(node->right);
            check_node(node->third);
            check_stmt_list(node->next_sibling);
    }
}

void semantic_check(ASTNode *root, ScopeManager *manager) {
    g_manager = manager;
    check_node(root);
}


static Type* derive_type_from_node(ASTNode* type_node) {
    if (!type_node) return NULL;
    
    // Use the global type constants for atomics
    switch(type_node->type) {
        case NODE_TYPE_INT:   return g_type_int;
        case NODE_TYPE_FLOAT: return g_type_float;
        case NODE_TYPE_VOID:  return g_type_void;
        case NODE_TYPE_ID: {
            // Placeholder: In a full compiler, this would resolve a class/type name
            // For now, return a basic type to allow flow to continue.
            return g_type_int; 
        }
        case NODE_TYPE_ARRAY: {
            // Recursively derive the base type (stored in 'left') and create a new array type
            Type* inner_type = derive_type_from_node(type_node->left);
            if (inner_type) {
                return create_array_type(inner_type);
            }
            return NULL;
        }
        default:
            return NULL;
    }
}