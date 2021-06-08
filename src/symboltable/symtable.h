#ifndef SYMBOLTABLE_SYMTAB_H
#define SYMBOLTABLE_SYMTAB_H

#define debug(s) printf("%s\n", #s);

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Function.h"
#include "Variable.h"
#include "symboltype.h"

int exit_flag;

typedef struct SymbolTableEntry {
    bool isVariable;
    bool isActive;
    union {
        Variable *varVal;
        Function *funcVal;
    } value;
    enum SymbolType type;
    
    struct SymbolTableEntry *table_next;

    struct SymbolTableEntry *scope_next;
    struct SymbolTableEntry *scope_prev;
} SymbolTableEntry;

typedef struct SymTable_S {
    SymbolTableEntry **HashTable;
    unsigned int buckets;
    unsigned int curr_size;
} SymTable;

typedef struct scoped_entries_list_s {
    SymbolTableEntry *scope_head;
    SymbolTableEntry *scope_tail;

    unsigned int scope;
    struct scoped_entries_list_s *next;

} scoped_entries_list;

/*Global pointer to scope list*/
scoped_entries_list *scope_list_head;

SymTable *create_table();

unsigned int variable_hash(Variable *entry, int buckets);
unsigned int function_hash(Function *entry, int buckets);

SymbolTableEntry* variable_insert(SymTable *table, Variable *var, enum SymbolType type);
SymbolTableEntry* function_insert(SymTable *table, Function *func, enum SymbolType type);

enum SymbolType distinguish_lookup(SymTable *table, const char *name, unsigned int curr_scope);
SymbolTableEntry *variable_lookup(SymTable *table, Variable *var, enum SymbolType type, unsigned int curr_scope);
SymbolTableEntry *function_lookup(SymTable *table, Function *func, enum SymbolType type, unsigned int curr_scope, bool isCall);
SymbolTableEntry *formal_lookup(SymTable *table, Variable *var, enum SymbolType type, unsigned int curr_scope);
SymbolTableEntry *libfunc_lookup(SymTable *table, const char* name);

int active_function_between_scopes(unsigned int scope_low, unsigned int scope_high);

bool nameAllowed(const char *name);

int hide(SymTable *table, unsigned int scope);
int hide_tmp_var(SymTable *table, unsigned int scope);
int greatest_tmp_var(int scope);

void print_table_scopes(SymTable *table, FILE *stream);
void print_table_buckets(SymTable *table, FILE *stream);

#endif