
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

void printToken(int token);
extern char getNextChar();
extern void reset();
extern void retract(int);

void initLexer(char *fileName) {
	line = 1;
	fptr = fopen(fileName, "r");
	assert(fptr != NULL);
	int size1 = fread(buff1, sizeof(char), BUFFER_SIZE, fptr);
	buff1[size1] = EOF;
	lexemeBegin = forward = buff1;
	read1 = read2 = true;
	lexemeSize = 0;
}

int main()
{
	initLexer("testcase.txt");

	//testing getNextChar
	/*
	char c;
	int i=0;
	while((c=getNextChar())!=EOF){
		fprintf(stderr, "%c",c );
		i++;
		if(i%11==0)
			retract(1);
		if(i%7==0)
			reset();
	}
	cleanup();
	*/

	// initLexer("testcase1.txt");

	token t;
	while(1) {
		t=getNextToken();
		if(t.tid==SENTINEL)
			break;
		printf("The Line is:%d The Token is: %d ",t.lineNo,t.tid);
		printToken(t.tid);

		if(t.lexeme!=NULL)
			printf("%s",t.lexeme);

		if(t.numVal!=NULL){
			if(t.tid==TK_NUM)
				printf("%d",*((int*)t.numVal));
			else
				printf("%lf",*((double*)t.numVal));
		}
		printf("\n");
	}
	removeComments()
	return 0;
}

void printToken(int token) {
	gterm base=eps;
	token -= base;
	switch(token)
	{
		case 1: printf("TK_ASSIGNOP ");
				break;
		case 2: printf("TK_COMMENT ");
				break;
		case 3: printf("TK_FIELDID ");
				break;
		case 4: printf("TK_ID ");
				break;
		case 5: printf("TK_NUM ");
				break;
		case 6: printf("TK_RNUM ");
				break;
		case 7: printf("TK_FUNID ");
				break;
		case 8: printf("TK_RECORDID ");
				break;
		case 9: printf("TK_WITH ");
				break;
		case 10: printf("TK_PARAMETERS ");
				break;
		case 11: printf("TK_END ");
				break;
		case 12: printf("TK_WHILE ");
				break;
		case 13: printf("TK_TYPE ");
				break;
		case 14: printf("TK_MAIN ");
				break;
		case 15: printf("TK_GLOBAL ");
				break;
		case 16: printf("TK_PARAMETER ");
				break;
		case 17: printf("TK_LIST ");
				break;
		case 18: printf("TK_SQL ");
				break;
		case 19: printf("TK_SQR ");
				break;
		case 20: printf("TK_INPUT ");
				break;
		case 21: printf("TK_OUTPUT ");
				break;
		case 22: printf("TK_INT ");
				break;
		case 23: printf("TK_REAL ");
				break;
		case 24: printf("TK_COMMA ");
				break;
		case 25: printf("TK_SEM ");
				break;
		case 26: printf("TK_COLON ");
				break;
		case 27: printf("TK_DOT ");
				break;
		case 28: printf("TK_ENDWHILE ");
				break;
		case 29: printf("TK_OPEN ");
				break;
		case 30: printf("TK_CLOSE ");
				break;
		case 31: printf("TK_IF ");
				break;
		case 32: printf("TK_THEN ");
				break;
		case 33: printf("TK_ENDIF ");
				break;
		case 34: printf("TK_READ ");
				break;
		case 35: printf("TK_WRITE ");
				break;
		case 36: printf("TK_RETURN ");
				break;
		case 37: printf("TK_PLUS ");
				break;
		case 38: printf("TK_MINUS ");
				break;
		case 39: printf("TK_MUL ");
				break;
		case 40: printf("TK_DIV ");
				break;
		case 41: printf("TK_CALL ");
				break;
		case 42: printf("TK_RECORD ");
				break;
		case 43: printf("TK_ENDRECORD ");
				break;
		case 44: printf("TK_ELSE ");
				break;
		case 45: printf("TK_AND ");
				break;
		case 46: printf("TK_OR ");
				break;
		case 47: printf("TK_NOT ");
				break;
		case 48: printf("TK_LT ");
				break;
		case 49: printf("TK_LE ");
				break;
		case 50: printf("TK_EQ ");
				break;
		case 51: printf("TK_GT ");
				break;
		case 52: printf("TK_GE ");
				break;
		case 53: printf("TK_NE ");
				break;
	}
}