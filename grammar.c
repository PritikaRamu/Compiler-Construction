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

// int main(){
// 	gterm trial = 68;

// 	Grammar G;
// 	G[0].symbol = 54;
// 	G[0].head = insertNode(G[0].head, 68);
// 	G[0].head = insertNode(G[0].head, 89);

// 	printRule(G[0]);
// }

