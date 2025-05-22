#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <gtk/gtk.h>

typedef enum
{
    OP_NONE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_EQUALS,
    OP_BACKSPACE,
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9
} CalcInput;

typedef enum
{
    HIGHER,
    LOWER
} PrecedenceIndicator;

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

typedef struct
{
    GtkWidget *running_formula;
    GtkWidget *sum;
    OutputStackData *output_stack;
    StackData *operation_stack;
    OutputStackData *token_stack;
    StackData *temp_num_stack;
    EvalStackData *eval_stack;
} OperationData;

typedef struct
{
    GtkWidget *running_formula;
    GtkWidget *sum;
    OutputStackData *output_stack;
    StackData *operation_stack;
    OutputStackData *token_stack;
    StackData *temp_num_stack;
    EvalStackData *eval_stack;
    char op;
} ButtonClickHandlerParams;

char *operation_to_string(CalcInput op);

StackData *create_stack(int size);

OutputStackData *create_output_stack(int size);

EvalStackData *create_eval_stack(int size);

void push(StackData *stack_data, StackItem operator_stack_item);

StackItem pop(StackData *stack_data);

OutputStackItem pop_output_stack(OutputStackData *stack_data);

void push_output_stack(OutputStackData *stack_data, const char *token);

EvalStackItem pop_eval_stack(EvalStackData *stack_data);

void push_eval_stack(EvalStackData *stack_data, int token);

PrecedenceIndicator get_precedence(CalcInput top, CalcInput incoming);

int precedence(CalcInput op);

#endif
