#ifndef _GRAMMARDEF_
#define _GRAMMARDEF_

#define NON_TERMINALS 49
#define TERMINALS 52

#include <stdio.h>
#include <stdbool.h>
#include "gterm.h"

typedef struct node {
	gterm symbol;
	bool isTerminal;
	g_Node* next;
} g_Node;

typedef struct cell{
 	gterm symbol;
 	struct node* head;
} g_cell;

typedef g_cell Grammar[200];
Grammar G;

bool isTerm(gterm symbol);

void printRule(g_cell lhs);


typedef g_cell Grammar[200];

#endif 