#ifndef LEXER_PRINT_H
#define LEXER_PRINT_H

#include <stdio.h>
#include "lexer.l"  // Include the lexer header for token definitions


void yyrestart(FILE *input_file);

//Helper function to get token name from token code

const char* getTokenName(int token) {
    switch (token) {
        // Standard Bison Tokens
        case 0:              return "EOF";
        case 256:            return "ERROR";
        case 257:            return "INVALID_TOKEN";

        // Keywords
        case CLASS:          return "CLASS";
        case IMPLEMENT:      return "IMPLEMENT";
        case FUNC:           return "FUNC";
        case ATTRIBUTE:      return "ATTRIBUTE";
        case PUBLIC:         return "PUBLIC";
        case PRIVATE:        return "PRIVATE";
        case LOCAL:          return "LOCAL";
        case IF:             return "IF";
        case THEN:           return "THEN";
        case ELSE:           return "ELSE";
        case WHILE:          return "WHILE";
        case READ:           return "READ";
        case WRITE:          return "WRITE";
        case RETURN:         return "RETURN";
        case ISA:            return "ISA";
        case VOID:           return "VOID";
        case SELF:           return "SELF";
        case CONSTRUCT:      return "CONSTRUCT";

        // Types
        case INTEGER:        return "INTEGER";
        case FLOAT:          return "FLOAT";

        // Data Containers
        case ID:             return "ID";
        case LITERAL_INTEGER:return "LITERAL_INTEGER";
        case LITERAL_FLOAT:  return "LITERAL_FLOAT";

        // Operators
        case OR:             return "OR";
        case AND:            return "AND";
        case NOT:            return "NOT";
        case ARROW_OP:       return "ARROW_OP";
        case ASSIGN_OP:      return "ASSIGN_OP";
        case EQ_OP:          return "EQ_OP";
        case NE_OP:          return "NE_OP";
        case LE_OP:          return "LE_OP";
        case GE_OP:          return "GE_OP";
        case UMINUS:         return "UMINUS";

        
        default:             return "OTHER";
    }
}


void print_lex_tokens(char *filename) {
    FILE *outFile = fopen(filename, "w");
    if (!outFile) {
        perror("Could not open lexer output file");
        return;
    }

    fprintf(outFile, "LEXICAL ANALYSIS REPORT\n");
    fprintf(outFile, "=======================\n");

    int token;
    // We must reset line number for the debug pass
    yylineno = 1; 

    while ((token = yylex())) {
        // Create a temporary buffer for single-char tokens so we can use %s consistently
        char symbolStr[2] = { (char)token, '\0' };

        if (token < 256) {
            // Single character tokens (e.g., '+', '{', ';')
            fprintf(outFile, "Line: %-4d  Token: \t %-20s  Lexeme: \t%s\n", 
                    yylineno, symbolStr, symbolStr);
        } else {
            // Named tokens (e.g., WHILE, ID, NUMBER)
            // yytext holds the actual string matched
            fprintf(outFile, "Line: %-4d  Token: \t %-20s  Lexeme: \t%s\n", 
                    yylineno, getTokenName(token), yytext);
        }
    }
    
    fprintf(outFile, "\nLexical Analysis Complete.\n");
    fclose(outFile);

    // CRITICAL: Reset the file stream so the Parser can read it from the start!
    rewind(yyin);
    yyrestart(yyin);
    yylineno = 1; // Reset line counter for the parser
}


#endif