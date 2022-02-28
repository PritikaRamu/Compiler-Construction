#include <stdio.h>
#include "narytree.h"

parseTree initTree(gterm symbol) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->symbol = symbol;
    node->isTerminal = isTerm(symbol);
    parseTree ptree = (parseTree)malloc(sizeof(struct tree));
    ptree->firstChild = NULL;
    ptree->nextSibling = NULL;
    ptree->parent = NULL;
    return ptree;
}

int main() {
    return 0;
} 