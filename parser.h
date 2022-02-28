#ifndef _PARSEDEF_
#define _PARSEDEF_

#include <stdio.h>
#include "grammar.h"
#include "narytree.h"

//typedef g_Node table[49][53];
typedef g_cell **table;


typedef struct firstAndFollowSet{
	int** first; 
	int** follow;
} FirstAndFollow;


// FIRST and FOLLOW 
void ComputeFirstAndFollowSets (Grammar G);

int* First(Grammar G, g_cell nt);

int* set_union(int* A, int*B, int len);

void printSet(int* array, int len);

//
table createParseTable(Grammar G, FirstAndFollow F, table T);

parseTree parseInputSourceCode(char *testcaseFile, table T);

void printParseTree(parseTree PT, char *outfile);

Grammar G;
FirstAndFollow FFSet;

void initGrammar();

/*
void initFF(){
	ComputeFirstAndFollowSets(G);
}*/

#endif