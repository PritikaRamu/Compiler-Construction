/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#ifndef _PARSEDEF_
#define _PARSEDEF_

#include "lexer.h"
#define NON_TERMINALS 53
#define TERMINALS 60
#define NUM_RULES 100


// PARSE TREE Data Structures
struct tree {
    g_Term symbol;
    bool isTerminal;
    int lineNo;
    struct tree* firstChild;
    struct tree* nextSibling;
    struct tree* parent;
    char * lex;
};

typedef struct tree* parseTree;

// GRAMMAR Data Structures
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

// PARSE TABLE Data Structures
g_RHS* parseTable[NON_TERMINALS][TERMINALS-1];

bool First[NON_TERMINALS + TERMINALS][TERMINALS];
bool Follow[NON_TERMINALS][TERMINALS];

// STACK Data Structures
typedef struct stack {
    g_RHS* data;
    struct stack* next;
} Stack;

#endif