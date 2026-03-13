%{
/* C Code Section: Declarations and Definitions */

// The following integer codes are passed to the parser (yyparse)
// They are high-value to distinguish them from single-character tokens (ASCII values).
// NOTE: For a real project, this would be imported from a shared header file (e.g., "y.tab.h").

#define OR 257
#define AND 258
#define NOT 259
#define ELSE 260
#define FLOAT 261
#define FUNC 262
#define IF 263
#define IMPLEMENT 264
#define CLASS 265
#define ATTRIBUTE 266
#define INTEGER 267
#define ISA 268
#define PRIVATE 269
#define PUBLIC 270
#define READ 271
#define RETURN 272
#define SELF 273
#define CONSTRUCT 274
#define THEN 275
#define LOCAL 276
#define VOID 277
#define WHILE 278
#define WRITE 279
#define ID 280
#define LITERAL_INTEGER 281
#define LITERAL_FLOAT 282
#define ARROW_OP 283  // =>
#define ASSIGN_OP 284 // :=
#define NE_OP 285     // <>
#define LE_OP 286     // <=
#define GE_OP 287     // >=
#define EQ_OP 288     // ==

// Global variable for tracking line numbers (crucial for error reporting)
extern int yylineno;

// Standard library includes for string operations (e.g., strdup, atoi, atof)
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

// NOTE: You would typically define the yylval union here for semantic value transfer.
// For simplicity, we are returning the token code directly.
%}

/* * Regular Definitions Section 
 * These are substituted into the Rules section below.
 */
letter      [a-zA-Z]
digit       [0-9]
nonzero     [1-9]
alphanum    ({letter}|{digit}|_)
integer     (0|{nonzero}{digit}*)
fraction    (\.({digit})*{nonzero}|\.0)
float       {integer}{fraction}([eE][+-]?{integer})?
comment     (\/\/.*|\/\*([^*]|\*+[^*/])*\*+\/)



%%

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



{float}             { return LITERAL_FLOAT;}
{integer}           { return LITERAL_INTEGER; }

{letter}({alphanum})* {return ID; }


"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"["                 { return '['; }
"]"                 { return ']'; }
";"                 { return ';'; }
","                 { return ','; }
"."                 { return '.'; }
":"                 { return ':'; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"*"                 { return '*'; }
"/"                 { return '/'; }
"<"                 { return '<'; }
">"                 { return '>'; }


.                   {fprintf(stderr, "Lexical Error: Unexpected character '%s' at line %d\n", yytext, yylineno);}
%%

/* * Auxiliary Functions Section 
 * This is where you would define the main() function to drive the scanner 
 * and any required error handling functions.
 */

int yywrap() {
    return 1;
}

/* Example main function to test the scanner in isolation */
int main(int argc, char **argv) {
    int token;
    
    printf("Starting Lexical Analysis...\n");
    while ((token = yylex())) {
        if (token <= 256) {
            // Single character token (Punctuation/Simple Op)
            printf("Line %d: Token: %c (ASCII %d)\n", yylineno, (char)token, token);
        } else {
            // Multi-character token (Keyword, ID, Literal, Multi-char Op)
            printf("Line %d: Token: %d (%s)\n", yylineno, token, yytext);
        }
    }
    printf("Lexical Analysis Complete.\n");
    return 0;
}