#include "common.h"
#include "stdlib.h"

char *operation_to_string(CalcInput op)
{
    switch (op)
    {
    case OP_ADD:
        return "+";
    case OP_SUBTRACT:
        return "-";
    case OP_MULTIPLY:
        return "*";
    case OP_DIVIDE:
        return "/";
    case OP_EQUALS:
        return "=";
    case OP_BACKSPACE:
        return "<";
    case NUM_0:
        return "0";
    case NUM_1:
        return "1";
    case NUM_2:
        return "2";
    case NUM_3:
        return "3";
    case NUM_4:
        return "4";
    case NUM_5:
        return "5";
    case NUM_6:
        return "6";
    case NUM_7:
        return "7";
    case NUM_8:
        return "8";
    case NUM_9:
        return "9";
    default:
        return "";
    }
}

StackData *create_stack(int size)
{
    StackData *stack_data = malloc(sizeof(StackData));

    stack_data->stack = malloc(sizeof(StackItem) * size); // properly initialize
    stack_data->top = 0;
    stack_data->capacity = size;

    return stack_data;
}

OutputStackData *create_output_stack(int size)
{
    OutputStackData *stack_data = malloc(sizeof(OutputStackData));

    stack_data->stack = malloc(sizeof(OutputStack) * size);
    stack_data->capacity = size;

    return stack_data;
}

EvalStackData *create_eval_stack(int size)
{
    EvalStackData *stack_data = malloc(sizeof(EvalStackData));

    stack_data->stack = malloc(sizeof(EvalStack) * size);
    stack_data->capacity = size;

    return stack_data;
}

void push(StackData *stack_data, StackItem operator_stack_item)
{
    // Resize if needed
    if (stack_data->top >= stack_data->capacity)
    {
        size_t new_capacity = stack_data->capacity * 2;
        Stack new_stack = realloc(stack_data->stack, sizeof(StackItem) * new_capacity);
        if (new_stack == NULL)
        {
            // Allocation failed; return without modifying
            fprintf(stderr, "Error: realloc failed\n");
            return;
        }
        stack_data->stack = new_stack;
        stack_data->capacity = new_capacity;
    }

    stack_data->stack[stack_data->top++] = operator_stack_item;
}

StackItem pop(StackData *stack_data)
{
    if (stack_data->top == 0)
    {
        exit(1);
    }

    stack_data->top--;
    StackItem item = stack_data->stack[stack_data->top];
    stack_data->stack[stack_data->top] = '\0';

    return item;
}

void push_output_stack(OutputStackData *stack_data, const char *token)
{
    if (stack_data->top >= stack_data->capacity)
    {
        size_t new_capacity = stack_data->capacity * 2;
        OutputStack new_stack = realloc(stack_data->stack, sizeof(OutputStackItem) * new_capacity);
        if (!new_stack)
        {
            fprintf(stderr, "Error: realloc failed\n");
            return;
        }
        stack_data->stack = new_stack;
        stack_data->capacity = new_capacity;
    }

    stack_data->stack[stack_data->top++] = strdup(token); // Make a copy of the token
}

void push_eval_stack(EvalStackData *stack_data, int token)
{
    if (stack_data->top >= stack_data->capacity)
    {
        size_t new_capacity = stack_data->capacity * 2;
        EvalStack new_stack = realloc(stack_data->stack, sizeof(EvalStackItem) * new_capacity);
        if (!new_stack)
        {
            fprintf(stderr, "Error: realloc failed\n");
            return;
        }
        stack_data->stack = new_stack;
        stack_data->capacity = new_capacity;
    }

    stack_data->stack[stack_data->top++] = token;
}

EvalStackItem pop_eval_stack(EvalStackData *stack_data)
{
    if (stack_data->top == 0)
    {
        exit(1);
    }

    stack_data->top--;
    EvalStackItem item = stack_data->stack[stack_data->top];
    stack_data->stack[stack_data->top] = 0;

    return item;
}

OutputStackItem pop_output_stack(OutputStackData *stack_data)
{
    if (stack_data->top == 0)
    {
        exit(1);
    }

    stack_data->top--;
    OutputStackItem item = stack_data->stack[stack_data->top];
    stack_data->stack[stack_data->top] = NULL;

    return item;
}

int precedence(CalcInput op)
{
    if (strncmp((char *)&op, operation_to_string(OP_MULTIPLY), 2) == 0 || strncmp((char *)&op, operation_to_string(OP_DIVIDE), 2) == 0)
        return 2;
    if (strncmp((char *)&op, operation_to_string(OP_ADD), 1) == 0 || strncmp((char *)&op, operation_to_string(OP_SUBTRACT), 2) == 0)
        return 1;

    return 0;
}

PrecedenceIndicator get_precedence(CalcInput top, CalcInput incoming)
{
    if (precedence(top) >= precedence(incoming))
    {
        return HIGHER;
    }
    else
    {
        return LOWER;
    }
}
