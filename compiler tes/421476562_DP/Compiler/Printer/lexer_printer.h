#ifndef LEXER_PRINTER_H
#define LEXER_PRINTER_H

/* Initialize output streams for tokens and errors */
void initLexerOutput(const char* outputPath, const char* errorPath);

/* Log a valid token to the output table */
void printLexerToken(int lineNo, const char* tokenName, const char* lexeme, int uid);

/* Log a lexical error and increment internal error count */
void printLexerError(int lineNo, const char* errorText);

/* Close streams and return total lexical error count */
int finalizeLexerOutput();

#endif