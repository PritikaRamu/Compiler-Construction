#ifndef _LEXERDEF_
#define _LEXERDEF_

#include <stdio.h>
#include "symbolTable.h"
#define BUFFER_SIZE (1 << 12)
//#define BUFFER_SIZE 

typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

#define COUNT_KEYWORDS 28
//#define BUFFER_SIZE 100

twinBuffer buffers;
//int line = 1;
//helps to keep track of the line number

bool file_end;
//tells us if we have reached the end of file

bool startedReading;
//tells us if we have started reading or not

char *lexemeBegin, *forward;
//two pointers for the twin buffer

int charCount;
//keeps track of the size of lexeme

bool readBuff1, readBuff2;
//tells us if both these buffers have been read or not

FILE *fp;

FILE* startLexer(char* testcaseFile);
FILE* getStream(FILE* fp);
tokenInfo getNextToken();
void removeComments(char* testcaseFile, char* cleanFile);
void reset();

#endif
