#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"

//will compute first set for alpha in production A->alpha
//will return a boolean array 


void computeParseTable()
{
	for(int i = 0; i < NON_TERMINALS; i++) 
	{
		ruleHead* iterator = G[i];
		while(!iterator){ // Goes Over Alternate Prodcutions
			fillParseTable(iterator->listHead, i);
			iterator = iterator->next;
		}
	}

}

void fillParseTable( g_RHS* rule, g_Term lhs)
{
	//Case 1
	bool* first_array = computeFirst(rule); // A->BCdE First[]
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
	
	g_RHS* iter = head; 
	bool* first = (bool*)malloc(sizeof(bool)*TERMINALS);
	memset(first,false,sizeof(bool)*TERMINALS);

	// Case 1 
	if(head->isTerminal){
		first[head->symbol-51] = true;
		return first;
	}

	// Case 2
	while(iter!=NULL){ // Iterating over the NTs of an RHS
	// A->BCDF
	// B-> eps | gHHJJ
	// C->d
		
		first = set_union(first, First[head->symbol],TERMINALS);
		first[0] = false;
		
		if(!checkEpsiloninFirst(head->symbol)){
			return first;
		}
		iter = iter->next;

	}

	first[0] = true;
	return first;

}


bool checkEpsiloninFirst(g_Term NT){
	return First[NT][51];
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
	
	initGrammar(G);
	populateFirstFollow("First.txt",true);
    populateFirstFollow("Follow.txt",false);


}
