%{
/* ========================================================================== */
/* 1. Declarations Section                                                    */
/* ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* AST and Semantic Headers */
#include "ast.h"
#include "symbol_table.h"
#include "../ErrorHandlers/syntax_error_handler.h"
#include "../ErrorHandlers/semantic_error_handler.h"

/* Link to Lexer */
extern int yylex();
extern int yylineno;
extern char* yytext;

/* Global Root Node (Owned by main.c) */
extern ASTNode* rootNode;

/* Error Handling Wrapper */
void yyerror(const char *s);

/* ========================================================================== */
/* Semantic Helpers                                                           */
/* ========================================================================== */

DataType getNodeType(ASTNode* node) {
    if (!node) return TYPE_UNKNOWN;

    switch (node->type) {
        case NODE_INT:      return TYPE_INT;
        case NODE_FLOAT:    return TYPE_FLOAT;
        case NODE_VAR:      return lookupType(node->metaData); /* Symbol Table Lookup */
        case NODE_OP:       
            /* Simplified: Assume arithmetic results in INT unless operands are FLOAT. */
            return TYPE_INT; 
        default:            return TYPE_UNKNOWN;
    }
}
%}

/* ========================================================================== */
/* 2. Bison Declarations Section                                              */
/* ========================================================================== */

%union {
    int     ival;       /* For LITERAL_INTEGER */
    float   fval;       /* For LITERAL_FLOAT */
    char   *sval;       /* For ID */
    struct ASTNode *node; /* For AST Construction */
}

/* --- Token Classes --- */
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

/* --- Punctuation --- */
%token L_PAREN R_PAREN      
%token L_BRACE R_BRACE      
%token L_BRACKET R_BRACKET   
%token SEMICOLON COMMA DOT COLON

/* --- Precedence & Associativity --- */
%left OR
%left AND
%right NOT
%nonassoc EQ_OP NE_OP LT GT LE_OP GE_OP
%left PLUS MINUS
%left MULT DIV
%left DOT L_BRACKET R_BRACKET

/* --- Non-Terminal Types (All return AST Nodes) --- */
%type <node> prog classOrImplOrFunc_ZM classOrImplOrFunc
%type <node> classDecl isa_Id_ZO commaID_ZM visibility_memberDecl_ZM
%type <node> implDef funcDef_ZM funcDef funcHead funcBody
%type <node> visibility memberDecl funcDecl attributeDecl
%type <node> localVarDecl varDecl arraySize_ZM arraySize
%type <node> varDeclOrStmt_ZM varDeclOrStmt statement matchedStmt unmatchedStmt statement_ZM
%type <node> expr relExpr arithExpr term factor
%type <node> variable primaryVar idOrSelf suffix_ZM suffix indice_ZM indice
%type <node> functionCall fParams fParamsTail_ZM fParamsTail aParams aParamsTail_ZM aParamsTail
%type <node> type returnType assignOp relOp addOp multOp sign

%%

/* ========================================================================== */
/* 3. Grammar Rules                                                           */
/* ========================================================================== */

/* ================= PROGRAM ================= */

prog
    : classOrImplOrFunc_ZM 
      { 
        rootNode = $1; /* Final assignment to the external root */
      }
    ;

classOrImplOrFunc_ZM
    : classOrImplOrFunc classOrImplOrFunc_ZM 
      { 
        $$ = $1; 
        addSibling($$, $2); 
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
    : CLASS ID isa_Id_ZO L_BRACE visibility_memberDecl_ZM R_BRACE SEMICOLON 
      {
          $$ = createNode(NODE_CLASS);
          $$->metaData = $2; 
      }
    ;
 
isa_Id_ZO
    : ISA ID commaID_ZM { $$ = NULL; }
    | /* ε */ { $$ = NULL; }
    ;

commaID_ZM
    : COMMA ID commaID_ZM { $$ = NULL; }
    | /* ε */   { $$ = NULL; }
    ;

visibility_memberDecl_ZM
    : visibility memberDecl visibility_memberDecl_ZM { $$ = NULL; }
    | /* ε */ { $$ = NULL; }
    ;

/* ================= IMPLEMENT ================= */

implDef
    : IMPLEMENT ID L_BRACE funcDef_ZM R_BRACE  
      {
          $$ = createNode(NODE_CLASS); 
          $$->metaData = $2;
      }
    ;

funcDef_ZM
    : funcDef funcDef_ZM 
      {
          $$ = $1;
          addSibling($$, $2);
      }
    | /* ε */ { $$ = NULL; }
    ;

/* ================= FUNCTIONS ================= */

funcDef
    : funcHead funcBody 
      {
          $$ = createNode(NODE_FUNC);
          $$->left = $1;  /* Head */
          $$->right = $2; /* Body */

          /* SEMANTICS: Exit Scope & Reset Return Type */
          exitScope(); 
          setGlobalReturnType(TYPE_UNKNOWN);
      }
    ;

funcHead
    : FUNC ID L_PAREN fParams R_PAREN ARROW_OP returnType 
      {
          DataType rType = TYPE_VOID;
          if ($7 && $7->metaData) {
              if (strcmp($7->metaData, "integer") == 0) rType = TYPE_INT;
              else if (strcmp($7->metaData, "float") == 0) rType = TYPE_FLOAT;
          }

          /* 1. Insert Function Name (Global) */
          insertSymbol($2, rType, "FUNC");

          /* 2. Enter Scope (For Body) */
          enterScope($2);

          /* 3. Set Context */
          setGlobalReturnType(rType);

          $$ = createIDNode($2); 
      }
    | CONSTRUCT L_PAREN fParams R_PAREN 
      { 
          /* Constructors often imply a scope change too, treating simpler here */
          setGlobalReturnType(TYPE_VOID);
          enterScope("construct");
          $$ = createIDNode("construct"); 
      }
    ;

funcBody
    : L_BRACE varDeclOrStmt_ZM R_BRACE 
      { 
          $$ = createNode(NODE_BLOCK);
          $$->left = $2; 
      }
    ;

visibility
    : PUBLIC  { $$ = NULL; }
    | PRIVATE { $$ = NULL; }
    ;

memberDecl
    : funcDecl { $$ = $1; }
    | attributeDecl  { $$ = $1; }
    ;

funcDecl
    : funcHead SEMICOLON { $$ = $1; }

/* ================= VARIABLES ================= */

attributeDecl
    : ATTRIBUTE varDecl { $$ = $2; }
    ;

localVarDecl
    : LOCAL varDecl { $$ = $2; }
    ;

varDecl
    : ID COLON type arraySize_ZM SEMICOLON 
    {
        DataType vType = TYPE_UNKNOWN;
        if ($3 && $3->metaData) {
             if (strcmp($3->metaData, "integer") == 0) vType = TYPE_INT;
             else if (strcmp($3->metaData, "float") == 0) vType = TYPE_FLOAT;
        }
        insertSymbol($1, vType, "VAR");

        $$ = createNode(NODE_VAR_DECL);
        $$->metaData = $1; 
        $$->left = $3;     
    }
    ;

arraySize_ZM
    : arraySize arraySize_ZM { $$ = NULL; }
    | /* ε */ { $$ = NULL; }
    ;

arraySize
    : L_BRACKET LITERAL_INTEGER R_BRACKET { $$ = NULL; }
    | L_BRACKET R_BRACKET { $$ = NULL; }
    ;

/* ================= STATEMENTS ================= */

varDeclOrStmt_ZM
    : varDeclOrStmt varDeclOrStmt_ZM  
      {
          $$ = $1;
          addSibling($$, $2);
      }
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
    : variable assignOp expr SEMICOLON 
      { 
          $$ = createNode(NODE_ASSIGN);
          $$->left = $1; 
          $$->right = $3; 
      }
    | functionCall SEMICOLON 
      { 
          $$ = createNode(NODE_FUNC_CALL);
          $$->left = $1;
      }
    | IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE matchedStmt SEMICOLON 
      { 
           $$ = createNode(NODE_IF);
           $$->left = $3; 
           $$->right = $6; 
      }
    | WHILE L_PAREN relExpr R_PAREN matchedStmt SEMICOLON 
      { 
           $$ = createNode(NODE_WHILE);
           $$->left = $3;
           $$->right = $5;
      }
    | READ L_PAREN variable R_PAREN SEMICOLON 
      { 
           $$ = createNode(NODE_READ);
           $$->left = $3;
      }
    | WRITE L_PAREN expr R_PAREN SEMICOLON 
      { 
           $$ = createNode(NODE_WRITE);
           $$->left = $3;
      }
    | RETURN L_PAREN expr R_PAREN SEMICOLON 
      { 
           /* SEMANTICS: RETURN TYPE CHECK */
           DataType exprType = getNodeType($3);
           
           if (currentFunctionReturnType != TYPE_UNKNOWN && exprType != TYPE_UNKNOWN) {
               if (currentFunctionReturnType != exprType) {
                   char msg[128];
                   sprintf(msg, "Return type mismatch. Expected type %d, got %d", currentFunctionReturnType, exprType);
                   handleSemanticError(yylineno, msg);
               }
           }

           $$ = createNode(NODE_RETURN);
           $$->left = $3;
      }
    | L_BRACE statement_ZM R_BRACE 
      { 
           $$ = createNode(NODE_BLOCK);
           $$->left = $2;
      }
    ;

unmatchedStmt
    : IF L_PAREN relExpr R_PAREN THEN statement 
      { 
           $$ = createNode(NODE_IF);
           $$->left = $3;
           $$->right = $6;
      }
    | IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE unmatchedStmt SEMICOLON 
      { 
           $$ = createNode(NODE_IF);
           $$->left = $3;
           $$->right = $6;
      }
    | WHILE L_PAREN relExpr R_PAREN unmatchedStmt SEMICOLON 
      { 
           $$ = createNode(NODE_WHILE);
           $$->left = $3;
           $$->right = $5;
      }
    ;

statement_ZM
    : statement statement_ZM 
      {
          $$ = $1;
          addSibling($$, $2);
      }
    | /* ε */ { $$ = NULL; }
    ;

/* ================= EXPRESSIONS ================= */

expr
    : relExpr { $$ = $1; }
    ;

relExpr
    : arithExpr relOp arithExpr 
      { 
          $$ = createOpNode("REL_OP", $1, $3); 
      }
    | arithExpr { $$ = $1; }
    ;

arithExpr
    : arithExpr addOp term  
      { 
          $$ = createOpNode($2->metaData, $1, $3); 
      }
    | term  { $$ = $1; }
    ;

sign 
    : PLUS  { $$ = createOpNode("+", NULL, NULL); }
    | MINUS { $$ = createOpNode("-", NULL, NULL); }
    ;

term
    : term multOp factor 
      { 
          $$ = createOpNode($2->metaData, $1, $3); 
      }
    | factor { $$ = $1; }
    ;

factor
    : variable { $$ = $1; }
    | functionCall { $$ = $1; }
    | LITERAL_INTEGER { $$ = createIntNode($1); }
    | LITERAL_FLOAT  { $$ = createFloatNode($1); }
    | L_PAREN arithExpr R_PAREN { $$ = $2; }
    | NOT factor { $$ = createOpNode("NOT", $2, NULL); }
    | sign factor 
      { 
          /* SEMANTICS: SIGN RECURSION CHECK (Max 2) */
          if ($2->type == NODE_OP && ($2->metaData[0] == '+' || $2->metaData[0] == '-')) {
              ASTNode* child = $2->left; 
              if (child && child->type == NODE_OP && (child->metaData[0] == '+' || child->metaData[0] == '-')) {
                   handleSemanticError(yylineno, "Too many consecutive signs (max 2 allowed)");
              }
          }
          $$ = createOpNode($1->metaData, $2, NULL); 
      }
    ;

/* ================= VARIABLE / ID NEST ================= */

variable
    : primaryVar suffix_ZM  
      { 
          $$ = $1; 
      }
    ;

primaryVar
    : idOrSelf { $$ = $1; }
    ;

idOrSelf
    : ID   { $$ = createIDNode($1); }
    | SELF { $$ = createIDNode("self"); }
    ;

suffix_ZM
    : suffix suffix_ZM { $$ = NULL; }
    | indice_ZM { $$ = NULL; }
    ;

suffix
    : indice_ZM DOT idOrSelf { $$ = NULL; }
    | L_PAREN aParams R_PAREN DOT idOrSelf { $$ = NULL; }
    ;

indice_ZM
    : indice indice_ZM { $$ = NULL; }
    | /* ε */ { $$ = NULL; }
    ;

indice
    : L_BRACKET arithExpr R_BRACKET { $$ = NULL; }
    ;

/* ================= FUNCTION CALL ================= */

functionCall
    : primaryVar L_PAREN aParams R_PAREN 
      {
          $$ = createNode(NODE_FUNC_CALL);
          $$->left = $1; 
          $$->right = $3; 
      }
    ;

/* ================= PARAMETERS ================= */

fParams
    : ID COLON type arraySize_ZM fParamsTail_ZM 
      {
          DataType pType = TYPE_UNKNOWN;
          if ($3 && $3->metaData) {
               if (strcmp($3->metaData, "integer") == 0) pType = TYPE_INT;
               else if (strcmp($3->metaData, "float") == 0) pType = TYPE_FLOAT;
          }
          insertSymbol($1, pType, "PARAM");
          $$ = NULL; 
      }
    | /* ε */ { $$ = NULL; }
    ;

fParamsTail_ZM
    : fParamsTail fParamsTail_ZM  { $$ = NULL; }
    | /* ε */ { $$ = NULL; }
    ;

fParamsTail
    : COMMA ID COLON type arraySize_ZM 
      {
          DataType pType = TYPE_UNKNOWN;
          if ($4 && $4->metaData) {
               if (strcmp($4->metaData, "integer") == 0) pType = TYPE_INT;
               else if (strcmp($4->metaData, "float") == 0) pType = TYPE_FLOAT;
          }
          insertSymbol($2, pType, "PARAM");
          $$ = NULL;
      }
    ;

aParams
    : expr aParamsTail_ZM  
      { 
          $$ = $1; 
          addSibling($$, $2);
      }
    | /* ε */ { $$ = NULL; }
    ;

aParamsTail_ZM
    : aParamsTail aParamsTail_ZM  
      { 
          $$ = $1;
          addSibling($$, $2);
      }
    | /* ε */  { $$ = NULL; }
    ;

aParamsTail
    : COMMA expr { $$ = $2; }
    ;

/* ================= TYPES & OPS ================= */

type
    : INTEGER { $$ = createIDNode("integer"); }
    | FLOAT  { $$ = createIDNode("float"); }
    | ID      { $$ = createIDNode($1); }
    ;

returnType
    : type { $$ = $1; }
    | VOID { $$ = createIDNode("void"); }
    ;

assignOp
    : ASSIGN_OP { $$ = NULL; }
    ;

relOp
    : EQ_OP { $$ = NULL; }
    | LT   { $$ = NULL; }
    | GT   { $$ = NULL; }
    | NE_OP { $$ = NULL; }
    | LE_OP { $$ = NULL; }
    | GE_OP { $$ = NULL; }
    ;

addOp
    : sign { $$ = $1; }
    | OR  { $$ = createOpNode("OR", NULL, NULL); }
    ;

multOp
    : MULT { $$ = createOpNode("*", NULL, NULL); } 
    | DIV { $$ = createOpNode("/", NULL, NULL); }
    | AND { $$ = createOpNode("AND", NULL, NULL); }
    ;

%%

void yyerror(const char *s) {
    handleSyntaxError(yylineno, s, yytext);
}