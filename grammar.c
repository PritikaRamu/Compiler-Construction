#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include "grammar.h"

void printRule(g_cell lhs){
	printToken(lhs.symbol);
	printf("->");
	g_Node* h = lhs.head;
	while(h!=NULL){
		printToken(h->symbol);
		h = h->next;
	}
	printf("\n");

}

g_Node* insertNode(g_Node* head, int i){
	g_Node* temp = (g_Node*)malloc(sizeof(g_Node));
	temp->symbol = i;

	temp->next = head;
	return temp;
}

int main(){
	gterm trial = 68;

	// printf("Hi\n");
	// printToken(trial);
	// printf("\n");

	Grammar G;
	G[0].symbol = 54;
	G[0].head = insertNode(G[0].head, 68);
	G[0].head = insertNode(G[0].head, 89);
	// G[0].symbol = 68;
	// G[0].head = (g_Node*)malloc(sizeof(g_Node));

	// G[0].head->symbol = 76;
	// g_Node* temp = (g_Node*)malloc(sizeof(g_Node));
	// temp->symbol = 88;

	// G[0].head->next = temp;

	printRule(G[0]);
}

