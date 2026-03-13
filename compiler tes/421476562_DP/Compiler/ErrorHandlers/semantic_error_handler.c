#include <stdio.h>
#include <stdlib.h>
#include "../Printer/semantic_printer.h"
#include "semantic_error_handler.h"

static int semErrorCount = 0;

void initSemanticErrorHandler(const char* outputPath) {
    initSemanticPrinter(outputPath);
    semErrorCount = 0;
}

void handleSemanticError(int lineNo, const char* msg) {
    semErrorCount++;
    fprintf(stderr, "[Semantic Error] Line %d: %s\n", lineNo, msg); /* Terminal */
    writeSemanticError(lineNo, msg); /* File */
}

int finalizeSemanticErrorHandler() {
    closeSemanticPrinter();
    return semErrorCount;
}