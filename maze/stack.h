#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct stack{
    int maxsize;
    int top;
    int *items;
};

struct stack *newStack(int capacity);

void delStack(struct stack *st);

int size(struct stack *st);

int isEmpty(struct stack *st);

int isFull(struct stack *st);

void push(struct stack *st, int val);

int peek(struct stack *st);

int pop(struct stack *st);


#endif
