%{
// 1. Declarations and Option Settings Section

#include "ast.h"
#include "parser.tab.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

extern int yylineno;


int chars = 0;
int words = 0;
int lines = 0;

%}



id          {letter}({alphanum})*   
letter      [a-zA-Z]
alphanum    ({letter}|{digit}|_)
digit       [0-9]

integer     (0|{nonzero}{digit}*)
nonzero     [1-9]

float       {integer}{fraction}([eE][+-]?{integer})?
fraction    (\.({digit})*{nonzero}|\.0)

comment     (\/\/.*|\/\*([^*]|\*+[^*/])*\*+\/)



%%

// 2. List of Patterns and Actions Section

{comment}         {};



[ \t\r]+            ;
\n                  { yylineno++; } 



"=>"                { return ARROW_OP; }
":="                { return ASSIGN_OP; }
"=="                { return EQ_OP; }
"<>"                { return NE_OP; }
"<="                { return LE_OP; }
">="                { return GE_OP; }
"or"                { return OR; }
"and"               { return AND; }
"not"               { return NOT; }



"else"              { return ELSE; }
"float"             { return FLOAT; }
"func"              { return FUNC; }
"if"                { return IF; }
"implement"         { return IMPLEMENT; }
"class"             { return CLASS; }
"attribute"         { return ATTRIBUTE; }
"integer"           { return INTEGER; }
"isa"               { return ISA; }
"private"           { return PRIVATE; }
"public"            { return PUBLIC; }
"read"              { return READ; }
"return"            { return RETURN; }
"self"              { return SELF; }
"construct"         { return CONSTRUCT; }
"then"              { return THEN; }
"local"             { return LOCAL; }
"void"              { return VOID; }
"while"             { return WHILE; }
"write"             { return WRITE; }


{id}                  { 
                            yylval.sval = strdup(yytext); 
                            return ID; 
                        }

{integer}               {
                            yylval.ival = atoi(yytext); 
                            return LITERAL_INTEGER; 
                        }
                   

{float}                 { 
                            yylval.fval = atof(yytext); 
                            return LITERAL_FLOAT; 
                        }




"("                 { return L_BRACE; }
")"                 { return R_BRACE; }
"{"                 { return L_CURLY; }
"}"                 { return R_CURLY; }
"["                 { return L_SQUARE; }
"]"                 { return R_SQUARE; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"."                 { return DOT; }
":"                 { return COLON; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULT; }
"/"                 { return DIV; }
"<"                 { return LT; }
">"                 { return GT; }


.                   {fprintf(stderr, "Lexical Error: Unexpected character '%s' at line %d\n", yytext, yylineno);}
%%



