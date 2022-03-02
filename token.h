#ifndef _TOKEN_
#define _TOKEN_

#include "gterm.h"

typedef struct token {
	g_Term tid; 
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;

void printToken(int token);
void printNonTerminal(int token);

#endif