/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/

#ifndef _LEXER_
#define _LEXER_

#include "lexerDef.h"

int line;int NO_OF_SEMANTIC_ERRORS;
//helps to keep track of the line number

// LOOKUP TABLE Functions
int hashcode(char* key);
g_Term getToken(char* key);
void insertID(char* key, g_Term token);
void initTable();

// LEXER Functions
FILE* startLexer(char* testcaseFile);
FILE* getStream(FILE* fp);
tokenInfo getNextToken();
void removeComments(char* testcaseFile, char* cleanFile);
void reset();
void printCompressionPercentage();

#endif