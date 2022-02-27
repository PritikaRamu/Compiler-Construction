#include <stdio.h>
#include "grammar.h"

typedef struct treeNodeInfo{
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
} parseTree;

typedef g_cell[49][53] parseTable;


typedef struct firstAndFollowSet{
	bool[100][52] first; // 100 is no. of rules
	bool[100][52] follow;
} FirstAndFollow;

void ComputeFirstAndFollowSets ();

table createParseTable(table T);

void parseInputSourceCode(char *testcaseFile);

void printParseTree(parseTree PT, char *outfile);

Grammar G;
FirstAndFollow FFSet;

void initGrammar(){

}

void initFF{
	FFSet = ComputeFirstAndFollowSets(G);
}