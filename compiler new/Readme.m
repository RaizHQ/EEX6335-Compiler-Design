% How to Compile & Run (Step-by-Step)

% From your project directory:



bison -d parser.yflex lexer.l
gcc -o mycompiler parser.tab.c lex.yy.c ast.c symbol_table.c code_gen.c


gcc lex.yy.c parser.tab.c ast.c symbol_table.c semantic_analysis.c code_gen.c -o compiler

% Run with input file:

./mycompiler < input.txt

% Bison Conflict Report
bison -v -Wcounterexamples parser.y


%% Plan for Integrating AST Generation
%% The integration requires modifications to three files:

Header (ast.h): Define the AST node structure and helper functions.

Implementation (ast.c): Implement the functions to create and manage nodes.

Parser (parser.y): Define the semantic value union and add semantic actions.

