#ifndef _LEXERDEF_
#define _LEXERDEF_

#include <stdio.h>
#include "stack.h"
#define BUFFER_SIZE (1 << 12)
//#define BUFFER_SIZE 

typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

#define COUNT_KEYWORDS 28
//#define BUFFER_SIZE 100

FILE* startLexer(char* testcaseFile);
FILE* getStream(FILE* fp);
tokenInfo getNextToken();
void removeComments(char* testcaseFile, char* cleanFile);
void reset();

#endif
