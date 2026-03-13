%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int yylineno = 1;

void yyerror(const char *s);
%}

LETTER      [a-zA-Z]
DIGIT       [0-9]
NONZERO     [1-9]
ALPHANUM    ({LETTER}|{DIGIT}|_)

ID_PATTERN  {LETTER}{ALPHANUM}*
INTEGER     (0|{NONZERO}{DIGIT}*)
FRACTION    (\.[0-9]+)
FLOAT       {INTEGER}{FRACTION}([eE][+-]?{INTEGER})?

COMMENT     (\/\/.*|\/\*([^*]|\*+[^*/])*\*+\/)

%%

[ \t]+          { /* skip whitespace */ }
\n              { yylineno++; }

{COMMENT}       { /* skip */ }

"("     { return '('; }
")"     { return ')'; }
"{"     { return '{'; }
"}"     { return '}'; }
"["     { return '['; }
"]"     { return ']'; }
";"     { return ';'; }
","     { return ','; }
"."     { return '.'; }
":"     { return ':'; }

"=>"    { return TOK_ARROW; }
":="    { return TOK_ASSIGN; }
"=="    { return TOK_EQ; }
"<>"    { return TOK_NE; }
"<="    { return TOK_LE; }
">="    { return TOK_GE; }
"+"     { return '+'; }
"-"     { return '-'; }
"*"     { return '*'; }
"/"     { return '/'; }
"<"     { return '<'; }
">"     { return '>'; }

"or"    { return TOK_OR; }
"and"   { return TOK_AND; }
"not"   { return TOK_NOT; }

"else"      { return TOK_ELSE; }
"float"     { return TOK_FLOAT; }
"func"      { return TOK_FUNC; }
"if"        { return TOK_IF; }
"implement" { return TOK_IMPLEMENT; }
"class"     { return TOK_CLASS; }
"attribute" { return TOK_ATTRIBUTE; }
"integer"   { return TOK_INTEGER; }
"isa"       { return TOK_ISA; }
"private"   { return TOK_PRIVATE; }
"public"    { return TOK_PUBLIC; }
"read"      { return TOK_READ; }
"return"    { return TOK_RETURN; }
"self"      { return TOK_SELF; }
"construct" { return TOK_CONSTRUCT; }
"then"      { return TOK_THEN; }
"local"     { return TOK_LOCAL; }
"void"      { return TOK_VOID; }
"while"     { return TOK_WHILE; }
"write"     { return TOK_WRITE; }

{FLOAT} {
    yylval.f_val = atof(yytext);
    return TOK_FLOATLIT;
}

{INTEGER} {
    yylval.i_val = atoi(yytext);
    return TOK_INTLIT;
}

{ID_PATTERN} {
    yylval.str_val = strdup(yytext);
    return TOK_ID;
}

. {
    fprintf(stderr,
        "Lexical Error at line %d: Unknown character '%c'\n",
        yylineno, yytext[0]);
    return TOK_ERROR;
}

%%

int yywrap() { return 1; }

int main() { return yylex(); }
