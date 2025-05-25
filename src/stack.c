#include <common.h>

StackData *create_stack(int size)
{
    StackData *stack_data = malloc(sizeof(StackData));

    stack_data->stack = malloc(sizeof(StackItem) * size);
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

void free_stack(StackData *stack_data)
{
    if (stack_data == NULL)
        return;

    if (stack_data->stack != NULL)
    {
        free(stack_data->stack); // Free the array of chars
    }

    free(stack_data); // Free the struct
}

void free_output_stack(OutputStackData *stack_data)
{
    if (stack_data == NULL)
        return;

    if (stack_data->stack != NULL)
    {
        free(stack_data->stack);
    }

    free(stack_data);
}

void free_eval_stack(EvalStackData *stack_data)
{
    if (stack_data == NULL)
        return;

    if (stack_data->stack != NULL)
    {
        free(stack_data->stack);
    }

    free(stack_data);
}
