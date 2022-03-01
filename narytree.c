#include <stdio.h>
#include "narytree.h"

parseTree initTree(g_Term symbol) {
    // TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    // node->symbol = symbol;
    // node->isTerminal = isTerm(symbol);
    parseTree ptree = (parseTree)malloc(sizeof(struct tree));
    ptree->symbol = symbol;
    ptree->isTerminal = isTerm(symbol);
    ptree->firstChild = NULL;
    ptree->nextSibling = NULL;
    ptree->parent = NULL;
    return ptree;
}


void addChildren(g_cell* lhs, parseTree ptree){
    g_Node* rhs = (g_Node*)malloc(sizeof(g_Node));
    rhs = lhs->head;
    parseTree curr;
    if(rhs){
        parseTree fChild = (parseTree)malloc(sizeof(struct tree));
        fChild->symbol = rhs->symbol;
        fChild->isTerminal = rhs->isTerminal;
        fChild->firstChild = NULL;
        fChild->nextSibling = NULL;
        fChild->parent = ptree;
        ptree->firstChild = fChild;
        curr = fChild;
        rhs = rhs->next;
    }
    while(rhs){
        //printf("%d ",curr->symbol);
        parseTree temp = (parseTree)malloc(sizeof(struct tree));
        temp->symbol = rhs->symbol;
        temp->isTerminal = rhs->isTerminal;
        temp->firstChild = NULL;
        temp->nextSibling = NULL;
        temp->parent = ptree;
        curr->nextSibling = temp;
        curr = temp;
        rhs = rhs->next;
    }
    //printf("%d ",curr->symbol);
}

int main() {
    parseTree ptree = initTree(program);
    g_cell* LHS = (g_cell*)malloc(sizeof(g_cell));
    LHS->symbol = program;
    g_Node* rhs1 = (g_Node*)malloc(sizeof(g_Node));
    g_Node* rhs2 = (g_Node*)malloc(sizeof(g_Node));
    g_Node* rhs3 = (g_Node*)malloc(sizeof(g_Node));
    rhs1->symbol = function;
    rhs2->symbol = eps;
    rhs3->symbol = mainFunction;
    rhs1->next = rhs2;
    rhs2->next = rhs3;
    LHS->head = rhs1;
    addChildren(LHS,ptree);
    return 0;
} 