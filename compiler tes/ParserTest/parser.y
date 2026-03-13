%{

/* 1. Declarations Section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Link to the Lexer */
extern int yylex();
extern int yylineno;
extern char* yytext;

int funcCount = 0;
int classCount = 0;
int implCount = 0;

void yyerror(const char *s);
%}

/* 2. Bison Declarations Section */

/* The Union: connects Lexer's yylval to the Parser */
%union {
    int     ival;       /* For LITERAL_INTEGER */
    float   fval;       /* For LITERAL_FLOAT */
    char   *sval;       /* For ID */
    void   *node;       /* For AST Nodes */
}

/* --- Token Classes  --- */
%token <sval> ID
%token <ival> LITERAL_INTEGER
%token <fval> LITERAL_FLOAT

/* --- Keywords --- */

%token CLASS IMPLEMENT ISA
%token PUBLIC PRIVATE LOCAL
%token FUNC CONSTRUCT
%token ATTRIBUTE 
%token IF THEN ELSE WHILE
%token READ WRITE RETURN
%token NOT OR AND
%token SELF
%token INTEGER FLOAT VOID

/* --- Operators --- */
%token ARROW_OP    /* => */
%token ASSIGN_OP   /* := */
%token EQ_OP       /* == */
%token NE_OP       /* <> */
%token LE_OP       /* <= */
%token GE_OP       /* >= */
%token LT          /* <  */
%token GT          /* >  */
%token PLUS MINUS MULT DIV

/* --- Punctuation  --- */
%token L_PAREN R_PAREN      
%token L_BRACE R_BRACE      
%token L_BRACKET R_BRACKET   
%token SEMICOLON COMMA DOT COLON

/* PRECEDENCE & ASSOCIATIVITY
/* This table handles the logic of "arithExpr", "term", "factor" automatically */

%left OR
%left AND
%right NOT
%nonassoc EQ_OP NE_OP LT GT LE_OP GE_OP
%left PLUS MINUS
%left MULT DIV
%left DOT L_BRACKET  R_BRACKET



/* Non-terminal types - CRITICAL: Everything returns a node */
/*
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
*/
%%

/* ================= PROGRAM ================= */

prog
    : classOrImplOrFunc_ZM { printf("prog -> classOrImplOrFunc_ZM \n");}
    ;

classOrImplOrFunc_ZM
    : classOrImplOrFunc classOrImplOrFunc_ZM { printf("classOrImplOrFunc_ZM -> classOrImplOrFunc classOrImplOrFunc_ZM \n"); }
    | /* ε */ { printf("classOrImplOrFunc_ZM -> ε \n"); }
    ;

classOrImplOrFunc
    : classDecl { printf("Content: %s ==> classOrImplOrFunc -> classDecl \n", yytext); }
    | implDef { printf("Content: %s ==> classOrImplOrFunc -> implDef \n", yytext); }  
    | funcDef  { printf("Content: %s ==> classOrImplOrFunc -> funcDef \n"); } 
    ;

/* ================= CLASS ================= */

classDecl
    : CLASS ID isa_Id_ZO L_BRACE visibility_memberDecl_ZM R_BRACE SEMICOLON { printf("classDecl -> CLASS ID isa_Id_ZO L_BRACE visibility_memberDecl_ZM R_BRACE SEMICOLON \n"); }
    ;
 
isa_Id_ZO
    : ISA ID commaID_ZM { printf("isa_Id_ZO -> ISA ID commaID_ZM \n"); }
    | /* ε */ { printf("isa_Id_ZO -> ε \n"); }
    ;

commaID_ZM
    : COMMA ID commaID_ZM { printf("commaID_ZM -> COMMA ID commaID_ZM \n"); }
    | /* ε */   { printf("commaID_ZM -> ε \n"); }
    ;

visibility_memberDecl_ZM
    : visibility memberDecl visibility_memberDecl_ZM { printf("visibility_memberDecl_ZM -> visibility memberDecl visibility_memberDecl_ZM \n"); }
    | /* ε */ { printf("visibility_memberDecl_ZM -> ε \n"); }
    ;

/* ================= IMPLEMENT ================= */

implDef
    : IMPLEMENT ID L_BRACE funcDef_ZM R_BRACE  { printf("implDef -> IMPLEMENT ID L_BRACE funcDef_ZM R_BRACE \n");}
    ;

funcDef_ZM
    : funcDef funcDef_ZM { printf("funcDef_ZM -> funcDef funcDef_ZM \n"); }
    | /* ε */ { printf("funcDef_ZM -> ε \n "); }
    ;

/* ================= FUNCTIONS ================= */

funcDef
    : funcHead funcBody { printf("funcDef -> funcHead funcBody \n"); }
    ;

funcHead
    : FUNC ID L_PAREN fParams R_PAREN ARROW_OP returnType { printf("funcHead -> FUNC ID L_PAREN fParams R_PAREN ARROW_OP returnType \n"); }
    | CONSTRUCT L_PAREN fParams R_PAREN { printf("funcHead -> CONSTRUCT L_PAREN fParams R_PAREN \n"); }
    ;

funcBody
    : L_BRACE varDeclOrStmt_ZM R_BRACE { printf("funcBody -> L_BRACE varDeclOrStmt_ZM R_BRACE \n"); }
    ;
visibility
    : PUBLIC  { printf("visibility -> PUBLIC \n"); }
    | PRIVATE { printf("visibility -> PRIVATE \n"); }
    ;

memberDecl
    : funcDecl { printf("memberDecl -> funcDecl \n"); }
    | attributeDecl  { printf("memberDecl -> attributeDecl \n"); }
    ;

funcDecl
    : funcHead SEMICOLON { printf("funcDecl -> funcHead SEMICOLON \n"); }

/* ================= VARIABLES ================= */

attributeDecl
    : ATTRIBUTE varDecl { printf("attributeDecl -> ATTRIBUTE varDecl \n"); }
    ;

localVarDecl
    : LOCAL varDecl { printf("localVarDecl -> LOCAL varDecl \n"); }
    ;

varDecl
    : ID COLON type arraySize_ZM SEMICOLON { printf("varDecl -> ID COLON type arraySize_ZM SEMICOLON \n"); }
    ;

arraySize_ZM
    : arraySize arraySize_ZM { printf("arraySize_ZM -> arraySize arraySize_ZM \n"); }
    | /* ε */ { printf("arraySize_ZM -> ε \n  "); }
    ;

arraySize
    : L_BRACKET LITERAL_INTEGER R_BRACKET { printf("arraySize -> L_BRACKET LITERAL_INTEGER R_BRACKET \n"); }
    | L_BRACKET R_BRACKET { printf("arraySize -> L_BRACKET R_BRACKET \n"); }
    ;

/* ================= STATEMENTS ================= */

varDeclOrStmt_ZM
    : varDeclOrStmt varDeclOrStmt_ZM  { printf("varDeclOrStmt_ZM -> varDeclOrStmt varDeclOrStmt_ZM \n"); }
    | /* ε */ { printf("varDeclOrStmt_ZM -> ε \n"); }
    ;

varDeclOrStmt
    : localVarDecl { printf("varDeclOrStmt -> localVarDecl \n"); }
    | statement    { printf("varDeclOrStmt -> statement \n"); }
        
    ;

statement
    : matchedStmt { printf("statement -> matchedStmt \n"); }
    | unmatchedStmt { printf("statement -> unmatchedStmt \n"); }
    ;

matchedStmt
    : variable assignOp expr SEMICOLON { printf("matchedStmt -> variable assignOp expr SEMICOLON \n"); }
    | functionCall SEMICOLON { printf("matchedStmt -> functionCall SEMICOLON \n"); }
    | IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE matchedStmt SEMICOLON { printf("matchedStmt -> IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE matchedStmt SEMICOLON \n"); }
    | WHILE L_PAREN relExpr R_PAREN matchedStmt SEMICOLON { printf("matchedStmt -> WHILE L_PAREN relExpr R_PAREN matchedStmt SEMICOLON \n"); }
    | READ L_PAREN variable R_PAREN SEMICOLON { printf("matchedStmt -> READ L_PAREN variable R_PAREN SEMICOLON \n"); }
    | WRITE L_PAREN expr R_PAREN SEMICOLON { printf("matchedStmt -> WRITE L_PAREN expr R_PAREN SEMICOLON \n"); }
    | RETURN L_PAREN expr R_PAREN SEMICOLON { printf("matchedStmt -> RETURN L_PAREN expr R_PAREN SEMICOLON \n"); }
    | L_BRACE statement_ZM R_BRACE { printf("matchedStmt -> L_BRACE statement_ZM R_BRACE \n"); }
    ;

unmatchedStmt
    : IF L_PAREN relExpr R_PAREN THEN statement { printf("unmatchedStmt -> IF L_PAREN relExpr R_PAREN THEN statement \n"); }
    | IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE unmatchedStmt SEMICOLON { printf("unmatchedStmt -> IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE unmatchedStmt SEMICOLON \n"); }
    | WHILE L_PAREN relExpr R_PAREN unmatchedStmt SEMICOLON { printf("unmatchedStmt -> WHILE L_PAREN relExpr R_PAREN unmatchedStmt SEMICOLON \n"); }
    ;

statement_ZM
    : statement statement_ZM { printf("statement_ZM -> statement statement_ZM \n"); }
    | /* ε */ { printf("statement_ZM -> ε \n"); }
    ;

/* ================= EXPRESSIONS ================= */

expr
    : relExpr { printf("expr -> relExpr \n"); }
    ;

relExpr
    : arithExpr relOp arithExpr { printf("relExpr -> arithExpr relOp arithExpr \n"); }
    | arithExpr { printf("relExpr -> arithExpr \n"); }
    ;

arithExpr
    : arithExpr addOp term  { printf("arithExpr -> arithExpr addOp term \n"); }
    | term  {printf("arithExpr -> term \n"); }
    ;

sign 
    : PLUS | MINUS  {printf("sign -> PLUS | MINUS \n"); }
    ;

term
    : term multOp factor { printf("term -> term multOp factor \n"); }
    | factor { printf("term -> factor \n"); }
    ;

factor
    : variable {printf("factor -> variable \n"); }
    | functionCall {printf("factor -> functionCall \n"); }
    | LITERAL_INTEGER {printf("factor -> LITERAL_INTEGER \n"); }
    | LITERAL_FLOAT  {printf("factor -> LITERAL_FLOAT \n"); }
    | L_PAREN arithExpr R_PAREN {printf("factor -> L_PAREN arithExpr R_PAREN \n"); }
    | NOT factor {printf("factor -> NOT factor \n"); }
    | sign factor {printf("factor -> sign factor \n"); }
    ;

/* ================= VARIABLE / ID NEST ================= */
/* This is complex: variable -> primaryVar -> suffix_ZM */
/* We will flatten this to a node tree */

variable
    : primaryVar suffix_ZM  { printf("variable -> primaryVar suffix_ZM \n"); }
    ;

primaryVar
    : idOrSelf { printf("primaryVar -> idOrSelf \n"); }
    ;

idOrSelf
    : ID  { printf("idOrSelf -> ID \n"); } 
    | SELF { printf("idOrSelf -> SELF \n"); }
    ;

suffix_ZM
    : suffix suffix_ZM { printf("suffix_ZM -> suffix suffix_ZM \n"); }
    | indice_ZM { printf("suffix_ZM -> indice_ZM \n"); }
    ;

suffix
    : indice_ZM DOT idOrSelf { printf("suffix -> indice_ZM DOT idOrSelf \n"); }
    | L_PAREN aParams R_PAREN DOT idOrSelf { printf("suffix -> L_PAREN aParams R_PAREN DOT idOrSelf \n"); }
    ;

indice_ZM
    : indice indice_ZM { printf("indice_ZM -> indice indice_ZM \n"); }
    | /* ε */ { printf("indice_ZM -> ε \n"); }
    ;

indice
    : L_BRACKET arithExpr R_BRACKET { printf("indice -> L_BRACKET arithExpr R_BRACKET \n"); }
    ;

/* ================= FUNCTION CALL ================= */

functionCall
    : primaryVar L_PAREN aParams R_PAREN { printf("functionCall -> primaryVar L_PAREN aParams R_PAREN \n"); }
    ;

/* ================= PARAMETERS ================= */

fParams
    : ID COLON type arraySize_ZM fParamsTail_ZM { printf("fParams -> ID COLON type arraySize_ZM fParamsTail_ZM \n"); }
    | /* ε */ { printf("fParams -> ε \n"); }
    ;

fParamsTail_ZM
    : fParamsTail fParamsTail_ZM  { printf("fParamsTail_ZM -> fParamsTail fParamsTail_ZM \n"); }
    | /* ε */ { printf("fParamsTail_ZM -> ε \n"); }
    ;

fParamsTail
    : COMMA ID COLON type arraySize_ZM { printf("fParamsTail -> COMMA ID COLON type arraySize_ZM \n "); }
    ;

aParams
    : expr aParamsTail_ZM  { printf("aParams -> expr aParamsTail_ZM \n"); }
    | /* ε */ { printf("aParams -> ε \n"); }
    ;

aParamsTail_ZM
    : aParamsTail aParamsTail_ZM  { printf("aParamsTail_ZM -> aParamsTail aParamsTail_ZM \n"); }
    | /* ε */  { printf("aParamsTail_ZM -> ε \n"); }
    ;

aParamsTail
    : COMMA expr { printf("aParamsTail -> COMMA expr \n"); }
    ;

/* ================= TYPES & OPS ================= */

type
    : INTEGER { printf("type -> INTEGER \n"); }
    | FLOAT  { printf("type -> FLOAT \n"); }
    | ID      { printf("type -> ID\n"); }
    ;

returnType
    : type {printf("returnType -> type \n"); }
    | VOID {printf("returnType -> VOID \n"); }
    ;

assignOp
    : ASSIGN_OP {printf("assignOp -> ASSIGN_OP \n"); }
    ;

relOp
    : EQ_OP { printf("relOp -> EQ_OP \n"); }
    | LT   { printf("relOp -> LT \n"); }
    | GT   { printf("relOp -> GT \n"); }
    | NE_OP { printf("relOp -> NE_OP \n"); }
    | LE_OP { printf("relOp -> LE_OP \n"); }
    | GE_OP { printf("relOp -> GE_OP \n"); }
    ;

addOp
    : sign { printf("addOp -> sign \n"); }
    | OR  { printf("addOp -> OR \n"); }
    ;

multOp
    : MULT { printf("multOp -> MULT \n"); } 
    | DIV { printf("multOp -> DIV \n"); }
    | AND { printf("multOp -> AND \n"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s. Token: %s\n", yylineno, s, yytext);
}

int main(void) {
    return yyparse();
}