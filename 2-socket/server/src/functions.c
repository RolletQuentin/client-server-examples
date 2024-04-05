#include "functions.h"

char* extract_operation(char* request)
{
    // regex
    char* regex = "([-+]?[0-9]+(.[0-9]+)?)\\s*[-+*/%]\\s*([0-9]+(.[0-9]+)?)";

    // compile regex
    regex_t regex_compiled;
    if (regcomp(&regex_compiled, regex, REG_EXTENDED))
    {
        perror("regcomp failed");
        exit(1);
    }

    // match regex
    regmatch_t match[3];
    if (regexec(&regex_compiled, request, 3, match, 0))
    {
        // perror("regexec failed");
        return NULL;
    }

    // extract operation
    int start = match[0].rm_so;
    int end = match[0].rm_eo;
    int operation_length = end - start;
    char* operation = (char*)malloc(operation_length + 1);
    strncpy(operation, request + start, operation_length);

    return operation;
}

double calculate(char* operation) {

    // parse operation
    double a, b;
    char operator;

    sscanf(operation, "%lf %c %lf", &a, &operator, &b);

    // calculate
    double result;
    switch (operator)
    {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        result = a / b;
        break; 
    case '%':
        result = (int)a % (int)b;
        break;
    default:
        result = 0;
        break;
    }

    return result;
}