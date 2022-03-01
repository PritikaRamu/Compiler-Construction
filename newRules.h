#define NON_TERMINALS 53
#define TERMINALS 60
#define NUM_RULES 100
#include "gterm.h"
#include <string.h>


// GRAMMAR DATA STRUCTURES
typedef struct rhs{ 
	g_Term symbol;
	bool isTerminal;
	struct RHS* next;
} g_RHS;

typedef struct ruleHead{
	
	g_RHS* listHead; 	//pointer to the linked list
	struct ruleHead* next; 	//pointer to next rule

}ruleHead;

typedef ruleHead* Grammar[NON_TERMINALS];
//

// PARSE TABLE DATA STRUCTURES

g_RHS* parseTable[NON_TERMINALS][TERMINALS];

bool First[NON_TERMINALS + TERMINALS][TERMINALS] = {false};
bool Follow[NON_TERMINALS][TERMINALS] = {false};

//


// GRAMMAR Functions
g_RHS* insertIntoRule(g_RHS* head, g_Term s, bool isTerm);
ruleHead* insertRuleList(ruleHead* head, g_RHS* rule);
void printGrammar(Grammar G, int len);
void printRules(Grammar G, int i, bool needArrow);
void printRule(g_RHS* head);
void printToken(int token);
void printNonTerminal(int token);

// PARSE TABLE Functions

bool* set_union(bool* A, bool*B, int len);

//bool* computeFirst(g_RHS* head, g_Term lhs);