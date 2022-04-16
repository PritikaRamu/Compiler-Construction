/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#ifndef _LEXERDEF_
#define _LEXERDEF_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


#define BUFFER_SIZE (1 << 25)
#define COUNT_KEYWORDS 28
#define SIZE 200 //max size of hash table array

typedef enum {
	program, mainFunction, otherFunctions, function, input_par, output_par,
	parameter_list, dataType, primitiveDatatype, constructedDatatype,
	remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
	fieldDefinition, moreFields, declarations, declaration, global_or_not,
	otherStmts, stmt, assignmentStmt, singleOrRecId, actualOrRedefined, funCallStmt,
	outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
	ioStmt, fieldtype, option_single_constructed, arithmeticExpression, expPrime, term, termPrime, factor,
	highPrecedenceOperators, lowPrecedenceOperators, oneExpansion, moreExpansions, booleanExpression,
	var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, definetypestmt, A,
	eps,  TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM,
	TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE
	
} g_Term;

typedef struct twin {
	char buff1[BUFFER_SIZE + 1];
	char buff2[BUFFER_SIZE + 1];
} twinBuffer;

typedef struct token {
	g_Term tid; 
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;

struct arrayitem 
{
	struct table *head;      
} arrayItem;

struct table {
   g_Term token;   
   char* key;
   struct table* next;
} tableEntry;

typedef struct keyword {
	char key[20];
	g_Term value;
}keyword;

struct arrayitem symbolTable[SIZE];

#endif