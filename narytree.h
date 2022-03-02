#ifndef _NARYTREE_
#define _NARYTREE_

#include <stdlib.h>
#include <stdbool.h>
#include "token.h"
// typedef struct treeNode {
//     gterm symbol;
//     bool isTerminal;
// } TreeNode;

struct tree {
    //TreeNode* data;
    g_Term symbol;
    bool isTerminal;
    int lineNo;
    struct tree* firstChild;
    struct tree* nextSibling;
    struct tree* parent;
};

typedef struct tree* parseTree;

#endif