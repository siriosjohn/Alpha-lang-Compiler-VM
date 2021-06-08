#ifndef SYMBOLTABLE_SYMBOL_TYPE__H
#define SYMBOLTABLE_SYMBOL_TYPE__H

enum SymbolType {
    TEMP_VAR,
    GENERAL,
    GLOBAL,
    LOCAL_t,
    FORMAL,
    USERFUNC,
    LIBFUNC
};

static inline char *toString(enum SymbolType t){
    static char* string[] = {"local varialbe","local variable", "global variable", "local variable", "formal argument", "user function", "library function"};

    return string[t];
}

#endif