#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>




/* --- 1. LEXER & TOKEN --- */

// All tokens returned by the lexer
typedef enum {
    TOK_UNKNOWN = 0,
    TOK_EOF = 1,
    
    // --- Start Custom Tokens Above ASCII (0-255) ---
    TOK_CUSTOM_START = 256,
    
    // Keywords
    TOK_CLASS = 256, TOK_ISA, TOK_IMPLEMENT, TOK_PUBLIC, TOK_PRIVATE,
    TOK_FUNC, TOK_CONSTRUCTOR, TOK_ATTRIBUTE, TOK_LOCAL,
    TOK_IF, TOK_THEN, TOK_ELSE, TOK_WHILE, TOK_READ, TOK_WRITE,
    TOK_RETURN, TOK_NOT, TOK_SELF, TOK_INTEGER, TOK_FLOAT,
    TOK_VOID, TOK_OR, TOK_AND,
    
    // Literals & IDs
    TOK_ID,
    TOK_INTLIT,
    TOK_FLOATLIT,
    
// Operators
    TOK_ARROW,      // => (High-numbered)
    TOK_ASSIGN,     // := (High-numbered)
    TOK_EQ,         // == (High-numbered)
    TOK_NEQ,        // <> (High-numbered)
    TOK_LE,         // <= (High-numbered)
    TOK_GE,         // >= (High-numbered)
    
    
} TokenType;


#define TOK_LBRACE    (TokenType)'{'
#define TOK_RBRACE     (TokenType)'}'
#define TOK_SEMI      (TokenType)';'
#define TOK_COMMA     (TokenType)','
#define TOK_LPAREN    (TokenType)'('
#define TOK_RPAREN    (TokenType)')'
#define TOK_COLON      (TokenType)':'
#define TOK_DOT        (TokenType)'.'
#define TOK_LBRACKET   (TokenType)'['
#define TOK_RBRACKET  (TokenType)']'

#define TOK_PLUS      (TokenType)'+'
#define TOK_MINUS     (TokenType)'-'
#define TOK_MULTIPLY   (TokenType)'*'
#define TOK_DIVIDE     (TokenType)'/'
#define TOK_LT        (TokenType)'<'
#define TOK_GT        (TokenType)'>'

// The struct from your lexer, now our standard Token
typedef struct {
    char* lexeme;
    int line;
    int column;
    TokenType type;
} Token;


/* --- 2. ABSTRACT SYNTAX TREE (AST) --- */

typedef enum {
    NODE_STMT_LIST,
    NODE_CLASS_DECL, NODE_IMPLEMENT_DEF, NODE_FUNC_DEF,
    NODE_CONSTRUCTOR_DEF,
    NODE_FUNC_DECL, NODE_ATTRIBUTE_DECL,
    NODE_VAR_DECL, NODE_LOCAL_VAR_DECL,
    NODE_IF_STMT, NODE_WHILE_STMT, NODE_READ_STMT, NODE_WRITE_STMT,
    NODE_RETURN_STMT, NODE_ASSIGN_STMT, NODE_CALL_STMT,
    NODE_ID, NODE_INTLIT, NODE_FLOATLIT, NODE_SELF,
    NODE_BINARY_OP, NODE_UNARY_OP,
    NODE_ARRAY_ACCESS, NODE_MEMBER_ACCESS, NODE_FUNC_CALL,
    NODE_PARAM_LIST, NODE_ARG_LIST,
    NODE_TYPE_INT, NODE_TYPE_FLOAT, NODE_TYPE_VOID, NODE_TYPE_ID,
    NODE_TYPE_ARRAY
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int lineno;
    
    // Pointers to children in the tree
    struct ASTNode *left;  // Often used for expressions, LHS
    struct ASTNode *right; // Often used for expressions, RHS
    struct ASTNode *third; // For 'if-then-else' (else block)
    
    // For lists (statements, params, args)
    struct ASTNode *next_sibling;
    
    // Value for terminals/leaves
    char *sval;     // For ID
    int ival;       // For INTLIT
    float fval;     // For FLOATLIT

    // Filled in by Phase 2 (Semantic Analysis)
    struct Type *expr_type; 
    struct SymbolInfo *symbol;

} ASTNode;


/* --- 3. TYPE SYSTEM --- */

typedef enum {
    TYPE_KIND_ATOM, TYPE_KIND_ARRAY, TYPE_KIND_CLASS, TYPE_KIND_FUNC
} TypeKind;
typedef enum { ATOM_INT, ATOM_FLOAT, ATOM_VOID } AtomicType;
struct SymbolTable;

typedef struct Type {
    TypeKind kind;
    union {
        AtomicType atom;
        struct {
            struct Type *base_type;
        } array;
        struct {
            char *name;
            struct SymbolTable *members;
        } class_ref;
        struct {
            struct Type *return_type;
            struct Type **param_types;
            int num_params;
        } func;
    } data;
} Type;


/* --- 4. SYMBOL TABLE --- */

typedef enum {
    SYMBOL_KIND_VAR, SYMBOL_KIND_FUNC, SYMBOL_KIND_CLASS
} SymbolKind;

typedef struct SymbolInfo {
    char *name;
    SymbolKind kind;
    Type *type;
    int scope_level;
    struct SymbolTable *child_table; // For functions (params) or classes (members)
    struct SymbolInfo *next;
} SymbolInfo;

typedef struct SymbolTable {
    SymbolInfo *buckets[1024];
    int scope_level;
    struct SymbolTable *parent;
} SymbolTable;

typedef struct {
    SymbolTable *current;
    int current_level;
    SymbolTable *global;
} ScopeManager;


/* --- 5. DIAGNOSTICS --- */

typedef enum {
    ERROR_LEXICAL, ERROR_SYNTAX, ERROR_SEMANTIC, WARNING_GENERAL
} DiagnosticType;


/* --- 6. GLOBAL FUNCTION PROTOTYPES --- */

// ast.c
// ast.c
ASTNode* create_node(NodeType type, int lineno, ASTNode *left, ASTNode *right, ASTNode *third);
ASTNode* create_leaf_id(int lineno, char *sval);

/* --- MODIFIED LINES --- */
ASTNode* create_leaf_intlit(int lineno, int ival, const char* lexeme);
ASTNode* create_leaf_floatlit(int lineno, float fval, const char* lexeme);
/* --- END MODIFIED LINES --- */

ASTNode* create_type_node(TokenType type, int lineno);
void free_ast(ASTNode *root);

// symboltable.c
ScopeManager* create_scope_manager();
void destroy_scope_manager(ScopeManager *manager);
void scope_enter(ScopeManager *manager);
void scope_exit(ScopeManager *manager);
SymbolInfo* scope_insert(ScopeManager *manager, char *name, SymbolKind kind, Type *type);
SymbolInfo* scope_lookup(ScopeManager *manager, char *name);
SymbolInfo* scope_lookup_current(ScopeManager *manager, char *name);
Type* create_atomic_type(AtomicType atom);
Type* create_array_type(Type *base_type);
int types_are_compatible(Type *t1, Type *t2);
extern Type *g_type_int, *g_type_float, *g_type_void; // Pre-defined types

// parser.c
ASTNode* parse_program(void); // The main entry point for parsing

// semantics.c
void semantic_check(ASTNode *root, ScopeManager *manager);

// diagnostics.c
void record_error(int lineno, DiagnosticType type, const char *fmt, ...);
void report_all_diagnostics(void);
int get_error_count(void);

// lexer.l (lex.yy.c)
int yylex(void); // The flex-generated function
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

// symboltable.c (helper function we will add)
char* type_to_string(Type *type); 

// symboltable_printer.c (new function)
void print_symbol_table_to_file(ScopeManager *manager, const char* filename);


// ------------------- code gen ------------------------------------------

// Assuming basic structure for TAC instructions:
typedef struct TACInstruction {
    enum { TAC_ADD, TAC_SUB, TAC_COPY, TAC_LABEL, TAC_JUMP, TAC_IFGOTO } op;
    char *result;
    char *op1;
    char *op2;
    // Other fields for next/prev pointers in a block
    struct TACInstruction *next;
} TACInstruction;

// --- Register Constants (x86-64 conceptual, 8 general-purpose registers) ---

#define R_COUNT 8
#define R0 0  // RAX (Return value / Volatile)
#define R1 1  // RBX (Callee-saved / Base pointer if needed)
#define R2 2  // RCX (Argument 1 / Volatile)
#define R3 3  // RDX (Argument 2 / Volatile)
#define R4 4  // RDI (Argument 4 / Volatile)
#define R5 5  // RSI (Argument 3 / Volatile)
#define R6 6  // R8 (Volatile)
#define R7 7  // R9 (Volatile)

// --- Register and Address Descriptors ---

#define MAX_VAR_NAME_LEN 32

// A single entry in the Register Descriptor Table (RDT)
typedef struct {
    bool is_empty;
    char var_name[MAX_VAR_NAME_LEN];
    // In a complex model, this would be a set of variables, but we simplify to one.
} RegDescriptor;

// A single entry in the Address Descriptor Table (ADT)
// Tracks locations for a single variable/temporary.
// Simplified: tracks up to 2 locations (Reg and Mem/Stack)
typedef struct {
    char name[MAX_VAR_NAME_LEN];
    int reg_location; // -1 if not in register; R0..R7 otherwise
    char mem_location[MAX_VAR_NAME_LEN]; // e.g., "[RBP - 16]" or "global_var"
} AddrDescriptor;

// Global Descriptors used by the Code Generator
extern RegDescriptor RDT[R_COUNT];
extern AddrDescriptor *ADT_map[MAX_VARIABLES]; // Map from variable ID/Name to descriptor

// --- Function Prototypes for Code Generation (in codegen.c) ---

/**
 * @brief Main function to generate code for a basic block.
 * @param block_start Pointer to the first instruction of the basic block.
 */
void CodeGen_BasicBlock(TACInstruction* block_start);

/**
 * @brief Heuristic to select or free a register for an operand or result.
 * @param var_name The name of the variable (source or destination).
 * @param is_dest True if this is the destination variable (Rx).
 * @param needed_later True if the variable's value is needed after the current instruction (live).
 * @return The ID of the physical register allocated.
 */
int getReg(const char* var_name, bool is_dest, bool needed_later);

/**
 * @brief Updates the Register and Address Descriptors after code is emitted.
 */
void updateDescriptors(const char* x, int Rx, const char* y, int Ry, const char* z, int Rz);





#endif // COMPILER_H