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
	g_Node* head;
} g_cell;

typedef g_cell Grammar[200];

void printRule(g_cell lhs);

