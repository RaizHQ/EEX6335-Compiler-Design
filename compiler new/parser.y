%{
/* =========================================================================
   COMPILER ARCHITECT: PARSER IMPLEMENTATION
   =========================================================================
   This file defines the Context-Free Grammar (CFG) and the Syntax-Directed
   Translation scheme to build the Abstract Syntax Tree (AST).
   
   Tools: Bison/Yacc
   Output: y.tab.c, y.tab.h
   ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"           // AST Node definitions and factory functions
#include "symbol_table.h"  // For semantic analysis phase
#include "code_gen.h"      // For assembly generation phase
#include "semantic_analysis.h"

// External Linkage
extern int yylex(void);    // Defined in lex.yy.c
extern int yylineno;       // Line number tracker from Lexer
extern char *yytext;       // Current token text

void yyerror(const char *s);

// Global AST Root
ASTNode *rootNode = NULL;

// Debug helper prototype
void print_lex_tokens(char *filename);

%}

/* =========================================================================
   UNION: Data flow between Lexer and Parser
   ========================================================================= */
%union {
    int ival;           // For LITERAL_INTEGER
    float fval;         // For LITERAL_FLOAT
    char *sval;         // For ID and Strings
    struct ASTNode *node; // THE CRITICAL FIELD: All non-terminals return this
}

/* =========================================================================
   TOKENS
   ========================================================================= */

/* Keywords */
%token CLASS IMPLEMENT FUNC ATTRIBUTE
%token PUBLIC PRIVATE LOCAL
%token IF THEN ELSE WHILE
%token READ WRITE RETURN
%token ISA
%token INTEGER FLOAT VOID
%token SELF
%token CONSTRUCT

/* Literals & Identifiers */
%token <sval> ID
%token <ival> LITERAL_INTEGER
%token <fval> LITERAL_FLOAT

/* Operators */
%token OR AND NOT
%token ARROW_OP ASSIGN_OP
%token EQ_OP NE_OP LE_OP GE_OP

/* =========================================================================
   NON-TERMINAL TYPES
   All grammar rules return a 'node' pointer to build the tree.
   ========================================================================= */
%type <node> prog classOrImplOrFunc_ZM classOrImplOrFunc
%type <node> classDecl isa_Id_ZO commaID_ZM visibility_memberDecl_ZM
%type <node> implDef funcDef_ZM funcDef funcHead funcBody
%type <node> visibility memberDecl funcDecl attributeDecl
%type <node> localVarDecl varDecl arraySize_ZM arraySize
%type <node> varDeclOrStmt_ZM varDeclOrStmt statement matchedStmt unmatchedStmt statement_ZM
%type <node> expr relExpr arithExpr term factor
%type <node> variable primaryVar idOrSelf suffix_ZM suffix indice_ZM indice
%type <node> functionCall fParams fParamsTail_ZM fParamsTail aParams aParamsTail_ZM aParamsTail
%type <node> type returnType assignOp relOp addOp multOp

/* =========================================================================
   OPERATOR PRECEDENCE
   Resolves shift/reduce conflicts (e.g., dangling else, order of operations).
   ========================================================================= */
%left OR
%left AND
%left EQ_OP NE_OP '<' '>' LE_OP GE_OP
%left '+' '-'
%left '*' '/'
%right NOT
%right UMINUS

/* Entry Point */
%start prog

%%

/* =========================================================================
   GRAMMAR RULES & SEMANTIC ACTIONS
   ========================================================================= */

/* --- Program Structure --- */

prog
    : classOrImplOrFunc_ZM 
      { 
        rootNode = $1; // Capture the head of the list as the global root
      }
    ;

classOrImplOrFunc_ZM
    : classOrImplOrFunc classOrImplOrFunc_ZM
      { 
        /* Link the current node to the rest of the list */
        $$ = $1; 
        if ($$ != NULL) $$->next = $2; 
        else $$ = $2; 
      }
    | /* ε */ 
      { $$ = NULL; }
    ;

classOrImplOrFunc
    : classDecl { $$ = $1; }
    | implDef   { $$ = $1; }
    | funcDef   { $$ = $1; }
    ;

/* --- Class Declarations --- */

classDecl
    : CLASS ID isa_Id_ZO '{' visibility_memberDecl_ZM '}' ';'
      {
        $$ = createNode(NODE_CLASS_DECL);
        $$->name = $2;         // Class Name
        $$->left = $3;         // Inheritance List
        $$->right = $5;        // Member List (attributes/methods)
      }
    ;

isa_Id_ZO
    : ISA ID commaID_ZM
      {
         $$ = createNode(NODE_LIST); 
         $$->name = $2;        // The first parent class
         $$->next = $3;        // Subsequent parent classes
      }
    | /* ε */ { $$ = NULL; }
    ;

commaID_ZM
    : ',' ID commaID_ZM
      {
         ASTNode *n = createNode(NODE_LIST);
         n->name = $2;
         n->next = $3;
         $$ = n;
      }
    | /* ε */ { $$ = NULL; }
    ;

visibility_memberDecl_ZM
    : visibility memberDecl visibility_memberDecl_ZM
      {
        /* Propagate visibility context down to the member.
           $2 is the member (func or var). $1 is the visibility node.
        */
        $$ = $2;
        if ($$) {
             $$->visibility = $1->name; // e.g., "public"
             $$->next = $3;             // Link to next member
        }
      }
    | /* ε */ { $$ = NULL; }
    ;

/* --- Implement Blocks --- */

implDef
    : IMPLEMENT ID '{' funcDef_ZM '}'
      {
        $$ = createNode(NODE_IMPL_DEF);
        $$->name = $2;
        $$->right = $4; // List of function definitions
      }
    ;

funcDef_ZM
    : funcDef funcDef_ZM
      {
        $$ = $1;
        if($$) $$->next = $2;
      }
    | /* ε */ { $$ = NULL; }
    ;

/* --- Function Definitions --- */

funcDef
    : funcHead funcBody
      {
        /* Wrap head and body into a Definition Node */
        $$ = createNode(NODE_FUNC_DEF);
        $$->name = $1->name;       // Hoist name for easier access
        $$->left = $1;             // Signature (params, return type)
        $$->right = $2;            // Body (statement list)
      }
    ;

funcHead
    : FUNC ID '(' fParams ')' ARROW_OP returnType
      {
        $$ = createNode(NODE_FUNC_HEAD);
        $$->name = $2;
        $$->left = $4;  // Parameters
        $$->right = $7; // Return Type
      }
    | CONSTRUCT '(' fParams ')'
      {
        $$ = createNode(NODE_FUNC_HEAD);
        $$->name = strdup("construct"); // Special reserved name
        $$->left = $3;
        $$->right = NULL; // Constructors have no return type
      }
    ;

funcBody
    : '{' varDeclOrStmt_ZM '}' { $$ = $2; } 
    ;

visibility
    : PUBLIC  { $$ = createNode(NODE_TYPE); $$->name = strdup("public"); }
    | PRIVATE { $$ = createNode(NODE_TYPE); $$->name = strdup("private"); }
    ;

memberDecl
    : funcDecl { $$ = $1; }
    | attributeDecl { $$ = $1; }
    ;

funcDecl
    : funcHead ';' 
      {
         $$ = $1; // Just a prototype, no body
      }
    ;

/* --- Variables & Attributes --- */

attributeDecl
    : ATTRIBUTE varDecl 
      { 
         $$ = $2; 
         // Note: Semantic analysis will mark this symbol as a class attribute
      }
    ;

localVarDecl
    : LOCAL varDecl { $$ = $2; }
    ;

varDecl
    : ID ':' type arraySize_ZM ';'
      {
        $$ = createNode(NODE_VAR_DECL);
        $$->name = $1;
        $$->type_name = $3->name; // Extract "integer"/"float" string
        $$->left = $4;            // Array dimensions (if any)
      }
    ;

arraySize_ZM
    : arraySize arraySize_ZM 
      { $$ = $1; $$->next = $2; }
    | /* ε */ { $$ = NULL; }
    ;

arraySize
    : '[' LITERAL_INTEGER ']'
      {
        $$ = createLiteralInt($2);
      }
    | '[' ']' 
      { 
        $$ = createLiteralInt(-1); // -1 signifies dynamic/empty array syntax
      }
    ;

/* --- Statements --- */

varDeclOrStmt_ZM
    : varDeclOrStmt varDeclOrStmt_ZM 
      { $$ = $1; if($$) $$->next = $2; }
    | /* ε */ { $$ = NULL; }
    ;

varDeclOrStmt
    : localVarDecl { $$ = $1; }
    | statement    { $$ = $1; }
    ;

statement
    : matchedStmt { $$ = $1; }
    | unmatchedStmt { $$ = $1; }
    ;

/* Handling Dangling Else via Matched/Unmatched pairs */
matchedStmt
    : variable assignOp expr ';'
      {
        $$ = createNode(NODE_ASSIGN);
        $$->left = $1;  // L-Value
        $$->right = $3; // R-Value
      }
    | functionCall ';' { $$ = $1; }
    | IF '(' relExpr ')' THEN matchedStmt ELSE matchedStmt ';'
      {
        $$ = createNode(NODE_IF);
        $$->left = $3;  // Condition
        $$->right = $6; // Then Block
        $$->extra = $8; // Else Block
      }
    | WHILE '(' relExpr ')' matchedStmt ';'
      {
        $$ = createNode(NODE_WHILE);
        $$->left = $3;
        $$->right = $5;
      }
    | READ '(' variable ')' ';'
      {
        $$ = createNode(NODE_READ);
        $$->left = $3;
      }
    | WRITE '(' expr ')' ';'
      {
        $$ = createNode(NODE_WRITE);
        $$->left = $3;
      }
    | RETURN '(' expr ')' ';'
      {
        $$ = createNode(NODE_RETURN);
        $$->left = $3;
      }
    | '{' statement_ZM '}'
      {
        $$ = createNode(NODE_BLOCK);
        $$->left = $2;
      }
    ;

unmatchedStmt
    : IF '(' relExpr ')' THEN statement
      {
        $$ = createNode(NODE_IF);
        $$->left = $3;
        $$->right = $6;
        $$->extra = NULL; // No Else
      }
    | IF '(' relExpr ')' THEN matchedStmt ELSE unmatchedStmt ';'
      {
        $$ = createNode(NODE_IF);
        $$->left = $3;
        $$->right = $6;
        $$->extra = $8;
      }
    | WHILE '(' relExpr ')' unmatchedStmt ';'
      {
        $$ = createNode(NODE_WHILE);
        $$->left = $3;
        $$->right = $5;
      }
    ;

statement_ZM
    : statement statement_ZM 
      { $$ = $1; if($$) $$->next = $2; }
    | /* ε */ { $$ = NULL; }
    ;

/* --- Expressions --- */

expr
    : relExpr { $$ = $1; }
    ;

relExpr
    : arithExpr relOp arithExpr 
      { 
          // $2 is a NODE_TYPE containing the operator string
          $$ = createBinary($2->op, $1, $3); 
      }
    | arithExpr { $$ = $1; }
    ;

arithExpr
    : arithExpr addOp term 
      { $$ = createBinary($2->op, $1, $3); }
    | term { $$ = $1; }
    ;

term
    : term multOp factor 
      { $$ = createBinary($2->op, $1, $3); }
    | factor { $$ = $1; }
    ;

factor
    : variable { $$ = $1; }
    | functionCall { $$ = $1; }
    | LITERAL_INTEGER { $$ = createLiteralInt($1); }
    | LITERAL_FLOAT   { $$ = createLiteralFloat($1); }
    | '(' arithExpr ')' { $$ = $2; }
    | NOT factor { $$ = createUnary("not", $2); }
    | '-' factor %prec UMINUS { $$ = createUnary("-", $2); }
    | '+' factor %prec UMINUS { $$ = createUnary("+", $2); }
    ;

/* --- Variables, Dot Notation, and Arrays --- */

variable
    : primaryVar suffix_ZM 
      {
        if ($2 == NULL) {
            $$ = $1; // Just the ID
        } else {
            /* If suffixes exist (e.g., x[i].y), we chain them.
               The AST structure here treats suffixes as a child chain.
            */
            $$ = $1;
            $$->right = $2; 
        }
      }
    ;

primaryVar
    : idOrSelf { $$ = $1; }
    ;

idOrSelf
    : ID   { $$ = createID($1); }
    | SELF { $$ = createID("self"); }
    ;

suffix_ZM
    : suffix suffix_ZM 
      { $$ = $1; $$->next = $2; }
    | indice_ZM 
      { 
          /* Indices alone are also suffixes */
          $$ = $1; 
      }
    ;

suffix
    : indice_ZM '.' idOrSelf
      {
        /* Represents: [indices].ID 
           We create a "." binary operator. 
           Left child: indices (if any). Right child: the ID.
        */
        ASTNode *dot = createBinary(".", NULL, $3);
        if ($1) {
             dot->left = $1; 
        }
        $$ = dot;
      }
    | '(' aParams ')' '.' idOrSelf
      {
        /* Represents: (args).ID  (Chained method call return access) */
        ASTNode *call = createNode(NODE_FUNC_CALL);
        call->left = $2; // params
        
        ASTNode *dot = createBinary(".", call, $5);
        $$ = dot;
      }
    ;

indice_ZM
    : indice indice_ZM { $$ = $1; $$->next = $2; }
    | /* ε */ { $$ = NULL; }
    ;

indice
    : '[' arithExpr ']' 
      {
        ASTNode *idx = createNode(NODE_VAR_ACCESS);
        idx->op = strdup("[]");
        idx->left = $2; // The index expression
        $$ = idx;
      }
    ;

/* --- Function Calls --- */

functionCall
    : primaryVar '(' aParams ')'
      {
        $$ = createNode(NODE_FUNC_CALL);
        $$->name = $1->name; // Function name
        $$->left = $3;       // Arguments
      }
    ;

/* --- Parameters --- */

fParams
    : ID ':' type arraySize_ZM fParamsTail_ZM
      {
         $$ = createNode(NODE_PARAM);
         $$->name = $1;
         $$->type_name = $3->name;
         $$->left = $4; // Array dimension info
         $$->next = $5; // Next param
      }
    | /* ε */ { $$ = NULL; }
    ;

fParamsTail_ZM
    : fParamsTail fParamsTail_ZM { $$ = $1; $$->next = $2; }
    | /* ε */ { $$ = NULL; }
    ;

fParamsTail
    : ',' ID ':' type arraySize_ZM
      {
         $$ = createNode(NODE_PARAM);
         $$->name = $2;
         $$->type_name = $4->name;
         $$->left = $5;
      }
    ;

aParams
    : expr aParamsTail_ZM 
      { 
        $$ = createNode(NODE_LIST); // Wrapper for argument list
        $$->left = $1; // First argument
        $$->next = $2; // Rest of list
      }
    | /* ε */ { $$ = NULL; }
    ;

aParamsTail_ZM
    : aParamsTail aParamsTail_ZM { $$ = $1; $$->next = $2; }
    | /* ε */ { $$ = NULL; }
    ;

aParamsTail
    : ',' expr 
      {
        $$ = createNode(NODE_LIST);
        $$->left = $2;
      }
    ;

/* --- Types and Operators --- */

type
    : INTEGER { $$ = createNode(NODE_TYPE); $$->name = strdup("integer"); }
    | FLOAT   { $$ = createNode(NODE_TYPE); $$->name = strdup("float"); }
    | ID      { $$ = createNode(NODE_TYPE); $$->name = $1; }
    ;

returnType
    : type { $$ = $1; }
    | VOID { $$ = createNode(NODE_TYPE); $$->name = strdup("void"); }
    ;

assignOp
    : ASSIGN_OP { $$ = NULL; }
    ;

/* Operator Helpers: Return a node just to transport the 'op' string */
relOp
    : EQ_OP { $$ = createNode(NODE_TYPE); $$->op = strdup("=="); }
    | '<'   { $$ = createNode(NODE_TYPE); $$->op = strdup("<"); }
    | '>'   { $$ = createNode(NODE_TYPE); $$->op = strdup(">"); }
    | NE_OP { $$ = createNode(NODE_TYPE); $$->op = strdup("<>"); }
    | LE_OP { $$ = createNode(NODE_TYPE); $$->op = strdup("<="); }
    | GE_OP { $$ = createNode(NODE_TYPE); $$->op = strdup(">="); }
    ;

addOp
    : '+' { $$ = createNode(NODE_TYPE); $$->op = strdup("+"); }
    | '-' { $$ = createNode(NODE_TYPE); $$->op = strdup("-"); }
    | OR  { $$ = createNode(NODE_TYPE); $$->op = strdup("or"); }
    ;

multOp
    : '*' { $$ = createNode(NODE_TYPE); $$->op = strdup("*"); }
    | '/' { $$ = createNode(NODE_TYPE); $$->op = strdup("/"); }
    | AND { $$ = createNode(NODE_TYPE); $$->op = strdup("and"); }
    ;

%%

/* =========================================================================
   ERROR HANDLING & MAIN DRIVER
   ========================================================================= */

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, s);
}



int main(void) {
    printf("===========================================\n");
    printf("Compiler Driver: Starting Compilation...\n");
    printf("===========================================\n");

    // 1. Parsing Phase
    printf("[Phase 1] Parsing & AST Generation...\n");
    if (yyparse() == 0) {
        printf(">> Parsing Successful.\n");
        
        if (rootNode) {
            // Dump AST
            ast_print(rootNode, "ast.txt");
            printf(">> AST written to 'ast.txt'.\n");

            // 2. Semantic Analysis Phase (Populates Symbol Table)
            //    CRITICAL: This function calls sym_init() and builds the table.
            printf("[Phase 2] Semantic Analysis...\n");
            semantic_analysis(rootNode);

            // 3. Symbol Table Dump
            //    CRITICAL: This prints the table built in Phase 2.
            printf("[Phase 3] Dumping Symbol Table...\n");
            sym_print("symbol_table.txt");
            
            // 4. Code Generation (Future)
            /* FILE *asmFile = fopen("output.asm", "w");
            if (asmFile) {
                code_gen_init(asmFile);
                code_gen(rootNode, asmFile);
                fclose(asmFile);
                printf("[Phase 4] Assembly written to 'output.asm'.\n");
            }
            */
            
        } else {
            printf("Warning: Parser succeeded but produced no AST (Empty program?).\n");
        }
    } else {
        printf("[FAILED] Parsing errors detected.\n");
        return 1;
    }

    printf("===========================================\n");
    printf("Compilation Complete.\n");
    return 0;
}