%{
#include "ast.h"
#include "parser.tab.h"   

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




{integer}   { yylval.ival = atoi(yytext); return LITERAL_INTEGER; }
{float}     { yylval.fval = atof(yytext); return LITERAL_FLOAT; }

{letter}({alphanum})* { 
    yylval.sval = strdup(yytext); 
    return ID; 
}


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

void yyrestart(FILE *input_file);

//Helper function to get token name from token code

const char* getTokenName(int token) {
    switch (token) {
        // Standard Bison Tokens
        case 0:              return "EOF";
        case 256:            return "ERROR";
        case 257:            return "INVALID_TOKEN";

        // Keywords
        case CLASS:          return "CLASS";
        case IMPLEMENT:      return "IMPLEMENT";
        case FUNC:           return "FUNC";
        case ATTRIBUTE:      return "ATTRIBUTE";
        case PUBLIC:         return "PUBLIC";
        case PRIVATE:        return "PRIVATE";
        case LOCAL:          return "LOCAL";
        case IF:             return "IF";
        case THEN:           return "THEN";
        case ELSE:           return "ELSE";
        case WHILE:          return "WHILE";
        case READ:           return "READ";
        case WRITE:          return "WRITE";
        case RETURN:         return "RETURN";
        case ISA:            return "ISA";
        case VOID:           return "VOID";
        case SELF:           return "SELF";
        case CONSTRUCT:      return "CONSTRUCT";

        // Types
        case INTEGER:        return "INTEGER";
        case FLOAT:          return "FLOAT";

        // Data Containers
        case ID:             return "ID";
        case LITERAL_INTEGER:return "LITERAL_INTEGER";
        case LITERAL_FLOAT:  return "LITERAL_FLOAT";

        // Operators
        case OR:             return "OR";
        case AND:            return "AND";
        case NOT:            return "NOT";
        case ARROW_OP:       return "ARROW_OP";
        case ASSIGN_OP:      return "ASSIGN_OP";
        case EQ_OP:          return "EQ_OP";
        case NE_OP:          return "NE_OP";
        case LE_OP:          return "LE_OP";
        case GE_OP:          return "GE_OP";
        case UMINUS:         return "UMINUS";

        
        default:             return "OTHER";
    }
}


void print_lex_tokens(char *filename) {
    FILE *outFile = fopen(filename, "w");
    if (!outFile) {
        perror("Could not open lexer output file");
        return;
    }

    fprintf(outFile, "LEXICAL ANALYSIS REPORT\n");
    fprintf(outFile, "=======================\n");

    int token;
    // We must reset line number for the debug pass
    yylineno = 1; 

    while ((token = yylex())) {
        // Create a temporary buffer for single-char tokens so we can use %s consistently
        char symbolStr[2] = { (char)token, '\0' };

        if (token < 256) {
            // Single character tokens (e.g., '+', '{', ';')
            fprintf(outFile, "Line: %-4d  Token: \t %-20s  Lexeme: \t%s\n", 
                    yylineno, symbolStr, symbolStr);
        } else {
            // Named tokens (e.g., WHILE, ID, NUMBER)
            // yytext holds the actual string matched
            fprintf(outFile, "Line: %-4d  Token: \t %-20s  Lexeme: \t%s\n", 
                    yylineno, getTokenName(token), yytext);
        }
    }
    
    fprintf(outFile, "\nLexical Analysis Complete.\n");
    fclose(outFile);

    // CRITICAL: Reset the file stream so the Parser can read it from the start!
    rewind(yyin);
    yyrestart(yyin);
    yylineno = 1; // Reset line counter for the parser
}

