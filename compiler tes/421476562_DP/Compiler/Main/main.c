#include <stdio.h>
#include <stdlib.h>
#include "../ErrorHandlers/lexical_error_handler.h"
#include "../ErrorHandlers/syntax_error_handler.h"
#include "../ErrorHandlers/semantic_error_handler.h" /* NEW */
#include "../Printer/symbol_table_printer.h"
#include "symbol_table.h"
#include "ast.h"
#include "../Printer/ast_printer.h"

extern int yyparse();
extern FILE* yyin;

/* DEFINE rootNode here (Main owns the memory) */
ASTNode* rootNode = NULL;

int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) { perror("Error opening file"); return 1; }
    } else {
        printf("Using stdin.\n");
    }

    /* 1. INITIALIZE ALL UTILITIES */
    printf("Initializing Compiler Modules...\n");
    
    // I/O & Handlers
    initLexicalErrorHandler("../Outputs/lexer_Output.txt", "../ErrorOutputs/lexical_errors.txt");
    initSyntaxErrorHandler("../ErrorOutputs/syntax_errors.txt");
    initSemanticErrorHandler("../ErrorOutputs/semantic_errors.txt"); /* NEW */
    
    // Symbol Table System
    initSymbolTablePrinter("../Outputs/symbol_table.txt"); /* NEW: Setup I/O first */
    initSymbolTable(); /* NEW: Setup Logic second */
    
    // AST System
    initASTPrinter("../Outputs/ast_Output.txt");

    printf("--- Compilation Started ---\n");

    /* 2. RUN PARSER */
    int parseResult = yyparse();

    /* 3. CHECK ERRORS */
    int lexErrors = finalizeLexicalErrorHandler();
    int synErrors = finalizeSyntaxErrorHandler();
    int semErrors = finalizeSemanticErrorHandler(); /* NEW */

    /* 4. LOGIC: ONLY PROCEED IF CODE IS CLEAN */
    if (parseResult == 0 && lexErrors == 0 && synErrors == 0 && semErrors == 0) {
        printf("\n--- Compilation Successful ---\n");
        
        /* Generate Outputs */
        if (rootNode != NULL) {
            printf("Generating AST Graph...\n");
            printAST(rootNode); 
        } else {
            printf("Warning: Parse successful but AST is empty.\n");
        }

        printf("Generating Symbol Table Snapshot...\n");
        printSymbolTableSnapshot();

    } else {
        fprintf(stderr, "\n--- Compilation Failed ---\n");
        fprintf(stderr, "Lexical Errors:  %d\n", lexErrors);
        fprintf(stderr, "Syntax Errors:   %d\n", synErrors);
        fprintf(stderr, "Semantic Errors: %d\n", semErrors);
        
        closeASTPrinter();
        closeSymbolTablePrinter();
        return 1;
    }

    /* 5. CLEANUP */
    closeASTPrinter();
    closeSymbolTablePrinter();
    return 0;
}