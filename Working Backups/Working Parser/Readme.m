% How to Compile & Run (Step-by-Step)

% From your project directory:


bison -d parser.y
flex lexer.l
gcc -o parser parser.tab.c lex.yy.c -lfl


% Run with input file:

./parser < test1.txt

% Bison Conflict Report
bison -v -Wcounterexamples parser.y


