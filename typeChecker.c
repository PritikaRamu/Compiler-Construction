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

    // //check for otherstmts
    // ast* currNext = curr->firstChild->nextSibling->nextSibling->nextSibling;
    // if(currNext->nodeType == OTHERSTMTS){
    //     validateStmts(currNext, currNode, opAssigned);
    // }
    // else return;


    for(int i = 0; i < n; i++){
        if(opAssigned[i] == true)
            continue;
        else{
            //semantic error -- op parameter not assigned, change flag
            //print semantic error to file
        }
    }
    free(opAssigned);

    ast* ip = curr->firstChild;
    if(ip->nodeType == INPUT_PARAMETERS)
        validateIP(ip);
    
    ast* nextKid = curr->firstChild->nextSibling;
    ast* child = NULL;
    if(nextKid->nodeType == OUTPUT_PARAMETERS) {
        ast* op = nextKid;
        validateOP(op);
    }
    else {
        child = nextKid;
    }

    if(child == NULL) {
        child = curr->firstChild->nextSibling->nextSibling;
    }

    for(; child != NULL; child = child->nextSibling) {
        int localOffset = 0;
        NodeType nType = child->nodeType;
        //validateStmts(ast* curr, functionNode* currNode, bool* opAssigned);

    }
}

// void validateStmts(ast* curr, functionNode* currNode, bool* opAssigned){

// }
int isOperator(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == DIVIDE || nType == MULTIPLY || nType == PLUS || nType == MINUS) {
        return 1;
    }
    else {
        return 0;
    }
}
void validateArithmetic(ast* curr) {
    if(isOperator(curr) == 1) {
        ast* fChild, *sChild;
        fChild = curr->firstChild;
        sChild = fChild->nextSibling;
        if(isOperator(fChild) == 1 && isOperator(sChild) == 1) {
            
        }
    }
}