#ifndef _GRAMMARDEF_
#define _GRAMMARDEF_

#define NON_TERMINALS 49
#define TERMINALS 52

#include <stdio.h>
#include <stdbool.h>
#include "lexer.h"

typedef struct node {
	gterm symbol;
	bool isTerminal;
	struct node* next;
} g_Node;

typedef struct cell{
 	gterm symbol;
 	struct node* head;
} g_cell;

bool isTerm(gterm symbol);

void printRule(g_cell lhs);


typedef g_cell Grammar[200];

#endif 