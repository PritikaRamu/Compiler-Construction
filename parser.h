#ifndef _PARSEDEF_
#define _PARSEDEF_

#include <stdio.h>
#include "narytree.h"
#include "stack.h"
#include "lexer.h"
#include "newRules.h"

//typedef g_Node table[49][53];
//typedef g_cell **table;


typedef struct firstAndFollowSet{
	int** first; 
	int** follow;
} FirstAndFollow;


// FIRST and FOLLOW 
void ComputeFirstAndFollowSets (Grammar G);

//int* First(Grammar G, g_cell nt);

//int* set_union(int* A, int*B, int len);

void ComputeFirstAndFollowSets();

void printSet(bool* array, int len);

//table createParseTable(Grammar G, FirstAndFollow F, table T);

parseTree parseInputSourceCode(char* testcaseFile);

void printParseTree(parseTree PT, char *outfile);

FirstAndFollow FFSet;

void initGrammar();

/*
void initFF(){
	ComputeFirstAndFollowSets(G);
}*/

#endif