#ifndef _LEXERDEF_
#define _LEXERDEF_

#include <stdio.h>
#define BUFFER_SIZE (1 << 12)

typedef enum {
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

tokenInfo getNextToken(twinBuffer B);

void removeComments(char* testcaseFile, char* cleanFile);

void printToken(int token);

void reset();

#endif