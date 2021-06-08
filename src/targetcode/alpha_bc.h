#ifndef TARGETCODE_ALPHA_BC_H
#define TARGETCODE_ALPHA_BC_H

#include "../icode/quads.h"
#include "../symboltable/symtable.h"

#define INS_EXPAND_SIZE 1024
#define INS_CURR_SIZE (ins_total * sizeof(instruction))
#define INS_NEW_SIZE (INS_EXPAND_SIZE * sizeof(instruction) + INS_CURR_SIZE)

typedef enum vmopcode {
    assign_v,
    add_v, sub_v, mul_v, div_v, mod_v, uminus_v,
    and_v, or_v, not_v, 
    jeq_v, jne_v, 
    jle_v, jge_v, 
    jlt_v, jgt_v,
    jump_v,
    call_v, pusharg_v,
    return_v, getretval_v, funcenter_v, funcexit_v,
    newtable_v, tablegetelem_v, tablesetelem_v,
    nop_v
} vmopcode;

static inline char* vmopcodeToString(vmopcode k) {
    static char* string[] = {
        "assign_v",
        "add_v", "sub_v", "mul_v", "div_v", "mod_v", "uminus_v",
        "and_v", "or_v", "not_v", 
        "jeq_v", "jne_v", 
        "jle_v", "jge_v", 
        "jlt_v", "jgt_v",
        "jump_v",
        "call_v", "pusharg_v", 
        "return_v", "getretval_v", "funcenter_v", "funcexit_v",
        "newtable_v", "tablegetelem_v", "tablesetelem_v",
        "nop_v"
    };

    return string[k];
}

typedef enum vmarg_t {
    label_a = 0,
    global_a = 1,
    formal_a = 2,
    local_a = 3,
    number_a = 4,
    string_a = 5,
    bool_a = 6,
    nil_a = 7,
    userfunc_a = 8,
    libfunc_a = 9,
    retval_a = 10,
    dummy_a = 11
} vmarg_t;

static inline char* vmarg_tToString(vmopcode k) {
    static char* string[] = {
        "label_a",
        "global_a",
        "formal_a",
        "local_a",
        "number_a",
        "string_a",
        "bool_a",
        "nil_a",
        "userfunc_a",
        "libfunc_a",
        "retval_a",
        ""
    };

    return string[k];
}

typedef struct vmarg {
    vmarg_t type;
    unsigned val;
} vmarg;

typedef struct userfunc {
    unsigned address;
    unsigned localSize;
    char* id;
} userfunc;

unsigned consts_newnumber(double n);
double* numConst;
unsigned numConstSize;
unsigned totalNumConsts;

unsigned const_newstring(char* s);
char** stringConsts;
unsigned stringConstsSize;
unsigned totalStringConsts;

unsigned libfuncs_newused(const char* s);
char** namedLibfuncs;
unsigned namedLibFuncsSize;
unsigned totalnamedLibFuncs;

unsigned userfuncs_newfunc(SymbolTableEntry* sym);
userfunc* userFuncs;
unsigned userFuncsSize;
unsigned totalUserFuncs;

typedef struct instruction {
    vmopcode opcode;
    vmarg result;
    vmarg arg1;
    vmarg arg2;
    unsigned srcLine;
} instruction;

instruction* newInstruction();

unsigned ins_total;
unsigned int currInst;
instruction* instructions;
unsigned int nextinstructionlabel();

void expand_inst_table(void);
instruction* emit_instruction(instruction* t);

void reset_operand(vmarg* arg);
void make_operand(expr* e, vmarg* arg);
void make_numberopearand(vmarg* arg, double val);
void make_booloperand(vmarg* arg, unsigned val);
void make_retvaloperand(vmarg* arg);

typedef struct incomplete_jump {
    unsigned instrNo;
    unsigned iaddress;
    struct incomplete_jump* next;
} incomplete_jump;

incomplete_jump* ij_head;
unsigned ij_total;

void add_incomplete_jump(unsigned instrNo, unsigned iaddress);
void patch_incomplete_jumps(void);

unsigned currprocessedquad;
unsigned get_currprocessedquad();

extern void generate_ASSIGN(quad*);

void generate(iopcode oc, quad* q);
extern void generate_ADD(quad* q);
extern void generate_SUB(quad* q);
extern void generate_MUL(quad* q);
extern void generate_DIV(quad* q);
extern void generate_MOD(quad* q);
extern void generate_UMINUS(quad* q);
extern void generate_AND(quad* q);
extern void generate_OR(quad* q);
extern void generate_NOT(quad* q);
extern void generate_relational(iopcode oc, quad* q);
extern void generate_IF_EQ(quad* q);
extern void generate_IF_NOTEQ(quad* q);
extern void generate_IF_LESSEQ(quad* q);
extern void generator_IF_GREATEREQ(quad* q);
extern void generate_IF_LESS(quad* q);
extern void generate_IF_GREATER(quad* q);
extern void generate_JUMP(quad* q);
extern void generate_CALL(quad* q);
extern void generate_PARAM(quad* q);
extern void generate_RETURN(quad* q);
extern void generate_GETRETVAL(quad* q);
extern void generate_FUNCSTART(quad* q);
extern void generate_FUNCEND(quad* q);
extern void generate_NEWTABLE(quad* q);
extern void generate_TABLEGETELM(quad* q);
extern void generate_TABLESETELEM(quad* q);
extern void generate_NOP(quad* q);

typedef void (*generator_func_t)(quad*);
generator_func_t generators[27];

void generate_tc(void);
void print_tc(FILE* stream);
void print_bc(FILE* stream);

#endif