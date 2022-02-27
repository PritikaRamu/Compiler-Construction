#include <stdlib.h>
#include "lexer.h"

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