#ifndef COMMON_H
#define COMMON_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stddef.h>

#include <stack.h>

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

PrecedenceIndicator get_precedence(CalcInput top, CalcInput incoming);

int precedence(CalcInput op);

bool is_number(const char *str);

#endif
