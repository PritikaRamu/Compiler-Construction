#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include "grammar.h"

void printRule(g_cell lhs){
	printToken(lhs.symbol);
	printf("->");
	g_Node h = lhs.head[0];
	while(h.next!=NULL){
		printToken(h.symbol);
	}
	printf("\n");

}

int main(){
	gterm trial = 68;

	// printf("Hi\n");
	// printToken(trial);
	// printf("\n");

	Grammar G;
	G[0].symbol = 68;
	G[0].head = (g_Node*)malloc(sizeof(g_Node*));

	(G[0].head[0]).symbol = 76;
	
	printf(sizeof(G[0].head));
	printf("\n");

	printRule(G[0]);
}