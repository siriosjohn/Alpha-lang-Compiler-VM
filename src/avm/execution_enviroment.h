#ifndef AVM_VM_H
#define AVM_VM_H

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

char** stringConsts;
unsigned totalStringConsts;

double* numConst;
unsigned totalNumConsts;

userfunc* userFuncs;
unsigned totalUserFuncs;

char** namedLibfuncs;
unsigned totalnamedLibFuncs;

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

long long magic_number;

int count_globals();
void read_binary(char* filepath);

#endif