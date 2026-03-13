#ifndef SYNTAX_ERROR_HANDLER_H
#define SYNTAX_ERROR_HANDLER_H

void initSyntaxErrorHandler(const char* outputPath);
void handleSyntaxError(int lineNo, const char* errorMsg, const char* currentToken);
int finalizeSyntaxErrorHandler();

#endif