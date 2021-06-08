#ifndef SYMBOLTABLE_FUNCTION_H
#define SYMBOLTABLE_FUNCTION_H

#include <string.h>
#include <stdlib.h>

#include "Variable.h"
#include "symboltype.h"

typedef struct argument_s
{
    Variable *variable;
    struct argument_s *next;
} Argument;

typedef struct Function
{
    const char *name;
    unsigned int scope;
    unsigned int line;

    unsigned int totalArgs;
    unsigned int totalLocals;
    unsigned int funcAddress;
    
    Argument *args;
} Function;

Function *create_function(const char *name, unsigned int scope, unsigned int line);
int insert_argument(Function *func, Variable *arg);

#endif