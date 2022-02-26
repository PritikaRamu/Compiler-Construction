#ifndef _STACKDEF_
#define _STACKDEF

#include <stdlib.h>
#include "lexer.h"

typedef struct stack {
    gterm data;
    struct stack* next;
} Stack;

Stack* initStack();
void push(Stack* head, gterm data);
void pop(Stack* head);
gterm top(Stack* head);

#endif
