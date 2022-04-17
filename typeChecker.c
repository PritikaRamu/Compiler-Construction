#include "typeChecker.h"

identifierNode* validateBoolean(ast* curr, ast* func);
void handleStmt(ast* curr, ast* func);
void validateConditional(ast* curr, ast* func);
recordField* searchInFieldList(ast* curr, recordField* fieldList);
void validateReturn(ast* curr, ast* func);
void validateFunction(ast* curr);

void printSemanticErrors() {
    printf("\nSEMANTIC ERRORS:\n");
    printf("_____________________________\n");
    for(int i=0;i<1000;i++){
        if(sem_error_array[i][0] != '\0'){
            printf("%s\n", sem_error_array[i]);
        }


    }
   
    printf("_____________________________\n");
    printf("No. of Semantic Errors: %d\n", semanticErrors);
}

void addSemErrortoArray(char* message){
    strcpy(sem_error_array[semanticErrors], message);
    semanticErrors++;
}

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
    //printf("In semantic analyser, starting while loop\n"); // Debugging
    while(curr){
        validateFunction(curr);
        curr = curr->nextSibling;
    }
    //printf("Exited Semantic Analyser while loop\n"); // Debugging
}

bool isOperator(ast* curr) {
    NodeType nType = curr->nodeType;
    if(nType == DIVIDE || nType == MULTIPLY || nType == PLUS || nType == MINUS) {
        return true;
    }
    else {
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

void validateAssign(ast* curr, ast* func) {
    int localOffset = 0; 
    // Debug
    //printf("In validateAssign, entering valArith, curr->firstChild->nextSibling lexeme: %s and func: %s and NodeType: %d\n", curr->firstChild->nextSibling->lex, func->lex, curr->firstChild->nextSibling->nodeType);
    identifierNode* ArithNode = validateArithmetic(curr->firstChild->nextSibling, func);
    // Debug
    //printf("In valAss Exited valArith successfully for lexeme: %s and NodeType: %d\n", curr->firstChild->nextSibling->lex, curr->firstChild->nextSibling->nodeType);
    // if(ArithNode != NULL) {
    //     //printf("In valAss Exited valArith successfully with lexeme: %s and Type: %d\n", ArithNode->token->lexeme, ArithNode->type);
    //     // Debug
    //    // printf("Here hoon\n"); 
    // }
    // else {
    //     // Debug
    //     printf("ArithNode returned NULL from valArith\n");
    // }
    ast* lChild = curr->firstChild;
    //Type lChildType = TypeUsingAST(lChild->nodeType, lChild->is_union);
    // Debug
    //printf("This lexeme in valAss: %s is in scope of function: %s with nodeType: %d\n", curr->lex, func->lex, curr->nodeType);
    
    identifierNode* lChildNode = createINode(lChild, func, -1, false, &localOffset);
    //identifierNode* lChildNode_global = createINode(lChild, func, -1, true, &localOffset);
    lChildNode = (identifierNode*)retrieve(SymbolTable, lChildNode, ID);
    // Debug
    //printf("Chcekpoint 1 post retrieve in ValAssign.\n");

    if(lChildNode == NULL) {
        identifierNode* lChildNode_global = createINode(lChild, func, -1, true, &localOffset);
        lChildNode_global = (identifierNode* )retrieve(SymbolTable, lChildNode_global, ID);
        // Debug
        //printf("Chcekpoint 2 post retrieve in ValAssign.\n");

        if(lChildNode_global == NULL) {
            //char string[200];
            printf("Line %d:ERROR- Undeclared Variable %s in Assignment.\n", lChild->line, lChild->lex);
            //addSemErrortoArray(string);
            //printf("Line no. %d: Undeclared Variable %s in Assignment.\n", lChild->line, lChild->lex);
            return;
        }
        else {
            lChildNode = lChildNode_global;
            // Debug
            //printf("Retrived global identifier for lex: %s, type: %d\n", lChildNode->token->lexeme, lChildNode->type);
        }
    }
    else {
        // Debug
        //printf("Retrived non global identifier for lex: %s type: %d nodetype: %d\n", lChildNode->token->lexeme, lChildNode->type, lChild->nodeType);
    }
    
    //Debug
    //printf("Before Arithnode check\n");

    // if(ArithNode == NULL ) {
    //     //printf("Line no. %d: Operand type mismatch\n", curr->line);
    //     return;
    // }

    // attribute from AST
    ast *attribute = NULL;
    if (lChild->firstChild != NULL)
    {
        attribute = lChild->firstChild;
    }

    // record/union from symbol table

    if ((lChildNode->type != RECORD_TYPE && lChildNode->type != UNION_TYPE) && attribute != NULL)
    {
        //printf("Identifier not of type record/ union but has fields.\n");
        // semanticErrors++;
        //char string[200];
        printf("Line %d: ERROR - Identifier %s not of type record/ union but has fields.\n", lChild->line, lChild->lex);
        //addSemErrortoArray(string);
        return;
    }
    //NESTED RECORDS
    else if ((lChildNode->type == RECORD_TYPE || lChildNode->type == UNION_TYPE) && attribute != NULL)
    {
        Type lChildType = lChildNode->type;

        char* fConcatLex = (char*)malloc(sizeof(char)*strlen(lChild->lex));
        strcpy(fConcatLex, lChild->lex);

        // Debug
        //printf("\nConcatenating Second Child in ValAss: \n\n\n\n\n");
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
            //printf("%s\n", fConcatLex);

            if(fidNode == NULL) {
                //fprintf(errorfp, "Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                //printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                //char string[200];
                printf("Line %d: EROOR- No field name %s in the Record or union\n", attribute->line, attribute->lex);
                //addSemErrortoArray(string);
                //semanticErrors++;
                return;
            }
            lChildNode = fidNode;
            attribute = attribute->nextSibling;
        }
        if(ArithNode != NULL && lChildNode->type == ArithNode->type) {
            //Debug
            //printf("In valAss, type of assigned node: %s match val of arith node: %s\n", lChild->lex, ArithNode->token->lexeme);
        }
        else {
            //fprintf(errorfp, "Line no %d: Type mismatch in assign.\n", lChild->line);
            //printf("Line no %d: Type mismatch in assign.\n", lChild->line);
            //char string[200];
            if(ArithNode != NULL) {
                printf("Line %d: ERROR- Type mismatch in assign.\n", lChild->line);
                return;
            }
            //addSemErrortoArray(string);
            // semanticErrors++;
        }
    }
    //int/real
    else if((lChildNode->type != RECORD_TYPE && lChildNode->type != UNION_TYPE))
    {
        //printf("In valAss, not a record or union\n");
        Type lChildType = lChildNode->type;
        if (ArithNode != NULL && lChildType != ArithNode->type)
        {
            //printf("Idhar to aana chahiye tha\n");
            //printf("Line no %d: Type mismatch in assign.\n", lChild->line);
            //char string[200];
            printf("Line %d: ERROR- Type mismatch in assigning %s to %s.\n", lChild->line, ArithNode->token->lexeme, lChild->lex);
            //addSemErrortoArray(string);
            // semanticErrors++;
            return;
        }
        else{
            //printf("In valAss, type of assigned node: %s match val of arith node: %s\n", lChild->lex, ArithNode->token->lexeme);
        }
    }
    else if((lChildNode->type == RECORD_TYPE || lChildNode->type == UNION_TYPE) && attribute == NULL) {
        Type lChildType = lChildNode->type;
        if (ArithNode != NULL && lChildType != ArithNode->type)
        {
            //printf("Line no %d: Type mismatch in assign.\n", lChild->line);
            //char string[200];
            printf("Line %d: ERROR- Type mismatch in assigning %s to %s.\n", lChild->line, ArithNode->token->lexeme, lChild->lex);
            //addSemErrortoArray(string);
            // semanticErrors++;
            return;
        }
        else{
            //printf("In valAss, type of assigned node: %s match val of arith node: %s\n", lChild->lex, ArithNode->token->lexeme);
        }
    }
    lChildNode->assigned = true;
}

identifierNode* validateArithmetic(ast* curr, ast* func) {
    //printf("Entering ValArith, curr lexeme: %s, func lexeme: %s\n", curr->lex, func->lex);
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
                //printf("Arith returns a num with lex: %s\n", curr->lex);
                return createINode(curr, func, INT_TYPE, false, &localOffset);
            }
            else {
                //printf("Arith returns a rnum with lex: %s\n", curr->lex);
                return createINode(curr, func, REAL_TYPE, false, &localOffset);
            }
        }
        //id
        
        //printf("\nIn valArith, not an operator, creating a node for ast lexeme: %s\n", curr->lex);
        identifierNode* currNode = createINode(curr, func, 0, false, &localOffset);
        identifierNode* currNode_global = createINode(curr, func, 0, true, &localOffset);
        currNode = (identifierNode*)retrieve(SymbolTable, currNode, ID);

        if(currNode == NULL) {
            identifierNode* currNode_global = createINode(curr, func, 0, true, &localOffset);
            currNode_global = (identifierNode*)retrieve(SymbolTable, currNode_global, ID);
            if(currNode_global == NULL) {
                //semanticErrors++;
                //char string[200];
                printf("Line %d: ERROR- Using undeclared variable %s\n", curr->line, curr->lex);
                //addSemErrortoArray(string);
                //printf("Line no. %d: Using undeclared variable %s\n", curr->line, curr->lex);
                return NULL;
            }
            else {
                currNode = currNode_global;
                //printf("Retrived global identifier for lex: %s, type: %d\n", currNode->token->lexeme, currNode->type);
            }
        }
        else {
            //printf("Retrived non global identifier for lex: %s, type: %d\n", currNode->token->lexeme, currNode->type);
        }

        //attribute from AST
        ast* attribute = NULL;
        if(curr->firstChild != NULL) {
            attribute = curr->firstChild;
        }

        //unassigned id (except nested records)
        // if(attribute == NULL) {
        //     if(currNode->assigned == false) {
        //         //semanticErrors++;
        //         //char string[200];
        //         printf("Line %d: ERROR Can not perform assignment on unassigned identifier %s\n", curr->line, curr->lex);
        //         //addSemErrortoArray(string);
        //         //printf("\n\n\n\nLine no. %d: Can not perform assignment on unassigned identifier %s\n", curr->line, curr->lex);
        //         return NULL;
        //     }
        // }

        //NESTED RECORDS

        if ((currNode->type == RECORD_TYPE) && attribute != NULL)
        {
            Type currType = currNode->type;
            char* fConcatLex = (char*)malloc(sizeof(char)*strlen(curr->lex));
            strcpy(fConcatLex, curr->lex);

            //printf("\n\nConcatenating Second Child in ValAss: \n\n\n\n\n");
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
                //printf("%s\n", fConcatLex);

                if(fidNode == NULL) {
                    //printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                    //semanticErrors++;
                    //char string[200];
                    printf("Line %d:ERROR No field name %s in the Record\n", attribute->line, attribute->lex);
                    //addSemErrortoArray(string);
                    return NULL;
                }
                currNode = fidNode;
                attribute = attribute->nextSibling;
            }
            // if(currNode->assigned == true) {
            //     return currNode;
            // }
            // else {
            //     //semanticErrors++;
            //     //char string[200];
            //     printf("Line %d:ERROR- Can not perform assignment from unassigned identifiers\n", curr->line);
            //     //addSemErrortoArray(string);
            //     //printf("Line no. %d: Can not perform assignment on unassigned identifiers\n", curr->line);
            //     return NULL;
            // }
            return currNode;
        }
        else if ((currNode->type == RECORD_TYPE) && attribute == NULL) {
            return currNode;
        }
        else if ((currNode->type != RECORD_TYPE) && attribute != NULL) {
            //printf("Line no. %d: Variale %s not of type record.\n", curr->line, curr->lex);
            //semanticErrors++;
            //char string[200];
            printf("Line %d:ERROR- Variable %s not of type record.\n", curr->line, curr->lex);
            //addSemErrortoArray(string);
            return NULL;

        }
        else if ((currNode->type != RECORD_TYPE) && attribute == NULL){
            return currNode;
        }
    }

    //operator node
    else if(isOp == true) {
        //printf("Ye enter hua kya?\n");

        ast* fChild, *sChild;
        fChild = curr->firstChild;
        sChild = fChild->nextSibling;
        //ast has 1 or no children
        if(fChild==NULL || sChild == NULL) {
            return NULL;
        }

        identifierNode* fNode = validateArithmetic(fChild, func);
        identifierNode* sNode = validateArithmetic(sChild, func);

        if(fNode == NULL || sNode == NULL) {
            return NULL;
        }

        //performing operations on unasigned identifiers
        // if(fNode->assigned == false || sNode->assigned == false) {
        //     //semanticErrors;
        //     //char string[200];
        //     printf("Line %d:ERROR Can not perform operations on unassigned identifiers\n", fChild->line);
        //     //addSemErrortoArray(string);
        //     //printf("Line no. %d: Can not perform operations on unassigned identifiers\n", fChild->line);
        //     return NULL;
        // }

        Type fType = fNode->type;
        Type sType = sNode->type;

        if(fType == sType){
            if (fType == RECORD_TYPE) {
                if(fNode->recordList != sNode->recordList) {
                    //char string[200];
                    printf("Line %d:ERROR Can not perform arithmetic operations on records of different types\n", fChild->line);
                    //addSemErrortoArray(string);
                    //printf("Line no. %d: Can not perform operations on records of different types\n", fChild->line);
                    //semanticErrors++;
                    return NULL;
                }
                else {
                    if(curr->nodeType == PLUS || curr->nodeType == MINUS) {
                        //printf("Type matches for record in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                        return fNode;
                    }
                    else {
                        //semanticErrors++;
                        //printf("Line no. %d ERROR: Multiplication or Division not allowed with records\n", curr->line);
                        //char string[200];
                        printf("Line %d: ERROR- Multiplication or Division not allowed with records\n", curr->line);
                        //addSemErrortoArray(string);
                        return NULL;
                    }
                }
            }
            if (fType == UNION_TYPE) {
                //semanticErrors++;
                //char string[200];
                printf("Line %d: ERROR- Arithmetic operations not allowed with union types\n", curr->line);
                //addSemErrortoArray(string);
                return NULL;
                //printf("Line no. %d ERROR: Arithmetic operations not allowed with union types\n", curr->line);
            }
            else if (fType == INT_TYPE || fType == REAL_TYPE) {
                if(fType == INT_TYPE) {
                    if(curr->nodeType == DIVIDE) {
                        //printf("Type matches for Int/Real in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                        return createINode(fChild, func, REAL_TYPE, false, &localOffset);
                    }
                    else {
                        //printf("Type matches for Int/Real in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                        return fNode;
                    }
                }
                //printf("Type matches for Int/Real in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                return fNode;
            }
        }
        //mismatched type
        else {
            if(fType == INT_TYPE) {
                if(sType == REAL_TYPE) {
                    if(curr->nodeType != DIVIDE) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Type mismatch operand: left side is of type int and right side is of type real\n", fChild->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Type mismatch operand %s is of type int and %s is of type real\n", fChild->line, fChild->lex, sChild->lex);
                        return NULL;
                    }
                    else {
                        //printf("Type matches for Int/Real in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                        return sNode;
                    }
                }
                else if(sType == RECORD_TYPE || sType == UNION_TYPE) {
                    if(sType == UNION_TYPE) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Arithmetic operations not allowed with union types\n", curr->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d ERROR: Arithmetic operations not allowed with union types\n", curr->line);
                        return NULL;
                    }
                    else {
                        if(curr->nodeType == MULTIPLY) {
                            //printf("Type matches for Int and Record in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                            return sNode;
                        }
                        else {
                            //semanticErrors++;
                            //char string[200];
                            printf("Line %d: ERROR- Disallowed Arithmetic operation %s with record and integer\n", curr->line, curr->lex);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d ERROR: Disallowed Arithmetic operation %s with record and integer\n", curr->line, curr->lex);
                            return NULL;
                        }
                    }
                }
            }
            else if(fType == REAL_TYPE) {
                if(sType == INT_TYPE) {
                    if(curr->nodeType != DIVIDE) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Type mismatch: left side is of type real and right side is of type int\n", fChild->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Type mismatch operand %s is of type int and %s is of type real\n", fChild->line, fChild->lex, sChild->lex);
                        return NULL;
                    }
                    else {
                        //printf("Type matches for Int/Real in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                        return sNode;
                    }
                }
                else if(sType == RECORD_TYPE || sType == UNION_TYPE) {
                    //semanticErrors++;
                    //char string[200];
                    printf("Line %d: ERROR- Disallowed Arithmetic operation %s with record/union and real\n", curr->line, curr->lex);
                    //addSemErrortoArray(string);
                    //printf("Line no. %d ERROR: Disallowed Arithmetic operation %s with record/union and real\n", curr->line, curr->lex);
                    return NULL;
                }
            }
            else if(fType == RECORD_TYPE || fType == UNION_TYPE) {
                if(fType == UNION_TYPE) {
                    //semanticErrors++;
                    //char string[200];
                    printf("Line %d: ERROR- Arithmetic operations not allowed with union types\n", curr->line);
                    //addSemErrortoArray(string);
                    //printf("Line no. %d ERROR: Arithmetic operations not allowed with union types\n", curr->line);
                    return NULL;
                }
                else {
                    if(sType == INT_TYPE) {
                        if(curr->nodeType == MULTIPLY) {
                            //printf("Type matches for Int and Record in valArith for lexemes: %s and %s\n", fNode->token->lexeme, sNode->token->lexeme);
                            return sNode;
                        }
                        else {
                            //semanticErrors++;
                            //char string[200];
                            printf("Line %d: ERROR- Disallowed Arithmetic operation %s with record and integer\n", curr->line, curr->lex);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d ERROR: Disallowed Arithmetic operation %s with record and integer\n", curr->line, curr->lex);
                            return NULL;
                        }
                    }
                    else if(sType == UNION_TYPE) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Type mismatch operand %s is of type Record and %s is of type Union\n", fChild->line, fChild->lex, sChild->lex);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Type mismatch operand %s is of type Record and %s is of type Union\n", fChild->line, fChild->lex, sChild->lex);
                        return NULL;
                    }
                    else if(sType == REAL_TYPE) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Disallowed Arithmetic operation %s with record and real\n", curr->line, curr->lex);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d ERROR: Disallowed Arithmetic operation %s with record and real\n", curr->line, curr->lex);
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

    //fetching calling func node from ST
    functionNode *callingFunctionNode = createFNode(func); // to be modified
    functionNode *tempCalling = callingFunctionNode;
    callingFunctionNode = (functionNode *)retrieve(SymbolTable, callingFunctionNode, FUNCTION_SEQ);
    // free(tempCalling);

    // fetching called func node from ST
    functionNode *calledFunctionNode = createFNode(funcIdentifier); // to be modified
    functionNode *tempCalled = calledFunctionNode;
    calledFunctionNode = (functionNode *)retrieve(SymbolTable, calledFunctionNode, FUNCTION_SEQ);
    // free(tempCalled);

    // calling a non-existing function
    if (calledFunctionNode == NULL)
    {
        // char string[200];
        printf("Line %d: ERROR- Undeclared function %s being called\n", funcIdentifier->line, funcIdentifier->lex);
        // addSemErrortoArray(string);
        // printf("Line no. %d: Undeclared function %s being called\n", funcIdentifier->line, funcIdentifier->lex);
        //  semanticerrors++;
        return;
    }
    // printf("Here in valfun\n");

    // function call inside function call;
    if (calledFunctionNode->token == callingFunctionNode->token)
    {
        // char string[200];
        printf("Line %d: ERROR- Recursion not allowed in functions.\n", call->line);
        // addSemErrortoArray(string);
        // printf("Line no. %d: Recursion not allowed in functions.\n", call->line);
        //  semanticErrors++;
        return;
    }

    // calling before definition
    if (calledFunctionNode->rank > callingFunctionNode->rank)
    {
        // char string[200];
        printf("Line %d: ERROR- Function %s called before being defined.\n", call->line, funcIdentifier->lex);
        // addSemErrortoArray(string);
        // printf("Line no. %d: Function %s called before being defined.\n", call->line, funcIdentifier->lex);
        //  semanticErrors++;
        return;
    }


    if(opCall->firstChild != NULL) {
        ast* opGivenPars = opCall->firstChild;
        parameters* opSTpars = calledFunctionNode->opParams;

        //printf("\n\n Number of op params: %d\n\n\n\n", calledFunctionNode->numOp);

        //printf("\n\n\n Before while loop for op pars, in ValFunCall");

        while (opSTpars != NULL && opGivenPars != NULL) {
            //ast* id, ast* func, Type type, bool is_global, int*offset

            //retrieve from ST node corresponding to AST
            //printf("\nEntering loop for opSTpars in the function %s\n", funcIdentifier->lex);
            Type opGivenType = TypeUsingAST(opGivenPars->nodeType, opGivenPars->is_union);
            
            identifierNode* opGivenNode = createINode(opGivenPars, func, opGivenType, false, &localOffset);
            identifierNode* opGivenNode_global = createINode(opGivenPars, func, opGivenType, true, &localOffset);
            opGivenNode = (identifierNode*)retrieve(SymbolTable, opGivenNode, ID);

            if (opGivenNode == NULL)
            {
                //printf("Retrieve se pehle, retrieve se zyada\n");
                //identifierNode *opGivenNode_global = createINode(opGivenPars, func, -1, true, &localOffset);
                opGivenNode_global = (identifierNode *)retrieve(SymbolTable, opGivenNode_global, ID);
                //printf("Chcekpoint 2 post retrieve in ValFunCall.\n");

                if (opGivenNode_global == NULL)
                {
                    // semanticErrors ++;
                    //char string[200];
                    printf("Line %d: ERROR- Undeclared Output Variable %s in function call.\n", opGivenPars->line, opGivenPars->lex);
                    //addSemErrortoArray(string);
                    //printf("Line no. %d: Undeclared given op Variable HERE IN ValFunCall %s.\n", opGivenPars->line, opGivenPars->lex);
                    opGivenPars = opGivenPars->nextSibling;
                    opSTpars = opSTpars->next;
                    //return;
                    continue;
                }
                else
                {
                    opGivenNode = opGivenNode_global;
                    //printf("Retrived global identifier for given op lex: %s, type: %d in ValFunCall\n", opGivenNode_global->token->lexeme, opGivenNode_global->type);
                }
            }
            else
            {
                //printf("Retrived non global identifier for given op lex: %s type: %d nodetype: %d in ValFunCall\n", opGivenNode->token->lexeme, opGivenNode->type, opGivenPars->nodeType);
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
            tempOPST = (identifierNode*)retrieve(SymbolTable, opSTnode, ID);
            opSTnode = tempOPST;
            
            //printf("Retrieved the op par from Symbol Table with lex: %s\n\n\n", opSTnode->token->lexeme);

            if(opSTnode->type == opGivenNode->type) {
                Type nType = opSTnode->type;
                switch(nType) {
                    case RECORD_TYPE:
                    {
                        if(opSTnode->recordList != opGivenNode->recordList) {
                            //char string[200];
                            printf("Line %d: ERROR- Type mismatch in output of function call.\n", opGivenPars->line);
                            //addSemErrortoArray(string);
                            //printf("Type mismatch in output of function call.\n");
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
                            //char string[200];
                            printf("Line %d: ERROR- Type mismatch in output of function call.\n", opGivenPars->line);
                            //addSemErrortoArray(string);
                            //printf("Type mismatch in output of function call.\n");
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
                //char string[200];
                printf("Line %d: ERROR- Type mismatch in output of function call.\n", opGivenPars->line);
                //addSemErrortoArray(string);
                //semanticErrors++;
                //return;
            }

            opGivenPars = opGivenPars->nextSibling;
            opSTpars = opSTpars->next;
        }

        if(opSTpars == NULL && opGivenPars == NULL) {
            
        }
        else if(opGivenPars != NULL) {
            //char string[200];
            printf("Line %d: ERROR- Function call has extra given output parameters\n", call->line);
            //addSemErrortoArray(string);
            //printf("Line no. %d: Function call has extra given output parameters\n", opGivenPars->line);
            //semanticErrors++;
            //return;
        }
        else if(opSTpars != NULL) {
            //char string[200];
            printf("Line %d: ERROR- Function call has insufficient output parameters.\n", call->line);
            //addSemErrortoArray(string);
            //printf("Line no. %d: Function call has insufficient ST output parameters.\n", opGivenPars->line);
            //semanticErrors++;
            //return;
        }
    }
    //INPUT PARAMETERS 
    //printf("Satring inputpar\n");

    ast* ipGivenPars = ipCall->firstChild;
    parameters* ipSTpars = calledFunctionNode->ipParams;

    while (ipSTpars != NULL && ipGivenPars != NULL) {
        //ast* id, ast* func, Type type, bool is_global, int*offset

        //printf("\n\nipGivenPars current lex: %s\n", ipGivenPars->lex);

        //retrieve from ST node corresponding to AST
        Type ipGivenType = TypeUsingAST(ipGivenPars->nodeType, ipGivenPars->is_union);
        identifierNode* ipGivenNode = createINode(ipGivenPars, func, ipGivenType, false, &localOffset);
        ipGivenNode = (identifierNode*)retrieve(SymbolTable, ipGivenNode, ID);
        //printf("retrieve ip 1\n");
        if (ipGivenNode == NULL)
        {
            identifierNode *ipGivenNode_global = createINode(ipGivenPars, func, -1, true, &localOffset);
            ipGivenNode_global = (identifierNode *)retrieve(SymbolTable, ipGivenNode_global, ID);
            //printf("Chcekpoint 2 post retrieve in ValFunCall.\n");

            if (ipGivenNode_global == NULL)
            {
                // semanticErrors ++;
                //char string[200];
                printf("Line %d: ERROR- Undeclared given input Variable %s.\n", ipGivenPars->line, ipGivenPars->lex);
                //addSemErrortoArray(string);
                //printf("Line no. %d: Undeclared given ip Variable HERE IN ValFunCall %s.\n", ipGivenPars->line, ipGivenPars->lex);
                //return;
                ipGivenPars = ipGivenPars->nextSibling;
                ipSTpars = ipSTpars->next;
                continue;
            }
            else
            {
                ipGivenNode = ipGivenNode_global;
                //printf("Retrived global identifier for given ip lex: %s, type: %d in ValFunCall %d.\n", ipGivenNode_global->token->lexeme, ipGivenNode_global->type,ipGivenPars->nodeType);
            }
        }
        else
        {
            //printf("Retrived non global identifier for ip lex: %s type: %d nodetype: %d in ValFun Call\n", ipGivenNode->token->lexeme, ipGivenNode->type, ipGivenPars->nodeType);
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

        //printf("Is this being printed when ipst, ipSTnode->type: %d and ipGivenNode->type: %d\n", ipSTnode->type, ipGivenNode->type);
        if(ipSTnode->type == ipGivenNode->type) {
            Type nType = ipSTnode->type;
            switch(nType) {
                case RECORD_TYPE:
                {
                    if(ipSTnode->recordList != ipGivenNode->recordList) {
                        //char string[200];
                        printf("Line %d: ERROR- Type mismatch in input of function call.\n", ipGivenPars->line);
                        //addSemErrortoArray(string);
                        //printf("Type mismatch in input of function call.\n");
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
                        //char string[200];
                        printf("Line %d: ERROR- Type mismatch in input of function call.\n", ipGivenPars->line);
                        //addSemErrortoArray(string);
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
            //char string[200];
            printf("Line %d: ERROR- Type mismatch in input of function call.\n", ipGivenPars->line);
            //addSemErrortoArray(string);
            //semanticErrors++;
            //return;
        }

        ipGivenPars = ipGivenPars->nextSibling;
        //printf("ipGivenPars end of while loop lex: %s\n", ipGivenPars->lex);
        ipSTpars = ipSTpars->next;
    }

    //printf("Somwhere\n");

    if(ipGivenPars != NULL) {
        //char string[200];
        printf("Line %d: ERROR- Function call has extra input parameters\n", call->line);
        //addSemErrortoArray(string);
        //printf("Function call has extra input parameters\n");
        //semanticErrors++;
        return;
    }
    

    if(ipSTpars != NULL) {
        //char string[200];
        //printf("Insuff\n");
        printf("Line %d: ERROR- Function call has insufficient input parameters.\n", call->line);
        //addSemErrortoArray(string);
        //printf("Function call has insufficient input parameters.\n");
        //semanticErrors++;
        return;
    }

    ast* opGivenPars = opCall->firstChild;
    while(opGivenPars != NULL) {
        //printf("\nEntering loop for opSTpars in the function %s\n", funcIdentifier->lex);
        Type opGivenType = TypeUsingAST(opGivenPars->nodeType, opGivenPars->is_union);
        
        identifierNode* opGivenNode = createINode(opGivenPars, func, opGivenType, false, &localOffset);
        identifierNode* opGivenNode_global = createINode(opGivenPars, func, opGivenType, true, &localOffset);
        opGivenNode = (identifierNode*)retrieve(SymbolTable, opGivenNode, ID);
        //printf("Retrieve madarchod in %s, retrieved with lex\n", funcIdentifier->lex);

        if (opGivenNode == NULL)
        {
            //printf("Retrieve se pehle, retrieve se zyada\n");
            //identifierNode *opGivenNode_global = createINode(opGivenPars, func, -1, true, &localOffset);
            opGivenNode_global = (identifierNode *)retrieve(SymbolTable, opGivenNode_global, ID);
            //printf("Chcekpoint 2 post retrieve in ValFunCall.\n");

            if (opGivenNode_global == NULL)
            {
                // semanticErrors ++;
                //char string[200];
                //printf("Line %d: ERROR- Undeclared given output variable %s.\n", opGivenPars->line, opGivenPars->lex);
                //addSemErrortoArray(string);
                //printf("Line no. %d: Undeclared given op Variable HERE IN ValFunCall %s.\n", opGivenPars->line, opGivenPars->lex);
                //return;
                opGivenPars = opGivenPars->nextSibling;
                //opSTpars = opSTpars->next;
                continue;
            }
            else
            {
                opGivenNode = opGivenNode_global;
                //printf("Retrived global identifier for given op lex: %s, type: %d in ValFunCall\n", opGivenNode_global->token->lexeme, opGivenNode_global->type);
            }
        }
        else
        {
            //printf("Retrived non global identifier for given op lex: %s type: %d nodetype: %d in ValFunCall\n", opGivenNode->token->lexeme, opGivenNode->type, opGivenPars->nodeType);
        }
        opGivenNode->assigned = true;
        opGivenPars = opGivenPars->nextSibling;
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
        //printf("Chcekpoint 2 post retrieve in ValRead.\n");

        if (currNode_global == NULL)
        {
            // semanticErrors ++;
            //char string[200];
            printf("Line %d: ERROR- Reading into undeclared variable %s\n", curr->line, curr->lex);
            //addSemErrortoArray(string);
            //printf("Line no. %d: Undeclared Variable HERE %s in ValRead.\n", curr->line, curr->lex);
            return;
        }
        else
        {
            currNode = currNode_global;
            //printf("Retrived global identifier for lex: %s, type: %d in ValRead\n", currNode_global->token->lexeme, currNode_global->type);
        }
    }
    else
    {
        //printf("Retrived non global identifier for lex: %s type: %d nodetype: %d in ValRead\n", currNode->token->lexeme, currNode->type, curr->nodeType);
    }

    bool flagNumRnum = isNumRnum(curr);
    if(flagNumRnum == true) {
        //char string[200];
        printf("Line %d: ERROR- Trying to read into a Num or Rnum for lex: %s", curr->line, curr->lex);
        //addSemErrortoArray(string);
        //printf("Line no: %d: Trying to read into a Num or Rnum for lex: %s", curr->line, curr->lex);
        //semanticErrors++;
        return;
    }

    if(curr->firstChild != NULL) {
        if((currNode->type != RECORD_TYPE)) {
            //char string[200];
            printf("Line %d: ERROR- Variable %s is not of type record/union.", curr->line, curr->lex);
            //addSemErrortoArray(string);
            //printf("Variable is not of type record/union.");
            //semanticErrors++:
            return;
        }
        else if (currNode->type == RECORD_TYPE) {
            ast* attribute = curr->firstChild;
            //check for Nested recods type;
            // if no match-- semnatic error
            char *fConcatLex = (char *)malloc(sizeof(char) * strlen(curr->lex));
            strcpy(fConcatLex, curr->lex);
            //printf("\nConcatenating Second Child in ValRead: \n\n\n\n\n");
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
                    //printf("%s\n", fConcatLex);

                    if (fidNode == NULL)
                    {   
                        //char string[200];
                        printf("Line %d: ERROR- No field name %s in the Record or union\n", attribute->line, attribute->lex);
                        //addSemErrortoArray(string);
                        //printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                        // semanticErrors++;
                        return;
                    }
                    attribute = attribute->nextSibling;
                }
            }
        }    
    }
    currNode->assigned = true;
}

void validateWrite(ast* curr, ast* func) {
    curr = curr->firstChild;
    int localOffset = 0;
    Type currType = TypeUsingAST(curr->nodeType, curr->is_union);
    identifierNode* currNode = createINode(curr, func, currType, false, &localOffset);
    currNode = (identifierNode*)retrieve(SymbolTable, currNode, ID);

    if (currNode == NULL)
    {
        identifierNode *currNode_global = createINode(curr, func, -1, true, &localOffset);
        currNode_global = (identifierNode *)retrieve(SymbolTable, curr, ID);
        //printf("Chcekpoint 2 post retrieve in ValRead.\n");

        if (currNode_global == NULL)
        {
            // semanticErrors ++;
            //char string[200];
            printf("Line %d: ERROR- Trying to write out of an undeclared Variable %s.\n", curr->line, curr->lex);
            //addSemErrortoArray(string);
            //printf("Line no. %d: Undeclared Variable HERE %s in ValWrite.\n", curr->line, curr->lex);
            return;
        }
        else
        {
            currNode = currNode_global;
            //printf("Retrived global identifier for lex: %s, type: %d in ValWrite\n", currNode_global->token->lexeme, currNode_global->type);
        }
    }
    else
    {
        //printf("Retrived non global identifier for lex: %s type: %d nodetype: %d in ValWrite\n", currNode->token->lexeme, currNode->type, curr->nodeType);
    }

    if(curr->firstChild != NULL) {
        if((currNode->type != RECORD_TYPE && currNode->type != UNION_TYPE)) {
            //char string[200];
            printf("Line %d: ERROR- Variable %s is not of type record.\n", curr->line, curr->lex);
            //addSemErrortoArray(string);
            //semanticErrors++:
            return;
        }
        else if (currNode->type == RECORD_TYPE || currNode->type == UNION_TYPE) {
            ast* attribute = curr->firstChild;
            //check for Nested recods type;
            // if no match-- semnatic error
            char *fConcatLex = (char *)malloc(sizeof(char) * strlen(curr->lex));
            strcpy(fConcatLex, curr->lex);
            //printf("\nConcatenating Second Child in ValWrite: \n\n\n\n\n");
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
                    //printf("%s\n", fConcatLex);

                    if (fidNode == NULL)
                    {
                        //char string[200];
                        printf("Line %d: ERROR- No field name %s in the Record\n", attribute->line, attribute->lex);
                        //addSemErrortoArray(string);
                        //printf("Line no %d: No field name %s in the Record or union\n", attribute->line, attribute->lex);
                        // semanticErrors++;
                        return;
                    }
                    attribute = attribute->nextSibling;
                }
            }
        }    
    }
    
    currNode->assigned = true;
}

void validateIterative(ast* curr, ast* func) {
    //printf("Entered valIter with current lexeme: %s, and function lexeme: %s\n", curr->lex, func->lex);
    ast* temp = curr->firstChild->nextSibling;
    for(; temp != NULL; temp = temp->nextSibling) {
        handleStmt(temp, func);
    }
    validateBoolean(curr->firstChild, func);
    //printf("Exitbool\n");
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
    //printf("Entered valBool with current lexeme: %s, and function lexeme: %s\n", curr->lex, func->lex);
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
        //printf("It is relOp with lexeme: %s\n", curr->lex);
        ast* fChild = curr->firstChild;
        ast* sChild = curr->firstChild->nextSibling;
        if(isNumRnum(fChild) == true && isNumRnum(sChild) == true) {
            if(fChild->nodeType == sChild->nodeType) {
                Type type = (fChild->nodeType == INTEGER) ? INT_TYPE : REAL_TYPE;
                return createINode(fChild, func, type, false, &localOffset);
            }
            else {
                //char string[200];
                printf("Line %d: ERROR- Operand Type error mismatch\n", curr->line);
                //addSemErrortoArray(string);
                //printf("Line no. %d: Operand Type error mismatch\n", curr->line);
                return NULL;
            }
        } 
        else {
            //printf("\nLine no. %d: Both are not integer or real\n", curr->line);
            identifierNode* fNode, *sNode;
            
            if(fChild->nodeType == ID && sChild->nodeType == ID) {
                //printf("\nBoth are identfiers\n");
                //create temp nodes to lookup in symbol table
                identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                fNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                //printf("In validateBoolean fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                if(fNode == NULL) {
                    fNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(fNode == NULL) {
                        //char string[200];
                        printf("Line %d: ERROR- Using undeclared identifier %s\n", fChild->line, fChild->lex);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Using undeclared identifier %s\n", fChild->line, fChild->lex);
                    }
                }
                
                t1 = createINode(sChild, func, -1, false, &localOffset);
                t1_global = createINode(sChild, func, -1, true, &localOffset);
                sNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                //printf("sNode retrieved with lexeme: %s\n", sNode->token->lexeme);
                if(sNode == NULL) {
                    sNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                    if(sNode == NULL) {
                        //char string[200];
                        printf("Line %d: ERROR- Using undeclared identifier %s\n", sChild->line, sChild->lex);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Using undeclared identifier %s\n", sChild->line, sChild->lex);
                        return NULL;
                    }
                }



                if((fNode->type == INT_TYPE || fNode->type == REAL_TYPE) && (sNode->type == INT_TYPE || sNode->type == REAL_TYPE)) {
                    
                    if(fNode->assigned == false || sNode->assigned == false) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Can not perform relational operation on unassigned identifiers\n", curr->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Can not perform relOp on unassigned identifiers\n", curr->line);
                        return NULL;
                    }
                    
                    if(fNode->type == sNode->type) {
                        //return createINode(, func, fNode->type, false, &localOffset);
                        
                        //printf("\nLeft and right nodes are not records\n");
                        return fNode;
                    }
                    else {
                        //char string[200];
                        printf("Line %d: ERROR- Operand Type error mismatch\n", curr->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Operand Type error mismatch\n", curr->line);
                    }
                }

                else {
                    //the node is a record or union
                    //printf("\nIn valBool, one of the nodes is a record or union\n");
                    int ferror = 0;
                    if(fChild->firstChild == NULL && (fNode->type == RECORD_TYPE || fNode->type == UNION_TYPE)) {
                        //char string[200];
                        printf("Line %d: ERROR- Record or Union type f can not be used with relational operator\n", fChild->line);
                        ferror = 1;
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Record or Union type can not be used with relational operator\n", fChild->line);
                    }
                    if(sChild->firstChild == NULL && (sNode->type == RECORD_TYPE || sNode->type == UNION_TYPE)) {
                        //char string[200];
                        printf("Line %d: ERROR- Record or Union type l can not be used with relational operator\n", sChild->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Record or Union type can not be used with relational operator\n", sChild->line);
                        return NULL;
                    }
                    if(ferror == 1) return NULL;

                    //this part will only be reached if there is an expansion
                    //iterate through the node, retrieve their node in each loop
                    //call searchInFieldList, if it is present then proceed
                    
                    ast* fFieldId = fChild->firstChild;
                    ast* sFieldId = sChild->firstChild;

                    char* fConcatLex = (char*)malloc(sizeof(char)*strlen(fChild->lex));
                    strcpy(fConcatLex, fChild->lex);

                    //printf("\nConcatenating Second Child: \n\n\n\n\n");
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
                        //printf("%s\n", fConcatLex);

                        if(fidNode == NULL) {
                            //char string[200];
                            printf("Line %d: ERROR- No field name %s in the Record or union\n", fFieldId->line, fFieldId->lex);
                            //addSemErrortoArray(string);
                            //printf("Line no %d: No field name %s in the Record or union\n", fFieldId->line, fFieldId->lex);
                            fNode = fidNode;
                            break;
                        }
                        fNode = fidNode;
                        fFieldId = fFieldId->nextSibling;
                    }

                    char* sConcatLex = (char*)malloc(sizeof(char)*strlen(sChild->lex));
                    strcpy(sConcatLex, sChild->lex);

                    //printf("\nConcatenating First Child: \n\n\n\n\n");
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
                        //printf("%s\n", sConcatLex);

                        if(fidNode == NULL) {
                            //char string[200];
                            printf("Line %d: ERROR- No field name %s in the Record or union\n", sFieldId->line, sFieldId->lex);
                            //addSemErrortoArray(string);
                            //printf("Line no %d: No field name %s in the Record or union\n", sFieldId->line, sFieldId->lex);
                            sNode = fidNode;
                            break;
                        }
                        sNode = fidNode;
                        sFieldId = sFieldId->nextSibling;
                    }
                    //printf("Concatenation ended\n");

                    if(fNode == NULL || sNode == NULL) {
                        return NULL;
                    }
                    
                    if(fNode->assigned == false || sNode->assigned == false) {
                        //semanticErrors++;
                        //char string[200];
                        printf("Line %d: ERROR- Can not perform relational operator on unassigned identifiers\n", curr->line);
                        //addSemErrortoArray(string);
                        printf("Line no. %d: Can not perform relOp on unassigned identifiers\n", curr->line);
                        return NULL;
                    }

                    if(fNode->type == sNode->type) {
                        if(fNode->type == INT_TYPE || fNode->type == REAL_TYPE) {
                            return fNode;
                        }
                        else {
                            //char string[200];
                            printf("Line %d: ERROR- Type error mismatch\n", fChild->line);
                            //addSemErrortoArray(string);
                            //printf("Line no %d: Type error mismatch\n", fNode->token->lineNo);
                            return NULL;
                        }
                    }
                    else {
                        //char string[200];
                        printf("Line %d: ERROR- Type error mismatch\n", fChild->line);
                        //addSemErrortoArray(string);
                        return NULL;
                    }
                }
            }
            else {
                //printf("\nOne is ID and other is not\n");
                if(fChild->nodeType == ID) {
                    identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                    identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                    fNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                    //printf("In validateBoolean fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                    if(fNode == NULL) {
                        fNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                        if(fNode == NULL) {
                            //char string[200];
                            printf("Line %d: ERROR- Using undeclared identifier %s\n", fChild->line, fChild->lex);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Using undeclared identifier %s\n", fChild->line, fChild->lex);
                            return NULL;
                        }
                    }

                    Type currType = fNode->type;
                    if(currType == RECORD_TYPE) {
                        if(fChild->firstChild == NULL) {
                            //char string[200];
                            printf("Line %d: ERROR- Relational operator can not be used with records\n", fChild->line);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Relational operator can not be used with Unions or records\n", fChild->line);
                            return NULL;
                        }
                        else {
                            ast* fFieldId = fChild->firstChild;

                            char* fConcatLex = (char*)malloc(sizeof(char)*strlen(fChild->lex));
                            strcpy(fConcatLex, fChild->lex);

                            //printf("\n\nConcatenation starting for fFieldId, current conclex: %s\n\n\n", fConcatLex);
                            while(fFieldId) {
                                int x = strlen(fConcatLex);
                                int y = strlen(fFieldId->lex);
                                fConcatLex = (char*)realloc(fConcatLex, sizeof(char)*(x+y+1));
                                char dot[] = ".";
                                strcat(fConcatLex, dot);
                                strcat(fConcatLex, fFieldId->lex);
                                //printf("Current state of conlex: %s\n", fConcatLex);
                                
                                identifierNode* retNode = createINode(fFieldId, func, -1, false, &localOffset);
                                retNode->token->lexeme = fConcatLex;
                                

                                identifierNode* fidNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
                                //printf("Retrieved fidNode dk if it is NULL or not tho, vibes XOXOXO yass kween\n");
                                if(fidNode == NULL) {
                                    //char string[200];
                                    printf("Line %d: ERROR- No field name %s in the Record or union\n", fFieldId->line, fFieldId->lex);
                                    //addSemErrortoArray(string);
                                    //printf("Line no %d: No field name %s in the Record or union\n", fFieldId->line, fFieldId->lex);
                                    fNode = fidNode;
                                    break;
                                }
                                else {
                                    //printf("Retrived successfully, fidNode lex: %s\n", fidNode->token->lexeme);
                                    fNode = fidNode;
                                    fFieldId = fFieldId->nextSibling;
                                }
                            }
                        }
                        if(fNode->assigned == false) {
                            //semanticErrors++;
                            //char string[200];
                            printf("Line %d: ERROR- Can not perform relational operation on unassigned identifier\n", curr->line);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Can not perform relOp on unassigned identifier\n", curr->line);
                            return NULL;
                        }
                    }
                    if((currType == INT_TYPE && sChild->nodeType == NUM) || (currType == REAL_TYPE && sChild->nodeType == RNUM)) {
                        //printf("Line no. %d: Operand type mismatch", fChild->line);
                        if(fNode->assigned == false) {
                            //semanticErrors++;
                            //char string[200];
                            printf("Line %d: ERROR- Can not perform relational operation on unassigned identifier\n", curr->line);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Can not perform relOp on unassigned identifiers\n", curr->line);
                            return NULL;
                        }
                        return fNode;
                    }
                    else {
                        //char string[200];
                        printf("Line %d: ERROR- Operand type mismatch\n", fChild->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Operand type mismatch\n", fChild->line);
                        return NULL;
                    }
                }
                else if(sChild->nodeType == ID) {
                    identifierNode* t1 = createINode(fChild, func, -1, false, &localOffset);
                    identifierNode* t1_global = createINode(fChild, func, -1, true, &localOffset);
                    sNode = (identifierNode*)retrieve(SymbolTable, t1, ID);
                    //printf("In validateBoolean fNode retrieved with lexeme: %s\n", fNode->token->lexeme);
                    if(sNode == NULL) {
                        sNode = (identifierNode*)retrieve(SymbolTable, t1_global, ID);
                        if(sNode == NULL) {
                             //char string[200];
                            printf("Line %d: ERROR- Using undeclared identifier %s\n", sChild->line, sChild->lex);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Using undeclared identifier %s\n", fChild->line, fChild->lex);
                            return NULL;
                        }
                    }
                    
                    Type currType = sNode->type;
                    if(currType == RECORD_TYPE || currType == UNION_TYPE) {
                        if(sChild->firstChild == NULL) {
                            //char string[200];
                            printf("Line %d: ERROR- Relational operator can not be used with Unions or records\n", sChild->line);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Relational operator can not be used with Unions or records\n", fChild->line);
                            return NULL;
                        }
                        else {
                            char *sConcatLex = (char *)malloc(sizeof(char) * strlen(sChild->lex));
                            strcpy(sConcatLex, sChild->lex);

                            ast* sFieldId = sChild->firstChild;

                            //printf("\nConcatenating Second Child: \n\n\n\n\n");
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
                                //printf("%s\n", sConcatLex);

                                if (fidNode == NULL)
                                {
                                    //char string[200];
                                    printf("Line %d: ERROR-  No field name %s in the Record\n", sFieldId->line, sFieldId->lex);
                                    //addSemErrortoArray(string);
                                    //printf("Line no %d: No field name %s in the Record or union\n", sFieldId->line, sFieldId->lex);
                                    sNode = fidNode;
                                    break;
                                }
                                sNode = fidNode;
                                sFieldId = sFieldId->nextSibling;
                            }
                            //printf("Concatenation ended\n");
                        }
                        if(sNode->assigned == false) {
                            //semanticErrors++;
                            //char string[200];
                            printf("Line no. %d: Can not perform relOp on unassigned identifiers\n", sChild->line);
                            //addSemErrortoArray(string);
                            //printf();
                            return NULL;
                        }
                    }
                    if((currType == INT_TYPE && fChild->nodeType == NUM) || (currType == REAL_TYPE && fChild->nodeType == RNUM)) {
                        //printf("Line no. %d: Operand type mismatch", fChild->line);
                        if(sNode->assigned == false) {
                            //semanticErrors++;
                            //char string[200];
                            printf("Line %d: ERROR- Can not perform relOp on unassigned identifiers\n", curr->line);
                            //addSemErrortoArray(string);
                            //printf("Line no. %d: Can not perform relOp on unassigned identifiers\n", curr->line);
                            return NULL;
                        }
                        return sNode;
                    }
                    else {
                        //char string[200];
                        printf("Line %d: ERROR- Operand type mismatch", fChild->line);
                        //addSemErrortoArray(string);
                        //printf("Line no. %d: Operand type mismatch", fChild->line);
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
    
    functionNode* retNode = createFNode(func);
    functionNode* funcInfo = (functionNode*)retrieve(SymbolTable, retNode, FUNCTION_SEQ);
    //printf("\n\nIn valReturn Function whose info was retrieved: %s and number of output params: %d\n", funcInfo->token->lexeme, funcInfo->numOp);

    parameters* outputParams = funcInfo->opParams;
    identifierNode* opInfo = NULL;
    
    //calculating num of return parameters according to AST
    int numReturn = 0;
    ast* tempOP = curr->firstChild;
    while(tempOP) {
        numReturn++;
        tempOP = tempOP->nextSibling;
    }

    if(numReturn != funcInfo->numOp) {
        //char string[200];
        printf("Line %d: ERROR- Number of return values: %d not equal to number of output parameters: %d in function %s\n", curr->line, numReturn, funcInfo->numOp, funcInfo->token->lexeme);
        //addSemErrortoArray(string);
        //printf("Line no. %d: Number of return values: %d not equal to number of output parameters: %d in function %s\n", curr->line, numReturn, funcInfo->numOp, funcInfo->token->lexeme);
        return;
    }

    //printf("In valret before for loop\n");

    int localOffset = 0;

    //return;
    identifierNode* currNode = (identifierNode*)malloc(sizeof(identifierNode));
    parameters* iterParams = outputParams;
    for(int i=0; i<numReturn; i++, child = child->nextSibling, iterParams = iterParams->next) {
        identifierNode* childNode = createINode(child, func, -1, false, &localOffset);
        childNode = (identifierNode*)retrieve(SymbolTable, childNode, ID);
        if(childNode == NULL) {
            identifierNode* childNode_global = createINode(child, func, -1, true, &localOffset);
            childNode_global = (identifierNode*)retrieve(SymbolTable, childNode_global, ID);
            if(childNode_global == NULL) {
                //char string[200];
                printf("Line %d: ERROR- undeclared variable %s is being returned\n", child->line, child->lex);
                //addSemErrortoArray(string);
                //printf("Line %d: ERROR- undeclared variable %s is being returned\n", child->line, child->lex);
                continue;
            }
            else {
                childNode = childNode_global;
            }
        }
        //printf("Retrieve block done\n");
        if(childNode->assigned == false) {
            //char string[200];
                printf("Line %d: ERROR- variable %s being returned without being assigned\n", child->line, child->lex);
                //addSemErrortoArray(string);
            //printf("Line %d: ERROR- variable %s being returned without being assigned\n", child->line, child->lex);
        }
        //use iterators for output parameters and return values
        else {
            identifierNode* iterNode = (identifierNode*)malloc(sizeof(identifierNode));
            iterNode->width = -1;
            iterNode->offset = -1;
            iterNode->type = -1;
            iterNode->token = (tokenInfo*)malloc(sizeof(tokenInfo));
            iterNode->token->lexeme = iterParams->token->lexeme;
            iterNode->function = (tokenInfo*)malloc(sizeof(tokenInfo));
            iterNode->function->lexeme = func->lex;
            iterNode->global = false;
            //printf("Reaching here\n");

            identifierNode* iterNode_global = (identifierNode*)malloc(sizeof(identifierNode));
            iterNode_global->width = -1;
            iterNode_global->offset = -1;
            iterNode_global->type = -1;
            iterNode_global->token = (tokenInfo*)malloc(sizeof(tokenInfo));
            iterNode_global->token->lexeme = iterParams->token->lexeme;
            iterNode_global->function = (tokenInfo*)malloc(sizeof(tokenInfo));
            iterNode_global->function->lexeme = func->lex;
            iterNode_global->global = true;

            iterNode = (identifierNode*)retrieve(SymbolTable, iterNode, ID);
            if(iterNode == NULL) {
                iterNode_global = (identifierNode*)retrieve(SymbolTable, iterNode_global, ID);
                if(iterNode_global == NULL) {
                    printf("How is it even reaching here?\n");
                }
                else {
                    iterNode = iterNode_global;
                }
            }

            if(childNode->type != iterNode->type) {
                //char string[200];
                printf("Line %d: ERROR- Unexpected return parameter found. Parameter returned: %s and Parameter expected: %s\n", child->line, child->lex, iterParams->token->lexeme);
                //addSemErrortoArray(string);
                //printf("Line no. %d: Unexpected return parameter found. Parameter returned: %s and Parameter expected: %s\n", child->line, child->lex, iterParams->token->lexeme);
            }
        }
    }
}

void validateConditional(ast* curr, ast* func) {
    //printf("Entered valCon with func: %s\n", func->lex);
    validateBoolean(curr->firstChild, func);
    handleStmt(curr->firstChild->nextSibling, func);
    //printf("Handled the first stmt in Conditional for lexeme %s\n", curr->firstChild->nextSibling->lex);
    ast* iter = curr->firstChild->nextSibling->nextSibling;
    if(iter == NULL) {
        //printf("Else part hai hi nahi\n");
        return;
    }
    while(iter != NULL && iter->nodeType != ELSE) {
       // printf("Entering while loop in valCond with lex: %s and function: %s\n", iter->lex, func->lex);
        handleStmt(iter, func);
       //printf("Handled the while loop stmt in Conditional for lexeme %s with firstChild lex: %s\n", iter->lex, iter->firstChild->lex);
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
            //printf("exited validateAssign for lex: %s\n", curr->lex);
            break;
        case ITERATIVE:
            validateIterative(curr, func);
            //printf("Exited validateIterative for lex: %s\n", curr->lex);
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
            //printf("\n\nEntered validateFunCall for lex: %s\n", curr->lex);
            validateFunCall(curr, func);
            //printf("\n\nExited validateFunCall for lex: %s\n", curr->lex);
            break;
    }
    handleStmt(curr->firstChild, func);
}

void validateFunction(ast* curr) {
    if(curr == NULL) {
        return;
    }
    
    int localOffset = 0;
    
    if(curr->nodeType != MAIN_FUNCTION) {
        ast* iparams = curr->firstChild->firstChild;
        //printf("iparams nodetype: %d\n", iparams->firstChild->nodeType);
        while(iparams != NULL) {
            //printf("Entering iparams loop with lex: %s\n", iparams->firstChild->lex);
            identifierNode* retNode = createINode(iparams->firstChild, curr, -1, false, &localOffset);
            //printf()
            retNode = (identifierNode*)retrieve(SymbolTable, retNode, ID);
            //printf("Retrieved iparams node\n");
            if(retNode == NULL) {
                identifierNode* retNode_global = createINode(iparams->firstChild, curr, -1, true, &localOffset);
                retNode_global = (identifierNode*)retrieve(SymbolTable, retNode_global, ID);
                if(retNode_global == NULL) {
                    //printf("Line %d: ERROR- invalid input parameter %s\n", iparams->firstChild->line, iparams->firstChild->lex);
                    //semanticErrors++;
                    return;
                }
                else {
                    retNode = retNode_global;
                }
            }
            retNode->assigned = true;
            iparams = iparams->nextSibling;
        }
    }

    //printf("\nIn validate function, current node name: %s\n", curr->lex);
    ast* child = curr->firstChild;
    for(; child!=NULL; child=child->nextSibling) {
        //printf("\nIn valFunc, going to handleStmt, child lexeme: %s and curr lexeme: %s\n", child->lex, curr->lex);
        handleStmt(child, curr);
        //printf("\nFinished validateFunction inside for loop, child lexeme: %s\n", child->lex);
    }
}