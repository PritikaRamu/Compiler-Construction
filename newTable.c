#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"

g_RHS* parseTable[NON_TERMINALS][TERMINALS];

bool First[NON_TERMINALS + TERMINALS + 1][TERMINALS + 1];
bool Follow[NON_TERMINALS][TERMINALS+2];

bool* computeFirst(g_RHS* head);
//will compute first set for alpha in production A->alpha
//will return a boolean array 

void computeParseTable(Grammar G, FirstAndFollow F)
{
	for(int i = 0; i < NON_TERMINALS; i++) 
	{
		ruleHead* iterator = G[i];
		while(!iterator){
			fillParseTable(F, iterator->listHead, i);
			iterator = iterator->next;
		}
	}

}

void fillParseTable(FirstAndFollow F, g_RHS* rule, g_Term lhs)
{
	//fills the parse table for the production A -> alpha
	
	//for each terminal 'a' in first(alpha), M[A,a] = A -> alpha
	
	//TO_DO

	//if epsilon is in First(alpha) then
	//for each terminal 'b' in Follow(A) 
	//M[A,b] = A -> alpha
	
	//TO_DO

	//if epsilon is in First(alpha) and $ is in follow(A)
	//M[A,$] = A-> alpha
	
	//TO_DO
	
}
