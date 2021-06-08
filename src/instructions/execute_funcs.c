#include <stdlib.h>
#include <assert.h>

#include "execute.h"

void execute_call(instruction* instr){
    char* s;
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);

    avm_callsaveenviroment();

    switch (func->type) {
        case userfunc_m:
            pc = func->data.funcVal;
            assert(pc < AVM_ENDING_PC);
            assert(code[pc].opcode == funcenter_v);
            break;

        case string_m:
            avm_calllibfunc(func->data.strVal);
            break;
        case libfunc_m:
            avm_calllibfunc(func->data.libfuncVal);
            break;

        default:
            s = avm_tostring(func);
            avm_error("call: cannot bind '%s' to function!", s);
            free(s);
            executionFinished = 1;
    }
}

void execute_pusharg (instruction* instr) {
	avm_memcell* arg = avm_translate_operand(&instr->result, &ax);
	assert(arg);
	
	avm_assign(&program_stack[top], arg);
	++totalActuals;
	avm_dec_top();
}

void execute_funcenter(instruction* instr) {
	avm_memcell* func = avm_translate_operand(&instr->result, &ax);
	assert(func);
	assert(pc == func->data.funcVal);
	
	totalActuals = 0;
	userfunc* funcInfo = avm_getfuncinfo(pc);
	topsp = top;
	top = top - funcInfo->localSize;

}

void execute_funcexit(instruction* unused) {
	unsigned oldTop = top;
	top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
	pc  = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
	topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	while(++oldTop <= top)
		avm_memcellclear(&program_stack[oldTop]);

}

extern void execute_ret(instruction* instr){

}

extern void execute_getretval(instruction* instr){

}