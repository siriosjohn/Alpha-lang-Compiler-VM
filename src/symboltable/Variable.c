#include "Variable.h"

unsigned programVarOffset = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset = 0;
unsigned scopeSpaceCounter = 1;

void resetformalargsOffset(){
    formalArgOffset = 0;
}

void resetfunctionlocalsOffset(){
    functionLocalOffset = 0;
}

scopespace_t currScopeSpace(void) {
    if (scopeSpaceCounter == 1)
        return programvar;
    else if (scopeSpaceCounter % 2 == 0)
        return formalarg;
    else
        return functionlocal;
}

unsigned currscopeoffset(void) {
    switch (currScopeSpace()) {
        case programvar:
            return programVarOffset;
        case formalarg:
            return formalArgOffset;
        case functionlocal:
            return functionLocalOffset;
        default:
            assert(0);
    }
}

void inccurrscopeoffset(void) {
    switch (currScopeSpace()) {
        case programvar:
            ++programVarOffset;
            break;
        case formalarg:
            ++formalArgOffset;
            break;
        case functionlocal:
            ++functionLocalOffset;
            break;
        default:
            assert(0);
    }
}
void enterscopespace(void) {
    ++scopeSpaceCounter;
}

void exitscopespace(void) {
    assert(scopeSpaceCounter >= 1);
    --scopeSpaceCounter;
}


Variable* create_variable(const char* name, unsigned int scope, unsigned int line){
    Variable *new = (Variable *)malloc(sizeof(Variable));

    if(!new)
        return NULL;

    new->space = currScopeSpace();
    new->offset = currscopeoffset();
    new->name = strdup(name);
    new->scope = scope;
    new->line = line;

    return new;
}