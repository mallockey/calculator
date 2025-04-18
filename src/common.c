#include "common.h"
#include "stdlib.h"

char *operation_to_string(Operation op)
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
    default:
        return "";
    }
}

OperatorStack create_operator_stack(int size)
{
    return malloc(sizeof(OperatorStackItem) * size);
}
