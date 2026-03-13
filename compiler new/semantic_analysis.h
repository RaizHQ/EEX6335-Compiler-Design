#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include "ast.h"

/* =========================================================================
   COMPILER ARCHITECT: SEMANTIC ANALYZER INTERFACE
   =========================================================================
   This module is responsible for:
   1. Traversing the AST.
   2. Populating the Symbol Table.
   3. Type Checking (Assignments, Return types).
   4. Scope Resolution.
   ========================================================================= */

// Main Entry Point
void semantic_analysis(ASTNode *root);

#endif