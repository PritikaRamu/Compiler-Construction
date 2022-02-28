#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"


//will compute first set for alpha in production A->alpha
//will return a boolean array 

void computeParseTable(Grammar G, FirstAndFollow F)
{
	for(int i = 0; i < NON_TERMINALS; i++) 
	{
		ruleHead* iterator = G[i];
		while(!iterator){
			fillParseTable(iterator->listHead, i);
			iterator = iterator->next;
		}
	}

}

void fillParseTable( g_RHS* rule, g_Term lhs)
{
	//Case 1
	bool* first_array = computeFirst(rule);
	for(int i=0;i<TERMINALS;i++){
		if(first_array[i]){
			parseTable[lhs][i] = rule;
		}
	}

	// Case 2 
	if(first_array[eps]){
		for(int i=0;i<TERMINALS;i++){
			if(Follow[lhs][i]){
				parseTable[lhs][i] = rule;
			}
		}
	}
	
	//Case 3

	// Manage Dollar Case

	// if(first_array[eps]){ 
	// 	if(Follow[lhs]["dollar"]){
	// 			parseTable[lhs]["dollar"] = rule;
			
	// 	}
	// }
	
}

bool* computeFirst(g_RHS* head){
	
	int size = sizeof(bool)*(TERMINALS + 1);
    bool* first_array = (bool*)malloc(size);

	memset(first_array, false, size);

	g_RHS* iter = head;
	while(iter!=NULL){
		if(iter->isTerminal){
			first_array[iter->symbol] = true;
			return first_array;
		}

		else{
			First[iter->symbol][eps] = false; // Epsilon Tricky Cases 
			first_array = set_union(first_array, First[iter->symbol], TERMINALS+1);
		}

		iter = iter->next;
	}

}

bool* set_union(bool* A, bool*B, int len){

    int size = sizeof(bool)*len;
    bool* union_array = (bool*)malloc(size);

    for(int i=0; i<len;i++){
        if(A[i]==true || B[i]==true)
            union_array[i] = true;
        else
            union_array[i] = false;
    }
    return union_array;
}


int main(){

}
