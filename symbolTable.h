#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gterm.h"

#define SIZE 200 //max size of hash table array

struct arrayitem 
{
	struct table *head;      
} arrayItem;

struct arrayitem symbolTable[SIZE];

int noEle = 0;

// typedef enum grammar_term {
// 	program, mainFunction, otherFunctions, function, input_par, output_par,
// 	parameter_list, dataType, primitiveDatatype, constructedDatatype,
// 	remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
// 	fieldDefinition, moreFields, declarations, declaration, global_or_not,
// 	otherStmts, stmt, assignmentStmt, singleOrRecId, new_24, funCallStmt,
// 	outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
// 	ioStmt, allVar, allVarTemp, arithmeticExpression, expPrime, term, termPrime, factor,
// 	highPrecedenceOperators, lowPrecedenceOperators, all, temp, booleanExpression,
// 	var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, eps,
// 	TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
// 	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
// 	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
// 	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
// 	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
// 	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE
// } gterm;

typedef struct keyword {
	char key[20];
	g_Term value;
}keyword;

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

struct table {
   g_Term token;   
   char* key;
   struct table* next;
} tableEntry;

int hash(char* key);

g_Term getToken(char* key);

void insertID(char* key, g_Term token);

void initTable();