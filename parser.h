/* GROUP 15
Group Members:
Pritika Ramu          ID: 2019A7PS1140P
Preetike Verma        ID: 2019A7PS0088P
Aadit Deshpande       ID: 2019A7PS0077P
Sneha                 ID: 2019A7PS0042P
Nandan B Parikh       ID: 2019A7PS0097P
*/
#include "parserDef.h"

// PARSER Functions
parseTree parseInputSourceCode(char* testcaseFile);
int printParseTree(parseTree PT, char* outfile);

// GRAMMAR Functions
void initGrammar();

// PARSE TABLE Functions
void computeParseTable();
void populateFirstFollow(char* textfile, bool first);
void printFirst();
void printFollow();

// FIRST/FOLLOW SET Functions
bool* set_union(bool* A, bool*B, int len);
bool* computeFirst(g_RHS* head);
void computeFirstAndFollow(); // Automating First Set Computation
bool* firstNT(g_Term i);
void firstRuleNT(g_Term lhs, ruleHead* rhs);

// STACK functions
bool isTerm(g_Term symbol);
Stack* initStack();
void push(Stack* head, g_Term data);
void pop(Stack* head);
g_Term top(Stack* head);

// Helper functions for verification
void printToken(int token);
void printNonTerminal(int token);
void printRules(Grammar G, int i, bool needArrow);
void printRule(g_RHS* head);
void printParseTableRow(g_Term NT);
void printGrammar(Grammar G, int len);
void printSymbol(int i);