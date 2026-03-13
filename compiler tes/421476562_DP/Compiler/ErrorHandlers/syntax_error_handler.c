#include <stdio.h>
#include <stdlib.h>
#include "../Printer/syntax_printer.h"
#include "syntax_error_handler.h"

static int syntaxErrorCount = 0;

void initSyntaxErrorHandler(const char* outputPath) {
    initSyntaxPrinter(outputPath);
    syntaxErrorCount = 0;
}

void handleSyntaxError(int lineNo, const char* errorMsg, const char* currentToken) {
    syntaxErrorCount++;

    // Logic: Terminal Output
    fprintf(stderr, "[Syntax Error]  Line %d: %s (near '%s')\n", lineNo, errorMsg, currentToken);

    // Logic: File Output
    writeSyntaxError(lineNo, errorMsg, currentToken);
}

int finalizeSyntaxErrorHandler() {
    finalizeSyntaxPrinter();
    return syntaxErrorCount;
}