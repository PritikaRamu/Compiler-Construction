#define NON_TERMINALS 49
#define TERMINALS 52

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gterm.h"

// typedef union symbol{
//     G_Non_Terminals NT;
//     G_Terminals T;
// } G_Term;
//node that contains terminal or non-terminal
typedef struct rhs{ 
	g_Term symbol;
	//bool isTerminal;
	struct RHS* next;
} g_RHS;

bool isTerminal[TERMINALS + NON_TERMINALS];

typedef struct lhs{
 	g_Term symbol;
 	g_RHS** head;
} g_LHS;

typedef g_LHS Grammar[NON_TERMINALS];

g_RHS* insertIntoRule(g_RHS* head, int s){

	//we insert the symbols to get the RHS for the rules
	//insertion has to be done in reverse order
	g_RHS* temp = (g_RHS *)malloc(sizeof(g_RHS));
	temp->symbol = s;
	temp->next = head;
	return temp;
}

void printRule(g_RHS* head){

	g_RHS* temp = head;
    // printf("%d ", head->symbol);
    
	while(temp != NULL)
	{
		printf("%d ", temp->symbol);
        temp = temp->next;
	} 
	printf("\n");
}


// insertRule(Grammar G, G_Non_Terminals NT, g_RHS* list){
//     g_RHS* ruleHead = G[NT].head;
//     while(ruleHead.next!=NULL){
//         ruleHead = ruleHead.next;
//     }

//     ruleHead.next = list;
// }


int main(){

    printf("Hi\n");
	g_RHS* myRule = (g_RHS*)malloc(sizeof(g_RHS));
    myRule->symbol = 3;
    myRule->next = NULL;
    printRule(myRule);

    
	myRule->next = insertIntoRule(myRule, 6);
	myRule = insertIntoRule(myRule,8);
	printRule(myRule);
}