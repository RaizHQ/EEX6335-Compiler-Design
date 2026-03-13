%{
// 1. Declarations and Option Settings Section


#include "parser.tab.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

extern int yylineno;


int chars = 0;
int words = 0;
int lines = 0;

%}

%option yylineno



id          {letter}({alphanum})*   
letter      [a-zA-Z]
alphanum    ({letter}|{digit}|_)
digit       [0-9]

integer     (0|{nonzero}{digit}*)
nonzero     [1-9]

float       {integer}{fraction}([eE][+-]?{integer})?
fraction    (\.({digit})*{nonzero}|\.0)

/* Start Condition for Block Comments */
%x COMMENT_BLOCK




%%


"//".* { /* Ignore single line comments */ }

"/*"                    { BEGIN(COMMENT_BLOCK); }
<COMMENT_BLOCK>"*/"     { BEGIN(INITIAL); }
<COMMENT_BLOCK>\n       { /* Track lines inside comments implicitly via %option */ }
<COMMENT_BLOCK>.        { /* Ignore content inside comments */ }

[ \t\r\n]+              { /* Skip all whitespace */ }



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


{id}                    { 
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




"("                 { return L_PAREN; }
")"                 { return R_PAREN; }
"{"                 { return L_BRACE; }
"}"                 { return R_BRACE; }
"["                 { return L_BRACKET; }
"]"                 { return R_BRACKET; }
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

int yywrap() {
    return 1;
}