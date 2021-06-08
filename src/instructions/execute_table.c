#include <assert.h>
#include <stdlib.h>

#include "execute.h"

void execute_newtable(instruction* instr){
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*)0);
	
    assert(lv && (&program_stack[AVM_STACKSIZE] >= lv && lv > &program_stack[top] || lv == &retval));
    avm_memcellclear(lv);
    lv->type = table_m;
    lv->data.tableVal = avm_tablenew();
    avm_tableincrefCounter(lv->data.tableVal);
	
}

void execute_tablesetelem(instruction* instr) {
	avm_memcell* t = avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* i = avm_translate_operand(&instr->arg1, &ax);
	avm_memcell* c = avm_translate_operand(&instr->arg2, &bx);
	
	assert(t && &program_stack[AVM_STACKSIZE] >= t & t > & program_stack[top] );
	assert(i && c);
	
		if(t->type != table_m)
			avm_error("illegal use of type %s as table!", typeStrings[t->type]);
		else
			avm_tablesetelem(t->data.tableVal, i, c);
}

void execute_tablegetelem(instruction* instr) {

	avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
	avm_memcell* t  = avm_translate_operand(&instr->arg1, (avm_memcell*) 0);
	avm_memcell* i  = avm_translate_operand(&instr->arg2,  &ax);
	
	assert(lv && (&program_stack[AVM_STACKSIZE] >= lv && lv > &program_stack[top] || lv == &retval));
	assert(t && &program_stack[AVM_STACKSIZE] >= t && t > &program_stack[top]);
	assert(i);
	
	avm_memcellclear(lv);
	lv->type = nil_m;
	
	if (t->type != table_m) {
		avm_error("illegal use of type %s as table!", typeStrings[t->type]);
	}
	else {
		avm_memcell* content = avm_tablegetelem(t->data.tableVal, i);
		if(content)
			avm_assign(lv, content);
		else {
			char* is = avm_tostring(i);
			avm_warning("table has no field \'%s\' ", is);
			free(is);
		}
	}
}