#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

#define SIZE 200 //max size of hash table array

struct arrayitem 
{
	struct table *head;      
} arrayItem;

struct arrayitem symbolTable[SIZE];

typedef struct keyword {
	char key[20];
	g_Term value;
}keyword;


struct table {
   g_Term token;   
   char* key;
   struct table* next;
} tableEntry;

int hashcode(char* key);
g_Term getToken(char* key);
void insertID(char* key, g_Term token);
void initTable();