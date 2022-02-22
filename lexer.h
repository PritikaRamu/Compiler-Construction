#include <stdio.h>
#define BUFFER_SIZE (1 << 12)

typedef struct token {
	char* tid; // String for now, later convert it to an enumeration with all Tokens
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;

typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

FILE* getStream(FILE* fp);

tokenInfo getNextToken(twinBuffer B);

void removeComments(char* testcaseFile, char* cleanFile);