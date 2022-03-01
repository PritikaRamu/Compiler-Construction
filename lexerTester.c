#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#define COUNT_KEYWORDS 28
#define BUFFER_SIZE 10

typedef enum grammar_term {
	program, mainFunction, otherFunctions, function, input_par, output_par,
	parameter_list, dataType, primitiveDatatype, constructedDatatype,
	remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
	fieldDefinition, moreFields, declarations, declaration, global_or_not,
	otherStmts, stmt, assignmentStmt, singleOrRecId, new_24, funCallStmt,
	outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
	ioStmt, allVar, allVarTemp, arithmeticExpression, expPrime, term, termPrime, factor,
	highPrecedenceOperators, lowPrecedenceOperators, all, temp, booleanExpression,
	var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, eps,
	TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE
} gterm;

typedef struct keyword {
	char key[20];
	gterm value;
}keyword;

typedef struct token {
	gterm tid; 
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;



typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

FILE* getStream(FILE* fp);

keyword keywords[] = {
	{"with",TK_WITH},
	{"parameters",TK_PARAMETERS},
	{"end",TK_END},
	{"while",TK_WHILE},
	{"union",TK_UNION},
	{"endunion",TK_ENDUNION},
	{"definetype",TK_DEFINETYPE},
	{"as",TK_AS},
	{"type",TK_TYPE},
	{"main",TK_MAIN},
	{"global",TK_GLOBAL},
	{"parameter",TK_PARAMETER},
	{"list",TK_LIST},
	{"input",TK_INPUT},
	{"int",TK_INT},
	{"real",TK_REAL},
	{"endwhile",TK_ENDWHILE},
	{"if",TK_IF},
	{"then",TK_THEN},
	{"endif",TK_ENDIF},
	{"read",TK_READ},
	{"write",TK_WRITE},
	{"return",TK_RETURN},
	{"call",TK_CALL},
	{"record",TK_RECORD},
	{"endrecord",TK_ENDRECORD},
	{"else",TK_ELSE}
};

twinBuffer buffers;
int line = 1;
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

void reset(){
	lexemeBegin = forward;
	charCount = 0;
}

void closeLexer(FILE* fp) {
	int err = fclose(fp);
}

FILE* getStream(FILE *fp) {
	
	
	if(!forward) 
	{
	//start case
	//read into buff1 and set readBuff1 as true
		int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
		buffers.buff1[size1] = EOF;
		lexemeBegin = buffers.buff1;
		forward = buffers.buff1;

		readBuff1 = true;
		readBuff2 = true;
		startedReading = true;
		//printf("check %c check",*forward);
	}
	
	else if(forward == buffers.buff1 + BUFFER_SIZE - 1)	{
		//forward is at the end of buffer1
		if(readBuff2) 
		{
			int size2 = fread(buffers.buff2, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff2[size2] = EOF;
		}
		forward = buffers.buff2;
		readBuff2 = true;
	}

	else if (forward == buffers.buff2 + BUFFER_SIZE - 1) {
		//forward is at the end of buffer2
		if(readBuff1) 
		{
			int size1 = fread(buffers.buff1, sizeof(char), BUFFER_SIZE, fp);
			buffers.buff1[size1] = EOF;
		}
		forward = buffers.buff1;
		readBuff1 = true;
	}

	else {
		//normal case where we have space in the buffer
		forward++;
	}
	return fp;
}


char getNextChar(FILE *fp) {
	
	if(!startedReading)
		{fp = getStream(fp); char curr = *forward; ++charCount;  forward++; return curr;}
	char curr = *forward;
	fp = getStream(fp);	
	++charCount;
	return curr;
}


FILE* startLexer(char* inputFile) {
	//must be called in the driver file before starting the lexer
	//initialising buffers to 0
	
	memset(buffers.buff1, 0, sizeof(buffers.buff1));
	memset(buffers.buff2, 0, sizeof(buffers.buff2));

	line = 1;
	file_end = false;
	charCount = 0;
	readBuff1 = false;
	readBuff2 = false;
	lexemeBegin = NULL;
	forward = NULL;
	startedReading = false;

	FILE* fp = fopen(inputFile, "r");


	if(fp == NULL) {
		printf("Error opening file\n");
		return NULL;
	}

	else {
		printf("\n Successfully opened the code file\n");
	}
	//fp = getStream(fp);
	return fp;
}



int main()
{

	fp = startLexer("sampleText.txt");

	printf("\nSIZE OF TWIN BUFFER %lu %lu %lu \n",sizeof(buffers), sizeof(buffers.buff1), sizeof(buffers.buff2));
	
	printf("Checking if twin buffers have been initialized or not \n %d %c %c %c \n", buffers.buff1[0],buffers.buff1[BUFFER_SIZE], buffers.buff2[0], buffers.buff2[BUFFER_SIZE]);	

	printf("Startlexer working fine\n");

	
		
	for(int i = 0; i < 20; i++)
		{
		char c = getNextChar(fp);
		printf("%c\n",c);
		}

	/*
	printf("\nFirst call to getStream function\n");	

 	
		
	printf("\nContents of buffer 1\n");		

	printf("\n%s", buffers.buff1);

	printf("\nContents of buffer 2\n");

	printf("\n%s", buffers.buff2);

	printf("\nSecond call to getStream function\n");	

 	fp = getStream(fp);
		
	printf("\nContents of buffer 1\n");		

	printf("\n%s", buffers.buff1);

	printf("\nContents of buffer 2\n");

	printf("\n%s", buffers.buff2);
	*/
	

	return 0;
}










