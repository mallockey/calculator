#ifndef COMMON_H
#define COMMON_H

typedef enum
{
    OP_NONE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_EQUALS,
    OP_BACKSPACE
} Operation;

char *operation_to_string(Operation op);

typedef struct
{
    Operation op;
    Operation *current_op_ptr;
} OperationData;

#endif
