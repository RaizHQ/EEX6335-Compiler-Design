#ifndef SEMANTIC_ERROR_HANDLER_H
#define SEMANTIC_ERROR_HANDLER_H

void initSemanticErrorHandler(const char* outputPath);
void handleSemanticError(int lineNo, const char* msg);
int finalizeSemanticErrorHandler();

#endif      

