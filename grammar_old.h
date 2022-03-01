#ifndef _GRAMMARDEF_
#define _GRAMMARDEF_

#define NON_TERMINALS 49
#define TERMINALS 52
#include <stdio.h>
#include <stdbool.h>
#include "gterm.h"

typedef struct node {
	g_Term symbol;
	bool isTerminal;
	g_Node* next;
} g_Node;

typedef struct cell{
 	g_Term symbol;
 	struct node* head;
} g_cell;

typedef g_cell Grammar[200];


bool isTerm(g_Term symbol);

void printRule(g_cell lhs);


typedef g_cell Grammar[200];

#endif 