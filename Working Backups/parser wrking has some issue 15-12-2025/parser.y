%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);

// Function for lexical analysis (provided by flex)
extern int yylex();

// >>> ADD THIS LINE:
extern int yylineno; 



%}

/* ========= TOKENS ========= */

%token CLASS IMPLEMENT FUNC ATTRIBUTE
%token PUBLIC PRIVATE LOCAL
%token IF THEN ELSE WHILE
%token READ WRITE RETURN
%token ISA
%token INTEGER FLOAT VOID
%token SELF
%token CONSTRUCT

%token ID
%token LITERAL_INTEGER
%token LITERAL_FLOAT

%token OR AND NOT
%token ARROW_OP ASSIGN_OP
%token EQ_OP NE_OP LE_OP GE_OP

/* ========= PRECEDENCE (expressions only) ========= */

%left OR
%left AND
%left EQ_OP NE_OP '<' '>' LE_OP GE_OP
%left '+' '-'
%left '*' '/'
%right NOT
%right UMINUS

/* ========= START ========= */

%start prog

%%

/* ================= PROGRAM ================= */

prog
    : classOrImplOrFunc_ZM
    ;

classOrImplOrFunc_ZM
    : classOrImplOrFunc classOrImplOrFunc_ZM
    | /* ε */
    ;

classOrImplOrFunc
    : classDecl
    | implDef
    | funcDef
    ;

/* ================= CLASS ================= */

classDecl
    : CLASS ID isa_Id_ZO '{' visibility_memberDecl_ZM '}' ';'
    ;

isa_Id_ZO
    : ISA ID commaID_ZM
    | /* ε */
    ;

commaID_ZM
    : ',' ID commaID_ZM
    | /* ε */
    ;

visibility_memberDecl_ZM
    : visibility memberDecl visibility_memberDecl_ZM
    | /* ε */
    ;

/* ================= IMPLEMENT ================= */

implDef
    : IMPLEMENT ID '{' funcDef_ZM '}'
    ;

funcDef_ZM
    : funcDef funcDef_ZM
    | /* ε */
    ;

/* ================= FUNCTIONS ================= */

funcDef
    : funcHead funcBody
    ;

funcHead
    : FUNC ID '(' fParams ')' ARROW_OP returnType
    | CONSTRUCT '(' fParams ')'
    ;

funcBody
    : '{' varDeclOrStmt_ZM '}'
    ;

visibility
    : PUBLIC
    | PRIVATE
    ;

memberDecl
    : funcDecl
    | attributeDecl
    ;

funcDecl
    : funcHead ';'
    ;

/* ================= VARIABLES ================= */

attributeDecl
    : ATTRIBUTE varDecl
    ;

localVarDecl
    : LOCAL varDecl
    ;

varDecl
    : ID ':' type arraySize_ZM ';'
    ;

arraySize_ZM
    : arraySize arraySize_ZM
    | /* ε */
    ;

arraySize
    : '[' LITERAL_INTEGER ']'
    | '[' ']'
    ;

/* ================= STATEMENTS ================= */

varDeclOrStmt_ZM
    : varDeclOrStmt varDeclOrStmt_ZM
    | /* ε */
    ;

varDeclOrStmt
    : localVarDecl
    | statement
    ;

statement
    : matchedStmt
    | unmatchedStmt
    ;

matchedStmt
    : variable assignOp expr ';'
    | functionCall ';'
    | IF '(' relExpr ')' THEN matchedStmt ELSE matchedStmt ';'
    | WHILE '(' relExpr ')' matchedStmt ';'
    | READ '(' variable ')' ';'
    | WRITE '(' expr ')' ';'
    | RETURN '(' expr ')' ';'
    | '{' statement_ZM '}'
    ;

unmatchedStmt
    : IF '(' relExpr ')' THEN statement
    | IF '(' relExpr ')' THEN matchedStmt ELSE unmatchedStmt ';'
    | WHILE '(' relExpr ')' unmatchedStmt ';'
    ;

statement_ZM
    : statement statement_ZM
    | /* ε */
    ;

/* ================= EXPRESSIONS ================= */

expr
    : relExpr
    ;

relExpr
    : arithExpr relOp arithExpr
    | arithExpr
    ;

arithExpr
    : arithExpr addOp term
    | term
    ;

term
    : term multOp factor
    | factor
    ;

factor
    : variable
    | functionCall
    | LITERAL_INTEGER
    | LITERAL_FLOAT
    | '(' arithExpr ')'
    | NOT factor
    | '-' factor %prec UMINUS
    | '+' factor %prec UMINUS
    ;

/* ================= VARIABLE / ID NEST ================= */

variable
    : primaryVar suffix_ZM
    ;

primaryVar
    : idOrSelf
    ;

idOrSelf
    : ID
    | SELF
    ;

suffix_ZM
    : suffix suffix_ZM
    | indice_ZM
    ;

suffix
    : indice_ZM '.' idOrSelf
    | '(' aParams ')' '.' idOrSelf
    ;

indice_ZM
    : indice indice_ZM
    | /* ε */
    ;

indice
    : '[' arithExpr ']'
    ;

/* ================= FUNCTION CALL ================= */

functionCall
    : primaryVar '(' aParams ')'
    ;

/* ================= PARAMETERS ================= */

fParams
    : ID ':' type arraySize_ZM fParamsTail_ZM
    | /* ε */
    ;

fParamsTail_ZM
    : fParamsTail fParamsTail_ZM
    | /* ε */
    ;

fParamsTail
    : ',' ID ':' type arraySize_ZM
    ;

aParams
    : expr aParamsTail_ZM
    | /* ε */
    ;

aParamsTail_ZM
    : aParamsTail aParamsTail_ZM
    | /* ε */
    ;

aParamsTail
    : ',' expr
    ;

/* ================= TYPES & OPS ================= */

type
    : INTEGER
    | FLOAT
    | ID
    ;

returnType
    : type
    | VOID
    ;

assignOp
    : ASSIGN_OP
    ;

relOp
    : EQ_OP | '<' | '>' | NE_OP | LE_OP | GE_OP
    ;

addOp
    : '+' | '-' | OR
    ;

multOp
    : '*' | '/' | AND
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, s);
}

int main(void) {
    printf("Starting Syntax Analysis...\n");
    if (yyparse() == 0)
        printf("Parsing completed successfully.\n");
    return 0;
}