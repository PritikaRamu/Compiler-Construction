#include <stdio.h>
#include "narytree.h"

parseTree initTree(g_Term symbol, int lineNo) {
    // TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    // node->symbol = symbol;
    // node->isTerminal = isTerm(symbol);
    parseTree ptree = (parseTree)malloc(sizeof(struct tree));
    ptree->symbol = symbol;
    ptree->isTerminal = isTerm(symbol);
    ptree->lineNo = lineNo;
    ptree->firstChild = NULL;
    ptree->nextSibling = NULL;
    ptree->parent = NULL;
    return ptree;
}