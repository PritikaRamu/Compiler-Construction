#ifndef _LEXERDEF_
#define _LEXERDEF_

#include <stdio.h>
#define BUFFER_SIZE (1 << 12)

typedef enum {
	program, mainFunction, otherFunctions, function, input_par, output_par,
	parameter_list, dataType, primitiveDatatype, constructedDatatype,
	remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
	fieldDefinition, moreFields, declarations, declaration, global_or_not,
	otherStmts, stmt, assignmentStmt, singleOrRecId, actualOrRedefined, funCallStmt,
	outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
	ioStmt, fieldtype, option_single_constructed, arithmeticExpression, expPrime, term, termPrime, factor,
	highPrecedenceOperators, lowPrecedenceOperators, oneExpansion, moreExpansions, booleanExpression,
	var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids,
	eps,  TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM,
	 TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE,
	definetypestmt, A,$
} g_Term;


typedef struct token {
	g_Term tid; 
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;

typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

typedef struct keyword {
	char key[20];
	g_Term value;
}keyword;

#define COUNT_KEYWORDS 28
//#define BUFFER_SIZE 100

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

FILE* getStream(FILE* fp);

tokenInfo getNextToken();

void removeComments(char* testcaseFile, char* cleanFile);

void printToken(int token);

void printNonTerminal(int token);

void reset();

#endif