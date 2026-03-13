#include <stdio.h>
#include <stdlib.h>
#include "semantic_printer.h"

static FILE* f_sem_err = NULL;

void initSemanticPrinter(const char* outputPath) {
    f_sem_err = fopen(outputPath, "w");
    if (!f_sem_err) { perror("Failed to open semantic error file"); exit(1); }
    fprintf(f_sem_err, "%-10s | %-40s\n", "Line", "Error Message");
    fprintf(f_sem_err, "-----------------------------------------------------\n");
}

void writeSemanticError(int lineNo, const char* msg) {
    if (f_sem_err) fprintf(f_sem_err, "%-10d | %s\n", lineNo, msg);
}

void closeSemanticPrinter() {
    if (f_sem_err) fclose(f_sem_err);
}