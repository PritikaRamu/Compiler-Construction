#include <stdio.h>
#include "parser.h"


tokenInfo arr[5];

parseTree parseInputSourceCode(char* testcaseFile, table T) {


    fp = startLexer(testcaseFile);

    //init stack, push $, push start symbol
    Stack* stack = initStack();
    push(stack, $);
    push(stack,program);
    
    parseTree ptree = initTree(program);

    parseTree curr = ptree;

    tokenInfo look = getNextToken();

    while(1){

    }




    
    return NULL;
}