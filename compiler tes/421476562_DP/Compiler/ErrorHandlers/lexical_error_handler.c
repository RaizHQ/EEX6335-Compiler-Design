#include <stdio.h>
#include <stdlib.h>
#include "../Printer/lexer_printer.h" /* Dependency Injection */
#include "lexical_error_handler.h"

static int lexicalErrorCount = 0;

void initLexicalErrorHandler(const char* outputTokenPath, const char* outputErrorPath) {
    /* The handler delegates the I/O setup to the printer */
    initLexerOutput(outputTokenPath, outputErrorPath);
    lexicalErrorCount = 0;
}

void handleLexicalError(int lineNo, const char* invalidText) {
    lexicalErrorCount++;
    
    // Logic: Print to terminal so the user sees it immediately
    fprintf(stderr, "[Lexical Error] Line %d: Invalid Token '%s'\n", lineNo, invalidText);
    
    // Logic: Send to file via Printer
    printLexerError(lineNo, invalidText);
}

int finalizeLexicalErrorHandler() {
    finalizeLexerOutput();
    return lexicalErrorCount;
}