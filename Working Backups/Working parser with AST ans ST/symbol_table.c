#include "symbol_table.h"
#include <string.h>
#include <stdio.h>

#define HASH_SIZE 211

// Global State
Scope *currentScope = NULL;
Scope *scopeListHead = NULL; // List of all scopes (for printing)
int scopeCount = 0;

// djb2 Hash Function
unsigned int hash(char *s) {
    unsigned int h = 5381;
    int c;
    if (!s) return 0;
    while ((c = *s++))
        h = ((h << 5) + h) + c; /* hash * 33 + c */
    return h % HASH_SIZE;
}

void sym_init() {
    // Initialize with a global scope
    scopeListHead = NULL;
    currentScope = NULL;
    scopeCount = 0;
    sym_enter_scope("Global");
}

void sym_enter_scope(char *name) {
    Scope *s = (Scope*)malloc(sizeof(Scope));
    if (!s) {
        perror("Failed to allocate scope");
        exit(1);
    }
    s->id = scopeCount++;
    s->name = strdup(name ? name : "unnamed_scope");
    s->table = (Symbol**)calloc(HASH_SIZE, sizeof(Symbol*));
    s->parent = currentScope;
    
    // Add to the global linked list (for printing later)
    s->next = scopeListHead;
    scopeListHead = s;
    
    // Push stack
    currentScope = s;
}

void sym_exit_scope() {
    if (currentScope) {
        currentScope = currentScope->parent;
    }
}

void sym_insert(char *name, char *type, SymbolKind kind) {
    if (!currentScope) return;
    if (!name) return;

    unsigned int h = hash(name);
    
    // Check for duplicates in CURRENT scope only (shadowing is usually allowed)
    Symbol *chk = currentScope->table[h];
    while (chk) {
        if (strcmp(chk->name, name) == 0) {
            fprintf(stderr, "Semantic Error: Symbol '%s' already declared in scope '%s'\n", 
                    name, currentScope->name);
            return; // Don't insert duplicate
        }
        chk = chk->next;
    }

    Symbol *sym = (Symbol*)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type ? strdup(type) : strdup("unknown");
    sym->kind = kind;
    
    // Insert at head of bucket
    sym->next = currentScope->table[h];
    currentScope->table[h] = sym;
}

void sym_print(char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open symbol table output");
        return;
    }
    
    fprintf(fp, "======================================================================================\n");
    fprintf(fp, "                                  SYMBOL TABLE DUMP                                   \n");
    fprintf(fp, "======================================================================================\n");
    
    // Iterate through the list of ALL scopes created
    Scope *s = scopeListHead;
    while (s) {
        fprintf(fp, "\n");
        fprintf(fp, "SCOPE ID: %-3d  |  NAME: %-25s  |  PARENT ID: %d\n", 
                s->id, s->name, s->parent ? s->parent->id : -1);
        fprintf(fp, "+----------------------+------------+--------------------------------+-----------+\n");
        fprintf(fp, "| %-20s | %-10s | %-30s | %-9s |\n", "Symbol Name", "Kind", "Type", "Array?");
        fprintf(fp, "+----------------------+------------+--------------------------------+-----------+\n");
        
        int count = 0;
        for (int i = 0; i < HASH_SIZE; i++) {
            Symbol *sym = s->table[i];
            while (sym) {
                char *kStr = "UNKNOWN";
                switch (sym->kind) {
                    case KIND_VAR:   kStr = "VAR"; break;
                    case KIND_FUNC:  kStr = "FUNC"; break;
                    case KIND_CLASS: kStr = "CLASS"; break;
                    case KIND_PARAM: kStr = "PARAM"; break;
                }
                
                // You can add logic here if you stored array dimensions
                // For now, I'll assume scalar (No)
                char *arrStr = "No"; 

                fprintf(fp, "| %-20s | %-10s | %-30s | %-9s |\n", 
                        sym->name, kStr, sym->type, arrStr);
                
                sym = sym->next;
                count++;
            }
        }
        
        if (count == 0) {
            fprintf(fp, "| %-77s |\n", " < Empty Scope >");
        }
        fprintf(fp, "+----------------------+------------+--------------------------------+-----------+\n");
        s = s->next;
    }
    
    fclose(fp);
    printf("Symbol Table (Formatted) written to %s\n", filename);
}

// === SEMANTIC TRAVERSAL ===

void build_symbol_table(ASTNode *node) {
    if (!node) return;

    int popped = 0;

    switch (node->kind) {
        case NODE_CLASS_DECL:
            // Insert Class name in current scope (likely global)
            sym_insert(node->name, "class", KIND_CLASS);
            // Enter Class Scope
            sym_enter_scope(node->name);
            popped = 1;
            break;

        case NODE_FUNC_DEF:
            // Insert Function name in current scope
            // Note: In strict Pascal-like languages, func name is in parent scope.
            sym_insert(node->name, "function", KIND_FUNC);
            // Enter Function Scope
            sym_enter_scope(node->name);
            popped = 1;
            break;
            
        case NODE_VAR_DECL:
            // Insert Variable
            // node->name is the ID
            // node->type_name holds the type string (e.g., "integer")
            sym_insert(node->name, node->type_name, KIND_VAR);
            break;

        case NODE_PARAM:
            // Insert Parameter
            sym_insert(node->name, node->type_name, KIND_PARAM);
            break;
            
        // Optional: Handle Blocks { } if they create new scopes in your language
        // case NODE_BLOCK: 
        //    sym_enter_scope("block"); 
        //    popped = 1; 
        //    break;

        default:
            break;
    }

    // Recursively visit children
    // The order depends on your language semantics. 
    // Usually: Left (Params/Inheritance) -> Right (Body) -> Extra (Else/etc)
    
    build_symbol_table(node->left);
    build_symbol_table(node->right);
    build_symbol_table(node->extra); // Handle the 3rd child (e.g. IF-ELSE)

    // If we entered a scope, we must exit it before processing siblings
    if (popped) {
        sym_exit_scope();
    }

    // Process the next statement/declaration in the list
    build_symbol_table(node->next);
}