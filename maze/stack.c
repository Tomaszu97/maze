#include "stack.h"
#include <stdio.h>

struct stack *newStack(int capacity){
    struct stack *st = (struct stack*)malloc(sizeof(struct stack));
    st->maxsize = capacity;
    st->top = -1;
    st->items = (int*)malloc(sizeof(int) * capacity);
    return st;
}

void delStack(struct stack *st){
    free(st->items);
    free(st);
    st = NULL;
}

int stackSize(struct stack *st){
    return st->top + 1;
}

int isEmpty(struct stack *st){
    return st->top == -1;
}

int isFull(struct stack *st){
    return st->top == st->maxsize - 1;
}

void push(struct stack *st, int val){
    if(isFull(st)){
        printf("error: stack overflow!");
        exit(EXIT_FAILURE);
    }
    st->items[++st->top] = val;
}

int peek(struct stack *st){
    if(isEmpty(st)){ 
        printf("error: empty stack read!");
        exit(EXIT_FAILURE);
    }
    return st->items[st->top];
}

int pop(struct stack *st){
    if(isEmpty(st)){
        printf("error: stack underflow!");
        exit(EXIT_FAILURE);
    }
    return st->items[st->top--];
}


