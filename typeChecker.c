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
            //semanticErrors ++;
            printf("Undeclred Variable\n");
            return NULL;
        }
        
        // free(notGlobalCurr);
        // free(globalCurr);
        
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

Type TypeUsingAST(NodeType nt) {

    NodeType currType = nt;
    if(currType == RECORD_OR_UNION) {
        if(curr->is_union == true) {
                return UNION_TYPE;
        }
        else {
            return RECORD_TYPE;
        }
    }
    else if (currType == INTEGER) {
        return INT_TYPE;
    }
    else {
        return REAL_TYPE;
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
        //ast* id, ast* func, Type type, bool is_global, int*offset

        //retrieve from ST node corresponding to AST
        Type opGivenType = TypeUsingAST(opGivenPars->nodeType);
        identifierNode* opGivenNode = createINode(opGivenPars, func, opGivenType, false, &localOffset);
        identifierNode* notGlobalOPgiven = opGivenNode;
        notGlobalOPgiven->global = false;
        notGlobalOPgiven = retrieve(SymbolTable, notGlobalOPgiven, opGivenPars->nodeType);
        identifierNode* globalOPgiven = opGivenNode;
        globalOPgiven->global = true;
        globalOPgiven = retrieve(SymbolTable, globalOPgiven, opGivenPars->nodeType);

        if(globalOPgiven){
            opGivenNode = globalOPgiven;
        }
        else if(notGlobalOPgiven){
            opGivenNode = notGlobalOPgiven;
        }
        else {
            opGivenNode = NULL;
        }

        if(opGivenNode == NULL) {
            printf("Undeclared Variable\n");
            //semanticErrors++;
            return NULL;
        }

        Type opSTtype = TypeUsingAST(opSTpars->nodeType);
        
        //initialising identifier node
        identifierNode* opSTnode;
        opSTnode->width = 0;
        opSTnode->offset = 0;
        opSTnode->type = opSTtype;
        opSTnode->token = opSTpars->token;
        opSTnode->function = calledFunctionNode->token;
        opSTnode->recordList = NULL;
        opSTnode->unionList = NULL;
        opSTnode->global = false;
        opSTnode->assigned = false;
        opSTnode->next = NULL; 

        identifierNode* tempOPST = opSTnode;
        opSTnode = retrieve(SymbolTable, opSTnode, opSTpars->nodeType);
        free(tempOPST);

        if(opSTnode->type == opGivenNode->type) {
            Type nType = opSTnode->type;
            switch(nType) {
                case RECORD_TYPE:
                {
                    if(opSTnode->recordList != opGivenNode->recordList) {
                        printf("Type mismatch in output of function call.\n");
                        //semanticErrors++;
                        //return NULL;
                    }
                    else {
                        //correct match for type
                    }
                    break;
                }
                case UNION_TYPE:
                {
                    if(opSTnode->unionList != opGivenNode->unionList) {
                        printf("Type Mismatch in output of function call.\n");
                        //semanticErrors++;
                        //return NULL;
                    }
                    else {
                        //correct type match
                    }
                    break;
                }
            }
        }
        else {
            printf("Type Mismatch in output of function call.\n");
            //semanticErrors++;
            //return NULL;
        }

        opGivenPars = opGivenPars->nextSibling;
        opSTpars = opSTpars->next;
    }

    if(opGivenPars != NULL) {
        printf("Function call has extra output parameters\n");
        //semanticErrors++;
        //return NULL;
    }

    if(opSTpars != NULL) {
        printf("Function call has insufficient output parameters.\n");
        //semanticErrors++;
        //return NULL;
    }

    //INPUT PARAMETERS 

    ast* ipGivenPars = ipCall->firstChild;
    parameters* ipSTpars = calledFunctionNode->ipParams;

    while (ipSTpars != NULL && ipGivenPars != NULL) {
        //ast* id, ast* func, Type type, bool is_global, int*offset

        //retrieve from ST node corresponding to AST
        Type ipGivenType = TypeUsingAST(ipGivenPars->nodeType);
        identifierNode* ipGivenNode = createINode(ipGivenPars, func, ipGivenType, false, &localOffset);
        identifierNode* notGlobalIPgiven = ipGivenNode;
        notGlobalIPgiven->global = false;
        notGlobalIPgiven = retrieve(SymbolTable, notGlobalIPgiven, ipGivenPars->nodeType);
        identifierNode* globalIPgiven = ipGivenNode;
        globalIPgiven->global = true;
        globalIPgiven = retrieve(SymbolTable, globalIPgiven, ipGivenPars->nodeType);

        if(globalIPgiven){
            ipGivenNode = globalIPgiven;
        }
        else if(notGlobalIPgiven){
            ipGivenNode = notGlobalIPgiven;
        }
        else {
            ipGivenNode = NULL;
        }

        if(ipGivenNode == NULL) {
            printf("Undeclared Variable in function call input\n");
            //semanticErrors++;
            return NULL;
        }

        Type ipSTtype = TypeUsingAST(ipSTpars->nodeType);
        
        //initialising identifier node
        identifierNode* ipSTnode;
        ipSTnode->width = 0;
        ipSTnode->offset = 0;
        ipSTnode->type = ipSTtype;
        ipSTnode->token = ipSTpars->token;
        ipSTnode->function = calledFunctionNode->token;
        ipSTnode->recordList = NULL;
        ipSTnode->unionList = NULL;
        ipSTnode->global = false;
        ipSTnode->assigned = false;
        ipSTnode->next = NULL; 

        identifierNode* tempIPST = ipSTnode;
        ipSTnode = retrieve(SymbolTable, ipSTnode, ipSTpars->nodeType);
        free(tempIPST);

        if(ipSTnode->type == ipGivenNode->type) {
            Type nType = ipSTnode->type;
            switch(nType) {
                case RECORD_TYPE:
                {
                    if(ipSTnode->recordList != ipGivenNode->recordList) {
                        printf("Type mismatch in input of function call.\n");
                        //semanticErrors++;
                        //return NULL;
                    }
                    else {
                        //correct match for type
                    }
                    break;
                }
                case UNION_TYPE:
                {
                    if(ipSTnode->unionList != ipGivenNode->unionList) {
                        printf("Type Mismatch in input of function call.\n");
                        //semanticErrors++;
                        //return NULL;
                    }
                    else {
                        //correct type match
                    }
                    break;
                }
            }
        }
        else {
            printf("Type Mismatch in input of function call.\n");
            //semanticErrors++;
            //return NULL;
        }

        ipGivenPars = ipGivenPars->nextSibling;
        ipSTpars = ipSTpars->next;
    }

    if(ipGivenPars != NULL) {
        printf("Function call has extra input parameters\n");
        //semanticErrors++;
        //return NULL;
    }

    if(ipSTpars != NULL) {
        printf("Function call has insufficient input parameters.\n");
        //semanticErrors++;
        //return NULL;
    }

}

void validateRead(ast* curr, ast* func) {
    curr = curr->firstChild;
    int localOffset = 0;
    Type currType = TypeUsingAST(curr->nodeType);
    identifierNode* currNode = createINode(curr, func, currType, false, &localOffset);    
    identifierNode* notGlobalCurr = currNode;
    notGlobalCurr->global = false;
    notGlobalCurr = retrieve(SymbolTable, notGlobalCurr, curr->nodeType);
    identifierNode* globalCurr = currNode;
    globalCurr->global = true;
    globalCurr = retrieve(SymbolTable, globalCurr, curr->nodeType);

    if(globalCurr){
            currNode = globalCurr;
        }
    else if(notGlobalCurr){
        currNode = notGlobalCurr;
    }
    else {
        currNode = NULL;
        printf("Undeclared Variable in read.\n");
        //semanticErrors++;
        return NULL;
    }

    if(curr->firstChild != NULL) {
        if(curr->nodeType != RECORD_OR_UNION) {
            printf("Variable is not of type record/union.");
            //semanticErrors++:
            return NULL;
        }
        else if (currNode->type == RECORD_TYPE) {
            //check for Nested recods type;
            // if no match-- semnatic error
        }
        else if (currNode->type == UNION_TYPE) {
            // check for nested union type;
            // if no match, semantic error
        }
    }
}

void validateWrite(ast* curr, ast* func) {
    curr = curr->firstChild;
    int localOffset = 0;
    Type currType = TypeUsingAST(curr->nodeType);
    identifierNode* currNode = createINode(curr, func, currType, false, &localOffset);    
    identifierNode* notGlobalCurr = currNode;
    notGlobalCurr->global = false;
    notGlobalCurr = retrieve(SymbolTable, notGlobalCurr, curr->nodeType);
    identifierNode* globalCurr = currNode;
    globalCurr->global = true;
    globalCurr = retrieve(SymbolTable, globalCurr, curr->nodeType);

    if(globalCurr){
            currNode = globalCurr;
        }
    else if(notGlobalCurr){
        currNode = notGlobalCurr;
    }
    else {
        currNode = NULL;
        printf("Undeclared Variable in read.\n");
        //semanticErrors++;
        return NULL;
    }

    if(curr->firstChild != NULL) {
        if(curr->nodeType != RECORD_OR_UNION) {
            printf("Variable is not of type record/union.");
            //semanticErrors++:
            return NULL;
        }
        else if (currNode->type == RECORD_TYPE) {
            //check for Nested recods type;
            // if no match-- semnatic error
        }
        else if (currNode->type == UNION_TYPE) {
            // check for nested union type;
            // if no match, semantic error
        }
    }
}

void validateIterative(ast* curr) {
    validateBoolean(curr->firstChild);
    ast* temp = curr->firstChild->nextSibling;
    for(; temp != NULL; temp = temp->nextSibling) {
        handleStmt(temp);
    }
}

int isRelOp(ast* curr) {
    if(curr->nodeType == LESS_THAN || curr->nodeType == LESS_EQUAL || curr->nodeType == EQUAL || curr->nodeType == GREATER_THAN || curr->nodeType == GREATER_EQUAL) {
        return 1;
    }
    else {
        return 0;
    }
}

identifierNode* validateBoolean(ast* curr, ast* func) {
    int localOffset = 0;
    if(curr->nodeType == AND || curr->nodeType == OR) {
        validateBoolean(curr->firstChild, func);
        validateBoolean(curr->firstChild->nextSibling, func);
    }
    else if(isRelOp(curr)) {
        //      relop
        //     /
        //    /
        //   ID---->ID
        ast* fChild = curr->firstChild;
        ast* sChild = curr->firstChild->nextSibling;
        if(isIntReal(fChild) == true && isIntReal(sChild) == true) {
            if(fChild->nodeType == sChild->nodeType) {
                Type type = (fChild->nodeType == INTEGER) ? INT_TYPE : REAL_TYPE;
                return createINode(NULL, func, type, false, &localOffset);
            }
            else {
                printf("Operand Type error mismatch on Line no. %d\n", curr->line);
            }
        } 
        else {
            identifierNode* fNode, *sNode;
            if(fChild->nodeType == ID && sChild->nodeType == ID) {
                //create temp nodes to lookup in symbol table
                identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                fNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                if(fNode == NULL) {
                    fNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(fNode == NULL) {
                        printf("Using undeclared identifier %s on line no. %d\n", fChild->lex, fChild->line);
                    }
                }
                t1 = createINode(sChild, func, -1, false, &localOffset);
                t1_global = createINode(sChild, func, -1, true, &localOffset);
                sNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                if(sNode == NULL) {
                    sNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(sNode == NULL) {
                        printf("Using undeclared identifier %s on line no. %d\n", sChild->lex, sChild->line);
                        return NULL;
                    }
                }

                if((fNode->type == INT_TYPE || fNode->type == REAL_TYPE) && (sNode->type == INT_TYPE || sNode->type == REAL_TYPE)) {
                    if(fNode->type == sNode->type) {
                        return createINode(NULL, func, fNode->type, false, &localOffset);
                    }
                    else {
                        printf("Operand Type error mismatch on Line no. %d\n", curr->line);
                    }
                }

                else {
                    //the node is a record or union
                    if(fChild->firstChild == NULL) {
                        printf("Line no. %d: Record or Union type can not be used with relational operator", fChild->line);
                    }
                    if(sChild->firstChild == NULL) {
                        printf("Line no. %d: Record or Union type can not be used with relational operator", sChild->line);
                        return NULL;
                    }

                    //this part will only be reached if there is an expansion
                    //iterate through the node, retrieve their node in each loop
                    //call searchInFieldList, if it is present then proceed
                    ast* fFieldId = fChild->firstChild;
                    ast* sFieldId = sChild->firstChild;
                    identifierNode* currNode = fNode;
                    while(fFieldId) {
                        recordUnionNode* temp = (currNode->recordList == NULL) ? currNode->unionList : currNode->recordList;
                        if(temp == NULL && fFieldId->nextSibling != NULL) {
                            printf("Line no. %d: Identifiers of type int or real can not have field IDs\n", fFieldId->line);
                            break;
                        }
                        recordField* currField = searchInFieldList(fFieldId, temp->fieldList);
                        if(currField == NULL) {
                            printf("Line no. %d: No such field identifier %s\n", fFieldId->line, fFieldId->lex);
                        }
                        identifierNode* retNode = (identifierNode*)malloc(sizeof(identifierNode));
                        retNode->width = currField->width;
                        retNode->offset = -1;
                        retNode->type = currField->type;
                        retNode->token = (tokenInfo*)malloc(sizeof(tokenInfo));
                        retNode->token->lexeme = currField->token->lexeme;
                        retNode->token->tid = currField->token->tid;
                        retNode->token->numVal = currField->token->numVal;
                        retNode->token->lineNo = currField->token->lineNo;
                        currNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                        fFieldId = fFieldId->nextSibling;
                    }

                    currNode = sNode;
                    while(sFieldId) {
                        recordUnionNode* temp = (currNode->recordList == NULL) ? currNode->unionList : currNode->recordList;
                        if(temp == NULL && sFieldId->nextSibling != NULL) {
                            printf("Line no. %d: Identifiers of type int or real can not have field IDs\n", fFieldId->line);
                            return NULL;
                        }
                        recordField* currField = searchInFieldList(fFieldId, temp->fieldList);
                        if(currField == NULL) {
                            printf("Line no. %d: No such field identifier %s\n", sFieldId->line, sFieldId->lex);
                        }
                        identifierNode* retNode = (identifierNode*)malloc(sizeof(identifierNode));
                        retNode->width = currField->width;
                        retNode->offset = -1;
                        retNode->type = currField->type;
                        retNode->token = (tokenInfo*)malloc(sizeof(tokenInfo));
                        retNode->token->lexeme = currField->token->lexeme;
                        retNode->token->tid = currField->token->tid;
                        retNode->token->numVal = currField->token->numVal;
                        retNode->token->lineNo = currField->token->lineNo;
                        currNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                        sFieldId = sFieldId->nextSibling;
                    }

                    //currently fFieldId and sFieldId are at the end
                    //they have to be INTEGER or REAL 


                }
            }
        }
    }
    else if(curr->nodeType == NOT_EQUAL) {
        return validateBoolean(curr->firstChild, func);
    }
}

void validConditional(ast* curr) {

}

void handleStmt(ast* curr) {
    switch(curr->nodeType) {
        case ASSIGNOP:
            //TODO: write the correct order of calling
            validateArithmetic(curr, NULL);
            break;
        case ITERATIVE:
            validateIterative(curr);
            break;
        case CONDITIONAL:
            validateConditional(curr);
            break;
    }
}