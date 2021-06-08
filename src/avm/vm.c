#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#define TR 116
#define FA 102

unsigned globmem = AVM_STACKSIZE - 1;

double const_getnumber(unsigned index){
    return numConst[index];
}

char* const_getstring(unsigned index){
    return stringConsts[index];
}

char* userfunc_getused(unsigned index){
    return userFuncs[index].id;
}

char* libfuncs_getused(unsigned index){
    return namedLibfuncs[index];
}

userfunc* avm_getfuncinfo(unsigned address){
    int i = 0;
    while(userFuncs[i].id != NULL){
        if(userFuncs[i].address == address)
            return &userFuncs[i];
        i++;
    }
}

avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg) {
    switch (arg->type) {
        case global_a:
            return &program_stack[AVM_STACKSIZE - 1 - (arg->val)];

        case local_a:
            return &program_stack[topsp - (arg->val)];

        case formal_a:
            return &program_stack[topsp + AVM_STACKENV_SIZE + 1 + (arg->val)];

        case retval_a:
            return &retval;

        case number_a:
            reg->type = number_m;
            reg->data.numVal = const_getnumber(arg->val);
            return reg;

        case string_a:
            reg->type = string_m;
            reg->data.strVal = strdup(const_getstring(arg->val));
            return reg;

        case bool_a:
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;

        case nil_a:
            reg->type = nil_m;
            return reg;

        case userfunc_a:
            reg->type = userfunc_m;
            reg->data.funcVal = arg->val;
            return reg;

        case libfunc_a:
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfuncs_getused(arg->val);
            return reg;

        default:
            assert(0);
    }
}

void avm_tableincrefCounter(avm_table* t) {
    ++t->refCounter;
}

void avm_tabledecrefCounter(avm_table* t) {
    assert(t->refCounter > 0);
    if (!--t->refCounter)
        avm_tabledestroy(t);
}

void avm_tablebucketsinit(avm_table_bucket** p) {
    unsigned i;
    for (i = 0; i < AVM_TABLE_HASHSIZE; ++i)
        p[i] = (avm_table_bucket*)0;
}

void avm_tableadd(avm_table* table, int spot, avm_memcell* index, avm_memcell* content){
    avm_table_bucket* tmp;
    avm_table_bucket* new = (avm_table_bucket*) malloc(sizeof(avm_table_bucket));
    new->key = *index;
    new->value = *content;
    new->next = NULL;

    if(spot >= 211 || spot < 0)
        assert(0);
    
    if(index->type == number_m){
        if(table->numIndexed[spot] == NULL){
            table->numIndexed[spot] = new;
        }else {
            tmp = table->numIndexed[spot];
            while(tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new;
        }
    }else if(index->type == string_m){
        if(table->strIndexed[spot] == NULL){
            table->strIndexed[spot] = new;
        }else {
            tmp = table->strIndexed[spot];
            while(tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new;
        }
    }
    
}

avm_table* avm_tablenew(void) {
    avm_table* t = (avm_table*)malloc(sizeof(avm_table));

    AVM_WIPEOUT(*t);
    t->refCounter = t->total = 0;
    avm_tablebucketsinit(t->numIndexed);
    avm_tablebucketsinit(t->strIndexed);

    return t;
}

void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content){
    int spot = index->type % AVM_TABLE_HASHSIZE;
    avm_tableadd(table, spot, index, content);
    table->total++;
}

avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index){
    int spot = index->type % AVM_TABLE_HASHSIZE;
    avm_table_bucket *tmp;
    if(index->type == number_m){
        tmp = table->numIndexed[spot];
        while(tmp != NULL){
            if(tmp->key.data.numVal == index->data.numVal)
                return &(tmp->value);
            else
               tmp = tmp->next; 
            
        }
    } else if(index->type == string_m) {
        tmp = table->strIndexed[spot];
        while(tmp != NULL){
            if(strcmp(tmp->key.data.strVal, index->data.strVal) == 0)
                return &(tmp->value);
            else
               tmp = tmp->next; 
        }
    }
    return NULL;
}

/* check back after vm construction */
void avm_memcellclear(avm_memcell* m) {
    if (m->type != undef_m)
        m->type = undef_m;
}

void avm_tablebucketsdestroy(avm_table_bucket** p) {
    avm_table_bucket* b;
    unsigned i;
    for (i = 0; i < AVM_TABLE_HASHSIZE; ++i, ++p) {
        for (b = *p; b;) {
            avm_table_bucket* del = b;
            b = b->next;
            avm_memcellclear(&del->key);
            avm_memcellclear(&del->value);
            free(del);
        }
        p[i] = (avm_table_bucket*)0;
    }
}

void avm_tabledestroy(avm_table* t) {
    avm_tablebucketsdestroy(t->strIndexed);
    avm_tablebucketsdestroy(t->numIndexed);
    free(t);
}

execute_func_t executeFuncs[] = {
    execute_assign,
    execute_add,
    execute_sub,
    execute_mul,
    execute_divide,
    execute_mod,
    execute_uminus,
    execute_and,
    execute_or,
    execute_not,
    execute_jeq,
    execute_jne,
    execute_jle,
    execute_jge,
    execute_jlt,
    execute_jgt,
    execute_jmp,
    execute_call,
    execute_pusharg,
    execute_ret,
    execute_getretval,
    execute_funcenter,
    execute_funcexit,
    execute_newtable,
    execute_tablegetelem,
    execute_tablesetelem,
    execute_nop
};

unsigned char executionFinished = 0;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;
instruction* code = (instruction*) 0;

void execute_cycle(void) {
    if (executionFinished)
        return;
    else if (pc == AVM_ENDING_PC) {
        executionFinished = 1;
        return;
    } else {
        assert(pc < AVM_ENDING_PC);
        instruction* instr = code + pc;
        assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
        if (instr->srcLine)
            currLine = instr->srcLine;
        unsigned oldPC = pc;
        (*executeFuncs[instr->opcode])(instr);
        if (pc == oldPC)
            ++pc;
    }
}

extern void memclear_string(avm_memcell* m) {
    assert(m->data.strVal);
    free(m->data.strVal);
}

extern void memclear_table(avm_memcell* m) {
    assert(m->data.tableVal);
    avm_tabledecrefCounter(m->data.tableVal);
}

memclear_func_t memclearFuncs[] = {
    0, /**/
    memclear_string,
    0, /**/
    memclear_table,
    0, /**/
    0, /**/
    0, /**/
    0  /**/
};

void amv_memcellclear(avm_memcell* m) {
    if (m->type != undef_m) {
        memclear_func_t f = memclearFuncs[m->type];
        if (f)
            (*f)(m);
        m->type = undef_m;
    }
}

void avm_warning(char* format, ...){
    va_list args;
    va_start(args, format);
    printf("Warning, ");
    vfprintf(stdout, format, args);
    va_end(args);
    printf("\n");
}

void avm_error(char* format, ...){
    va_list args;
    va_start(args, format);
    printf("Error, ");
    vfprintf(stdout, format, args);
    va_end(args);
    printf("\n");
}

void avm_assign(avm_memcell* lv, avm_memcell* rv) {
    if (lv == rv)
        return;

    if (lv->type == table_m &&
        rv->type == table_m &&
        lv->data.tableVal == rv->data.tableVal)
        return;

    if (rv->type == undef_m)
        avm_warning("assigning from 'undef' content!");

    amv_memcellclear(lv);

    memcpy(lv, rv, sizeof(avm_memcell));

    if (lv->type == string_m)
        lv->data.strVal = strdup(rv->data.strVal);
    else if (lv->type == table_m)
        avm_tableincrefCounter(lv->data.tableVal);    
}

unsigned totalActuals = 0;

void avm_dec_top(void){
	if(!top) {
		avm_error("stack overflow");
		executionFinished = 1;
    }
	else
		--top;
}

void avm_push_envvalue(unsigned val){
	program_stack[top].type = number_m;
	program_stack[top].data.numVal = val;
	avm_dec_top();
}

void avm_callsaveenviroment (void) {
    
	avm_push_envvalue(totalActuals);
	avm_push_envvalue(pc+1);
	avm_push_envvalue(top + totalActuals + 2);
	avm_push_envvalue(topsp);
}

unsigned avm_get_envvalue(unsigned i) {
	assert(program_stack[i].type == number_m);
	unsigned val = (unsigned) program_stack[i].data.numVal;
	assert (program_stack[i].data.numVal == ((double) val));
	return val;
}

library_func_t avm_getlibraryfunc (char* id){
    if(strcmp(id, "print") == 0)
        return libfunc_print;
    if(strcmp(id, "input") == 0)
        return libfunc_input;
    if(strcmp(id, "objectmemberkeys") == 0)
        return NULL;
    if(strcmp(id, "objecttotalmembers") == 0)
        return libfunc_objecttotalmembers;
    if(strcmp(id, "objectcopy") == 0)
        return NULL;
    if(strcmp(id, "totalarguments") == 0)
        return libfunc_totalarguments;
    if(strcmp(id, "argument") == 0)
        return libfunc_argument;
    if(strcmp(id, "typeof") == 0)
        return libfunc_typeof;
    if(strcmp(id, "strtonum") == 0)
        return libfunc_strtonum;
    if(strcmp(id, "sqrt") == 0)
        return libfunc_sqrt;
    if(strcmp(id, "cos") == 0)
        return libfunc_cos;
    if(strcmp(id, "sin") == 0)
        return libfunc_sin;
}

void avm_calllibfunc(char* id) {
	library_func_t f = avm_getlibraryfunc(id);
	if(!f) {
		avm_error("library function '%s' is not supported in this version of 'alpha'", id);
		executionFinished = 1;
    }
	else {
        topsp = top;
        totalActuals = 0;
        (*f)();
        if (!executionFinished)
            execute_funcexit((instruction*) 0);
    }
}

unsigned avm_totalactuals (void) {
	return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual (unsigned i) {
	assert(i < avm_totalactuals());
	return &program_stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

void avm_registerlibfunc(char* id, library_func_t addr){

}

char* number_tostring (avm_memcell* m){
    char* str = (char*)malloc(20*sizeof(char));
    sprintf(str, "%lf", m->data.numVal);
    return str;
}

char* string_tostring (avm_memcell* m){
    return m->data.strVal;
}

char* bool_tostring (avm_memcell* m){
    switch (m->data.boolVal)
    {
    case 116:
        return strdup("true");
    case 102:
        return strdup("false");  
    default:
        assert(0);
    }
    return NULL;
}

char* table_tostring (avm_memcell* m){
    avm_table_bucket* tmp_bucket;
    char* ret = (char*)malloc(AVM_TABLE_HASHSIZE*420*sizeof(char));
    strcat(ret, "[");
    int i;
    
    for(i = 0; i < AVM_TABLE_HASHSIZE; i++){
        tmp_bucket = m->data.tableVal->strIndexed[i];
        if(tmp_bucket == NULL)
            continue;

        while(tmp_bucket != NULL){
            strcat(ret, "{");
            strcat(ret, avm_tostring(&(tmp_bucket->key)));
            strcat(ret, " : ");
            if(&(tmp_bucket->value) == m)
                strcat(ret, "...");
            else
                strcat(ret, avm_tostring(&(tmp_bucket->value)));
            strcat(ret, "}");
            tmp_bucket = tmp_bucket->next;
        }
    }

    for(i = 0; i < AVM_TABLE_HASHSIZE; i++){
        tmp_bucket = m->data.tableVal->numIndexed[i];
        if(tmp_bucket == NULL)
            continue;

        while(tmp_bucket != NULL){
            strcat(ret, "{");
            strcat(ret, avm_tostring(&(tmp_bucket->key)));
            strcat(ret, " : ");
            if(&(tmp_bucket->value) == m)
                strcat(ret, "...");
            else
                strcat(ret, avm_tostring(&(tmp_bucket->value)));
            strcat(ret, "}");
            tmp_bucket = tmp_bucket->next;
        }
    }
    strcat(ret, "]");
    return ret;
}

char* userfunc_tostring (avm_memcell* m){return strdup(userfunc_getused(m->data.numVal));}
char* libfunc_tostring (avm_memcell* m){return strdup(libfuncs_getused(m->data.numVal));}
char* nil_tostring (avm_memcell* m){return strdup("nil");}
char* undef_tostring (avm_memcell* m){return strdup("nil");}

tostring_func_t tostringFuncs[] = {
	number_tostring,
	string_tostring,
	bool_tostring,
	table_tostring,
	userfunc_tostring,
	libfunc_tostring,
	nil_tostring,
	undef_tostring
};

char* avm_tostring (avm_memcell* m) {
	assert(m->type >= 0 && m->type <= undef_m);
	return (*tostringFuncs[m->type])(m);
}

unsigned char number_tobool (avm_memcell* m){   
    if(m->data.numVal != 0)
        return TR;
    else
        return FA;
}
unsigned char string_tobool (avm_memcell* m){
    if(m->data.strVal[0] != 0)
        return TR;
    else
        return FA;
}
unsigned char bool_tobool (avm_memcell* m){return m->data.boolVal;}
unsigned char table_tobool (avm_memcell* m){return TR;}
unsigned char userfunc_tobool (avm_memcell* m){return TR;}
unsigned char libfunc_tobool (avm_memcell* m){return TR;}
unsigned char nil_tobool (avm_memcell* m){return FA;}
unsigned char undef_tobool (avm_memcell* m){assert(0); return FA;}

tobool_func_t toboolFuncs[]={
	number_tobool,
	string_tobool,
	bool_tobool,
	table_tobool,
	userfunc_tobool,
	libfunc_tobool,
	nil_tobool,
	undef_tobool
};

unsigned char avm_tobool (avm_memcell* m) {
	assert(m->type >= 0 && m->type < undef_m);
	return (*toboolFuncs[m->type])(m);
}

char* typeStrings[]={
 "number",
 "string",
 "bool",
 "table",
 "userfunc",
 "libfunc",
 "nil",
 "undef"
};

static void avm_initstack(void) {
    int i;
    for (i = 0; i < AVM_STACKSIZE; ++i) {
        AVM_WIPEOUT(program_stack[i]);
        program_stack[i].type = undef_m;
    }
}

void avm_initialize(void) {

	avm_initstack();
	
	avm_registerlibfunc("print", libfunc_print);
    avm_registerlibfunc("input", libfunc_input);
    avm_registerlibfunc("objectmemberkeys", libfunc_objectmemberkeys);
    avm_registerlibfunc("objecttotalmembers", libfunc_objecttotalmembers);
    avm_registerlibfunc("objectcopy", libfunc_objectcopy);
    avm_registerlibfunc("total arguments", libfunc_totalarguments);
    avm_registerlibfunc("argument", libfunc_argument);
    avm_registerlibfunc("typeof", libfunc_typeof);
    avm_registerlibfunc("strtonum", libfunc_strtonum);
    avm_registerlibfunc("sqrt", libfunc_sqrt);
    avm_registerlibfunc("cos", libfunc_cos);
    avm_registerlibfunc("sin", libfunc_sin);

    top = globmem - 1;
    topsp = globmem - 1;
    globmem = AVM_STACKSIZE - 1;
    codeSize = ins_total;
    code = instructions;
}

void main(int argc, char** argv) {
    if(argc == 1){
        printf("Error, Please enter a filepath as an argument\n");
        exit(1);
    }
    read_binary(argv[1]);
    globmem = AVM_STACKSIZE - 1 - count_globals();

    avm_initialize();

    while(executionFinished != 1)
        execute_cycle();
}
