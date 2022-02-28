#include <stdio.h>
#include <string.h>
#include "parser.h"

parseTree parseInputSourceCode(char* testcaseFile, table T) {
    printf("Not implemented yet\n");
    return NULL;
}


void ComputeFirstAndFollowSets (Grammar G){

}

// int* First(Grammar G, gterm NT){
    
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

int* set_union(int* A, int*B, int len){

    int size = sizeof(int)*len;
    int* union_array = (int*)malloc(size);

    for(int i=0; i<len;i++){
        if(A[i]==1 || B[i]==1)
            union_array[i] = 1;
        else
            union_array[i] = 0;
    }
    return union_array;
}

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