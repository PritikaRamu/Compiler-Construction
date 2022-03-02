#ifndef _NEWRULES_
#define _NEWRULES_

#define NON_TERMINALS 53
#define TERMINALS 60
#define NUM_RULES 100
#include "symbolTable.h"
#include <string.h>


// GRAMMAR DATA STRUCTURES
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

//


// GRAMMAR Functions
g_RHS* insertIntoRule(g_RHS* head, g_Term s, bool isTerm);
ruleHead* insertRuleList(ruleHead* head, g_RHS* rule);
void printGrammar(Grammar G, int len);
void printRules(Grammar G, int i, bool needArrow);
void printRule(g_RHS* head);
void printToken(int token);
void printNonTerminal(int token);
void initGrammar();

// PARSE TABLE Functions
void populateFirstFollow(char* textfile, bool first); // Works
void printFirst();
void printFollow();


bool* set_union(bool* A, bool*B, int len); // Works

bool* computeFirst(g_RHS* head); // Works
bool checkEpsiloninFirst(g_Term NT); // Works

void segFaultsSuck();
void fillParseTable( g_RHS* rule, g_Term lhs);
void printParseTableRow(g_Term NT);

#endif