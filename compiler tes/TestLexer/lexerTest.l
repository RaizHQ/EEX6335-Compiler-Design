%{
// 1. Declarations and Option Settings Section
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


"//".* { /* Ignore single line comments */printf( "%s Single Line Comment\n" , yytext); }

"/*"                    { BEGIN(COMMENT_BLOCK); }
<COMMENT_BLOCK>"*/"     { BEGIN(INITIAL); }
<COMMENT_BLOCK>\n       printf( "%s New Line\n" , yytext);
<COMMENT_BLOCK>.        { printf( "%s IGNORE\n" , yytext); }



[ \t\r\n]+              { /* Skip all whitespace */ }



"=>"                { printf( "%s ARROW_OP\n" , yytext);  }
":="                { printf( "%s ASSIGN_OP\n" , yytext); }
"=="                { printf( "%s EQ_OP\n" , yytext); }
"<>"                { printf( "%s NEQ_OP\n" , yytext); }
"<="                { printf( "%s LE_OP\n" , yytext);  }
">="                { printf( "%s GE_OP\n" , yytext);  }
"or"                { printf( "%s OR_OP\n" , yytext);  }
"and"               { printf( "%s AND_OP\n" , yytext);  }
"not"               { printf( "%s NOT_OP\n" , yytext);  }

"else"              { printf( "%s ELSE_OP\n" , yytext);  }
"float"             { printf( "%s FLOAT_TYPE\n" , yytext);  }
"func"              { printf( "%s FUNC\n" , yytext); }
"if"                { printf( "%s IF\n" , yytext); }
"implement"         { printf( "%s IMPLEMENT\n" , yytext); }
"class"             { printf( "%s CLASS\n" , yytext); }
"attribute"         { printf( "%s ATTRIBUTE\n" , yytext); }
"integer"           { printf( "%s INTEGER_TYPE\n" , yytext); }
"isa"               { printf( "%s ISA\n" , yytext); }
"private"           { printf( "%s PRIVATE\n" , yytext); }
"public"            { printf( "%s PUBLIC\n" , yytext); }
"read"              { printf( "%s READ\n" , yytext); }
"return"            { printf( "%s RETURN\n" , yytext); }
"self"              { printf( "%s SELF\n" , yytext); }
"construct"         { printf( "%s CONSTRUCT\n" , yytext); }
"then"              { printf( "%s THEN\n" , yytext); }
"local"             { printf( "%s LOCAL\n" , yytext); }
"void"              { printf( "%s VOID\n" , yytext); }
"while"             { printf( "%s WHILE\n" , yytext); }
"write"             { printf( "%s WRITE\n" , yytext); }

{id}                  { 
                         
                            printf( "%s ID\n" , yytext); 
                        }


{integer}               {
                            printf( "%s INTEGER\n" , yytext); 
                        }
                   

{float}                 { 
                            printf( "%s FLOAT\n" , yytext) ; 
                        }



"("                 { printf( "%s L_BRACE\n" , yytext) ; }
")"                 { printf( "%s R_BRACE\n" , yytext) ; }
"{"                 { printf( "%s L_CURLY\n" , yytext) ; }
"}"                 { printf( "%s R_CURLY\n" , yytext) ; }
"["                 { printf( "%s L_SQUARE\n" , yytext) ; }
"]"                 { printf( "%s R_SQUARE\n" , yytext) ; }
";"                 { printf( "%s SEMICOLON\n" , yytext) ; }
","                 { printf( "%s COMMA\n" , yytext) ; }
"."                 { printf( "%s DOT\n" , yytext) ; }
":"                 { printf( "%s COLON\n" , yytext) ; }
"+"                 { printf( "%s PLUS\n" , yytext) ; }
"-"                 { printf( "%s MINUS\n" , yytext) ; }
"*"                 { printf( "%s MULTIPLY\n" , yytext) ; }
"/"                 { printf( "%s DIVIDE\n" , yytext) ; }
"<"                 { printf( "%s LESS_THAN\n" , yytext) ; }
">"                 { printf( "%s GREATER_THAN\n" , yytext) ; }

.                   {fprintf(stderr, "Lexical Error: Unexpected character '%s' at line %d\n", yytext, yylineno);}
%%

int main(void) {
    yylex();
}
