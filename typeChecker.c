#include "typeChecker.h"

identifierNode* validateBoolean(ast* curr, ast* func);
void handleStmt(ast* curr);
void validateConditional(ast* curr);
recordField* searchInFieldList(ast* curr, recordField* fieldList);

recordField* searchInFieldList(ast* curr, recordField* fieldList) {
    recordField* temp = fieldList;
    while(temp) {
        if(strcmp(curr->lex, temp->token->lexeme) == 0 && curr->symbol == temp->token->tid) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

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

    //identifier node
    if(isOperator(curr) == false){
        identifierNode* currNode = createINode(curr, func, curr->nodeType, false, &localOffset);
        identifierNode* notGlobalCurr = currNode;
        notGlobalCurr->global = false;
        notGlobalCurr = retrieve(SymbolTable, notGlobalCurr, curr->nodeType);
        identifierNode* globalCurr = currNode;
        globalCurr->global = true;
        globalCurr = retrieve(SymbolTable, globalCurr, curr->nodeType);
        if (globalCurr)
        {
            currNode = globalCurr;
        }
        else if (notGlobalCurr){
            currNode = notGlobalCurr;
        }
        else {
            free(notGlobalCurr);
            free(globalCurr);
            //semanticErrors ++;
            printf("Undeclred Variable\n");
            return NULL;
        }
        
        free(notGlobalCurr);
        free(globalCurr);
        
        //attribute from AST
        ast* attribute = NULL;
        if(!curr->firstChild) {
            attribute = curr->firstChild;
        }

        //record/union from symbol table

        //NESTED RECORDS
        recordField * fieldList = NULL;
        if(currNode->type == RECORD_TYPE){
            fieldList = currNode->recordList->fieldList;
        }
        else if (currNode->type == UNION_TYPE){
            fieldList = currNode->unionList->fieldList;
        }
    
        if(curr->nodeType != RECORD_OR_UNION && attribute != NULL){
            printf("Variable not of type record or union.\n");
            //semanticErrors++;
        }
        else if (curr->nodeType == RECORD_OR_UNION && attribute != NULL) {
            for(;fieldList!=NULL;fieldList->next){
                if(strcmp(attribute->lex, fieldList->token->lexeme) == 0){
                    return createINode(NULL,func,fieldList->type,false,&localOffset);
                }
            }
            if(fieldList == NULL) {
                printf("Field not present in the record.\n");
                // semanticErrors++;
                return NULL;
            }
        }
        else if (curr->nodeType != RECORD_OR_UNION && attribute == NULL){
            return currNode;
        }
    }

    //operator node
    else if(isOperator(curr) == true) {

        ast* fChild, *sChild;
        fChild = curr->firstChild;
        sChild = fChild->nextSibling;
        //ast has 1 or no children
        if(fChild==NULL || sChild == NULL) {
            return;
        }

        identifierNode *fNode, *sNode;
        fNode = retrieve(SymbolTable, fNode, fChild->nodeType);
        sNode = retrieve(SymbolTable, sNode, sChild->nodeType);
        //node(s) not found in symbol table
        if(fNode==NULL || sNode == NULL) {
            return;
        }
        Type fType = fNode->type;
        Type sType = sNode->type;

        if(isOperator(fChild) == true){
            fNode = validateArithmetic(fChild, func);
            fType = fNode->type;
        }
        else if (isOperator(sChild) == true){
            sNode = validateArithmetic(sChild, func);
            sType = sNode->type;
        }

        //both are identifiers
        else if(isOperator(fChild) == false && isOperator(sChild) == false) {

            //both int/real
            if(isIntReal(fChild) == true && isIntReal(sChild) == true){
                //both int, both real;
                if(fType == sType) {
                    return createINode(fChild, func, fChild->nodeType, false, &localOffset);
                }
                //int with real operations  
                else {
                    return NULL;
                }
            }

            //both records/unions
            else if(fChild->nodeType == RECORD_OR_UNION && sChild->nodeType == RECORD_OR_UNION) {
                if(fType == sType){
                    //record
                    if(fType == RECORD_TYPE && fNode->recordList == sNode->recordList) {
                        return fNode;
                    }
                    //union
                    if(fType == UNION_TYPE && fNode->recordList == sNode->recordList){
                        return fNode;
                    }
                }
                //operations bw rec and union
                else {
                    return NULL;
                }
            }

            //record/union and int/real
            else {
                if(curr->nodeType != DIVIDE){
                    if(fType == RECORD_TYPE) {
                        return fNode;
                    }
                    else if (fType == UNION_TYPE) {
                        return fNode;
                    }
                    else if(sType == RECORD_TYPE) {
                        return sNode;
                    }
                    else if (sType == UNION_TYPE) {
                        return sNode;
                    }
                }
                else{
                    if(sChild->nodeType == RECORD_OR_UNION){
                        printf("Can't divide scalar by record/union");
                        //semantic_errors++;
                        return NULL;
                    }
                    else{
                        if(fType == RECORD_TYPE) {
                            return fNode;
                        }
                        else if (fType == UNION_TYPE) {
                            return fNode;
                    }
                }
            }   
        }

        // end of both identifiers
    }

    }

    else {
        return NULL;
    }
}

void validateFunCall (ast* call, ast* func) {
    ast* opCall = call->firstChild;
    ast* funcIdentifier = opCall->nextSibling;
    ast* ipCall = funcIdentifier->nextSibling;
    int localOffset = 0;
    // no output parameters
    if(opCall->firstChild == NULL) {
        return NULL;
    }

    //fetching calling func node from ST
    functionNode* callingFunctionNode = createFnode(func); //to be modified
    functionNode* tempCalling = callingFunctionNode;
    callingFunctionNode = retrieve(SymbolTable, callingFunctionNode, call->nodeType);
    free(tempCalling);

    //fetching called func node from ST
    functionNode* calledFunctionNode = createFNode(funcIdentifier); //to be modified
    functionNode* tempCalled = calledFunctionNode;
    calledFunctionNode = retrieve(SymbolTable, calledFunctionNode, funcIdentifier->nodeType);
    free(tempCalled);

    //calling a non-existing function
    if(calledFunctionNode == NULL){
        printf("No such function exists.\n");
        // semanticerrors++;
        return NULL;
    }

    //function call inside function call; 
    if(calledFunctionNode->token == callingFunctionNode->token) {
        printf("Recusion not allowed in functions.\n");
        // semanticErrors++;
        return NULL;
    }

    //calling before definition
    if(calledFunctionNode->rank > callingFunctionNode->rank) {
        printf("Function called before being defined.\n");
        // semanticErrors++;
        return NULL;
    }

    ast* opGivenPars = opCall->firstChild;
    parameters* opSTpars = calledFunctionNode->opParams;
    while (opSTpars != NULL && opGivenPars != NULL) {
        
    }

}