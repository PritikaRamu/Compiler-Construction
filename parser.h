
#include "parserDef.h"
#include <stdlib.h>
#include <stdbool.h>

// FIRST and FOLLOW 
void ComputeFirstAndFollowSets (Grammar G);

//int* First(Grammar G, g_cell nt);

//int* set_union(int* A, int*B, int len);

void ComputeFirstAndFollowSets();

void printSet(bool* array, int len);

//table createParseTable(Grammar G, FirstAndFollow F, table T);

parseTree parseInputSourceCode(char* testcaseFile);

void printParseTree(parseTree PT, char *outfile);

void initGrammar();

void printToken(int token);
void printNonTerminal(int token);
void printRules(Grammar G, int i, bool needArrow);

// GRAMMAR Functions
void printGrammar(Grammar G, int len);
void initGrammar();

// PARSE TABLE Functions
void populateFirstFollow(char* textfile, bool first); // Works
void printFirst();
void printFollow();


bool* set_union(bool* A, bool*B, int len); // Works

bool* computeFirst(g_RHS* head); // Works

void segFaultsSuck();
void printParseTableRow(g_Term NT);


// Own First, Follow
bool* firstNT(g_Term i);
void firstRuleNT(g_Term lhs, ruleHead* rhs);
void fillFirstArray(g_Term lhs, bool* A);
void trialFirstAndFollow();
bool containsEpsilon(g_Term lhs);

bool isTerm(g_Term symbol);
Stack* initStack();
void push(Stack* head, g_Term data);
void pop(Stack* head);
g_Term top(Stack* head);


