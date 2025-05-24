#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef char StackItem;
typedef StackItem *Stack;

typedef char *OutputStackItem;
typedef OutputStackItem *OutputStack;

typedef int EvalStackItem;
typedef EvalStackItem *EvalStack;

typedef struct
{
    Stack stack;
    size_t top;
    size_t capacity;
} StackData;

typedef struct
{
    OutputStack stack;
    size_t top;
    size_t capacity;
} OutputStackData;

typedef struct
{
    EvalStack stack;
    size_t top;
    size_t capacity;
} EvalStackData;

StackData *create_stack(int size);

OutputStackData *create_output_stack(int size);

EvalStackData *create_eval_stack(int size);

void push(StackData *stack_data, StackItem operator_stack_item);

StackItem pop(StackData *stack_data);

void push_output_stack(OutputStackData *stack_data, const char *token);

void push_eval_stack(EvalStackData *stack_data, int token);

EvalStackItem pop_eval_stack(EvalStackData *stack_data);

OutputStackItem pop_output_stack(OutputStackData *stack_data);

#endif