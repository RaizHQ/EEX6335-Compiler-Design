#include <stdio.h>
#include <stdlib.h>
#include "lexer_printer.h"

static FILE* f_out = NULL;
static FILE* f_err = NULL;
static int errorCount = 0;

void initLexerOutput(const char* outputPath, const char* errorPath) {
    f_out = fopen(outputPath, "w");
    if (!f_out) {
        perror("Failed to open lexer output file");
        exit(1);
    }
    
    f_err = fopen(errorPath, "w");
    if (!f_err) {
        perror("Failed to open lexer error file");
        fclose(f_out);
        exit(1);
    }

    /* Print Table Header */
    fprintf(f_out, "%-10s | %-20s | %s\n", "Line", "Token", "Lexeme");
    fprintf(f_out, "---------------------------------------------------\n");
}

void printLexerToken(int lineNo, const char* tokenName, const char* lexeme, int uid) {
    if (f_out) {
        /* Added UID column */
        fprintf(f_out, "%-5d | %-20s | %-20s | %d\n", lineNo, tokenName, lexeme, uid);
    }
}

void printLexerError(int lineNo, const char* errorText) {
    errorCount++;

    /* 1. Print to File */
    if (f_err) {
        fprintf(f_err, "Line %d: Unexpected character '%s'\n", lineNo, errorText);
    }
}

int finalizeLexerOutput() {
    if (f_out) fclose(f_out);
    if (f_err) fclose(f_err);
    return errorCount;
}