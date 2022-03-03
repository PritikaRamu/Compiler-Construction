#ifndef _PARSEDEF_
#define _PARSEDEF_


#define NON_TERMINALS 53
#define TERMINALS 60
#define NUM_RULES 100
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lexer.h"


struct tree {
    //TreeNode* data;
    g_Term symbol;
    bool isTerminal;
    int lineNo;
    struct tree* firstChild;
    struct tree* nextSibling;
    struct tree* parent;
};

typedef struct tree* parseTree;

typedef struct rhs{ 
	g_Term symbol;
	bool isTerminal;
	struct rhs* next;
} g_RHS;

typedef struct ruleHead{
	
	g_RHS* listHead; 	//pointer to the linked list
	struct ruleHead* next; 	//pointer to next rule

}ruleHead;

typedef ruleHead* Grammar[NON_TERMINALS];
Grammar G;
//

// PARSE TABLE DATA STRUCTURES

g_RHS* parseTable[NON_TERMINALS][TERMINALS-1];

bool First[NON_TERMINALS + TERMINALS][TERMINALS];
bool Follow[NON_TERMINALS][TERMINALS];

typedef struct stack {
    g_RHS* data;
    struct stack* next;
} Stack;

#endif