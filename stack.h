#ifndef _STACKDEF_
#define _STACKDEF

#include <stdlib.h>
#include <stdbool.h>
#include "newRules.h"

typedef struct stack {
    g_RHS* data;
    struct stack* next;
} Stack;

bool isTerm(g_Term symbol);
Stack* initStack();
void push(Stack* head, g_Term data);
void pop(Stack* head);
g_Term top(Stack* head);

#endif
