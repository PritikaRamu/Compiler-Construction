#include "typeChecker.h"

void semanticAnalyser(ast* root){
    //populate symbol tables
    ast * curr = root->firstChild;

    if(curr==NULL)
        return;

    while(curr->nodeType == FUNCTION_SEQ){
        validateFunction(curr);
        curr = curr->nextSibling;
    }
    validateMain(curr);
}

void validateFunction(ast* curr){
    if(curr == NULL)
        return;

    functionNode* currNode = (functionNode*)malloc(sizeof(functionNode));
    functionNode* discardNode = currNode;
    currNode = (functionNode*)retrieve(); //update according to retrieve
    free(discardNode);
    int n = currNode->numOp;
    bool* opAssigned = (bool*)malloc(sizeof(bool)*n);

    //check for otherstmts
    ast* currNext = curr->firstChild->nextSibling->nextSibling->nextSibling;
    if(currNext->nodeType == OTHERSTMTS){
        validateStmts(currNext, currNode, opAssigned);
    }
    else return;


    for(int i = 0; i < n; i++){
        if(opAssigned[i] == true)
            continue;
        else{
            //semantic error -- op parameter not assigned, change flag
            //print semantic error to file
        }
    }
    free(opAssigned);
}

void validateStmts(ast* curr, functionNode* currNode, bool* opAssigned){

}