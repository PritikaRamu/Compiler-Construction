#include "typeChecker.h"

identifierNode* validateBoolean(ast* curr, ast* func);
void handleStmt(ast* curr, ast* func);
void validateConditional(ast* curr, ast* func);
recordField* searchInFieldList(ast* curr, recordField* fieldList);
void validateReturn(ast* curr, ast* func);
void validateFunction(ast* curr);

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

Type TypeUsingAST(NodeType currType, bool is_union) {

    if(currType == RECORD_OR_UNION) {
        if(is_union) {
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

void semanticAnalyser(ast* root){
    //populate symbol tables
    ast * curr = root->firstChild;

    if(curr==NULL)
        return;
    printf("In semantic analyser, starting while loop\n");
    while(curr){
        validateFunction(curr);
        curr = curr->nextSibling;
    }
    printf("Exited Semantic Analyser while loop\n");
}

// void validateStmts(ast* curr, functionNode* currNode, bool* opAssigned){

// }
bool isOperator(ast* curr) {
    printf("In isOperator, current lex: %s\n", curr->lex);
    NodeType nType = curr->nodeType;
    printf("Idhar segfault thodi ho sakta?\n");
    if(nType == DIVIDE || nType == MULTIPLY || nType == PLUS || nType == MINUS) {
        return true;
    }
    else {
        printf("Idhar segfault thodi ho sakta?\n");
        return false;
    }
}

bool isNumRnum(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == NUM || nType == RNUM) {
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

void validateAssign(ast* curr, ast* func) {
    int localOffset = 0; 
    printf("In validateAssign, entering valArith, curr->firstChild->nextSibling lexeme: %s and func: %s and NodeType: %d\n", curr->firstChild->nextSibling->lex, func->lex, curr->firstChild->nextSibling->nodeType);
    identifierNode* ArithNode = validateArithmetic(curr->firstChild->nextSibling, func);
    printf("In valAss Exited valArith successfully for lexeme: %s and NodeType: %d\n", curr->firstChild->nextSibling->lex, curr->firstChild->nextSibling->nodeType);
    if(ArithNode != NULL) {
        //printf("In valAss Exited valArith successfully with lexeme: %s and Type: %d\n", ArithNode->token->lexeme, ArithNode->type);
        printf("Here hoon\n"); 
    }
    else {
        printf("ArithNode returned NULL from valArith\n");
    }
    ast* lChild = curr->firstChild;
    //Type lChildType = TypeUsingAST(lChild->nodeType, lChild->is_union);
    printf("This lexeme in valAss: %s is in scope of function: %s with nodeType: %d\n", curr->lex, func->lex, curr->nodeType);
    identifierNode* lChildNode = createINode(lChild, func, -1, false, &localOffset);

    //identifierNode* tempLChild = lChildNode;
    lChildNode = (identifierNode*)retrieve(SymbolTable, lChildNode, ID);
    printf("Chcekpoint 1 post retrieve in ValAssign.\n");

    if(lChildNode == NULL) {
        identifierNode* lChildNode_global = createINode(lChild, func, -1, true, &localOffset);
        lChildNode_global = (identifierNode* ) retrieve(SymbolTable, lChildNode, ID);
        printf("Chcekpoint 2 post retrieve in ValAssign.\n");

        if(lChildNode_global == NULL) {
            // semanticErrors ++;
            printf("Line no. %d: Undeclared Variable HERE IN VALass %s in Assignment.\n", lChild->line, lChild->lex);
            return;
        }
        else {
            lChildNode = lChildNode_global;
            printf("Retrived global identifier for lex: %s, type: %d\n", lChildNode_global->token->lexeme, lChildNode_global->type);
        }
    }
    else {
        printf("Retrived non global identifier for lex: %s type: %d nodetype: %d\n", lChildNode->token->lexeme, lChildNode->type, lChild->nodeType);
    }

    // identifierNode* notGlobalCurr = lChildNode;
    // notGlobalCurr->global = false;
    // notGlobalCurr = (identifierNode*)retrieve(SymbolTable, notGlobalCurr, ID);
    // printf("Chcekpoint 2 post retrieve in ValAssign.\n");

    // //if(notGlobalCurr) printf("Type of notGlobalCurr: %d\n", notGlobalCurr->type);
    // identifierNode* globalCurr = lChildNode;
    // globalCurr->global = true;
    // globalCurr = (identifierNode*)retrieve(SymbolTable, globalCurr, ID);
    // printf("Chcekpoint 3 post retrieve in ValAssign.\n");
    // if (notGlobalCurr)
    // {
    //     lChildNode = notGlobalCurr;
    //     printf("Retrived non global identifier for lex: %s type: %d nodetype: %d\n", lChildNode->token->lexeme, lChildNode->type, lChild->nodeType);
    // }
    // else if (globalCurr)
    // {
    //     printf("Retrived global identifier for lex: %s, type: %d\n", lChildNode->token->lexeme, lChildNode->type);
    //     lChildNode = globalCurr;
    // }
    // else
    // {
    //     // semanticErrors ++;
    //     printf("Line no. %d: Undeclared Variable HERE IN VALass %s in Assignment.\n", lChild->line, lChild->lex);
    //     return;
    // }
    
    printf("Before Arithnode check\n");

    if(ArithNode == NULL) {
        printf("Arithnode returned NULL so returning from valASS\n");
        return;
    }

    // free(notGlobalCurr);
    // free(globalCurr);

    // attribute from AST
    ast *attribute = NULL;
    if (lChild->firstChild != NULL)
    {
        attribute = lChild->firstChild;
    }

    // record/union from symbol table

    if ((lChildNode->type != RECORD_TYPE && lChildNode->type != UNION_TYPE) && attribute != NULL)
    {
        printf("Identifier not of type record/ union but has fields.\n");
        // semanticErrors++;
        return;
    }
    //NESTED RECORDS
    else if ((lChildNode->type == RECORD_TYPE || lChildNode->type == UNION_TYPE) && attribute != NULL)
    {
        Type lChildType = lChildNode->type;

        char* fConcatLex = (char*)malloc(sizeof(char)*strlen(lChild->lex));
        strcpy(fConcatLex, lChild->lex);

        printf("\nConcatenating Second Child in ValAss: \n\n\n\n\n");
        while(attribute) {
            int x = strlen(fConcatLex);
            int y = strlen(attribute->lex);
            fConcatLex = (char*)realloc(fConcatLex, sizeof(char)*(x+y+1));
            char dot[] = ".";
            strcat(fConcatLex, dot);
            strcat(fConcatLex, attribute->lex);
                        
            identifierNode* retNode = createINode(attribute, func, -1, false, &localOffset);
            retNode->token->lexeme = fConcatLex;

            identifierNode* fidNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
            printf("%s\n", fConcatLex);

            if(fidNode == NULL) {
                printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                //semanticErrors++;
                return;
            }
            lChildNode = fidNode;
            attribute = attribute->nextSibling;
        }
        if(lChildNode->type == ArithNode->type) {
            printf("In valAss, type of assigned node: %s match val of arith node: %s\n", lChild->lex, ArithNode->token->lexeme);
        }
        else {
            printf("Line no %d: Type mismatch in assign.\n", lChild->line);
            // semanticErrors++;
            return;
        }
    }
    //int/real
    else if((lChildNode->type != RECORD_TYPE && lChildNode->type != UNION_TYPE))
    {
        Type lChildType = lChildNode->type;
        if (lChildType != ArithNode->type)
        {
            printf("Line no %d: Type mismatch in assign.\n", lChild->line);
            // semanticErrors++;
            return;
        }
        else{
            printf("In valAss, type of assigned node: %s match val of arith node: %s\n", lChild->lex, ArithNode->token->lexeme);
        }
    }
    else if((lChildNode->type == RECORD_TYPE || lChildNode->type == UNION_TYPE) && attribute == NULL) {
        Type lChildType = lChildNode->type;
        if (lChildType != ArithNode->type)
        {
            printf("Line no %d: Type mismatch in assign.\n", lChild->line);
            // semanticErrors++;
            return;
        }
        else{
            printf("In valAss, type of assigned node: %s match val of arith node: %s\n", lChild->lex, ArithNode->token->lexeme);
        }
    }
}

identifierNode* validateArithmetic(ast* curr, ast* func) {
    printf("Entering ValArith, curr lexeme: %s, func lexeme: %s\n", curr->lex, func->lex);
    if (curr == NULL)
    {
        return NULL;
    }
    int localOffset = 0;
    //int numTempVar = 0;

    bool isOp = isOperator(curr);

    //identifier node/ int-real
    if(isOp == 0){
        //num or rnum
        if(curr->nodeType == NUM || curr->nodeType == RNUM){
            if(curr->nodeType == NUM) {
                printf("Arith returns a num with lex: %s\n", curr->lex);
                return createINode(curr, func, INT_TYPE, false, &localOffset);
            }
            else {
                printf("Arith returns a rnum with lex: %s\n", curr->lex);
                return createINode(curr, func, REAL_TYPE, false, &localOffset);
            }
        }
        //id
        printf("\nIn valArith, not an operator, creating a node for ast lexeme: %s\n", curr->lex);
        identifierNode* currNode = createINode(curr, func, -1, false, &localOffset);
        
        if(currNode == NULL) {
            identifierNode* currNode_global = createINode(curr, func, -1, true, &localOffset);
            currNode_global = (identifierNode*)retrieve(SymbolTable, currNode_global, ID);
            if(currNode_global == NULL) {
                printf("Line no. %d: Using undeclared variable %s\n", curr->line, curr->lex);
                return NULL;
            }
            else {
                currNode = currNode_global;
                printf("Retrived global identifier for lex: %s, type: %d\n", currNode_global->token->lexeme, currNode_global->type);
            }
        }
        else {
            printf("Retrived global identifier for lex: %s, type: %d\n", currNode->token->lexeme, currNode->type);
        }

        //printf("ValArith mai createINode hua kya?\n");
        // identifierNode* notGlobalCurr = currNode;
        // notGlobalCurr->global = false;
        // notGlobalCurr = (identifierNode*)retrieve(SymbolTable, notGlobalCurr, curr->nodeType);
        // //printf("ValArith mai retrieve hua kya?\n");
        // identifierNode* globalCurr = currNode;
        // globalCurr->global = true;
        // globalCurr = (identifierNode*)retrieve(SymbolTable, globalCurr, curr->nodeType);
        // if (globalCurr)
        // {
        //     printf("In valArith globalCurr, retrived type is %d for lexeme: %s\n", globalCurr->type, globalCurr->token->lexeme);
        //     currNode = globalCurr;
        // }
        // else if (notGlobalCurr){
        //     printf("In valArith notglobalCurr, retrived type is %d for lexeme: %s\n", globalCurr->type, globalCurr->token->lexeme);
        //     currNode = notGlobalCurr;
        // }
        // else {
        //     //semanticErrors ++;
        //     printf("Line no. %d: Undeclared Variable %s\n", curr->line, curr->lex);
        //     return NULL;
        // }
        
        // free(notGlobalCurr);
        // free(globalCurr);
        
        //attribute from AST
        ast* attribute = NULL;
        if(!curr->firstChild) {
            attribute = curr->firstChild;
        }

        //record/union from symbol table

        //NESTED RECORDS

        else if ((currNode->type == RECORD_TYPE || currNode->type == UNION_TYPE) && attribute != NULL)
        {
            Type currType = currNode->type;
            char* fConcatLex = (char*)malloc(sizeof(char)*strlen(curr->lex));
            strcpy(fConcatLex, curr->lex);

            printf("\nConcatenating Second Child in ValAss: \n\n\n\n\n");
            while(attribute) {
                int x = strlen(fConcatLex);
                int y = strlen(attribute->lex);
                fConcatLex = (char*)realloc(fConcatLex, sizeof(char)*(x+y+1));
                char dot[] = ".";
                strcat(fConcatLex, dot);
                strcat(fConcatLex, attribute->lex);
                            
                identifierNode* retNode = createINode(attribute, func, -1, false, &localOffset);
                retNode->token->lexeme = fConcatLex;

                identifierNode* fidNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                printf("%s\n", fConcatLex);

                if(fidNode == NULL) {
                    printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                    //semanticErrors++;
                    return NULL;
                }
                currNode = fidNode;
                attribute = attribute->nextSibling;
            }
            return currNode;
        }
        else if ((currNode->type == RECORD_TYPE || currNode->type == UNION_TYPE) && attribute == NULL) {
            return currNode;
        }
        else if ((currNode->type != RECORD_TYPE && currNode->type != UNION_TYPE) && attribute != NULL) {
            printf("Line no. %d: Variale %s not of type record.\n", curr->line, curr->lex);
            //semanticErrors++;
            return NULL;

        }
        else if ((currNode->type != RECORD_TYPE || currNode->type != UNION_TYPE) && attribute == NULL){
            return currNode;
        }
    }

    //operator node
    else if(isOp == true) {
        printf("Ye enter hua kya?\n");

        ast* fChild, *sChild;
        fChild = curr->firstChild;
        sChild = fChild->nextSibling;
        //ast has 1 or no children
        if(fChild==NULL || sChild == NULL) {
            return NULL;
        }

        //identifierNode *fNode, *sNode;
        //fNode = (identifierNode*)retrieve(SymbolTable, fNode, fChild->nodeType);
        //sNode = retrieve(SymbolTable, sNode, sChild->nodeType);
        //node(s) not found in symbol table
       
        // Type fType = typeUsingAst(fChild);
        // Type sType = typeUsingAst(sChild);

        // if(isOperator(fChild) == true){
        //     fNode = validateArithmetic(fChild, func);
        //     fType = fNode->type;
        // }
        // else if (isOperator(sChild) == true){
        //     sNode = validateArithmetic(sChild, func);
        //     sType = sNode->type;
        // }

        identifierNode* fNode = validateArithmetic(fChild, func);
        identifierNode* sNode = validateArithmetic(sChild, func);

        if(fNode == NULL || sNode == NULL) {
            return NULL;
        }

        Type fType = fNode->type;
        Type sType = sNode->type;

        if(fType == sType){
            if (fType == RECORD_TYPE) {
                if(fNode->recordList != sNode->recordList) {
                    printf("Line no. %d: Can not perform operations on records of different types\n", fNode->token->lineNo);
                    //semanticErrors++;
                    return NULL;
                }
                else {
                    printf("Type matches for record in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                    return fNode;
                }
            }
            if (fType == UNION_TYPE) {
                if(fNode->unionList != sNode-> unionList) {
                    printf("Line no. %d: Can not perform operations on unions of different types\n", fNode->token->lineNo);
                    //semanticErrors++;
                    return NULL;
                }
                else {
                    printf("Type matches for union in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                    return fNode;
                }
            }
            else if (fType == INT_TYPE || fType == REAL_TYPE) {
                printf("Type matches for Int/Real in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                return fNode;
            }
        }
        //mismatched type
        else {
            if(fType == INT_TYPE) {
                if(sType == REAL_TYPE) {
                    printf("Line no. %d: Type mismatch operand %s is of type int and %s is of type real\n", fChild->line, fChild->lex, sChild->lex);
                    return NULL;
                }
                else if(sType == RECORD_TYPE || sType == UNION_TYPE) {
                    if(curr->nodeType == PLUS || curr->nodeType == MINUS || curr->nodeType == MULTIPLY) {
                        return sNode;
                    }
                }
            }
            else if(fType == REAL_TYPE) {
                if(sType == INT_TYPE) {
                    printf("Line no. %d: Type mismatch operand %s is of type int and %s is of type real\n", fChild->line, fChild->lex, sChild->lex);
                    return NULL;
                }
                else if(sType == RECORD_TYPE || sType == UNION_TYPE) {
                    if(curr->nodeType == PLUS || curr->nodeType == MINUS) {
                        return sNode;
                    }
                }
            }
            else if(fType == RECORD_TYPE || fType == UNION_TYPE) {
                if(sType == INT_TYPE) {
                    if(curr->nodeType == PLUS || curr->nodeType == MINUS || curr->nodeType == MULTIPLY) {
                        //printf("Line no. %d: Type mismatch operand %s is of type int and %s is of type real\n", fChild->line, fChild->lex, sChild->lex);
                        return fNode;
                    }
                    else {
                        printf("Line no. %d: Division not allowed with type record\n", fChild->line);
                        return NULL;
                    }
                }
                else if(sType == UNION_TYPE || sType == RECORD_TYPE) {
                    printf("Line no. %d: Type mismatch operand %s is of type Record and %s is of type Union\n", fChild->line, fChild->lex, sChild->lex);
                    return NULL;
                }
                else if(sType == REAL_TYPE) {
                    if(curr->nodeType == PLUS || curr->nodeType == MINUS) {
                        return fNode;
                    }
                    else {
                        printf("Line no. %d: Division or multiplication not allowed with type record and real\n", fChild->line);
                        return NULL;
                    }
                }
            }
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
        return;
    }

    //fetching calling func node from ST
    functionNode* callingFunctionNode = createFNode(func); //to be modified
    functionNode* tempCalling = callingFunctionNode;
    callingFunctionNode =(functionNode *)retrieve(SymbolTable, callingFunctionNode, FUNCTION_SEQ);
    free(tempCalling);

    //fetching called func node from ST
    functionNode* calledFunctionNode = createFNode(funcIdentifier); //to be modified
    functionNode* tempCalled = calledFunctionNode;
    calledFunctionNode = (functionNode*)retrieve(SymbolTable, calledFunctionNode, FUNCTION_SEQ);
    free(tempCalled);

    //calling a non-existing function
    if(calledFunctionNode == NULL){
        printf("No such function exists.\n");
        // semanticerrors++;
        return;
    }

    //function call inside function call; 
    if(calledFunctionNode->token == callingFunctionNode->token) {
        printf("Recusion not allowed in functions.\n");
        // semanticErrors++;
        return;
    }

    //calling before definition
    if(calledFunctionNode->rank > callingFunctionNode->rank) {
        printf("Function called before being defined.\n");
        // semanticErrors++;
        return;
    }

    ast* opGivenPars = opCall->firstChild;
    parameters* opSTpars = calledFunctionNode->opParams;

    while (opSTpars != NULL && opGivenPars != NULL) {
        //ast* id, ast* func, Type type, bool is_global, int*offset

        //retrieve from ST node corresponding to AST
        Type opGivenType = TypeUsingAST(opGivenPars->nodeType, opGivenPars->is_union);
        
        identifierNode* opGivenNode = createINode(opGivenPars, func, opGivenType, false, &localOffset);
        opGivenNode = (identifierNode*)retrieve(SymbolTable, opGivenNode, ID);

        if (opGivenNode == NULL)
        {
            identifierNode *opGivenNode_global = createINode(opGivenPars, func, -1, true, &localOffset);
            opGivenNode_global = (identifierNode *)retrieve(SymbolTable, opGivenPars, ID);
            printf("Chcekpoint 2 post retrieve in ValFunCall.\n");

            if (opGivenNode_global == NULL)
            {
                // semanticErrors ++;
                printf("Line no. %d: Undeclared given op Variable HERE IN ValFunCall %s.\n", opGivenPars->line, opGivenPars->lex);
                return;
            }
            else
            {
                opGivenNode = opGivenNode_global;
                printf("Retrived global identifier for given op lex: %s, type: %d in ValFunCall\n", opGivenNode_global->token->lexeme, opGivenNode_global->type);
            }
        }
        else
        {
            printf("Retrived non global identifier for given op lex: %s type: %d nodetype: %d in ValFunCall\n", opGivenNode->token->lexeme, opGivenNode->type, opGivenPars->nodeType);
        }

        Type opSTtype = TypeUsingAST(opSTpars->nodeType, opSTpars->is_union);
        
        //initialising identifier node
        identifierNode* opSTnode = (identifierNode*)malloc(sizeof(identifierNode));
        opSTnode->width = 0;
        opSTnode->offset = 0;
        opSTnode->type = opSTtype;
       
        opSTnode->token = (tokenInfo*)malloc(sizeof(tokenInfo));
        opSTnode->token = opSTpars->token;

        opSTnode->function = (tokenInfo*)malloc(sizeof(tokenInfo));
        opSTnode->function = calledFunctionNode->token;
        opSTnode->recordList = NULL;
        opSTnode->unionList = NULL;
        opSTnode->global = false;
        opSTnode->assigned = false;
        opSTnode->next = NULL; 

        identifierNode* tempOPST = opSTnode;
        opSTnode = (identifierNode*)retrieve(SymbolTable, opSTnode, ID);
        free(tempOPST);

        if(opSTnode->type == opGivenNode->type) {
            Type nType = opSTnode->type;
            switch(nType) {
                case RECORD_TYPE:
                {
                    if(opSTnode->recordList != opGivenNode->recordList) {
                        printf("Type mismatch in output of function call.\n");
                        //semanticErrors++;
                        //return;
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
                        //return;
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
            //return;
        }

        opGivenPars = opGivenPars->nextSibling;
        opSTpars = opSTpars->next;
    }

    if(opSTpars == NULL && opGivenPars == NULL) {
        
    }
    else if(opGivenPars != NULL) {
        printf("Line no. %d: Function call has extra output parameters\n", opGivenPars->line);
        //semanticErrors++;
        //return;
    }
    else if(opSTpars != NULL) {
        printf("Line no. %d: Function call has insufficient output parameters.\n", opGivenPars->line);
        //semanticErrors++;
        //return;
    }

    //INPUT PARAMETERS 

    ast* ipGivenPars = ipCall->firstChild;
    parameters* ipSTpars = calledFunctionNode->ipParams;

    while (ipSTpars != NULL && ipGivenPars != NULL) {
        //ast* id, ast* func, Type type, bool is_global, int*offset

        //retrieve from ST node corresponding to AST
        Type ipGivenType = TypeUsingAST(ipGivenPars->nodeType, ipGivenPars->is_union);
        identifierNode* ipGivenNode = createINode(ipGivenPars, func, ipGivenType, false, &localOffset);
        ipGivenNode = (identifierNode*)retrieve(SymbolTable, ipGivenNode, ID);

        if (ipGivenNode == NULL)
        {
            identifierNode *ipGivenNode_global = createINode(ipGivenPars, func, -1, true, &localOffset);
            ipGivenNode_global = (identifierNode *)retrieve(SymbolTable, ipGivenPars, ID);
            printf("Chcekpoint 2 post retrieve in ValFunCall.\n");

            if (ipGivenNode_global == NULL)
            {
                // semanticErrors ++;
                printf("Line no. %d: Undeclared given ip Variable HERE IN ValFunCall %s.\n", ipGivenPars->line, ipGivenPars->lex);
                return;
            }
            else
            {
                ipGivenNode = ipGivenNode_global;
                printf("Retrived global identifier for given ip lex: %s, type: %d in ValFunCall %d.\n", ipGivenNode_global->token->lexeme, ipGivenNode_global->type,ipGivenPars->nodeType);
            }
        }
        else
        {
            printf("Retrived non global identifier for ip lex: %s type: %d nodetype: %d in ValFun Call\n", ipGivenNode->token->lexeme, ipGivenNode->type, ipGivenPars->nodeType);
        }

        Type ipSTtype = TypeUsingAST(ipSTpars->nodeType, ipSTpars->is_union);
        
        //initialising identifier node
        identifierNode* ipSTnode = (identifierNode*)malloc(sizeof(identifierNode));
        ipSTnode->width = 0;
        ipSTnode->offset = 0;
        ipSTnode->type = ipSTtype;
        
        ipSTnode->token = (tokenInfo *)malloc(sizeof(tokenInfo));
        ipSTnode->token = ipSTpars->token;
        
        ipSTnode->function = (tokenInfo *)malloc(sizeof(tokenInfo));
        ipSTnode->function = calledFunctionNode->token;
        ipSTnode->recordList = NULL;
        ipSTnode->unionList = NULL;
        ipSTnode->global = false;
        ipSTnode->assigned = false;
        ipSTnode->next = NULL; 

        identifierNode* tempIPST = ipSTnode;
        ipSTnode = (identifierNode*)retrieve(SymbolTable, ipSTnode, ID);
        free(tempIPST);

        printf("Is this being printed when ipst\n");
        if(ipSTnode->type == ipGivenNode->type) {
            Type nType = ipSTnode->type;
            switch(nType) {
                case RECORD_TYPE:
                {
                    if(ipSTnode->recordList != ipGivenNode->recordList) {
                        printf("Type mismatch in input of function call.\n");
                        //semanticErrors++;
                        //return;
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
                        //return;
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
            //return;
        }

        ipGivenPars = ipGivenPars->nextSibling;
        ipSTpars = ipSTpars->next;
    }

    if(ipGivenPars != NULL) {
        printf("Function call has extra input parameters\n");
        //semanticErrors++;
        //return;
    }

    if(ipSTpars != NULL) {
        printf("Function call has insufficient input parameters.\n");
        //semanticErrors++;
        //return;
    }

}

void validateRead(ast* curr, ast* func) {
    curr = curr->firstChild;
    int localOffset = 0;
    Type currType = TypeUsingAST(curr->nodeType, curr->is_union);
    identifierNode* currNode = createINode(curr, func, currType, false, &localOffset);    
    currNode = (identifierNode*)retrieve(SymbolTable, currNode, ID);

    if (currNode == NULL)
    {
        identifierNode *currNode_global = createINode(curr, func, -1, true, &localOffset);
        currNode_global = (identifierNode *)retrieve(SymbolTable, curr, ID);
        printf("Chcekpoint 2 post retrieve in ValRead.\n");

        if (currNode_global == NULL)
        {
            // semanticErrors ++;
            printf("Line no. %d: Undeclared Variable HERE %s in ValRead.\n", curr->line, curr->lex);
            return;
        }
        else
        {
            currNode = currNode_global;
            printf("Retrived global identifier for lex: %s, type: %d in ValRead\n", currNode_global->token->lexeme, currNode_global->type);
        }
    }
    else
    {
        printf("Retrived non global identifier for lex: %s type: %d nodetype: %d in ValRead\n", currNode->token->lexeme, currNode->type, curr->nodeType);
    }

    bool flagNumRnum = isNumRnum(curr);
    if(flagNumRnum == true) {
        printf("Line no: %d: Trying to read into a Num or Rnum for lex: %s", curr->line, curr->lex);
        //semanticErrors++;
        return;
    }

    if(curr->firstChild != NULL) {
        if((currNode->type != RECORD_TYPE && currNode->type != UNION_TYPE)) {
            printf("Variable is not of type record/union.");
            //semanticErrors++:
            return;
        }
        else if (currNode->type == RECORD_TYPE || currNode->type == UNION_TYPE) {
            ast* attribute = curr->firstChild;
            //check for Nested recods type;
            // if no match-- semnatic error
            char *fConcatLex = (char *)malloc(sizeof(char) * strlen(curr->lex));
            strcpy(fConcatLex, curr->lex);
            printf("\nConcatenating Second Child in ValRead: \n\n\n\n\n");
            while (attribute) {
                {
                    int x = strlen(fConcatLex);
                    int y = strlen(attribute->lex);
                    fConcatLex = (char *)realloc(fConcatLex, sizeof(char) * (x + y + 1));
                    char dot[] = ".";
                    strcat(fConcatLex, dot);
                    strcat(fConcatLex, attribute->lex);

                    identifierNode *retNode = createINode(attribute, func, -1, false, &localOffset);
                    retNode->token->lexeme = fConcatLex;

                    identifierNode *fidNode = (identifierNode *)retrieve(SymbolTable, retNode, ID);
                    printf("%s\n", fConcatLex);

                    if (fidNode == NULL)
                    {
                        printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                        // semanticErrors++;
                        return;
                    }
                    attribute = attribute->nextSibling;
                }
            }
        }    
    }
}

void validateWrite(ast* curr, ast* func) {
    curr = curr->firstChild;
    int localOffset = 0;
    Type currType = TypeUsingAST(curr->nodeType, curr->is_union);
    identifierNode* currNode = createINode(curr, func, currType, false, &localOffset);

        if (currNode == NULL)
    {
        identifierNode *currNode_global = createINode(curr, func, -1, true, &localOffset);
        currNode_global = (identifierNode *)retrieve(SymbolTable, curr, ID);
        printf("Chcekpoint 2 post retrieve in ValRead.\n");

        if (currNode_global == NULL)
        {
            // semanticErrors ++;
            printf("Line no. %d: Undeclared Variable HERE %s in ValWrite.\n", curr->line, curr->lex);
            return;
        }
        else
        {
            currNode = currNode_global;
            printf("Retrived global identifier for lex: %s, type: %d in ValWrite\n", currNode_global->token->lexeme, currNode_global->type);
        }
    }
    else
    {
        printf("Retrived non global identifier for lex: %s type: %d nodetype: %d in ValWrite\n", currNode->token->lexeme, currNode->type, curr->nodeType);
    }

    if(curr->firstChild != NULL) {
        if((currNode->type != RECORD_TYPE && currNode->type != UNION_TYPE)) {
            printf("Variable is not of type record/union.");
            //semanticErrors++:
            return;
        }
        else if (currNode->type == RECORD_TYPE || currNode->type == UNION_TYPE) {
            ast* attribute = curr->firstChild;
            //check for Nested recods type;
            // if no match-- semnatic error
            char *fConcatLex = (char *)malloc(sizeof(char) * strlen(curr->lex));
            strcpy(fConcatLex, curr->lex);
            printf("\nConcatenating Second Child in ValWrite: \n\n\n\n\n");
            while (attribute) {
                {
                    int x = strlen(fConcatLex);
                    int y = strlen(attribute->lex);
                    fConcatLex = (char *)realloc(fConcatLex, sizeof(char) * (x + y + 1));
                    char dot[] = ".";
                    strcat(fConcatLex, dot);
                    strcat(fConcatLex, attribute->lex);

                    identifierNode *retNode = createINode(attribute, func, -1, false, &localOffset);
                    retNode->token->lexeme = fConcatLex;

                    identifierNode *fidNode = (identifierNode *)retrieve(SymbolTable, retNode, ID);
                    printf("%s\n", fConcatLex);

                    if (fidNode == NULL)
                    {
                        printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                        // semanticErrors++;
                        return;
                    }
                    attribute = attribute->nextSibling;
                }
            }
        }    
    }
}

void validateIterative(ast* curr, ast* func) {
    printf("Entered valIter with current lexeme: %s, and function lexeme: %s\n", curr->lex, func->lex);
    validateBoolean(curr->firstChild, func);
    ast* temp = curr->firstChild->nextSibling;
    for(; temp != NULL; temp = temp->nextSibling) {
        handleStmt(temp, func);
    }
}

int isRelOp(ast* curr) {
    if(curr->nodeType == LESS_THAN || curr->nodeType == LESS_EQUAL || curr->nodeType == EQUAL || curr->nodeType == GREATER_THAN || curr->nodeType == GREATER_EQUAL || curr->nodeType == NOT_EQUAL) {
        return 1;
    }
    else {
        return 0;
    }
}

identifierNode* validateBoolean(ast* curr, ast* func) {
    int localOffset = 0;
    printf("Entered valBool with current lexeme: %s, and function lexeme: %s\n", curr->lex, func->lex);
    if(curr->nodeType == AND || curr->nodeType == OR) {
        identifierNode* fNode = validateBoolean(curr->firstChild, func);
        identifierNode* sNode = validateBoolean(curr->firstChild->nextSibling, func);

        if(fNode == NULL || sNode == NULL) {
            return NULL;
        }
        else {
            return fNode;
        }
    }
    else if(isRelOp(curr)) {
        //      relop
        //     /
        //    /
        //   ID---->ID
        printf("It is relOp with lexeme: %s\n", curr->lex);
        ast* fChild = curr->firstChild;
        ast* sChild = curr->firstChild->nextSibling;
        if(isNumRnum(fChild) == true && isNumRnum(sChild) == true) {
            if(fChild->nodeType == sChild->nodeType) {
                Type type = (fChild->nodeType == INTEGER) ? INT_TYPE : REAL_TYPE;
                return createINode(fChild, func, type, false, &localOffset);
            }
            else {
                printf("Line no. %d: Operand Type error mismatch\n", curr->line);
                return NULL;
            }
        } 
        else {
            //printf("\nLine no. %d: Both are not integer or real\n", curr->line);
            identifierNode* fNode, *sNode;
            
            if(fChild->nodeType == ID && sChild->nodeType == ID) {
                printf("\nBoth are identfiers\n");
                //create temp nodes to lookup in symbol table
                identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                fNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                printf("In validateBoolean fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                if(fNode == NULL) {
                    fNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(fNode == NULL) {
                        printf("Line no. %d: Using undeclared identifier %s\n", fChild->line, fChild->lex);
                    }
                }
                t1 = createINode(sChild, func, -1, false, &localOffset);
                t1_global = createINode(sChild, func, -1, true, &localOffset);
                sNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                printf("sNode retrieved with lexeme: %s\n", sNode->token->lexeme);
                if(sNode == NULL) {
                    sNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(sNode == NULL) {
                        printf("Line no. %d: Using undeclared identifier %s\n", sChild->line, sChild->lex);
                        return NULL;
                    }
                }

                if((fNode->type == INT_TYPE || fNode->type == REAL_TYPE) && (sNode->type == INT_TYPE || sNode->type == REAL_TYPE)) {
                    if(fNode->type == sNode->type) {
                        //return createINode(, func, fNode->type, false, &localOffset);
                        printf("\nLeft and right nodes are not records\n");
                        return fNode;
                    }
                    else {
                        printf("Line no. %d: Operand Type error mismatch\n", curr->line);
                    }
                }

                else {
                    //the node is a record or union
                    printf("\nIn valBool, one of the nodes is a record or union\n");
                    if(fChild->firstChild == NULL && (fNode->type == RECORD_TYPE || fNode->type == UNION_TYPE)) {
                        printf("Line no. %d: Record or Union type can not be used with relational operator\n", fChild->line);
                    }
                    if(sChild->firstChild == NULL && (sNode->type == RECORD_TYPE || sNode->type == UNION_TYPE)) {
                        printf("Line no. %d: Record or Union type can not be used with relational operator\n", sChild->line);
                        return NULL;
                    }

                    //this part will only be reached if there is an expansion
                    //iterate through the node, retrieve their node in each loop
                    //call searchInFieldList, if it is present then proceed
                    
                    ast* fFieldId = fChild->firstChild;
                    ast* sFieldId = sChild->firstChild;

                    char* fConcatLex = (char*)malloc(sizeof(char)*strlen(fChild->lex));
                    strcpy(fConcatLex, fChild->lex);

                    printf("\nConcatenating Second Child: \n\n\n\n\n");
                    while(fFieldId) {
                        int x = strlen(fConcatLex);
                        int y = strlen(fFieldId->lex);
                        fConcatLex = (char*)realloc(fConcatLex, sizeof(char)*(x+y+1));
                        char dot[] = ".";
                        strcat(fConcatLex, dot);
                        strcat(fConcatLex, fFieldId->lex);
                        
                        identifierNode* retNode = createINode(fFieldId, func, -1, false, &localOffset);
                        retNode->token->lexeme = fConcatLex;

                        identifierNode* fidNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                        printf("%s\n", fConcatLex);

                        if(fidNode == NULL) {
                            printf("Line no %d: No field name %s in the Record or union\n", fFieldId->line, fFieldId->lex);
                            fNode = fidNode;
                            break;
                        }
                        fNode = fidNode;
                        fFieldId = fFieldId->nextSibling;
                    }

                    char* sConcatLex = (char*)malloc(sizeof(char)*strlen(sChild->lex));
                    strcpy(sConcatLex, sChild->lex);

                    printf("\nConcatenating First Child: \n\n\n\n\n");
                    while(sFieldId) {
                        int x = strlen(sConcatLex);
                        int y = strlen(sFieldId->lex);
                        sConcatLex = (char*)realloc(sConcatLex, sizeof(char)*(x+y+1));
                        char dot[] = ".";
                        strcat(sConcatLex, dot);
                        strcat(sConcatLex, sFieldId->lex);
                        
                        identifierNode* retNode = createINode(sFieldId, func, -1, false, &localOffset);
                        retNode->token->lexeme = sConcatLex;

                        identifierNode* fidNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                        printf("%s\n", sConcatLex);

                        if(fidNode == NULL) {
                            printf("Line no %d: No field name %s in the Record or union\n", sFieldId->line, sFieldId->lex);
                            sNode = fidNode;
                            break;
                        }
                        sNode = fidNode;
                        sFieldId = sFieldId->nextSibling;
                    }
                    printf("Concatenation ended\n");

                    if(fNode == NULL || sNode == NULL) {
                        return NULL;
                    }
                    
                    if(fNode->type == sNode->type) {
                        if(fNode->type == INT_TYPE || fNode->type == REAL_TYPE) {
                            return fNode;
                        }
                        else {
                            printf("Line no %d: Type error mismatch\n", fNode->token->lineNo);
                            return NULL;
                        }
                    }
                    else {
                        printf("Line no %d: Type error mismatch\n", fNode->token->lineNo);
                        return NULL;
                    }
                }
            }
            else {
                printf("\nOne is ID and other is not\n");
                if(fChild->nodeType == ID) {
                    identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                    identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                    fNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                    printf("In validateBoolean fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                    if(fNode == NULL) {
                        fNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                        if(fNode == NULL) {
                            printf("Line no. %d: Using undeclared identifier %s\n", fChild->line, fChild->lex);
                            return NULL;
                        }
                    }

                    Type currType = fNode->type;
                    if(currType == RECORD_TYPE || currType == UNION_TYPE) {
                        if(fChild->firstChild == NULL) {
                            printf("Line no. %d: Relational operator can not be used with Unions or records\n", fChild->line);
                            return NULL;
                        }
                        else {
                            ast* fFieldId = fChild->firstChild;

                            char* fConcatLex = (char*)malloc(sizeof(char)*strlen(fChild->lex));
                            strcpy(fConcatLex, fChild->lex);

                            printf("\n\nConcatenation starting for fFieldId, current conclex: %s\n\n\n", fConcatLex);
                            while(fFieldId) {
                                int x = strlen(fConcatLex);
                                int y = strlen(fFieldId->lex);
                                fConcatLex = (char*)realloc(fConcatLex, sizeof(char)*(x+y+1));
                                char dot[] = ".";
                                strcat(fConcatLex, dot);
                                strcat(fConcatLex, fFieldId->lex);
                                printf("Current state of conlex: %s\n", fConcatLex);
                                
                                identifierNode* retNode = createINode(fFieldId, func, -1, false, &localOffset);
                                retNode->token->lexeme = fConcatLex;
                                

                                identifierNode* fidNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                                printf("Retrieved fidNode dk if it is NULL or not tho, vibes XOXOXO yass kween\n");
                                if(fidNode == NULL) {
                                    printf("Line no %d: No field name %s in the Record or union\n", fFieldId->line, fFieldId->lex);
                                    fNode = fidNode;
                                    break;
                                }
                                else {
                                    printf("Retrived successfully, fidNode lex: %s\n", fidNode->token->lexeme);
                                    fNode = fidNode;
                                    fFieldId = fFieldId->nextSibling;
                                }
                            }
                        }
                    }
                    if((currType == INT_TYPE && sChild->nodeType == NUM) || (currType == REAL_TYPE && sChild->nodeType == RNUM)) {
                        //printf("Line no. %d: Operand type mismatch", fChild->line);
                        return fNode;
                    }
                    else {
                        printf("Line no. %d: Operand type mismatch\n", fChild->line);
                        return NULL;
                    }
                }
                else if(sChild->nodeType == ID) {
                    identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                    identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                    sNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                    printf("In validateBoolean fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                    if(sNode == NULL) {
                        sNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                        if(sNode == NULL) {
                            printf("Line no. %d: Using undeclared identifier %s\n", fChild->line, fChild->lex);
                            return NULL;
                        }
                    }
                    
                    Type currType = sNode->type;
                    if(currType == RECORD_TYPE || currType == UNION_TYPE) {
                        if(sChild->firstChild == NULL) {
                            printf("Line no. %d: Relational operator can not be used with Unions or records\n", fChild->line);
                            return NULL;
                        }
                        else {
                            char *sConcatLex = (char *)malloc(sizeof(char) * strlen(sChild->lex));
                            strcpy(sConcatLex, sChild->lex);

                            ast* sFieldId = sChild->firstChild;

                            printf("\nConcatenating Second Child: \n\n\n\n\n");
                            while (sFieldId)
                            {
                                int x = strlen(sConcatLex);
                                int y = strlen(sFieldId->lex);
                                sConcatLex = (char *)realloc(sConcatLex, sizeof(char) * (x + y + 1));
                                char dot[] = ".";
                                strcat(sConcatLex, dot);
                                strcat(sConcatLex, sFieldId->lex);

                                identifierNode *retNode = createINode(sFieldId, func, -1, false, &localOffset);
                                retNode->token->lexeme = sConcatLex;

                                identifierNode *fidNode = (identifierNode *)retrieve(SymbolTable, retNode, ID);
                                printf("%s\n", sConcatLex);

                                if (fidNode == NULL)
                                {
                                    printf("Line no %d: No field name %s in the Record or union\n", sFieldId->line, sFieldId->lex);
                                    sNode = fidNode;
                                    break;
                                }
                                sNode = fidNode;
                                sFieldId = sFieldId->nextSibling;
                            }
                            printf("Concatenation ended\n");
                        }
                    }
                    if((currType == INT_TYPE && fChild->nodeType == NUM) || (currType == REAL_TYPE && fChild->nodeType == RNUM)) {
                        //printf("Line no. %d: Operand type mismatch", fChild->line);
                        return sNode;
                    }
                    else {
                        printf("Line no. %d: Operand type mismatch", fChild->line);
                        return NULL;
                    }
                }
            }
        }
    }
    else if(curr->nodeType == NOT_BOOL) {
        return validateBoolean(curr->firstChild, func);
    }
}

void validateReturn(ast* curr, ast* func) {
    ast* child = curr->firstChild;
    
    //declre dummy node for retrieval
    functionNode* retNode = (functionNode *)malloc(sizeof(functionNode));
    retNode->width = -1;
    retNode->rank = -1;
    retNode->opParams = NULL;
    retNode->ipParams = NULL;
    retNode->tmpVars = -1;

    retNode->token = (tokenInfo *)malloc(sizeof(tokenInfo));
    retNode->token->lexeme = func->lex;
    retNode->token->lineNo = func->line;
    retNode->token->tid = func->symbol;
    //declaration finished

    functionNode* funcInfo = (functionNode*)retrieve(SymbolTable, retNode, FUNCTION_SEQ);
    printf("Function whose info was retrieved: %s and number of output params: %d\n", funcInfo->token->lexeme, funcInfo->numOp);

    parameters* outputParams = funcInfo->opParams;
    identifierNode* opInfo = NULL;
    int numReturn = 0;
    ast* tempOP = curr->firstChild;
    while(tempOP) {
        numReturn++;
        tempOP = tempOP->nextSibling;
    }
    if(numReturn != funcInfo->numOp) {
        printf("Line no. %d: Number of return values not equal to number of output parameters\n", curr->line);
        return;
    }

    identifierNode* currNode = (identifierNode*)malloc(sizeof(identifierNode));
    parameters* iterParams = outputParams;
    for(int i=0; i<numReturn; i++, child = child->nextSibling, iterParams = iterParams->next) {
        //use iterators for output parameters and return values
        if(strcmp(child->lex, iterParams->token->lexeme) != 0) {
            printf("Line no. %d: Unexpected return parameter found. Parameter returned: %s and Parameter expected: %s\n", child->line, child->lex, iterParams->token->lexeme);
        }
    }
}

void validateConditional(ast* curr, ast* func) {
    printf("Entered valCon with func: %s\n", func->lex);
    validateBoolean(curr->firstChild, func);
    handleStmt(curr->firstChild->nextSibling, func);
    printf("Handled the first stmt in Conditional for lexeme %s\n", curr->firstChild->nextSibling->lex);
    ast* iter = curr->firstChild->nextSibling->nextSibling;
    if(iter == NULL) {
        printf("Else part hai hi nahi\n");
        return;
    }
    while(iter != NULL && iter->nodeType != ELSE) {
        printf("Entering while loop in valCond with lex: %s and function: %s\n", iter->lex, func->lex);
        handleStmt(iter, func);
        printf("Handled the while loop stmt in Conditional for lexeme %s with firstChild lex: %s\n", iter->lex, iter->firstChild->lex);
        iter = iter->nextSibling;
    }
    if(iter != NULL && iter->nodeType == ELSE) {
        handleStmt(iter->firstChild, func);
        iter = iter->nextSibling;
        while(iter != NULL) {
            handleStmt(iter, func);
            iter = iter->nextSibling;
        }
    }
    else {
        return;
    }
}

void handleStmt(ast* curr, ast* func) {
    if(curr == NULL) {
        return;
    }
    switch(curr->nodeType) {
        case ASSIGNOP:
            //TODO: write the correct order of calling
            validateAssign(curr, func);
            printf("exited validateAssign for lex: %s\n", curr->lex);
            break;
        case ITERATIVE:
            validateIterative(curr, func);
            printf("Exited validateIterative for lex: %s\n", curr->lex);
            break;
        case CONDITIONAL:
            validateConditional(curr, func);
            break;
        case RETURN:
            validateReturn(curr, func);
            break;
        case IOREAD:
            validateRead(curr, func);
            break;
        case IOWRITE:
            validateWrite(curr, func);
            break;
        case CALL:
            printf("\n\nEntered validateFunCall for lex: %s\n", curr->lex);
            validateFunCall(curr, func);
            printf("\n\nExited validateFunCall for lex: %s\n", curr->lex);
            break;
    }
    handleStmt(curr->firstChild, func);
}

void validateFunction(ast* curr) {
    if(curr == NULL) {
        return;
    }
    printf("\nIn validate function, current node name: %s\n", curr->lex);
    ast* child = curr->firstChild;
    for(; child!=NULL; child=child->nextSibling) {
        printf("\nIn valFunc, going to handleStmt, child lexeme: %s and curr lexeme: %s\n", child->lex, curr->lex);
        handleStmt(child, curr);
        printf("\nFinished validateFunction inside for loop, child lexeme: %s\n", child->lex);
    }
}
