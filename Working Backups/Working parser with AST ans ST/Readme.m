% How to Compile & Run (Step-by-Step)

% From your project directory:


bison -d parser.y
flex lexer.l
gcc -o compiler parser.tab.c lex.yy.c ast.c symbol_table.c


% Run with input file:

./compiler < input.src

% Bison Conflict Report
bison -v -Wcounterexamples parser.y


%% Plan for Integrating AST Generation
%% The integration requires modifications to three files:

Header (ast.h): Define the AST node structure and helper functions.

Implementation (ast.c): Implement the functions to create and manage nodes.

Parser (parser.y): Define the semantic value union and add semantic actions.

