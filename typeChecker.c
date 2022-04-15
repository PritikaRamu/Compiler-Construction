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

void validateAssign(ast* curr, ast* func) {
    int localOffset = 0; 
    printf("In validateAssign, entering valArith, curr->firstChild->nextSibling lexeme: %s and NodeType: %d\n", curr->firstChild->nextSibling->lex, curr->firstChild->nextSibling->nodeType);
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
    identifierNode* lChildNode = createINode(lChild, func, lChild->nodeType, false, &localOffset);
    identifierNode* notGlobalCurr = lChildNode;
    notGlobalCurr->global = false;
    notGlobalCurr = (identifierNode*)retrieve(SymbolTable, notGlobalCurr, lChild->nodeType);
    //if(notGlobalCurr) printf("Type of notGlobalCurr: %d\n", notGlobalCurr->type);
    identifierNode* globalCurr = lChildNode;
    globalCurr->global = true;
    globalCurr = (identifierNode*)retrieve(SymbolTable, globalCurr, lChild->nodeType);
    if (globalCurr)
    {
        lChildNode = globalCurr;
        printf("Retrived global identifier for lex: %s type: %d nodetype: %d\n", lChildNode->token->lexeme, lChildNode->type, lChild->nodeType);
    }
    else if (notGlobalCurr)
    {
        printf("Retrived non global identifier for lex: %s, type: %d\n", lChildNode->token->lexeme, lChildNode->type);
        lChildNode = notGlobalCurr;
    }
    else
    {
        // semanticErrors ++;
        printf("Line no. %d: Undeclared Variable %s in Assignment.\n", lChild->line, lChild->lex);
        return;
    }
    if(ArithNode == NULL) {
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

    if (lChild->nodeType != RECORD_OR_UNION && attribute != NULL)
    {
        printf("Identifier not of type record/ union but has fields.\n");
        // semanticErrors++;
        return;
    }
    else if (lChild->nodeType == RECORD_OR_UNION && attribute != NULL)
    {
        // NESTED RECORDS
    }
    //int/real
    else if(lChild->nodeType != RECORD_OR_UNION)
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
}

identifierNode* validateArithmetic(ast* curr, ast* func) {
    printf("Entering ValArith, curr lexeme: %s, func lexeme: %s\n", curr->lex, func->lex);
    if (curr == NULL)
    {
        return NULL;
    }
    int localOffset = 0;
    //int numTempVar = 0;

    //identifier node/ int-real
    bool isOp = isOperator(curr);

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
        printf("\nIn valArith, not an operator, creating a node for ast lexeme: %s\n", curr->lex);
        identifierNode* currNode = createINode(curr, func, curr->nodeType, false, &localOffset);
        //printf("ValArith mai createINode hua kya?\n");
        identifierNode* notGlobalCurr = currNode;
        notGlobalCurr->global = false;
        notGlobalCurr = (identifierNode*)retrieve(SymbolTable, notGlobalCurr, curr->nodeType);
        //printf("ValArith mai retrieve hua kya?\n");
        identifierNode* globalCurr = currNode;
        globalCurr->global = true;
        globalCurr = (identifierNode*)retrieve(SymbolTable, globalCurr, curr->nodeType);
        if (globalCurr)
        {
            printf("In valArith globalCurr, retrived type is %d for lexeme: %s\n", globalCurr->type, globalCurr->token->lexeme);
            currNode = globalCurr;
        }
        else if (notGlobalCurr){
            printf("In valArith notglobalCurr, retrived type is %d for lexeme: %s\n", globalCurr->type, globalCurr->token->lexeme);
            currNode = notGlobalCurr;
        }
        else {
            //semanticErrors ++;
            printf("Line no. %d: Undeclared Variable %s\n", curr->line, curr->lex);
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

        //both are identifiers
        // else if(isOperator(fChild) == false && isOperator(sChild) == false) {

        //     //both int/real
        //     if(isIntReal(fChild) == true && isIntReal(sChild) == true){
        //         //both int, both real;
        //         if(fType == sType) {
        //             return createINode(fChild, func, fChild->nodeType, false, &localOffset);
        //         }
        //         //int with real operations  
        //         else {
        //             return NULL;
        //         }
        //     }

        //     //both records/unions
        //     else if(fChild->nodeType == RECORD_OR_UNION && sChild->nodeType == RECORD_OR_UNION) {
        //         if(fType == sType){
        //             //record
        //             if(fType == RECORD_TYPE && fNode->recordList == sNode->recordList) {
        //                 return fNode;
        //             }
        //             //union
        //             if(fType == UNION_TYPE && fNode->recordList == sNode->recordList){
        //                 return fNode;
        //             }
        //         }
        //         //operations bw rec and union
        //         else {
        //             return NULL;
        //         }
        //     }

        //     //record/union and int/real
        //     else {
        //         if(curr->nodeType != DIVIDE){
        //             if(fType == RECORD_TYPE) {
        //                 return fNode;
        //             }
        //             else if (fType == UNION_TYPE) {
        //                 return fNode;
        //             }
        //             else if(sType == RECORD_TYPE) {
        //                 return sNode;
        //             }
        //             else if (sType == UNION_TYPE) {
        //                 return sNode;
        //             }
        //         }
        //         else{
        //             if(sChild->nodeType == RECORD_OR_UNION){
        //                 printf("Can't divide scalar by record/union");
        //                 //semantic_errors++;
        //                 return NULL;
        //             }
        //             else{
        //                 if(fType == RECORD_TYPE) {
        //                     return fNode;
        //                 }
        //                 else if (fType == UNION_TYPE) {
        //                     return fNode;
        //             }
        //         }
        //     }   
        // }

        // end of both identifiers
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
            return;
        }

        Type opSTtype = TypeUsingAST(opSTpars->nodeType, opSTpars->is_union);
        
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

    if(opGivenPars != NULL) {
        printf("Function call has extra output parameters\n");
        //semanticErrors++;
        //return;
    }

    if(opSTpars != NULL) {
        printf("Function call has insufficient output parameters.\n");
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
            return;
        }

        Type ipSTtype = TypeUsingAST(ipSTpars->nodeType, ipSTpars->is_union);
        
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
        return;
    }

    if(curr->firstChild != NULL) {
        if(curr->nodeType != RECORD_OR_UNION) {
            printf("Variable is not of type record/union.");
            //semanticErrors++:
            return;
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
    Type currType = TypeUsingAST(curr->nodeType, curr->is_union);
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
        return;
    }

    if(curr->firstChild != NULL) {
        if(curr->nodeType != RECORD_OR_UNION) {
            printf("Variable is not of type record/union.");
            //semanticErrors++:
            return;
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

void validateIterative(ast* curr, ast* func) {
    printf("Entered valIter with current lexeme: %s, and function lexeme: %s\n", curr->lex, func->lex);
    validateBoolean(curr->firstChild, func);
    ast* temp = curr->firstChild->nextSibling;
    for(; temp != NULL; temp = temp->nextSibling) {
        handleStmt(temp, func);
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
    printf("Entered valBool with current lexeme: %s, and function lexeme: %s\n", curr->lex, func->lex);
    if(curr->nodeType == AND || curr->nodeType == OR) {
        validateBoolean(curr->firstChild, func);
        return validateBoolean(curr->firstChild->nextSibling, func);
    }
    else if(isRelOp(curr)) {
        //      relop
        //     /
        //    /
        //   ID---->ID
        printf("It is relOp with lexeme: %s\n", curr->lex);
        ast* fChild = curr->firstChild;
        ast* sChild = curr->firstChild->nextSibling;
        if(isIntReal(fChild) == true && isIntReal(sChild) == true) {
            if(fChild->nodeType == sChild->nodeType) {
                Type type = (fChild->nodeType == INTEGER) ? INT_TYPE : REAL_TYPE;
                return createINode(fChild, func, type, false, &localOffset);
            }
            else {
                printf("Operand Type error mismatch on Line no. %d\n", curr->line);
                return NULL;
            }
        } 
        else {
            printf("Both are not integer or real\n");
            identifierNode* fNode, *sNode;
            if(fChild->nodeType == ID && sChild->nodeType == ID) {
                //create temp nodes to lookup in symbol table
                identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                fNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                printf("fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                if(fNode == NULL) {
                    fNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(fNode == NULL) {
                        printf("Using undeclared identifier %s on line no. %d\n", fChild->lex, fChild->line);
                    }
                }
                t1 = createINode(sChild, func, -1, false, &localOffset);
                t1_global = createINode(sChild, func, -1, true, &localOffset);
                sNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                printf("sNode retrieved with lexeme: %s\n", sNode->token->lexeme);
                if(sNode == NULL) {
                    sNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(sNode == NULL) {
                        printf("Using undeclared identifier %s on line no. %d\n", sChild->lex, sChild->line);
                        return NULL;
                    }
                }

                if((fNode->type == INT_TYPE || fNode->type == REAL_TYPE) && (sNode->type == INT_TYPE || sNode->type == REAL_TYPE)) {
                    if(fNode->type == sNode->type) {
                        //return createINode(, func, fNode->type, false, &localOffset);
                        return fNode;
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

                        retNode->function = (tokenInfo*)malloc(sizeof(tokenInfo));
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
    validateBoolean(curr->firstChild, func);
    handleStmt(curr->firstChild->nextSibling, func);
    ast* iter = curr->firstChild->nextSibling->nextSibling;
    while(iter != NULL && iter->nodeType != ELSE) {
        handleStmt(iter, func);
        iter = iter->nextSibling;
    }
    if(iter->nodeType == ELSE) {
        handleStmt(iter->firstChild, func);
        iter = iter->nextSibling;
        while(iter != NULL) {
            handleStmt(iter, func);
            iter = iter->nextSibling;
        }
    }
}

void handleStmt(ast* curr, ast* func) {
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
            validateFunCall(curr, func);
            break;
    }
}

void validateFunction(ast* curr) {
    if(curr == NULL) {
        return;
    }
    printf("\nIn validate function, current function name: %s\n", curr->lex);
    ast* child = curr->firstChild;
    for(; child!=NULL; child=child->nextSibling) {
        printf("In valFunc, going to handleStmt, child lexeme: %s\n", child->lex);
        handleStmt(child, curr);
        validateFunction(child);
        printf("Finished validateFunction inside for loop, child lexeme: %s\n", child->lex);
    }
}