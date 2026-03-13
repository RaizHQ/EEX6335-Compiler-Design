%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "code_gen.h"

// Forward declarations
int yylex(void);
void yyerror(const char *s);
extern int yylineno;

// Global root for the AST
ASTNode *rootNode = NULL;
%}

/* Union for passing data between Lexer and Parser */
%union {
    int ival;
    float fval;
    char *sval;
    struct ASTNode *node;
}

/* Tokens */
%token CLASS IMPLEMENT FUNC ATTRIBUTE
%token PUBLIC PRIVATE LOCAL
%token IF THEN ELSE WHILE
%token READ WRITE RETURN
%token ISA
%token INTEGER FLOAT VOID
%token SELF
%token CONSTRUCT

%token <sval> ID
%token <ival> LITERAL_INTEGER
%token <fval> LITERAL_FLOAT

%token OR AND NOT
%token ARROW_OP ASSIGN_OP
%token EQ_OP NE_OP LE_OP GE_OP

/* Non-terminal types - CRITICAL: Everything returns a node */
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

/* Precedence */
%left OR
%left AND
%left EQ_OP NE_OP '<' '>' LE_OP GE_OP
%left '+' '-'
%left '*' '/'
%right NOT
%right UMINUS

%start prog

%%

/* ================= PROGRAM ================= */

prog
    : classOrImplOrFunc_ZM 
      { rootNode = $1; }
    ;

classOrImplOrFunc_ZM
    : classOrImplOrFunc classOrImplOrFunc_ZM
      { 
        $$ = $1; 
        if ($$ != NULL) $$->next = $2; 
        else $$ = $2; // Logic safety
      }
    | /* ε */ 
      { $$ = NULL; }
    ;

classOrImplOrFunc
    : classDecl { $$ = $1; }
    | implDef   { $$ = $1; }
    | funcDef   { $$ = $1; }
    ;

/* ================= CLASS ================= */

classDecl
    : CLASS ID isa_Id_ZO '{' visibility_memberDecl_ZM '}' ';'
      {
        $$ = createNode(NODE_CLASS_DECL);
        $$->name = $2;
        $$->left = $3;  // Inheritance list
        $$->right = $5; // Members list
      }
    ;

isa_Id_ZO
    : ISA ID commaID_ZM
      {
         $$ = createNode(NODE_LIST); // Placeholder for inheritance list
         $$->name = $2;
         $$->next = $3;
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
        // memberDecl ($2) is the content. We attach visibility ($1) to it.
        // But $2 might already be constructed. Let's modify $2's visibility field.
        $$ = $2;
        if ($$) {
             $$->visibility = $1->name; // Transfer "public"/"private" string
             $$->next = $3;
        }
      }
    | /* ε */ { $$ = NULL; }
    ;

/* ================= IMPLEMENT ================= */

implDef
    : IMPLEMENT ID '{' funcDef_ZM '}'
      {
        $$ = createNode(NODE_IMPL_DEF);
        $$->name = $2;
        $$->right = $4; // Functions
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

/* ================= FUNCTIONS ================= */

funcDef
    : funcHead funcBody
      {
        // funcHead returns a node with signature info.
        // We wrap it in a FUNC_DEF node.
        $$ = createNode(NODE_FUNC_DEF);
        $$->name = $1->name;       // Extract name from head
        $$->left = $1;             // Keep head details (params/type) as left child
        $$->right = $2;            // Body as right child
      }
    ;

funcHead
    : FUNC ID '(' fParams ')' ARROW_OP returnType
      {
        $$ = createNode(NODE_FUNC_HEAD);
        $$->name = $2;
        $$->left = $4; // Params
        $$->right = $7; // Return type
      }
    | CONSTRUCT '(' fParams ')'
      {
        $$ = createNode(NODE_FUNC_HEAD);
        $$->name = strdup("construct");
        $$->left = $3;
        $$->right = NULL; // No return type
      }
    ;

funcBody
    : '{' varDeclOrStmt_ZM '}' { $$ = $2; } // Body is just a list of stmts
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
         $$ = $1; // Just return the head info
      }
    ;

/* ================= VARIABLES ================= */

attributeDecl
    : ATTRIBUTE varDecl 
      { 
         $$ = $2; 
         // Mark as attribute if needed, or rely on context in class
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
        $$->type_name = $3->name; // Extract type string
        $$->left = $4; // Array dimensions
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
        $$ = createNode(NODE_LITERAL_INT);
        $$->int_val = $2;
      }
    | '[' ']' 
      { 
        $$ = createNode(NODE_LITERAL_INT); 
        $$->int_val = -1; // -1 represents empty []
      }
    ;

/* ================= STATEMENTS ================= */

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

matchedStmt
    : variable assignOp expr ';'
      {
        $$ = createNode(NODE_ASSIGN);
        $$->left = $1;
        $$->right = $3;
      }
    | functionCall ';' { $$ = $1; }
    | IF '(' relExpr ')' THEN matchedStmt ELSE matchedStmt ';'
      {
        $$ = createNode(NODE_IF);
        $$->left = $3;  // Condition
        $$->right = $6; // Then block
        $$->extra = $8; // Else block
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
        $$->extra = NULL; // No else
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

/* ================= EXPRESSIONS ================= */

expr
    : relExpr { $$ = $1; }
    ;

relExpr
    : arithExpr relOp arithExpr 
      { $$ = createBinary($2->op, $1, $3); }
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

/* ================= VARIABLE / ID NEST ================= */
/* This is complex: variable -> primaryVar -> suffix_ZM */
/* We will flatten this to a node tree */

variable
    : primaryVar suffix_ZM 
      {
        if ($2 == NULL) {
            $$ = $1; // Just the ID
        } else {
            // Suffixes exist (array indexing or dot notation)
            // We attach suffix list to the primary var
            $$ = $1;
            $$->right = $2; // Attach suffixes to right
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
    | indice_ZM { $$ = $1; } /* Indices are also suffixes effectively */
    ;

suffix
    : indice_ZM '.' idOrSelf
      {
        // Complex: array access then dot? 
        // We'll create a DOT node.
        ASTNode *dot = createBinary(".", NULL, $3);
        if ($1) {
             // If indices exist, they apply to the *left* of the dot (the prev object)
             // But here we are returning a suffix chain.
             // Strategy: Return a chain node.
             dot->left = $1; // Store indices here
        }
        $$ = dot;
      }
    | '(' aParams ')' '.' idOrSelf
      {
        // Method call then dot access
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
        // Represent array index as a specialized node
        ASTNode *idx = createNode(NODE_VAR_ACCESS);
        idx->op = strdup("[]");
        idx->left = $2; // The expression inside [ ]
        $$ = idx;
      }
    ;

/* ================= FUNCTION CALL ================= */

functionCall
    : primaryVar '(' aParams ')'
      {
        $$ = createNode(NODE_FUNC_CALL);
        $$->name = $1->name; // Function name
        $$->left = $3;       // Params
      }
    ;

/* ================= PARAMETERS ================= */

fParams
    : ID ':' type arraySize_ZM fParamsTail_ZM
      {
         $$ = createNode(NODE_PARAM);
         $$->name = $1;
         $$->type_name = $3->name;
         $$->left = $4; // Array size info
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
        $$ = createNode(NODE_LIST); // Wrapper for arg list
        $$->left = $1;
        $$->next = $2; 
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

/* ================= TYPES & OPS ================= */

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

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, s);
}

int main(void) {
    if (yyparse() == 0) {
        printf("Parsing Successful.\n");
        
        // ---------------------------------------------------------
        // PHASE 1: FRONTEND VISUALIZATION (From Old)
        // ---------------------------------------------------------
        // Helpful to ensure the Tree is built correctly before processing
        ast_print(rootNode, "ast.txt");
        
        // ---------------------------------------------------------
        // PHASE 2: SEMANTICS & MEMORY LAYOUT (Shared)
        // ---------------------------------------------------------
        // Builds symbol table, checks errors, calculates Stack Offsets
        semantic_analysis(rootNode); 
        
        // ---------------------------------------------------------
        // PHASE 3: SYMBOL TABLE DUMP (From Old)
        // ---------------------------------------------------------
        // CRITICAL: Check this file to see if 'Offset' and 'Size' 
        // columns are correct before looking at the Assembly.
        sym_print("symbol_table.txt");
        
        // ---------------------------------------------------------
        // PHASE 4: CODE GENERATION (From New)
        // ---------------------------------------------------------
        // Only generate code if we passed semantic checks.
        // (Assuming semantic_analysis sets a global error flag, 
        //  but for now we run it regardless).
        
        FILE *asmFile = fopen("output.asm", "w");
        if (!asmFile) {
            perror("Failed to open output.asm");
            return 1;
        }

        code_gen_init(asmFile);
        
        // Traverse top-level nodes (Classes, Funcs, etc.)
        ASTNode *curr = rootNode;
        while(curr) {
            code_gen(curr, asmFile);
            curr = curr->next;
        }
        
        code_gen_finalize(asmFile);
        fclose(asmFile);
        
        printf("Compilation Complete. \n");
        printf(" - AST: ast.txt\n");
        printf(" - Symbols: symbol_table.txt\n");
        printf(" - Assembly: output.asm\n");
        
    } else {
        printf("Parsing Failed.\n");
    }
    return 0;
}