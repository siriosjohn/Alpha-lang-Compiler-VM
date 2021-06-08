#ifndef INSTRUCTIONS_EXEC_EXECUTE_H
#define INSTRUCTIONS_EXEC_EXECUTE_H

#include "../avm/execution_enviroment.h"
#include "../avm/vm.h"

extern void execute_assign(instruction*);

#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_divide execute_arithmetic
#define execute_mod execute_arithmetic
#define execute_uminus execute_arithmetic

typedef double (*arithmetic_func_t) (double x, double y);

double add_impl (double x, double y);
double sub_impl (double x, double y);
double mul_impl (double x, double y);
double div_impl (double x, double y);
double mod_impl (double x, double y);

arithmetic_func_t arithmeticFuncs[5];

void execute_arithmetic(instruction* instr);
extern void execute_add(instruction*);
extern void execute_sub(instruction*);
extern void execute_mul(instruction*);
extern void execute_divide(instruction*);
extern void execute_mod(instruction*);
extern void execute_uminus(instruction*);

extern void execute_and(instruction*);
extern void execute_or(instruction*);
extern void execute_not(instruction*);

extern void execute_jeq(instruction*);
extern void execute_jne(instruction*);
extern void execute_jle(instruction*);
extern void execute_jge(instruction*);
extern void execute_jlt(instruction*);
extern void execute_jgt(instruction*);
extern void execute_jmp(instruction*);

extern void execute_call(instruction*);
extern void execute_pusharg(instruction*);
extern void execute_ret(instruction*);
extern void execute_getretval(instruction*);
extern void execute_funcenter(instruction*);
extern void execute_funcexit(instruction*);

extern void execute_newtable(instruction*);
extern void execute_tablegetelem(instruction*);
extern void execute_tablesetelem(instruction*);

extern void execute_nop(instruction*);

#endif