#include <stdio.h>
#include <string.h>
#include "parser.h"

#define offset eps

tokenInfo arr[5];

parseTree initTree(g_Term symbol, int lineNo) {
    // TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    // node->symbol = symbol;
    // node->isTerminal = isTerm(symbol);
    parseTree ptree = (parseTree)malloc(sizeof(struct tree));
    ptree->symbol = symbol;
    ptree->isTerminal = isTerm(symbol);
    ptree->lineNo = lineNo;
    ptree->firstChild = NULL;
    ptree->nextSibling = NULL;
    ptree->parent = NULL;
    return ptree;
}

parseTree getNearestUncle(parseTree curr){
	//reached root of tree
	if(!curr->parent){
		return NULL;
	}
	if(curr->parent->nextSibling){
		return curr->parent->nextSibling;
	}
	else return getNearestUncle(curr->parent);
}

void pushRuleIntoStack(g_RHS* rule, Stack* stack){
    Stack* temp = initStack();
    while(rule){
        push(temp, rule->symbol);
        rule = rule->next;
    }
    while(top(temp)!=-1){
        g_Term topStack = top(temp);
        pop(temp);
        push(stack, topStack);
    }
    printf("Pushed the rule into stack.\n Top of stack is: ");
    printNonTerminal(top(stack));
}

parseTree addChildren(g_Term nonterm, g_Term term, Stack* stack, parseTree ptree){
    g_RHS* rhs = (g_RHS*)malloc(sizeof(g_RHS)); 
    rhs = parseTable[nonterm][term-51];
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


parseTree parseInputSourceCode(char* testcaseFile) {

    printf("In source code\n");
    FILE* fp = startLexer(testcaseFile);
    initTable();

    //init stack, push SENTINEL, push start symbol
    Stack* stack = initStack();
    push(stack, SENTINEL);
   // printf("Bruh\n");
    //printToken(top(stack));
    //printf("\n");
     push(stack, program);
     printf("Top of the stack: %d\n", top(stack));
    
    parseTree ptree = initTree(program, 1);
    //printf("Check");
    parseTree curr = ptree;

    tokenInfo look = getNextToken();
    // if(look.tid>=eps)
    //  printToken(look.tid);
    // else 
    //     printNonTerminal(look.tid);
    // printf("\n");
    g_RHS* rule;

     while(look.tid == TK_COMMENT) {
        look = getNextToken(); //ignore the comments until a newline
    }

    // while(1)
    //     {
    //         look = getNextToken();
    //         if(look.tid==SENTINEL)
    //             break;
    //         if(look.tid>=eps)
    //             printToken(look.tid);
    //         else 
    //             printNonTerminal(look.tid);
    //         printf("\n");
    //     }
    //printToken(look.tid);

    while(1){
        g_Term topStack = top(stack);
        pop(stack);
        printToken(topStack);
        if(topStack == -1) {
            break;
        }
        while(isTerm(topStack)) {
            printf("Inner While loop\n");
            if(topStack==eps){
                topStack = top(stack);
                pop(stack);
                if(topStack == -1) {
                    break;
                }
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
                if(topStack == SENTINEL && look.tid==SENTINEL){
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
                    //if topStack is SENTINEL then it is an error, will be handled later in code
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
                            while(look.tid != SENTINEL) look = getNextToken();
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
        }

        //get the corresponidng rule from the parse table
        rule = parseTable[topStack][look.tid-offset];
        printRule(rule);
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
                    while(look.tid != SENTINEL) look = getNextToken();
                    printf("Syntax Error: Input is consumed but stack not empty\n");
                    fclose(fp);
                    return ptree;
                }
                continue;
            }

        }
        printRule(rule);
        printf("\nPushing into Stack\n");
        pushRuleIntoStack(rule, stack);
        curr = addChildren(topStack, look.tid, stack, curr);
    }
    return NULL;
}

void inorderNary(parseTree curr, int* numNodes) {
    if(!curr) return;

    parseTree temp = (parseTree)malloc(sizeof(struct tree));
    
    if(curr->firstChild) {
        inorderNary(curr->firstChild, numNodes);
        printToken(curr->firstChild->symbol);
        *numNodes = *numNodes + 1;
    }
    else {
        printToken(curr->firstChild->symbol);
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

void printSet(bool* array, int len){
    printf("[");
    for(int i=0; i<len;i++){
        printf("%d ", array[i]);
    }
    printf("]\n");

}

void printSetSymbols(int*array, int len){
    
}

/*table createParseTable(Grammar G, FirstAndFollow F, table T){

}*/

int main(){

// printf("No. of terminals is %d and no. of NTs is %d\n", TERMINALS, NON_TERMINALS);
    bool A[5] = {1, 0, 0, 1, 0};
    bool B[5] = {0, 0, 1, 1, 0};
    printSet(set_union(A, B, 5), 5);


    char* testFile = "custom1.txt";
    initGrammar(G);
	populateFirstFollow("First.txt",true);
    populateFirstFollow("Follow.txt",false);
    segFaultsSuck();
    printRule(parseTable[program][TK_MAIN-51]);


    parseTree ptree = parseInputSourceCode(testFile);
    printf("Check");
    int numNodes = 0;
    inorderNary(ptree, &numNodes);
    printf("\n\nNumber of nodes in the tree is: %d\n\n", numNodes);
}