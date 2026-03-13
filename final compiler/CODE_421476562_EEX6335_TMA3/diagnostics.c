#include "compiler.h"

typedef struct {
    int lineno;
    DiagnosticType type;
    char* message;
} DiagnosticMessage;

static DiagnosticMessage** g_error_list = NULL;
static int g_error_count = 0;
static int g_error_capacity = 0;

static int compare_diagnostics(const void* a, const void* b) {
    DiagnosticMessage* msgA = *(DiagnosticMessage**)a;
    DiagnosticMessage* msgB = *(DiagnosticMessage**)b;
    return (msgA->lineno - msgB->lineno);
}

void record_error(int lineno, DiagnosticType type, const char *fmt, ...) {
    if (g_error_count >= g_error_capacity) {
        g_error_capacity = (g_error_capacity == 0) ? 16 : g_error_capacity * 2;
        g_error_list = (DiagnosticMessage**) realloc(g_error_list, 
            g_error_capacity * sizeof(DiagnosticMessage*));
    }

    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    DiagnosticMessage* msg = (DiagnosticMessage*) malloc(sizeof(DiagnosticMessage));
    msg->lineno = lineno;
    msg->type = type;
    msg->message = strdup(buffer);
    
    g_error_list[g_error_count++] = msg;
}

void report_all_diagnostics(void) {
    if (g_error_count == 0) {
        printf("Compilation successful. No errors.\n");
        return;
    }
    
    qsort(g_error_list, g_error_count, sizeof(DiagnosticMessage*), compare_diagnostics);
    
    for (int i = 0; i < g_error_count; i++) {
        DiagnosticMessage* msg = g_error_list[i];
        const char* type_str = "Error";
        if (msg->type == ERROR_LEXICAL) type_str = "Lexical Error";
        if (msg->type == ERROR_SYNTAX) type_str = "Syntax Error";
        if (msg->type == ERROR_SEMANTIC) type_str = "Semantic Error";
        
        fprintf(stderr, "%s (line %d): %s\n", type_str, msg->lineno, msg->message);
        
        free(msg->message);
        free(msg);
    }
    
    free(g_error_list);
    g_error_list = NULL;
    g_error_count = 0;
    g_error_capacity = 0;
}

int get_error_count(void) {
    return g_error_count;
}