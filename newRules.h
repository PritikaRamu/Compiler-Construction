#define NON_TERMINALS 49
#define TERMINALS 52
#define NUM_RULES 100
#include "gterm.h"

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

typedef struct firstAndFollowSet{
	int** first; 
	int** follow;
} FirstAndFollow;

g_RHS* insertIntoRule(g_RHS* head, g_Term s, bool isTerm);

void printRule(g_RHS* head);

ruleHead* insertRuleList(ruleHead* head, g_RHS* rule);


void printRules(ruleHead* head);

void printToken(int token);

void printNonTerminal(int token);