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
        return "<-";
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

Stack create_operator_stack(int size)
{
    Stack operator_stack = malloc(sizeof(Stack) * size);

    if (operator_stack == NULL)
    {
    }

    return operator_stack;
}

StackData create_operation_data(int size)
{
    StackData operation_data;

    operation_data.stack = create_operator_stack(size); // properly initialize
    operation_data.top = 0;
    operation_data.capacity = size;

    return operation_data;
}

StackData push(StackData operation_data, StackItem operator_stack_item)
{
    // Resize if needed
    if (operation_data.top >= operation_data.capacity)
    {
        size_t new_capacity = operation_data.capacity * 2;
        Stack new_stack = realloc(operation_data.stack, sizeof(StackItem) * new_capacity);
        if (new_stack == NULL)
        {
            // Handle allocation failure (for now just return unchanged)
            return operation_data;
        }
        operation_data.stack = new_stack;
        operation_data.capacity = new_capacity;
    }

    operation_data.stack[operation_data.top++] = operator_stack_item;
    return operation_data;
}

StackItem pop(StackData operation_data)
{
    if (operation_data.top == 0)
    {
        exit(1);
    }

    operation_data.top--;
    StackItem item = operation_data.stack[operation_data.top];
    operation_data.stack[operation_data.top] = NULL;

    return item;
}