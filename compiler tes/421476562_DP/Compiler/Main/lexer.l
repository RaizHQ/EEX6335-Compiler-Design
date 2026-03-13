%{
#include "parser.tab.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "../Printer/lexer_printer.h"
#include "../ErrorHandlers/lexical_error_handler.h"
#include "id_registry.h"

extern int yylineno;

/* Macro to Log Token and Return to Parser */
#define LOG_AND_RETURN(tokenName, tokenType) \
    int uid = getOrCreateID(yytext); \
    printLexerToken(yylineno, tokenName, yytext, uid); \
    return tokenType;
%}

%option yylineno
%option noinput
%option nounput

/* Regex Definitions */
id          {letter}({alphanum})* 
letter      [a-zA-Z]
alphanum    ({letter}|{digit}|_)
digit       [0-9]

integer     (0|{nonzero}{digit}*)
nonzero     [1-9]

float       {integer}{fraction}([eE][+-]?{integer})?
fraction    (\.({digit})*{nonzero}|\.0)

%x COMMENT_BLOCK

%%

"//".* { /* Ignore single line comments */ }

"/*"                    { BEGIN(COMMENT_BLOCK); }
<COMMENT_BLOCK>"*/"     { BEGIN(INITIAL); }
<COMMENT_BLOCK>\n       { }
<COMMENT_BLOCK>.        { }

[ \t\r\n]+              { /* Skip whitespace */ }

"=>"                { LOG_AND_RETURN("ARROW_OP", ARROW_OP); }
":="                { LOG_AND_RETURN("ASSIGN_OP", ASSIGN_OP); }
"=="                { LOG_AND_RETURN("EQ_OP", EQ_OP); }
"<>"                { LOG_AND_RETURN("NE_OP", NE_OP); }
"<="                { LOG_AND_RETURN("LE_OP", LE_OP); }
">="                { LOG_AND_RETURN("GE_OP", GE_OP); }
"or"                { LOG_AND_RETURN("OR", OR); }
"and"               { LOG_AND_RETURN("AND", AND); }
"not"               { LOG_AND_RETURN("NOT", NOT); }

"else"              { LOG_AND_RETURN("ELSE", ELSE); }
"float"             { LOG_AND_RETURN("FLOAT", FLOAT); }
"func"              { LOG_AND_RETURN("FUNC", FUNC); }
"if"                { LOG_AND_RETURN("IF", IF); }
"implement"         { LOG_AND_RETURN("IMPLEMENT", IMPLEMENT); }
"class"             { LOG_AND_RETURN("CLASS", CLASS); }
"attribute"         { LOG_AND_RETURN("ATTRIBUTE", ATTRIBUTE); }
"integer"           { LOG_AND_RETURN("INTEGER", INTEGER); }
"isa"               { LOG_AND_RETURN("ISA", ISA); }
"private"           { LOG_AND_RETURN("PRIVATE", PRIVATE); }
"public"            { LOG_AND_RETURN("PUBLIC", PUBLIC); }
"read"              { LOG_AND_RETURN("READ", READ); }
"return"            { LOG_AND_RETURN("RETURN", RETURN); }
"self"              { LOG_AND_RETURN("SELF", SELF); }
"construct"         { LOG_AND_RETURN("CONSTRUCT", CONSTRUCT); }
"then"              { LOG_AND_RETURN("THEN", THEN); }
"local"             { LOG_AND_RETURN("LOCAL", LOCAL); }
"void"              { LOG_AND_RETURN("VOID", VOID); }
"while"             { LOG_AND_RETURN("WHILE", WHILE); }
"write"             { LOG_AND_RETURN("WRITE", WRITE); }

{id}                    { 
                           yylval.sval = strdup(yytext); 
                            int uid = getOrCreateID(yytext);
                            printLexerToken(yylineno, "ID", yytext, uid);
                            return ID;
                        }

{integer}               {
                            yylval.ival = atoi(yytext); 
                            printLexerToken(yylineno, "LITERAL_INTEGER", yytext, 0);
                            return LITERAL_INTEGER; 
                        }

{float}                 { 
                            yylval.fval = atof(yytext); 
                            printLexerToken(yylineno, "LITERAL_FLOAT", yytext, 0);
                            return LITERAL_FLOAT; 
                        }

"("                 { LOG_AND_RETURN("L_PAREN", L_PAREN); }
")"                 { LOG_AND_RETURN("R_PAREN", R_PAREN); }
"{"                 { LOG_AND_RETURN("L_BRACE", L_BRACE); }
"}"                 { LOG_AND_RETURN("R_BRACE", R_BRACE); }
"["                 { LOG_AND_RETURN("L_BRACKET", L_BRACKET); }
"]"                 { LOG_AND_RETURN("R_BRACKET", R_BRACKET); }
";"                 { LOG_AND_RETURN("SEMICOLON", SEMICOLON); }
","                 { LOG_AND_RETURN("COMMA", COMMA); }
"."                 { LOG_AND_RETURN("DOT", DOT); }
":"                 { LOG_AND_RETURN("COLON", COLON); }
"+"                 { LOG_AND_RETURN("PLUS", PLUS); }
"-"                 { LOG_AND_RETURN("MINUS", MINUS); }
"*"                 { LOG_AND_RETURN("MULT", MULT); }
"/"                 { LOG_AND_RETURN("DIV", DIV); }
"<"                 { LOG_AND_RETURN("LT", LT); }
">"                 { LOG_AND_RETURN("GT", GT); }

.                   {
                        
                        handleLexicalError(yylineno, yytext);
                    }
%%

int yywrap() {
    return 1;
}