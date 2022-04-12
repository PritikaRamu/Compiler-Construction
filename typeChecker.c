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
bool isOperator(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == DIVIDE || nType == MULTIPLY || nType == PLUS || nType == MINUS) {
        return true;
    }
    else {
        return false;
    }
}

bool isIntReal(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == INTEGER || nType == REAL) {
        return true;
    }
    else {
        return false;
    }
}

// bool isRecUnion(identifierNode* currNode) {
//     Type nType = currNode->type;
//     if(nType == RECORD_TYPE || nType == UNION_TYPE) {
//         return true;
//     }
//     else {
//         return false;
//     }
// }

identifierNode* validateArithmetic(ast* curr, ast* func) {
    if (curr == NULL)
    {
        return;
    }
    int localOffset = 0;
    //int numTempVar = 0;


    if(isOperator(curr) == true) {

        ast* fChild, *sChild;
        fChild = curr->firstChild;
        sChild = fChild->nextSibling;
        //ast has 1 or no children
        if(fChild==NULL || sChild == NULL) {
            return;
        }

        //both are identifiers
        if(isOperator(fChild) == false && isOperator(sChild) == false) {

            //both int/real
            if(isIntReal(fChild) == true && isIntReal(sChild) == true){

                identifierNode *fNode, *sNode;
                fNode = retrieve(fChild);
                sNode = retrieve(sChild);
                //node(s) not found in symbol table
                if(fNode==NULL || sNode == NULL) {
                    return;
                }
                Type fType = fNode->type;
                Type sType = sNode->type;

                //both int, both real;
                if(fType == sType) {
                    return createINode(fNode, func, fType, false, &localOffset);
                }
                //int with real operations  
                else {
                    return NULL;
                }
            }

            //both records/unions
            else if(fChild->nodeType == RECORD_OR_UNION && sChild->nodeType == RECORD_OR_UNION) {
                //both records
                recordUnionNode *fNode, *sNode;
                fNode = retrieve(fChild);
                sNode = retrieve(sChild);
                //node(s) not found in symbol table
                if(fNode==NULL || sNode == NULL) {
                    return;
                }
                
            }

        }
        //one record, one identifier
        
        //                       
            // switch(fNode->type){
            //     case (INT_TYPE):
            //     {          
            //                                                                        
            //     }
            // }
    }
}