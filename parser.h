#ifndef _PARSEDEF_
#define _PARSEDEF_

#include <stdio.h>
#include "grammar.h"
#include "narytree.h"
#include "stack.h"
#include "lexer.h"

/*typedef struct treeNodeInfo{
	int term_or_nonterm;
	// Symbol_node_type type;
} TreeNodeInfo;

typedef struct linkedlist{//Linked List of siblings
	struct treeNode* head;
	int no_siblings;
} Children;

typedef struct treeNode{
	struct treeNode* next; //The node itself is a part of some linked list

	TreeNodeInfo* info;
	tokenInfo* lexicalTOKEN; //If it's a leaf node

	Children* children;	//Linked list of Children

	struct treeNode* parent; //Pointer to parent node

	struct astNode* addr;
	struct astNode* inh_addr;
	
	int rule_no;
} TreeNode;

typedef struct tree{
	TreeNode* root;
} parseTree;*/

//typedef g_Node table[49][53];
typedef g_Node **table;


typedef struct firstAndFollowSet{
	bool first[100][52]; // 100 is no. of rules
	bool follow[100][52];
} FirstAndFollow;

//remove and add appropriate header file
typedef struct token {
	gterm tid; 
	char* lexeme;
	void* numVal; //Int, Float values for numbers
	int lineNo;
} tokenInfo;

void ComputeFirstAndFollowSets ();

table createParseTable(table T);

parseTree parseInputSourceCode(char* testcaseFile, table T);

void printParseTree(parseTree PT, char *outfile);

FirstAndFollow FFSet;

void initGrammar();

/*
void initFF(){
	ComputeFirstAndFollowSets(G);
}*/

#endif