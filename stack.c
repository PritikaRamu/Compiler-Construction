#include "stack.h"
#include <stdio.h>

gterm top(Stack* head) {
    if(!head->next) {
        printf("Stack is empty\n");
        return -1;
    }
    else {
        gterm symbol = head->next->data->symbol;
        return symbol;
    }
}

Stack* initStack() {
    Stack* head = (Stack*)malloc(sizeof(Stack*));
    head->next = NULL;
    return head;
}

void pop(Stack* head) {
    if(!head->next) {
        printf("Empty stack, can't pop from stack\n");
    }
    else {
        Stack* temp = head->next;
        head->next = head->next->next;
        free(temp);
    }
}

void push(Stack* head, gterm symbol) {
    Stack* newNode = (Stack*)malloc(sizeof(Stack));
    g_Node* stackData = (g_Node*)malloc(sizeof(g_Node));
    newNode->data = stackData;
    newNode->data->symbol = symbol;
    newNode->data->next = NULL;
    newNode->data->isTerminal = isTerm(symbol);
    newNode->next = head->next;
    head->next = newNode;
}

void printStack(Stack* head) {
    Stack* temp = (Stack*)malloc(sizeof(Stack));
    temp->next = head->next;
    while(temp->next) {
        printf("%d\n", temp->next->data->symbol);
        temp->next = temp->next->next;
    }
    printf("End of stack\n");
}

int main() {
    gterm trial[] = {TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_TYPE, TK_MAIN,
	TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT,
	TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
	TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_AS,
	TK_EQ, TK_GT, TK_GE, TK_NE, SENTINEL, LEX_ERROR, TK_RUID, TK_UNION, TK_ENDUNION, TK_DEFINETYPE};

    int size = sizeof(trial)/sizeof(gterm);

    Stack *head = initStack();

    for(int i=0; i<size; i++) {
        push(head, trial[i]);
    }

    //printf("%d\n", head->next->data);
    printStack(head);
    //printf("%d\n", head->next->data);

    gterm t1 = top(head);
    pop(head);
    printf("%d\n", t1);

    gterm t2 = top(head);
    pop(head);
    printf("%d\n", t2);

    printf("While started\n");

    while(head->next) {
        gterm temp = top(head);
        pop(head);
        printf("%d\n", temp);
    }

    return 0;
}