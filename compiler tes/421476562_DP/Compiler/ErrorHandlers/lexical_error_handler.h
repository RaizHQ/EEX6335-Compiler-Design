#ifndef LEXICAL_ERROR_HANDLER_H
#define LEXICAL_ERROR_HANDLER_H

/* Sets up the printer */
void initLexicalErrorHandler(const char* outputTokenPath, const char* outputErrorPath);

/* Called by Lexer when a pattern fails */
void handleLexicalError(int lineNo, const char* invalidText);

/* Returns total errors found */
int finalizeLexicalErrorHandler();

#endif