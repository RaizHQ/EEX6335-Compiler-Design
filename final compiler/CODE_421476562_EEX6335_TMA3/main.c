#include "compiler.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror(argv[1]);
        return 1;
    }

    /* --- Phase 1: Parsing --- */
    printf("--- Phase 1: Parsing ---\n");
    ASTNode* ast_root = parse_program();
    printf("Parsing complete.\n");

    /* Stop if parsing failed */
    if (get_error_count() > 0) {
        report_all_diagnostics();
        fprintf(stderr, "Compilation aborted due to syntax errors.\n");
        return 1;
    }

    /* --- Phase 2: Semantic Analysis --- */
    printf("--- Phase 2: Semantic Analysis ---\n");
    ScopeManager* manager = create_scope_manager();
    semantic_check(ast_root, manager);
    printf("Semantic check complete.\n");
    printf("--- Phase 2b: Dumping Symbol Table ---\n");
    print_symbol_table_to_file(manager, "symbol_table.txt");

    /* --- Phase 3: Error Reporting --- */
    printf("--- Phase 3: Reporting ---\n");
    report_all_diagnostics();

    /* --- Cleanup --- */
    free_ast(ast_root);
    destroy_scope_manager(manager);
    fclose(yyin);

    if (get_error_count() > 0) {
        fprintf(stderr, "Compilation failed.\n");
        return 1;
    }

    return 0;
}