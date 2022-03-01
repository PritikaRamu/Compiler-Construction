#include <stdio.h>
#include <string.h>
#include "parser.h"


tokenInfo arr[5];


void pushRuleIntoStack(g_Term nonterm, g_Term term, Stack* stack){
    Stack* temp = initStack();
    g_Node* rhs = (g_Node*)malloc(sizeof(g_Node)); 
    rhs = parseTable[nonterm][term];
    while(rhs){
        push(temp,rhs->symbol);
        rhs=rhs->next;
    }
    while(top(temp)!=$){
        g_Term topStack = top(temp);
        pop(temp);
        push(stack, topStack);
    }
}

parseTree addChildren(g_Term nonterm, g_Term term, Stack* stack, parseTree ptree){
    g_Node* rhs = (g_Node*)malloc(sizeof(g_Node)); 
    rhs = parseTable[nonterm][term];
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
    return ptree->firstChild;
    //printf("%d ",curr->symbol);
}


parseTree parseInputSourceCode(char* testcaseFile, table T) {


    fp = startLexer(testcaseFile);

    //init stack, push $, push start symbol
    Stack* stack = initStack();
    push(stack, $);
    push(stack,program);
    
    parseTree ptree = initTree(program, 1);

    parseTree curr = ptree;

    tokenInfo look = getNextToken();

    g_RHS* rule;

    while(look.tid == TK_COMMENT) {
        look = getNextToken(); //ignore the comments until a newline
    }

    while(1){
        g_Term topStack = top(stack);
        pop(stack);

        while(isTerm(topStack)) {
            
            if(topStack==eps){
                topStack = top(stack);
                pop(stack);
                curr->lineNo = look.lineNo;
                if(curr->nextSibling){
                    curr= curr->nextSibling;
                }
                else if(getNearestUncle(curr)){
                    curr = getNearestUncle(curr);
                }

                continue;
            }

            if(topStack!=look.tid){
                //code is consumed along with the stack
                if(topStack == $ && look.tid==SENTINEL){
                    fclose(fp);
                    return ptree;
                }
                //input is consumed, stack not empty
                else if(look.tid == SENTINEL){
                    printf("Syntax error: Expected ");
                    printToken(topStack);
                    printf(" at line %d \n",look.lineNo);
                    return ptree;
                }
                //stack may or may not be empty, input still coming in
                else{
                    printf("Syntax error\n");
                    topStack = top(stack);
                    pop(stack);
                    //if topStack is $ then it is an error, will be handled later in code
                    curr->lineNo = look.lineNo;
                    
                    //update curr to sibling if it has one else to nearest uncle
                    if(curr->nextSibling){
                        curr= curr->nextSibling;
                    }
                    else if(getNearestUncle(curr)){
                        curr = getNearestUncle(curr);
                    }

                    if(!isTerm(topStack) && !parseTable[topStack][look.tid]) break; //go to part of code where we expand the non-terminal

                    //top of the stack is not the same as token so we get next token
                    //top of the stack is terminal or non terminal which does not match with look
                    if(topStack != look.tid) {
                        look = getNextToken();
                        while(look.tid == TK_COMMENT) look = getNextToken();
                        //remove the comments and check if lookahead token is now equal to top of the stack
                        if(topStack != look.tid) {
                            //if token still not equal then syntax error
                            printf("Syntax error, expected %d token but got %d", topStack, look.tid);
                            while(look.tid != $) look = getNextToken();
                            printf("Syntax Error: Input is consumed but stack not empty\n");
                            fclose(fp);
                            return ptree;
                        }
                        continue;
                    }
                }
            }

            //the top of the stack is a terminal and matches the input
            //pop the stack and move the pointer ahead
            else {
                topStack = top(stack);
                pop(stack);
                curr->lineNo = look.lineNo;

                if(curr->nextSibling) {
                    curr = curr->nextSibling;
                }
                else if(getNearestUncle(curr)) {
                    curr = getNearestUncle(curr);
                }
                else continue;

                look = getNextToken();
            }

            
            // if(look.tid == topStack || topStack == eps){
            //    if(curr->nextSibling){
            //         curr= curr->nextSibling;
            //     }
            //     else if(getNearestUncle(curr)){
            //         curr = getNearestUncle(curr);
            //     }
            //     else continue;
            // }
            // if(topStack != eps){
            //     look = getNextToken();
            // }
            
        }

        //get the corresponidng rule from the parse table
        rule = parseTable[topStack][look.tid];

        //error handling, rule 
        if(!rule) {
            printf("Syntax error: unexpected token\n");

            topStack = top(stack);
            pop(stack);

            if(curr->nextSibling){
                curr = curr->nextSibling;
            }
            else if(getNearestUncle(curr)){
                curr = getNearestUncle(curr);
            }

            if(isTerm(topStack) && !parseTable[topStack][look.tid]) continue;

            if(topStack != look.tid){
                look = getNextToken();
                while(look.tid == TK_COMMENT) look = getNextToken();

                if(topStack != look.tid) {
                    while(look.tid != $) look = getNextToken();
                    printf("Syntax Error: Input is consumed but stack not empty\n");
                    fclose(fp);
                    return ptree;
                }
                continue;
            }

            pushRuleIntoStack(topStack, look.tid, stack);
            curr = addChildren(topStack, look.tid, stack, curr);
        }
    }
    return NULL;
}

void inorderNary(parseTree curr, int* numNodes) {
    if(!curr) return;

    parseTree temp = (parseTree)malloc(sizeof(struct tree));
    
    if(curr->firstChild) {
        inorderNary(curr->firstChild, numNodes);
        *numNodes = *numNodes + 1;
    }
    else {
        *numNodes = *numNodes + 1;
    }
}

void ComputeFirstAndFollowSets (Grammar G){

}

// int* First(Grammar G, g_Term NT){
    
//     int array_size = sizeof(int)*TERMINALS;
//     int* first_array = (int*)malloc(array_size);
//     memset(first_array, 0, array_size);
//     // printf("Size is %d and element is %d \n", array_size, first_array[59]);

//     g_Node* head = nt.head;

//     if(head->isTerminal){
//         first_array[head->symbol]=1;
//     }

//     else{
//         int* other_first = First()
//     }
    

// }

void printSet(int* array, int len){
    printf("[");
    for(int i=0; i<len;i++){
        printf("%d ", array[i]);
    }
    printf("]\n");

}

void printSetSymbols(int*array, int len){
    
}

table createParseTable(Grammar G, FirstAndFollow F, table T){

}

int main(){

// printf("No. of terminals is %d and no. of NTs is %d\n", TERMINALS, NON_TERMINALS);
    int A[5] = {1, 0, 0, 1, 0};
    int B[5] = {0, 0, 1, 1, 0};
    printSet(set_union(A, B, 5), 5);


}