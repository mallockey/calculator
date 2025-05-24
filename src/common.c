#include <common.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

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

int precedence(CalcInput op)
{

    if (op == OP_MULTIPLY || op == OP_DIVIDE)
    {
        return 2;
    }
    if (op == OP_ADD || op == OP_SUBTRACT)
    {
        return 1;
    }

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

bool is_number(const char *str)
{
    if (str == NULL || *str == '\0')
    {
        return false;
    }

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}