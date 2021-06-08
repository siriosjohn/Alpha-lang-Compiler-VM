#ifndef AVM_ALPHA_EXEC_ENV_H
#define AVM_ALPHA_EXEC_ENV_H

#include "execution_enviroment.h"
#include "../libfuncs/libfuncs.h"
#include "../instructions/execute.h"

#include <string.h>
#include <stdarg.h>

#define AVM_STACKSIZE 4096
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))
#define AVM_STACKENV_SIZE 4

typedef enum avm_memcell_t {
    number_m = 0,
    string_m = 1,
    bool_m = 2,
    table_m = 3,
    userfunc_m = 4,
    libfunc_m = 5,
    nil_m = 6,
    undef_m = 7
} avm_memcell_t;

struct avm_table;

typedef struct avm_memcell {
    avm_memcell_t type;
    union {
        double numVal;
        char* strVal;
        unsigned char boolVal;
        struct avm_table* tableVal;
        unsigned funcVal;
        char* libfuncVal;
    } data;
} avm_memcell;

avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned top, topsp;
unsigned globmem;

double const_getnumber(unsigned index);
char* const_getstring(unsigned index);
char* userfunc_getused(unsigned index);
char* libfuncs_getused(unsigned index);
userfunc* avm_getfuncinfo(unsigned address);

avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg);

avm_memcell program_stack[AVM_STACKSIZE];

static void avm_initstack(void);

#define AVM_TABLE_HASHSIZE 211

typedef struct avm_table_bucket {
    avm_memcell key;
    avm_memcell value;
    struct avm_table_bucket* next;
} avm_table_bucket;

typedef struct avm_table {
    unsigned refCounter;
    avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
    unsigned total;
} avm_table;

void avm_tableincrefCounter(avm_table* t);
void avm_tabledecrefCounter(avm_table* t);
void avm_tablebucketsinit(avm_table_bucket** p);

avm_table* avm_tablenew(void);
avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index);
void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content);

void avm_tableadd(avm_table* table, int spot, avm_memcell* index, avm_memcell* content);

void avm_memcellclear(avm_memcell* m);
void avm_tablebucketsdestroy(avm_table_bucket** p);
void avm_tabledestroy(avm_table* t);

typedef void(*execute_func_t)(instruction*);

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v

execute_func_t executeFuncs[27];

unsigned char executionFinished;
unsigned pc;
unsigned currLine;
unsigned codeSize;
instruction* code;

#define AVM_ENDING_PC codeSize

void execute_cycle(void);

typedef void (*memclear_func_t)(avm_memcell*);

extern void memclear_string(avm_memcell* m);
extern void memclear_table(avm_memcell* m);

memclear_func_t memclearFuncs[8];

void amv_memcellclear (avm_memcell* m);

extern void avm_warning(char* format, ...);
extern void avm_assign (avm_memcell* lv, avm_memcell* rv);

extern void avm_error(char* format, ...);
extern char* avm_tostring (avm_memcell*);
extern void avm_calllibfunc (char* funcName);
extern void avm_callsaveenviroment(void);

unsigned totalActuals;

void avm_dec_top(void);
void avm_push_envvalue(unsigned val);
void avm_callsaveenviroment(void);

unsigned avm_get_envvalue(unsigned i);

#define AVM_NUMACTUALS_OFFSET +4
#define AVM_SAVEDPC_OFFSET +3
#define AVM_SAVEDTOP_OFFSET +2
#define AVM_SAVEDTOPSP_OFFSET +1

typedef void (*library_func_t)(void);
library_func_t avm_getlibraryfunc (char* id);

void avm_calllibfunc(char* id);
unsigned avm_totalactuals (void);
avm_memcell* avm_getactual (unsigned i);
void avm_registerlibfunc (char* id, library_func_t addr);   

typedef char* (*tostring_func_t)(avm_memcell*);

extern char* number_tostring (avm_memcell*);
extern char* string_tostring (avm_memcell*);
extern char* bool_tostring (avm_memcell*);
extern char* table_tostring (avm_memcell*);
extern char* userfunc_tostring (avm_memcell*);
extern char* libfunc_tostring (avm_memcell*);
extern char* nil_tostring (avm_memcell*);
extern char* undef_tostring (avm_memcell*); 

tostring_func_t tostringFuncs[8];

char* avm_tostring (avm_memcell* m);

typedef unsigned char (*tobool_func_t) (avm_memcell*);

unsigned char number_tobool (avm_memcell* m);
unsigned char string_tobool (avm_memcell* m);
unsigned char bool_tobool (avm_memcell* m);
unsigned char table_tobool (avm_memcell* m);
unsigned char userfunc_tobool (avm_memcell* m);
unsigned char libfunc_tobool (avm_memcell* m);
unsigned char nil_tobool (avm_memcell* m);
unsigned char undef_tobool (avm_memcell* m);

tobool_func_t toboolFuncs[8];

unsigned char avm_tobool (avm_memcell* m);

char* typeStrings[8];

void avm_initialize (void);

#endif