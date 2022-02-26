#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 200 //max size of hash table array

struct table {
   char* token;   
   char* key;
   struct table* next;
} tableEntry;

struct arrayitem 
{
	struct table *head;      
} arrayItem;

struct arrayitem symbolTable[SIZE];

int noEle = 0;

typedef struct keyword {
	char key[20];
	char value[20];
}keyword;

keyword keywords[] = {
	{"with","TK_WITH"},
	{"parameters","TK_PARAMETERS"},
	{"end","TK_END"},
	{"while","TK_WHILE"},
	{"union","TK_UNION"},
	{"endunion","TK_ENDUNION"},
	{"definetype","TK_DEFINETYPE"},
	{"as","TK_AS"},
	{"type","TK_TYPE"},
	{"main","TK_MAIN"},
	{"global","TK_GLOBAL"},
	{"parameter","TK_PARAMETER"},
	{"list","TK_LIST"},
	{"input","TK_INPUT"},
	{"int","TK_INT"},
	{"real","TK_REAL"},
	{"endwhile","TK_ENDWHILE"},
	{"if","TK_IF"},
	{"then","TK_THEN"},
	{"endif","TK_ENDIF"},
	{"read","TK_READ"},
	{"write","TK_WRITE"},
	{"return","TK_RETURN"},
	{"call","TK_CALL"},
	{"record","TK_RECORD"},
	{"endrecord","TK_ENDRECORD"},
	{"else","TK_ELSE"}
};

int hash(char* key);

char* getToken(char* key);

void insertID(char* key, char* token);

void initTable();