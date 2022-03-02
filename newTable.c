#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"

//will compute first set for alpha in production A->alpha
//will return a boolean array 

void segFaultsSuck(){
	for(int i = 0; i < NON_TERMINALS; i++) // Goes over all NTs
	{	
		// printf("%d\t",i);
		ruleHead* iterator = G[i];
		while(iterator!=NULL){ // Goes Over Alternate Prodcutions of an NT
			fillParseTable(iterator->listHead, i);
			iterator = iterator->next;
		}
	}

}

void fillParseTable( g_RHS* rule, g_Term lhs){
	//Case 1
	bool* first_array = computeFirst(rule); // A->BCdE First[]

	for(int i=54;i<=112;i++){
		if(first_array[i-eps]==true){
			parseTable[lhs][i-eps] = rule;
		}
	}

	// Case 2 
	if(first_array[0]==true){
		for(int b=54;b<=112;b++){
			if(Follow[lhs][b-eps] == true){
				parseTable[lhs][b-eps] = rule;
			}
		}
	}


	
}

// Chnage 51
void printParseTableRow(g_Term NT){
	printf("NT is: ");
	printNonTerminal(NT);
	printf(" The Parse Table entries are: ");
	for(int i=0;i<TERMINALS-1;i++){
		if(parseTable[NT][i]){
			printf("[ Terminal: ");
			printToken(i+eps);
			printf(", Rule: ");
			printRule(parseTable[NT][i]);
			printf("], ");
		}
	}

}

// Chnage 51
bool checkEpsiloninRHSFirst(g_Term NT){
	return First[NT][0];
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

// Chnage 51
bool* computeFirst(g_RHS* head){
	
	g_RHS* iter = head; 
	bool* first = (bool*)malloc(sizeof(bool)*TERMINALS);
	memset(first,false,sizeof(bool)*TERMINALS);

	// Case 1 
	if(head->isTerminal){
		first[head->symbol-eps] = true;
		return first;
	}

	// Case 2
	while(iter!=NULL){ // Iterating over the NTs of an RHS
	// A->BCDF
	// B-> eps | gHHJJ
	// C-> d
		
		first = set_union(first, First[iter->symbol],TERMINALS);
		//first[0] = false;
		
		if(!checkEpsiloninRHSFirst(iter->symbol)){
			first[0] = false;
			return first;
		}
		iter = iter->next;

	}

	first[0] = true;
	return first;

}

// Works
void printFirstArray(bool* array, int n){
	for(int i=0; i<n; i++){
		if(array[i]==true)
			//printf("%d\t",i);
			printToken(i+eps);
	}

	printf("\n");
}




int main(){
	
	initGrammar(G);
	populateFirstFollow("First.txt",true);
    populateFirstFollow("Follow.txt",false);

	// int i = 25;
	// ruleHead* list = G[i];
	// while(list!=NULL){

	// 	// if(checkEpsiloninRHSFirst(list->listHead->symbol)){
	// 	// 	printf("This rule contains epsilon\n");
	// 	// }

	// 	// else{
	// 	// 	printf("This rule does NOT contain epsilon\n");
			
	// 	// }

	// 	bool* first = computeFirst(list->listHead);
	// 	printf("The first of RHS is:\n");
	// 	printFirstArray(first, TERMINALS);


	// 	// printRule(list->listHead);
	// 	list = list->next;
	// }

	// computeParseTable();
	segFaultsSuck();
	for(int i=0; i<NON_TERMINALS; i++){
		printParseTableRow(i);
	}
	



}
