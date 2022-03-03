// #ifndef _LEXER_
// #define _LEXER_

#include "lexerDef.h"

int hashcode(char* key);
g_Term getToken(char* key);
void insertID(char* key, g_Term token);
void initTable();


FILE* startLexer(char* testcaseFile);
FILE* getStream(FILE* fp);
tokenInfo getNextToken();
void removeComments(char* testcaseFile, char* cleanFile);
void reset();

// #endif
