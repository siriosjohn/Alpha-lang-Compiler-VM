#include "Function.h"

Function* create_function(const char* name, unsigned int scope, unsigned int line){
    Function *new = (Function *)malloc(sizeof(Function));

    if(!new)
        return NULL;


    new->name = strdup(name);
    new->scope = scope;
    new->line = line;
    new->funcAddress = 0;
    new->totalArgs = 0;
    new->totalLocals = 0;

    return new;
}

int insert_argument(Function *func, Variable *arg){
    Argument *tmp = func->args;
    if(!func->args){
        func->args->variable = arg;
    }

    else{
        while(tmp->next != NULL)
            tmp = tmp->next;

        tmp->next->variable = arg;
    }

    return 1;
}