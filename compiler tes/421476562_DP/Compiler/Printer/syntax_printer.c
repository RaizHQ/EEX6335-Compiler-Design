#include <stdio.h>
#include <stdlib.h>
#include "syntax_printer.h"

static FILE* f_syn_err = NULL;

void initSyntaxPrinter(const char* outputPath) {
    f_syn_err = fopen(outputPath, "w");
    if (!f_syn_err) {
        perror("Failed to open syntax error output file");
        exit(1);
    }
    fprintf(f_syn_err, "%-10s | %-30s | %s\n", "Line", "Error Message", "Offending Token");
    fprintf(f_syn_err, "---------------------------------------------------------------\n");
}

void writeSyntaxError(int lineNo, const char* errorMsg, const char* offendingToken) {
    if (f_syn_err) {
        
        fprintf(f_syn_err, "%-10d | %-30s | %s\n", lineNo, errorMsg, offendingToken);
    }
}

void finalizeSyntaxPrinter() {
    if (f_syn_err) fclose(f_syn_err);
}