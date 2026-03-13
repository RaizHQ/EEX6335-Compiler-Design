#include "symbol_table.h"
#include <string.h>
#include <stdio.h>

#define HASH_SIZE 211

// Global State
Scope *currentScope = NULL;
Scope *scopeListHead = NULL;
int scopeCount = 0;

// Error Reporting File
FILE *errFile = NULL;
int errorCount = 0;

// --- HASH FUNCTION ---
unsigned int hash(char *s) {
    unsigned int h = 5381;
    int c;
    if (!s) return 0;
    while ((c = *s++)) h = ((h << 5) + h) + c;
    return h % HASH_SIZE;
}

// --- ERROR HELPER ---
void log_error(int lineno, const char *msg, const char *detail) {
    if (!errFile) return;
    fprintf(errFile, "Line %d: Semantic Error: %s %s\n", lineno, msg, detail ? detail : "");
    printf("Line %d: Semantic Error: %s %s\n", lineno, msg, detail ? detail : "");
    errorCount++;
}

// --- SYMBOL TABLE OPS ---

void sym_init() {
    scopeListHead = NULL;
    currentScope = NULL;
    scopeCount = 0;
    sym_enter_scope("Global");
}

void sym_enter_scope(char *name) {
    Scope *s = (Scope*)malloc(sizeof(Scope));
    s->id = scopeCount++;
    s->name = strdup(name ? name : "unnamed");
    s->table = (struct Symbol**)calloc(HASH_SIZE, sizeof(Symbol*));
    s->parent = currentScope;
    
    // Maintain global list for printing
    s->next = scopeListHead;
    scopeListHead = s;

    // --- OFFSET INITIALIZATION (8086 Model) ---
    // BP+0 = Old BP
    // BP+2 = Return Address
    // BP+4 = First Parameter
    s->paramOffset = 4;

    // BP-0 = Old BP (start)
    // BP-2 = First Local
    s->localOffset = -2;
    s->totalStackSize = 0;
    
    currentScope = s;
}

void sym_exit_scope() {
    if (currentScope) {
        // Calculate total size used by locals (convert negative offset to positive size)
        // e.g., if localOffset reached -6, we used 4 bytes (starts at -2) + 2 bytes. 
        // Math: abs(localOffset + 2) is wrong. 
        // Logic: If localOffset is -6, we used -2 and -4. Size is 4.
        currentScope->totalStackSize = -(currentScope->localOffset) - 2;
        if (currentScope->totalStackSize < 0) currentScope->totalStackSize = 0;
        
        currentScope = currentScope->parent;
    }
}

void sym_insert(char *name, char *type, SymbolKind kind) {
    if (!currentScope || !name) return;
    unsigned int h = hash(name);
    
    Symbol *sym = (Symbol*)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type ? strdup(type) : strdup("unknown");
    sym->kind = kind;
    sym->next = currentScope->table[h];
    currentScope->table[h] = sym;

    // --- OFFSET CALCULATION ---
    // Assuming 16-bit (2 byte) integers/pointers for simplicity
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
        sym->offset = 0; // Funcs/Classes don't have stack offsets
    }

}


// Lookup up the scope chain
Symbol* sym_lookup(char *name) {
    if (!name) return NULL;
    Scope *s = currentScope;
    unsigned int h = hash(name);
    
    while (s) {
        Symbol *sym = s->table[h];
        while (sym) {
            if (strcmp(sym->name, name) == 0) return sym;
            sym = sym->next;
        }
        s = s->parent;
    }
    return NULL;
}

// Helper to check duplicates in CURRENT scope only
int sym_exists_in_current_scope(char *name) {
    if (!currentScope || !name) return 0;
    unsigned int h = hash(name);
    Symbol *sym = currentScope->table[h];
    while (sym) {
        if (strcmp(sym->name, name) == 0) return 1;
        sym = sym->next;
    }
    return 0;
}

char* sym_lookup_type(char *name) {
    Symbol *s = sym_lookup(name);
    return s ? s->type : NULL;
}

// --- TYPE INFERENCE HELPER ---
char* infer_type(ASTNode *node) {
    if (!node) return "void";
    
    if (node->kind == NODE_LITERAL_INT) return "integer";
    if (node->kind == NODE_LITERAL_FLOAT) return "float";
    
    if (node->kind == NODE_VAR_ACCESS) {
        // Handle "self" or basic IDs
        if (strcmp(node->name, "self") == 0) return "class_instance";
        // Handle dot notation (simplified: return unknown or assume last part)
        if (strchr(node->name, '.')) return "unknown"; 

        char *t = sym_lookup_type(node->name);
        return t ? t : "unknown";
    }
    
    if (node->kind == NODE_EXPR_BINOP) {
        // If Logic Op, return integer/boolean
        if (strcmp(node->op, "==") == 0 || strcmp(node->op, "<") == 0 || 
            strcmp(node->op, "or") == 0) return "integer"; 
        
        // Otherwise, inherit from left child (Simplified)
        return infer_type(node->left);
    }
    
    if (node->kind == NODE_FUNC_CALL) {
        // Lookup function return type
        char *t = sym_lookup_type(node->name);
        return t ? t : "unknown";
    }
    
    return "unknown";
}

// ==========================================================
// MERGED TRAVERSAL: BUILD TABLE + CHECK SEMANTICS
// ==========================================================

void traverse_and_check(ASTNode *node) {
    if (!node) return;
    
    int popped = 0;

    // 1. DECLARATION HANDLING
    if (node->kind == NODE_CLASS_DECL) {
        // Check Duplicate Class
        if (sym_exists_in_current_scope(node->name)) {
            log_error(node->lineno, "Duplicate class declaration:", node->name);        
        
        } else {
            sym_insert(node->name, "class", KIND_CLASS);
        }
        
        // Enter Scope
        sym_enter_scope(node->name);
        node->scope = currentScope; // LINK AST TO SCOPE
        popped = 1;
    }
    else if (node->kind == NODE_FUNC_DEF) {
        // Check Duplicate Function
        if (sym_exists_in_current_scope(node->name)) {
            log_error(node->lineno, "Duplicate function declaration:", node->name);
        } else {
            // Note: In strict Pascal/C, func name goes in parent scope.
            // We insert into parent scope (already there), THEN enter new scope.
            sym_insert(node->name, "function", KIND_FUNC); 
        }
        
        sym_enter_scope(node->name);
        // *** CRITICAL: LINK AST NODE TO THIS NEW SCOPE ***
        node->scope = currentScope; 
        popped = 1;
    }
    else if (node->kind == NODE_VAR_DECL || node->kind == NODE_PARAM) {
        // Check Duplicate Variable
        if (sym_exists_in_current_scope(node->name)) {
            log_error(node->lineno, "Duplicate variable/param declaration:", node->name);
        } else {
            sym_insert(node->name, node->type_name, 
                       node->kind == NODE_PARAM ? KIND_PARAM : KIND_VAR);
        }
    }

    // 2. USAGE CHECKS
    if (node->kind == NODE_VAR_ACCESS) {
        // Skip 'self' and complex dot access for this simplified phase
        if (strcmp(node->name, "self") != 0 && strchr(node->name, '.') == NULL) {
            Symbol *s = sym_lookup(node->name);
            if (!s) {
                log_error(node->lineno, "Undeclared variable:", node->name);
            }
        }
    }
    
    if (node->kind == NODE_FUNC_CALL) {
         Symbol *s = sym_lookup(node->name);
         if (!s) {
             log_error(node->lineno, "Call to undeclared function:", node->name);
         }
    }

    // 3. TYPE CHECKS (Assignments)
    if (node->kind == NODE_ASSIGN) {
        char *lhsType = infer_type(node->left);
        char *rhsType = infer_type(node->right);
        
        // If either is unknown, we probably already reported an undeclared error
        if (strcmp(lhsType, "unknown") != 0 && strcmp(rhsType, "unknown") != 0) {
            // Strict match check
            if (strcmp(lhsType, rhsType) != 0) {
                // Allow int-to-float promotion
                if (!(strcmp(lhsType, "float") == 0 && strcmp(rhsType, "integer") == 0)) {
                    char msg[100];
                    sprintf(msg, "Type mismatch. Cannot assign '%s' to '%s'", rhsType, lhsType);
                    log_error(node->lineno, msg, NULL);
                }
            }
        }
    }

    // 4. RECURSION (Depth First)
    traverse_and_check(node->left);
    traverse_and_check(node->right);
    traverse_and_check(node->extra); // Don't forget the 'extra' child (else block)

    // 5. SCOPE EXIT
    if (popped) {
        sym_exit_scope();
    }

    // 6. PROCESS SIBLINGS (Linked Lists)
    traverse_and_check(node->next);
}

// Entry Point
void semantic_analysis(ASTNode *root) {
    errFile = fopen("semantic_errors.txt", "w");
    if (!errFile) {
        perror("Failed to open error file");
        return;
    }
    
    errorCount = 0;
    sym_init(); // Initialize Global Scope
    
    // Run the merged pass
    traverse_and_check(root);
    
    if (errorCount == 0) {
        fprintf(errFile, "Success: No semantic errors found.\n");
    } else {
        fprintf(errFile, "\nTotal Errors: %d\n", errorCount);
    }
    
    fclose(errFile);
    printf("Semantic Analysis Complete. %d errors found. See semantic_errors.txt\n", errorCount);
}

void sym_print(char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;

    fprintf(fp, "======================================================================================================\n");
    fprintf(fp, "                                     SYMBOL TABLE DUMP                                                \n");
    fprintf(fp, "======================================================================================================\n");

    Scope *s = scopeListHead;
    while (s) {
        // --- HEADER: Added "Stack Size" ---
        fprintf(fp, "\nSCOPE ID: %-3d | NAME: %-20s | PARENT ID: %-3d | STACK SIZE: %d bytes\n", 
                s->id, s->name, s->parent ? s->parent->id : -1, s->totalStackSize);
        
        fprintf(fp, "+----------------------+------------+---------------------------+------------+\n");
        fprintf(fp, "| %-20s | %-10s | %-25s | %-10s |\n", "Symbol Name", "Kind", "Type", "Offset");
        fprintf(fp, "+----------------------+------------+---------------------------+------------+\n");

        for (int i = 0; i < HASH_SIZE; i++) {
            Symbol *sym = s->table[i];
            while (sym) {
                // Determine Kind String
                char *kStr = "UNKNOWN";
                if (sym->kind == KIND_VAR) kStr = "VAR";
                else if (sym->kind == KIND_FUNC) kStr = "FUNC";
                else if (sym->kind == KIND_CLASS) kStr = "CLASS";
                else if (sym->kind == KIND_PARAM) kStr = "PARAM";

                // --- OFFSET FORMATTING ---
                // We format the offset string (e.g., "BP-2", "BP+4", or "-")
                char offsetStr[16];
                if (sym->kind == KIND_VAR) {
                    snprintf(offsetStr, 16, "%d (BP%d)", sym->offset, sym->offset);
                } else if (sym->kind == KIND_PARAM) {
                    snprintf(offsetStr, 16, "%d (BP+%d)", sym->offset, sym->offset);
                } else {
                    snprintf(offsetStr, 16, "-");
                }

                fprintf(fp, "| %-20s | %-10s | %-25s | %-10s |\n", 
                        sym->name, kStr, sym->type, offsetStr);
                
                sym = sym->next;
            }
        }
        fprintf(fp, "+----------------------+------------+---------------------------+------------+\n");
        s = s->next;
    }
    fclose(fp);
    printf("Symbol table dumped to %s\n", filename);
}