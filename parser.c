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

void pushRuleIntoStack(g_RHS* rule1, Stack* stack){
    Stack* temp = initStack();
    g_RHS* rule = rule1;
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
    if(top(stack)>=eps)
        printToken(top(stack));
    else
        printNonTerminal(top(stack));
}

parseTree addChildren(g_RHS* rule1, parseTree ptree){
    //g_RHS* rhs = (g_RHS*)malloc(sizeof(g_RHS)); 
    //rule = parseTable[nonterm][term-eps];
    g_RHS* rule = rule1;
    parseTree curr;
    if(rule!=NULL){
        parseTree fChild = (parseTree)malloc(sizeof(struct tree));
        fChild->symbol = rule->symbol;
        fChild->isTerminal = rule->isTerminal;
        fChild->firstChild = NULL;
        fChild->nextSibling = NULL;
        fChild->parent = ptree;
        ptree->firstChild = fChild;
        curr = fChild;
        rule = rule->next;
    }
    while(rule){
        //printf("%d ",curr->symbol);
        parseTree temp = (parseTree)malloc(sizeof(struct tree));
        temp->symbol = rule->symbol;
        temp->isTerminal = rule->isTerminal;
        temp->firstChild = NULL;
        temp->nextSibling = NULL;
        temp->parent = ptree;
        curr->nextSibling = temp;
        curr = temp;
        rule = rule->next;
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
    push(stack, program);
    printf("Top of the stack: ");
    if(top(stack)>=eps)
        printToken(top(stack));
    else
        printNonTerminal(top(stack));
    
    parseTree ptree = initTree(program, 1);
    parseTree curr = ptree;
    tokenInfo look = getNextToken();
    
    g_RHS* rule;
    if(look.tid==SENTINEL)
        return ptree;
     while(look.tid == TK_COMMENT) {
        look = getNextToken(); //ignore the comments until a newline
    }
    if(look.tid==SENTINEL)
        return ptree;

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
        if(topStack>=eps)
            printToken(topStack);
        else
            printNonTerminal(topStack);
        
        printf("check\n\n");
        if(topStack == -1) {
            break;
        }

        while(topStack>=eps) {
            printf("Inner While loop\n");
            if(topStack==eps){
                topStack = top(stack);
                pop(stack);
                printf("after eps pop, %d\n",topStack);
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
                
                //input is consumed, stack not empty
                if(look.tid == SENTINEL){
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
                        if(look.tid==SENTINEL)
                            return ptree;
                            
                        while(look.tid == TK_COMMENT) look = getNextToken();
                        //remove the comments and check if lookahead token is now equal to top of the stack
                        if(topStack != look.tid) {
                            //if token still not equal then syntax error
                            printf("Syntax error, expected %d token but got %d", topStack, look.tid);
                            while(look.tid != SENTINEL) 
                            look = getNextToken();

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
                //code is consumed along with the stack

                topStack = top(stack);
                pop(stack);
                printf("Top of stack %d\n",topStack);
                curr->lineNo = look.lineNo;
                look = getNextToken();
                while(look.tid == TK_COMMENT) look = getNextToken();
                printf("Look.tid is ");
                printToken(look.tid);
                if(topStack == SENTINEL && look.tid==SENTINEL){
                    printf("Input consumed and stack clear\n");
                    fclose(fp);
                    return ptree;
                }
                //printToken(look.tid);
                if(curr->nextSibling) {
                    curr = curr->nextSibling;
                }
                else if(getNearestUncle(curr)) {
                    curr = getNearestUncle(curr);
                }
                else continue;

                
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
        printf("Look.tid is %d\n", look.tid);
        curr = addChildren(rule,curr);
        printf("check2");
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


    char* testFile = "t3.txt";
    initGrammar(G);
	populateFirstFollow("First.txt",true);
    populateFirstFollow("Follow.txt",false);
    // printFirst();
    // printFollow();
    segFaultsSuck();
    printRule(parseTable[otherStmts][TK_RETURN-eps]);

    parseTree ptree = parseInputSourceCode(testFile);
    //printf("\n\n\nCheck");
    // for(int NT = 0; NT < NON_TERMINALS; NT++)
    //     printParseTableRow(NT);
    int numNodes = 0;
    // // inorderNary(ptree, &numNodes);
    // printf("\n\nNumber of nodes in the tree is: %d\n\n", numNodes);
}