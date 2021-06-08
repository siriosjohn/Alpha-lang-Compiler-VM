#ifndef SYMBOLTABLE_VARIABLE_H
#define SYMBOLTABLE_VARIABLE_H

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "symboltype.h"

typedef enum scopespace_t {
    programvar,
    functionlocal,
    formalarg
} scopespace_t;

static inline char *spacetoString(enum scopespace_t s){
    static char* string[] = {"programvar", "functionlocal", "formalarg"};

    return string[s];
}

typedef struct Variable {
    const char *name;
    unsigned int scope;
    unsigned int line;

    scopespace_t space;
    unsigned offset;

} Variable;

Variable* create_variable(const char* name, unsigned int scope, unsigned int line);

unsigned programVarOffset;
unsigned functionLocalOffset;
unsigned formalArgOffset;
unsigned scopeSpaceCounter;

void resetformalargsOffset();
void resetfunctionlocalsOffset();

scopespace_t currScopeSpace(void);
unsigned currscopeoffset(void);
void inccurrscopeoffset(void);
void enterscopespace(void);
void exitscopespace(void);

#endif