### Step 1: Compilation and Setup
Before testing, you must convert the .l file into an executable program.

Generate C Code (Flex): Use the Flex tool to process your lex.l file.

`Bash` 
flex lex.l

This generates the C source file, lex.yy.c, which contains the full DFA implementation and the yylex() function.

# Compile the Scanner: Compile the generated C file using a standard C compiler (like GCC). You must link with the Flex library (-lfl).

`Bash` 
gcc lex.yy.c -o scanner -lfl

This creates the executable scanner.

# Prepare Test Input Files: Create several text files (.txt or .in) containing the code examples outlined in the previous response. These will serve as the input to your scanner.

### Step 2: Execution and Verification
The main function provided in your lex.l file drives the testing by continuously calling yylex() and printing the results. The goal is to compare the printed output to the expected token stream.

# Test Case A: Core Functionality (Happy Path)
Goal: Verify that basic tokens, keywords, and identifiers are correctly recognized, and the Longest Match Rule is enforced.

`Input File: test_core.in`

integer a := 10;
float b = 3.14159E-2;
while (i <= 5 and not done) return;
Execution:

`Bash`
./scanner < test_core.in

* Verification Checklist:

* Keywords: integer, float, while, and, not, return must be recognized as their respective token codes (e.g., INTEGER, WHILE).

* Identifiers: a, b, i, done must be recognized as ID.

* Literals: 10 must be LITERAL_INTEGER; 3.14159E-2 must be LITERAL_FLOAT.

* Multi-Char Operators: :=, <=, and the reserved words and, not must be single tokens. The parser must not see : and = separately, nor < and =.

# Test Case B: Boundary and Conflict Resolution
Goal: Specifically test the DFA's tie-breaking rules, which are based on Rule Precedence (keywords before ID) and Longest Match.

`Input File: test_conflict.in`

float_var := 1.0.0;
if_then_else <= and_or

`Execution:`

`Bash`
./scanner < test_conflict.in

Verification Checklist:

* Keyword vs. ID: float_var must be recognized as a single ID, confirming the keyword rule for "float" was correctly skipped because the lexeme was longer.

* Float vs. Punctuation: 1.0 must be a single LITERAL_FLOAT, and the second . must be recognized as a separate . token.

* Keyword Combination: if_then_else must be a single ID, not a sequence of keywords.

* Operator Combination: and_or must be a single ID, not the AND and OR keywords.

# Test Case C: Context and Error Reporting
Goal: Verify correct handling of non-tokenizing elements (whitespace, comments) and robust error messages that include line numbers.

`Input File: test_error_line.in`

// Line 1: Start
class Test {
/* Line 3: Multi-line 
   Line 4: comment */
private @ := 10;
}
Execution:

`Bash`
./scanner < test_error_line.in

==> Verification Checklist:

* Comment Suppression: The lines containing // and /* ... */ must produce no tokens.

* Line Tracking: The lexical error for the illegal character @ must correctly report:

* Lexical Error: Unexpected character '@' at line 5
This verifies that the \n { yylineno++; } action is correctly executed.

* Error Recovery: After the @ error, the scanner must successfully tokenize the rest of line 5 (:= 10;) and line 6 (}). This confirms the recovery strategy of skipping the illegal character and continuing.

### Step 3: Formal Testing (The Compiler Architect's Recommendation)
For a production-quality compiler, manual verification is insufficient. You should implement Automated Regression Testing using a script or testing framework.

* Establish the Gold Standard: For each input file (test_X.in), create a corresponding "gold" output file (test_X.expected). This file contains the exact, correct token stream you expect.

* Script the Comparison: Write a shell script that runs your scanner for every input file and uses the diff utility to compare the actual output with the expected output.

`Bash`
./scanner < test_core.in > test_core.actual
diff test_core.actual test_core.expected


* Success/Failure: If diff reports no differences, the test case passes. If there are differences, the test fails, and you must debug the specific rule in your lex.l that caused the token mismatch, incorrect line number, or failure to ignore whitespace/comments.