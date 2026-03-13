/* lexer.l - Corrected for Recursive-Descent Parser */
%{
#include "compiler.h"

/* This global token struct is the "semantic value"
   that our hand-written parser will inspect. */
Token g_current_token; 

/* We use the built-in yylineno */
extern int yylineno;
int column = 1;

void update_token(TokenType type) {
    g_current_token.lexeme = strdup(yytext);
    g_current_token.line = yylineno;
    g_current_token.column = column;
    g_current_token.type = type;
    column += yyleng;
}

%}

letter   [a-zA-Z]
digit    [0-9]
nonzero  [1-9]
alphanum ({letter}|{digit}|_)
id       {letter}({alphanum})*
integer  (0|{nonzero}{digit}*)
fraction (\.({digit})*{nonzero}|\.0)
float    {integer}{fraction}([eE][+-]?{integer})?
comment  (\/\/.*|\/\*([^*]|\*+[^*/])*\*+\/)

%%

[ \t]+          { column += yyleng; /* ignore whitespace */ }
\n              { yylineno++; column = 1; }
{comment}       { /* ignore comments */ }


"class"         { update_token(TOK_CLASS); return TOK_CLASS; }
"isa"           { update_token(TOK_ISA); return TOK_ISA; }
"implement"     { update_token(TOK_IMPLEMENT); return TOK_IMPLEMENT; }
"public"        { update_token(TOK_PUBLIC); return TOK_PUBLIC; }
"private"       { update_token(TOK_PRIVATE); return TOK_PRIVATE; }
"func"          { update_token(TOK_FUNC); return TOK_FUNC; }
"constructor"   { update_token(TOK_CONSTRUCTOR); return TOK_CONSTRUCTOR; }
"attribute"     { update_token(TOK_ATTRIBUTE); return TOK_ATTRIBUTE; }
"local"         { update_token(TOK_LOCAL); return TOK_LOCAL; }
"if"            { update_token(TOK_IF); return TOK_IF; }
"then"          { update_token(TOK_THEN); return TOK_THEN; }
"else"          { update_token(TOK_ELSE); return TOK_ELSE; }
"while"         { update_token(TOK_WHILE); return TOK_WHILE; }
"read"          { update_token(TOK_READ); return TOK_READ; }
"write"         { update_token(TOK_WRITE); return TOK_WRITE; }
"return"        { update_token(TOK_RETURN); return TOK_RETURN; }
"not"           { update_token(TOK_NOT); return TOK_NOT; }
"self"          { update_token(TOK_SELF); return TOK_SELF; }
"integer"       { update_token(TOK_INTEGER); return TOK_INTEGER; }
"float"         { update_token(TOK_FLOAT); return TOK_FLOAT; }
"void"          { update_token(TOK_VOID); return TOK_VOID; }
"or"            { update_token(TOK_OR); return TOK_OR; }
"and"           { update_token(TOK_AND); return TOK_AND; }


"=>"            { update_token(TOK_ARROW); return TOK_ARROW; }
":="            { update_token(TOK_ASSIGN); return TOK_ASSIGN; }
"=="            { update_token(TOK_EQ); return TOK_EQ; }
"<>"            { update_token(TOK_NEQ); return TOK_NEQ; }
"<="            { update_token(TOK_LE); return TOK_LE; }
">="            { update_token(TOK_GE); return TOK_GE; }
"<"             { update_token(TOK_LT); return TOK_LT; }
">"             { update_token(TOK_GT); return TOK_GT; }
"+"             { update_token(TOK_PLUS); return TOK_PLUS; }
"-"             { update_token(TOK_MINUS); return TOK_MINUS; }
"*"             { update_token(TOK_MULTIPLY); return TOK_MULTIPLY; }
"/"             { update_token(TOK_DIVIDE); return TOK_DIVIDE; }


"{"             { update_token(TOK_LBRACE); return TOK_LBRACE; }
"}"             { update_token(TOK_RBRACE); return TOK_RBRACE; }
";"             { update_token(TOK_SEMI); return TOK_SEMI; }
","             { update_token(TOK_COMMA); return TOK_COMMA; }
"("             { update_token(TOK_LPAREN); return TOK_LPAREN; }
")"             { update_token(TOK_RPAREN); return TOK_RPAREN; }
":"             { update_token(TOK_COLON); return TOK_COLON; }
"."             { update_token(TOK_DOT); return TOK_DOT; }
"["             { update_token(TOK_LBRACKET); return TOK_LBRACKET; }
"]"             { update_token(TOK_RBRACKET); return TOK_RBRACKET; }


{float}         { update_token(TOK_FLOATLIT); return TOK_FLOATLIT; }
{integer}       { update_token(TOK_INTLIT); return TOK_INTLIT; }
{id}            { update_token(TOK_ID); return TOK_ID; }


.               {
                    char msg[100];
                    snprintf(msg, 99, "Illegal character '%s'", yytext);
                    record_error(yylineno, ERROR_LEXICAL, msg);
                    column++;
                    update_token(TOK_UNKNOWN);
                    return TOK_UNKNOWN;
                }
%%

int yywrap() { return 1; }