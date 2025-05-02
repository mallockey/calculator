#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <gtk/gtktypes.h>

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

typedef char *StackItem;

typedef StackItem *Stack;

typedef struct
{
    Stack stack;
    size_t top;
    size_t capacity;
} StackData;

typedef struct
{
    CalcInput op;
    CalcInput *current_op_ptr;
    GtkWidget *running_formula;
    StackData *output_data;
    StackData *operation_data;
} OperationData;

char *operation_to_string(CalcInput op);

StackData create_operation_data(int size);

StackData push(StackData operation_data, StackItem operator_stack_item);

StackItem pop(StackData operation_data);

#endif
