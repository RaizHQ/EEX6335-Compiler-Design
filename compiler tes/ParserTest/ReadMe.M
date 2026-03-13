flex lexer.l
bison -d parser.y

gcc parser.tab.c lex.yy.c -o compiler
./compiler


