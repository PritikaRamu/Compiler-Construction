#ifndef _NARYTREE_
#define _NARYTREE_

#include <stdlib.h>
#include <stdbool.h>
#include "grammar.h"

typedef struct treeNode {
    gterm symbol;
    bool isTerminal;
} TreeNode;

struct tree {
    TreeNode* data;
    struct tree* firstChild;
    struct tree* nextSibling;
    struct tree* parent;
};

typedef struct tree* parseTree;

#endif