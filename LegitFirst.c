#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"

void fillFirstArray(g_Term lhs, bool* A){ // Union of bool array and First Table Row
    for(int i=0; i<TERMINALS;i++){ 

        if(A[i]==true || First[lhs][i]==true)
            First[lhs][i] = true;
        else
            First[lhs][i] = false;
    }
}

void firstRuleNT(g_Term lhs, ruleHead* rhs){ // GOES OVER 1 rule of 1 Non-Terminal
	
    g_RHS* iter = rhs->listHead; 
	      		
    if(iter->isTerminal){ // If First Symbol is a Terminal
            First[lhs][(iter->symbol)-eps] = true;
            return First[lhs];
    }
	else{ // If First Symbol is a Non-Terminal
		bool* array = firstNT(iter->symbol);
        fillFirstArray(lhs, array);
	}
    if(containsEpsilon(iter->symbol)){ // If it contains an e-rule, check next too
        iter = iter->next;
        bool* array = firstNT((iter)->symbol);
        fillFirstArray(lhs, array);
    }
}

bool* firstNT(g_Term i){ // GOES OVER all rules of 1 Non-Terminal
    ruleHead* iter = G[i];
    while(iter!=NULL){
        firstRuleNT(i, iter);
        iter = iter->next;
    }

    return First[i];
}

void trialFirstAndFollow(){ // GOES OVER ALL grammar symbols

    // Initialize ALL to False
    for(int i=0;i<NON_TERMINALS+TERMINALS;i++){
        for(int j=0; j<TERMINALS; j++){
            First[i][j] = false;
        }
    }
    // Real Computation
	for(int i = 0; i < TERMINALS + NON_TERMINALS; i++){	
		
        if(i>eps){ // First Set of all Terminals are themselves!
            First[i][i-eps] = true;
        }
        
        else{ // First Set of Non-Terminals
            firstNT(i);
        }
	}
}

bool containsEpsilon(g_Term lhs){

    bool containsERule = false;
    ruleHead* iter = G[lhs];
    while(iter!=NULL){
        g_RHS* rhs = iter->listHead; 
      		
        if(rhs->symbol == eps){ // If First Symbol is a Terminal
            containsERule = true;
    }
        iter = iter->next;
    }

    return containsERule;
}


int main(){
   

	initGrammar();
    // printGrammar(G, 100);

    printf("Legit\n");
    trialFirstAndFollow();
    printFirst();

    // Terminal: gcc LegitFirst.c newRules.c -o dua
     
}