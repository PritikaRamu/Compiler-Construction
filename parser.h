#include <stdio.h>
#include "lexer.h"

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

typedef struct parseTable {

} table;


typedef struct firstAndFollowSet{

} FirstAndFollow;

typedef struct Gram{

} grammar;

FirstAndFollow ComputeFirstAndFollowSets (grammar G);

table createParseTable(FirstAndFollow F, table T);

void parseInputSourceCode(char *testcaseFile, table T);

void printParseTree(parseTree PT, char *outfile);
