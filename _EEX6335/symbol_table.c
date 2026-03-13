#include "symbol_table.h"
#include <string.h>
#include <stdio.h>

/*
 * HASH_SIZE defines the number of buckets in each scope's hash table.
 * A prime number is chosen to reduce collisions.
 */
#define HASH_SIZE 211

// ===================== GLOBAL STATE =====================

/*
 * currentScope:
 *   Points to the scope currently being analyzed during AST traversal.
 *   Changes when entering/exiting classes, functions, etc.
 */
Scope *currentScope = NULL;

/*
 * scopeListHead:
 *   Maintains a linked list of ALL scopes created during compilation.
 *   Used only for final symbol table printing.
 */
Scope *scopeListHead = NULL;

/*
 * scopeCount:
 *   Generates unique numeric IDs for each scope.
 *   Useful for debugging and symbol table dumps.
 */
int scopeCount = 0;

// ===================== ERROR REPORTING =====================

/*
 * errFile:
 *   File pointer for semantic error logging.
 *   Opened once during semantic analysis.
 */
FILE *errFile = NULL;

/*
 * errorCount:
 *   Keeps track of total semantic errors detected.
 */
int errorCount = 0;

/*
 * currentFuncType:
 * Tracks the expected return type of the function currently being analyzed.
 * Used to validate 'return' statements.
 */

char *currentFuncType = NULL;



// =========================================================
// HASH FUNCTION
// =========================================================

/*
 * djb2 hash function
 * Converts an identifier name into a hash bucket index.
 * Same identifier always hashes to same bucket within a scope.
 */
unsigned int hash(char *s) {
    unsigned int h = 5381;
    int c;

    // Defensive check
    if (!s) return 0;

    // Process each character
    while ((c = *s++))
        h = ((h << 5) + h) + c; // h * 33 + c

    return h % HASH_SIZE;
}

// =========================================================
// ERROR HANDLING HELPER
// =========================================================

/*
 * log_error:
 *   Centralized semantic error reporter.
 *   Writes error to file AND prints to console.
 *   Keeps error counting consistent.
 */
void log_error(int lineno, const char *msg, const char *detail) {
    if (!errFile) return;

    fprintf(errFile, "Line %d: Semantic Error: %s %s\n",
            lineno, msg, detail ? detail : "");
    printf("Line %d: Semantic Error: %s %s\n",
           lineno, msg, detail ? detail : "");

    errorCount++;
}

// =========================================================
// SYMBOL TABLE INITIALIZATION
// =========================================================

/*
 * sym_init:
 *   Resets symbol table state and creates the global scope.
 *   Called once at the start of semantic analysis.
 */
void sym_init() {
    scopeListHead = NULL;
    currentScope = NULL;
    scopeCount = 0;

    // Always begin with a global scope
    sym_enter_scope("Global");
}

// =========================================================
// SCOPE MANAGEMENT
// =========================================================

/*
 * sym_enter_scope:
 *   Creates a new scope and makes it the current scope.
 *   Used for:
 *     - Global scope
 *     - Class scope
 *     - Function scope
 */
void sym_enter_scope(char *name) {

    // Allocate memory for the new scope
    Scope *s = (Scope*)malloc(sizeof(Scope));

    // Assign unique ID
    s->id = scopeCount++;

    // Store scope name (e.g., function name, class name)
    s->name = strdup(name ? name : "unnamed");

    // Allocate hash table for this scope
    s->table = (struct Symbol**)calloc(HASH_SIZE, sizeof(Symbol*));

    // Link to enclosing (parent) scope
    s->parent = currentScope;

    // Maintain a global linked list of scopes (for printing)
    s->next = scopeListHead;
    scopeListHead = s;

    // -----------------------------------------------------
    // OFFSET INITIALIZATION (8086 STACK FRAME MODEL)
    // -----------------------------------------------------
    // Stack layout (simplified):
    // BP+0 : Old Base Pointer
    // BP+2 : Return Address
    // BP+4 : First Function Parameter
    s->paramOffset = 4;

    // Local variables grow downward:
    // BP-2 : First Local Variable
    s->localOffset = -2;

    // Will be computed when scope exits
    s->totalStackSize = 0;

    // Update current scope pointer
    currentScope = s;
}

/*
 * sym_exit_scope:
 *   Restores the parent scope.
 *   Also computes total stack size needed for locals.
 */
void sym_exit_scope() {
    if (currentScope) {

        /*
         * Calculate total local stack usage.
         * Example:
         *   localOffset = -6 → locals at -2 and -4 → total 4 bytes
         */
        currentScope->totalStackSize = -(currentScope->localOffset) - 2;

        if (currentScope->totalStackSize < 0)
            currentScope->totalStackSize = 0;

        // Move back to enclosing scope
        currentScope = currentScope->parent;
    }
}

// =========================================================
// SYMBOL INSERTION
// =========================================================

/*
 * sym_insert:
 *   Inserts a symbol into the CURRENT scope only.
 *   Handles stack offset assignment for parameters and locals.
 */
void sym_insert(char *name, char *type, SymbolKind kind) {
    if (!currentScope || !name) return;

    // Compute hash index
    unsigned int h = hash(name);

    // Allocate new symbol entry
    Symbol *sym = (Symbol*)malloc(sizeof(Symbol));

    sym->name = strdup(name);
    sym->type = type ? strdup(type) : strdup("unknown");
    sym->kind = kind;

    // Insert at head of collision chain
    sym->next = currentScope->table[h];
    currentScope->table[h] = sym;

    // -----------------------------------------------------
    // STACK OFFSET ASSIGNMENT
    // -----------------------------------------------------
    // Assumes 16-bit machine: each variable occupies 2 bytes
    int width = 2;

    if (kind == KIND_PARAM) {
        sym->offset = currentScope->paramOffset;
        currentScope->paramOffset += width;
    }
    else if (kind == KIND_VAR) {
        sym->offset = currentScope->localOffset;
        currentScope->localOffset -= width;
    }
    else {
        // Functions and classes do not live on stack
        sym->offset = 0;
    }
}

// =========================================================
// SYMBOL LOOKUP
// =========================================================

/*
 * sym_lookup:
 *   Searches identifier from current scope outward to global scope.
 *   Implements lexical scoping rules.
 */
Symbol* sym_lookup(char *name) {
    if (!name) return NULL;

    Scope *s = currentScope;
    unsigned int h = hash(name);

    // Traverse scope chain
    while (s) {
        Symbol *sym = s->table[h];

        // Traverse collision chain
        while (sym) {
            if (strcmp(sym->name, name) == 0)
                return sym;
            sym = sym->next;
        }

        s = s->parent;
    }

    return NULL;
}

/*
 * sym_exists_in_current_scope:
 *   Used for duplicate declaration detection.
 *   Does NOT search parent scopes.
 */
int sym_exists_in_current_scope(char *name) {
    if (!currentScope || !name) return 0;

    unsigned int h = hash(name);
    Symbol *sym = currentScope->table[h];

    while (sym) {
        if (strcmp(sym->name, name) == 0)
            return 1;
        sym = sym->next;
    }

    return 0;
}

/*
 * sym_lookup_type:
 *   Convenience wrapper to get symbol type directly.
 */
char* sym_lookup_type(char *name) {
    Symbol *s = sym_lookup(name);
    return s ? s->type : NULL;
}

// =========================================================
// TYPE INFERENCE
// =========================================================

/*
 * infer_type:
 *   Attempts to infer the type of an AST expression.
 *   Used during assignment and expression checking.
 */
char* infer_type(ASTNode *node) {
    if (!node) return "void";

    if (node->kind == NODE_LITERAL_INT) return "integer";
    if (node->kind == NODE_LITERAL_FLOAT) return "float";

    if (node->kind == NODE_INDICE) {

        // Special keyword "self"
        if (strcmp(node->name, "self") == 0)
            return "class_instance";

        // Dot notation not fully handled here
        if (strchr(node->name, '.'))
            return "dot_notation";

        char *t = sym_lookup_type(node->name);
        return t ? t : "unknown";
    }

    if (node->kind == NODE_EXPR_BINOP) {

        // Logical operators return boolean/integer
        if (strcmp(node->op, "==") == 0 ||
            strcmp(node->op, "<") == 0 ||
            strcmp(node->op, "or") == 0)
            return "integer";

        // Otherwise propagate left operand type
        return infer_type(node->left);
    }

    if (node->kind == NODE_FUNC_CALL) {
        char *t = sym_lookup_type(node->name);
        return t ? t : "unknown";
    }

    return "unknown";
}

// ==========================================================
// MERGED SEMANTIC PASS
// ==========================================================

/*
 * traverse_and_check:
 *   Single DFS traversal that:
 *     - Builds symbol tables
 *     - Performs semantic checks
 *     - Manages scopes
 */
void traverse_and_check(ASTNode *node) {
    if (!node) return;

    // Indicates whether a scope was opened at this node
    int popped = 0;

    // ------------------------------------------------------
    // 1. DECLARATIONS
    // ------------------------------------------------------

if (node->kind == NODE_CLASS_DECL) {

    if (sym_exists_in_current_scope(node->name)) {
        log_error(node->lineno, "Duplicate class declaration:", node->name);
    } else {
        // CHANGE HERE: Optionally use node->name or "class" depending on your needs.
        // If you want the type of the class symbol to be the class name itself:
        sym_insert(node->name, node->name, KIND_CLASS);
    }

    sym_enter_scope(node->name);
    node->scope = currentScope;
    popped = 1;
}
    else if (node->kind == NODE_FUNC_DEF) {

    currentFuncType = node->type_name;

    if (sym_exists_in_current_scope(node->name)) {
        log_error(node->lineno, "Duplicate function declaration:", node->name);
    } else {
        // CHANGE HERE: Use node->type_name instead of "func"
        // This stores "int", "float", or "void" as the symbol's type
        sym_insert(node->name, node->type_name, KIND_FUNC); 
    }

    sym_enter_scope(node->name);
    node->scope = currentScope;
    popped = 1;
}

    else if (node->kind == NODE_VAR_DECL || node->kind == NODE_PARAM) {

        if (sym_exists_in_current_scope(node->name)) {
            log_error(node->lineno,
                      "Duplicate variable/param declaration:",
                      node->name);
        } else {
            sym_insert(node->name,
                       node->type_name,
                       node->kind == NODE_PARAM ? KIND_PARAM : KIND_VAR);
        }
    }

    // ------------------------------------------------------
    // 2. USAGE CHECKS
    // ------------------------------------------------------
    //(Variables used in code)

    if (node->kind == NODE_INDICE) {
        if (strcmp(node->name, "self") != 0 &&
            strchr(node->name, '.') == NULL) {

            if (!sym_lookup(node->name)) {
                log_error(node->lineno,
                          "Undeclared variable:",
                          node->name);
            }
        }
    }

    if (node->kind == NODE_FUNC_CALL) {
        if (!sym_lookup(node->name)) {
            log_error(node->lineno,
                      "Call to undeclared function:",
                      node->name);
        }
    }

    // ------------------------------------------------------
    // 3. TYPE CHECKING (ASSIGNMENTS)
    // ------------------------------------------------------
        /* Type checking happens "Post-Order" logically,
       because we need to infer the types of children first.
       However, we do the check here before recursing, calling infer_type 
       which does its own mini-recursion.
    */

    if (node->kind == NODE_ASSIGN) {
        char *lhsType = infer_type(node->left);
        char *rhsType = infer_type(node->right);

        if (strcmp(lhsType, "unknown") != 0 &&
            strcmp(rhsType, "unknown") != 0) {

            if (strcmp(lhsType, rhsType) != 0) {

                // Allow implicit int → float conversion
                if (!(strcmp(lhsType, "float") == 0 &&
                      strcmp(rhsType, "integer") == 0)) {

                    char msg[100];
                    sprintf(msg,
                            "Type mismatch. Cannot assign '%s' to '%s'",
                            rhsType, lhsType);
                    log_error(node->lineno, msg, NULL);
                }
            }
        }
    }

    // --- [NEW LOGIC] Return Statement Checking ---
    if (node->kind == NODE_RETURN) {
        // node->left is the expression being returned (e.g., return 5;)
        char *actualType = infer_type(node->left);

        if (currentFuncType != NULL) {
            if (strcmp(currentFuncType, "void") == 0) {
                // Void functions should not return a value (other than empty)
                if (strcmp(actualType, "void") != 0) {
                     log_error(node->lineno, "Return mismatch: Void function cannot return a value.", actualType);
                }
            }
            else if (strcmp(actualType, currentFuncType) != 0) {
                 // Check for valid implicit casting (Int -> Float)
                 int isCompatible = (strcmp(currentFuncType, "float") == 0 && 
                                     strcmp(actualType, "integer") == 0);

                 if (!isCompatible) {
                     char msg[100];
                     sprintf(msg, "Return type mismatch. Expected '%s' but got '%s'", 
                             currentFuncType, actualType);
                     log_error(node->lineno, msg, NULL);
                 }
            }
        }
    }

    // ------------------------------------------------------
    // 4. RECURSIVE DFS TRAVERSAL
    // ------------------------------------------------------
    // This allows us to process the *inside* of the class/function
    // while the scope we just pushed is still active.

    traverse_and_check(node->left);
    traverse_and_check(node->right);
    traverse_and_check(node->extra); // else blocks, etc.

    // ------------------------------------------------------
    // 5. EXIT SCOPE IF ENTERED
    // ------------------------------------------------------
    // If we opened a scope at the start of this node, we must close it now
    // so that siblings of this node are back in the outer scope.

    if (popped) {
        sym_exit_scope();
    }

    // ------------------------------------------------------
    // 6. PROCESS SIBLING NODES
    // ------------------------------------------------------
    // Process the next statement in the block

    traverse_and_check(node->next);
}

// =========================================================
// SEMANTIC ANALYSIS ENTRY POINT
// =========================================================

void semantic_analysis(ASTNode *root) {

    errFile = fopen("semantic_errors.txt", "w");
    if (!errFile) {
        perror("Failed to open error file");
        return;
    }

    errorCount = 0;

    // Create global scope
    sym_init();

    // Run semantic analysis pass
    traverse_and_check(root);

    if (errorCount == 0) {
        fprintf(errFile, "Success: No semantic errors found.\n");
    } else {
        fprintf(errFile, "\nTotal Errors: %d\n", errorCount);
    }

    fclose(errFile);

    printf("Semantic Analysis Complete. %d errors found. See semantic_errors.txt\n",
           errorCount);
}

// =========================================================
// HELPER FUNCTIONS (Missing in your provided code)
// =========================================================

const char* get_kind_name(SymbolKind kind) {
    switch (kind) {
        case KIND_VAR:   return "VAR";
        case KIND_PARAM: return "PARAM";
        case KIND_FUNC:  return "FUNC";
        case KIND_CLASS: return "CLASS";
        default:         return "UNKNOWN";
    }
}

int get_type_size(char *type) {
    // In a real compiler, this recursively calculates size.
    // For now, we assume standard sizes.
    if (!type) return 0;
    if (strcmp(type, "integer") == 0) return 2; // 16-bit target
    if (strcmp(type, "float") == 0) return 4;
    return 0; // Void or unknown
}

// =========================================================
// SYMBOL TABLE PRINTING (FIXED)
// =========================================================

void sym_print(char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open output file for symbol table");
        return;
    }

    fprintf(fp, "======================================================================================================\n");
    fprintf(fp, "                                     SYMBOL TABLE DUMP                                                \n");
    fprintf(fp, "======================================================================================================\n");

    Scope *s = scopeListHead;
    
    // We iterate through the linked list of scopes
    while (s) {
        fprintf(fp, "\nSCOPE ID: %d | NAME: %s | PARENT: %d\n", 
                s->id, s->name, s->parent ? s->parent->id : -1);
        
        // FIX 1: Changed %d to %s for the headers "Kind" and "Type Sig"
        fprintf(fp, "| %-20s | %-8s | %-8s | %-12s | %-20s |\n", 
                "Symbol", "Kind", "Size", "Offset", "Type Sig");
        
        fprintf(fp, "|----------------------+----------+----------+--------------+----------------------|\n");

        for (int i = 0; i < HASH_SIZE; i++) {
            Symbol *sym = s->table[i];
            while (sym) {
                char offsetStr[32];
                if (sym->kind == KIND_VAR) {
                    snprintf(offsetStr, 32, "BP - %d", abs(sym->offset));
                } else if (sym->kind == KIND_PARAM) {
                    snprintf(offsetStr, 32, "BP + %d", abs(sym->offset));
                } else {
                    snprintf(offsetStr, 32, "(static)");
                }

                // FIX 2: Completed the fprintf statement
                fprintf(fp, "| %-20s | %-8s | %-8d | %-12s | %-20s |\n",
                        sym->name,
                        get_kind_name(sym->kind), // Ensure this returns char*
                        get_type_size(sym->type), // Ensure this returns int
                        offsetStr,
                        sym->type);               // Added the missing argument

                sym = sym->next;
            }
        }
        s = s->next; // Don't forget to move to the next scope!
    }
    
    fclose(fp);
}
