#include <string.h>

#include "alpha_bc.h"

unsigned int nextinstructionlabel(){
    return currInst;
}

void expand_inst_table(void){
    assert(ins_total == currInst);
    instruction* t = (instruction*)malloc(INS_NEW_SIZE);
    if (instructions) {
        memcpy(t, instructions, INS_CURR_SIZE);
        free(instructions);
    }
    instructions = t;
    ins_total += INS_EXPAND_SIZE;
}

instruction* emit_instruction(instruction* t){
    if (currInst == ins_total) expand_inst_table();

    instruction* new = instructions + currInst;
    new->opcode = t->opcode;
    new->arg1 = t->arg1;
    new->arg2 = t->arg2;
    new->result = t->result;
    new->srcLine = t->srcLine;

    currInst++;

    return new;
}

double* numConst = NULL;
unsigned numConstSize = 50;
unsigned totalNumConsts = 0;
unsigned consts_newnumber(double n){
    unsigned i;
    unsigned ret = totalNumConsts;
    if(numConst == NULL)
        numConst = (double*)malloc(numConstSize*sizeof(double));
    if(totalNumConsts == numConstSize){
        numConstSize *= 2;
        numConst = (double*)realloc(numConst, numConstSize*sizeof(double));
    }

    for(i = 0; i < totalNumConsts; i++){
        if(n == numConst[i])
            return i;
    }
    numConst[totalNumConsts++] = n;
    return ret;
}

char** stringConsts = NULL;
unsigned stringConstsSize = 50;
unsigned totalStringConsts = 0;

unsigned const_newstring(char* s){
    unsigned i;
    unsigned ret = totalStringConsts;
    if(stringConsts == NULL)
        stringConsts = (char **)malloc(stringConstsSize*sizeof(char*));
        
    if(totalStringConsts == stringConstsSize){
        stringConstsSize *=2;
        stringConsts = (char **)realloc(stringConsts, stringConstsSize*sizeof(char*));
    }

    for(i = 0; i < totalStringConsts; i++){
        if(strcmp(s, stringConsts[i]) == 0)
            return i;
    }
    stringConsts[totalStringConsts++] = strdup(s);
    return ret;
}

char** namedLibfuncs = NULL;
unsigned namedLibFuncsSize = 50;
unsigned totalnamedLibFuncs = 0;
unsigned libfuncs_newused(const char* s){
    unsigned i;
    unsigned ret = totalnamedLibFuncs;
    if(namedLibfuncs == NULL)
        namedLibfuncs = (char **)malloc(namedLibFuncsSize*sizeof(char*));

    else if(totalnamedLibFuncs == namedLibFuncsSize){
        totalnamedLibFuncs *=2;
        namedLibfuncs = (char **)realloc(namedLibfuncs, namedLibFuncsSize*sizeof(char*));
    }

    for(i = 0; i < totalnamedLibFuncs; i++){
        if(strcmp(s, namedLibfuncs[i]) == 0)
            return i;
    }
    namedLibfuncs[totalnamedLibFuncs++] = strdup(s);
    return ret;
}

unsigned userFuncsSize = 50;
unsigned totalUserFuncs = 0;
unsigned userfuncs_newfunc(SymbolTableEntry* sym){
    unsigned ret = totalUserFuncs;
    if(userFuncs == NULL)
        userFuncs = (userfunc*)malloc(userFuncsSize*sizeof(userfunc));
    if(totalUserFuncs == userFuncsSize){
        userFuncsSize *= 2;
        userFuncs = (userfunc*)realloc(userFuncs, userFuncsSize*sizeof(userfunc));
    }
    userFuncs[totalUserFuncs].address = sym->value.funcVal->funcAddress;
    userFuncs[totalUserFuncs].localSize = sym->value.funcVal->totalLocals;
    userFuncs[totalUserFuncs++].id = strdup(sym->value.funcVal->name);

    return ret;
}

instruction* newInstruction(){
    instruction* i = (instruction*)malloc(sizeof(instruction));
    i->opcode = 0;
    i->result.type = dummy_a;
    i->result.val = 0;
    i->arg1.type = dummy_a;
    i->arg1.val = 0;
    i->arg2.type = dummy_a;
    i->arg2.val = 0;
    i->srcLine = 0;
}

void reset_operand(vmarg* arg){
    arg->type = nil_a;
    arg->val = 0;
}

void make_operand(expr* e, vmarg* arg) {
        switch (e->type) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e: {
            assert(e->sym);
            arg->val = e->sym->value.varVal->offset;
            switch (e->sym->value.varVal->space) {
                case programvar:
                    arg->type = global_a;
                    break;
                case functionlocal:
                    arg->type = local_a;
                    break;
                case formalarg:
                    arg->type = formal_a;
                    break;
                default:
                    assert(0);
            }
            break;
        }

        case constbool_e: {
            arg->val = e->boolConst;
            arg->type = bool_a;
            break;
        }
        case conststring_e: {
            arg->val = const_newstring(e->strConst);
            arg->type = string_a;
            break;
        }

        case constint_e: {
            arg->val = consts_newnumber(e->numConst);
            arg->type = number_a;
            break;
        }

        case constdouble_e: {
            arg->val = consts_newnumber(e->numConst);
            arg->type = number_a;
            break;
        }

        case nil_e:
            arg->type = nil_a;
            break;

        case programmfunc_e: {
            arg->type = userfunc_a;
            arg->val = e->sym->value.funcVal->funcAddress;
            break;
        }

        case libraryfunc_e: {
            arg->type = libfunc_a;
            arg->val = libfuncs_newused(e->sym->value.funcVal->name);
            break;
        }

        default:
            assert(0);
    }
}

void make_labeloperand(vmarg* arg, int val){
    arg->val = val;
    arg->type = label_a;
}

void make_numberopearand(vmarg* arg, double val) {
    arg->val = consts_newnumber(val);
    arg->type = number_a;
}

void make_booloperand(vmarg* arg, unsigned val) {
    arg->val = val;
    arg->type = bool_a;
}

void make_retvaloperand(vmarg* arg) {
    arg->type = retval_a;
}

incomplete_jump* ij_head = (incomplete_jump*)0;
unsigned ij_total = 0;

void add_incomplete_jump(unsigned instrNo, unsigned iaddress) {
    incomplete_jump* new = (incomplete_jump*)malloc(sizeof(incomplete_jump));
    new->instrNo = instrNo;
    new->iaddress = iaddress;
    new->next = NULL;

    if (ij_head == NULL) {
        ij_head = new;
    } else {
        incomplete_jump* tmp = ij_head;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = new;
    }
}

void patch_incomplete_jumps(void) {
    incomplete_jump* tmp = ij_head;
    while (tmp != NULL) {
        if (tmp->iaddress == currQuad)
            make_labeloperand(&(instructions[tmp->instrNo].result), currInst);
        else
            make_labeloperand(&(instructions[tmp->instrNo].result), quads[tmp->iaddress].taddress);
        tmp = tmp->next;
    }
}

unsigned currprocessedquad = 0;

unsigned get_currprocessedquad(){
    return currprocessedquad;
}

void generate(iopcode oc, quad* q) {
    instruction* i = newInstruction();

    i->opcode = oc;
    if(q->arg1)
        make_operand(q->arg1, &(i->arg1));
    if(q->arg2)
        make_operand(q->arg2, &(i->arg2));
    make_operand(q->result, &(i->result));
    q->taddress = nextinstructionlabel();
    emit_instruction(i);
}

void generate_ASSIGN(quad* q){generate(assign, q);}
void generate_ADD(quad* q) {generate(add, q);}
void generate_SUB(quad* q) {generate(sub, q);}
void generate_MUL(quad* q) {generate(mul, q);}
void generate_DIV(quad* q) {generate(divide, q);}
void generate_MOD(quad* q) {generate(mod, q);}
void generate_UMINUS(quad* q) {
    instruction* i = newInstruction();
    i->opcode = mul_v;
    make_operand(q->result, &(i->result));
    make_operand(q->arg1, &(i->arg1));
    make_numberopearand(&(i->arg2), -1);
    emit_instruction(i);
    return;
}

void generate_relational(iopcode oc, quad* q) {
    instruction* i = newInstruction();
    i->opcode = oc;

    if(q->arg1)
        make_operand(q->arg1, &(i->arg1));
    if(q->arg2)
        make_operand(q->arg2, &(i->arg2));
    i->result.type = label_a;
    if (q->label < get_currprocessedquad()){
        i->result.val = quads[q->label].taddress;
    } else
        add_incomplete_jump(nextinstructionlabel(), q->label);

    q->taddress = nextinstructionlabel();
    emit_instruction(i);
}

void generate_IF_EQ(quad* q) {generate_relational(if_eq, q);}
void generate_IF_NOTEQ(quad* q) {generate_relational(if_noteq, q);}
void generate_IF_LESSEQ(quad* q) {generate_relational(if_lesseq, q);}
void generator_IF_GREATEREQ(quad* q) {generate_relational(if_greatereq, q);}
void generate_IF_LESS(quad* q) {generate_relational(if_less, q);}
void generate_IF_GREATER(quad* q) {generate_relational(if_greater, q);}
void generate_JUMP(quad* q) {generate_relational(jump, q);}

void generate_AND(quad* q){/*empty*/}
void generate_OR(quad* q){/*emppty*/}
void generate_NOT(quad* q){/*empty*/}

void generate_PARAM(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction* i = newInstruction();
    i->opcode = pusharg_v;
    make_operand(q->result, &(i->result));
    emit_instruction(i);
}

void generate_CALL(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction* i = newInstruction();
    i->opcode = call_v;
    make_operand(q->result, &(i->result));
    emit_instruction(i);
}

void generate_GETRETVAL(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction* i = newInstruction();
    i->opcode = assign_v;
    make_operand(q->result, &(i->result));
    make_retvaloperand(&(i->arg1));
    emit_instruction(i);
}

void generate_FUNCSTART(quad* q) {
    SymbolTableEntry* f = q->result->sym;
    f->value.funcVal->funcAddress = nextinstructionlabel();
    q->taddress = nextinstructionlabel();
    
    userfuncs_newfunc(f);

    instruction* i = newInstruction();
    i->opcode = funcenter_v;
    make_operand(q->result, &(i->result));
    emit_instruction(i);
}

void generate_RETURN(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction* i = newInstruction();
    i->opcode = assign_v;
    make_retvaloperand(&(i->result));
    make_operand(q->result, &(i->arg1));
    emit_instruction(i);
}

void generate_FUNCEND(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction* i = newInstruction();
    i->opcode = funcexit_v;
    make_operand(q->result, &(i->result));
    emit_instruction(i);
}

void generate_NEWTABLE(quad* q){generate(tablecreate, q);}
void generate_TABLEGETELM(quad* q){generate(tablegetelem, q);}
void generate_TABLESETELEM(quad* q){generate(tablesetelem, q);}
void generate_NOP(quad* q) {
    instruction* i = newInstruction();
    i->opcode = nop_v;
    emit_instruction(i);
}

generator_func_t generators[] = {
    generate_ASSIGN, 
    generate_ADD, generate_SUB, generate_MUL, generate_DIV, generate_MOD, generate_UMINUS,
    generate_AND, generate_OR, generate_NOT,
    generate_IF_EQ, generate_IF_NOTEQ, 
    generate_IF_LESSEQ, generator_IF_GREATEREQ, 
    generate_IF_LESS, generate_IF_GREATER, 
    generate_JUMP, 
    generate_CALL, generate_PARAM, 
    generate_RETURN, generate_GETRETVAL, generate_FUNCSTART,  generate_FUNCEND,
    generate_NEWTABLE, generate_TABLEGETELM, generate_TABLESETELEM,
    generate_NOP
};

void generate_tc(void) {
    int i;
    for (i = 0; i < nextQuad(); i++){
        currprocessedquad = i;
        (*generators[quads[i].op])(&quads[i]);
    }
    patch_incomplete_jumps();
}

void print_tc(FILE* stream) {
    int i;
    fprintf(stream, "magic number: %li\n", 384838503862);

    fprintf(stream, "arrays:\n");

        fprintf(stream, "\tstrConsts: %d\n", totalStringConsts);
        for(i = 0; i < totalStringConsts; i++) 
           fprintf(stream, "\t\tsize: %ld, value: %s\n", strlen(stringConsts[i]), stringConsts[i]);

        fprintf(stream, "\tnumConsts: %d\n",totalNumConsts);
        for(i = 0; i < totalNumConsts; i++)
           fprintf(stream, "\t\tindex: %d, value: %lf\n", i, numConst[i]);
        
        fprintf(stream, "\tuserFunctions: %u\n", totalUserFuncs);
        for(i = 0; i < totalUserFuncs; i++) 
           fprintf(stream, "\t\taddress: %d, localsize: %d, size: %zu, id: %s\n", userFuncs[i].address, userFuncs[i].localSize, strlen(userFuncs[i].id), userFuncs[i].id);

        fprintf(stream, "\tlibFunctions: %d\n", totalnamedLibFuncs);
        for(i = 0; i < totalnamedLibFuncs; i++) 
           fprintf(stream, "\t\tsize: %zu, id: %s\n", strlen(namedLibfuncs[i]), namedLibfuncs[i]);


    fprintf(stream, "\ntotal instructions: %d\n", currInst);
    fprintf(stream, "\nInd   Opcode          Result                Arg1                  Arg2\n\n");
    for (i = 0; i < currInst; i++) {
        fprintf(stream, "[%03d] ",i);
        fprintf(stream, "%-16s", vmopcodeToString(instructions[i].opcode));
        if(instructions[i].result.type != 11)
            fprintf(stream, "%-10s(%02d) %-7i", vmarg_tToString(instructions[i].result.type), instructions[i].result.type, instructions[i].result.val);
        if(instructions[i].arg1.type != 11)
            fprintf(stream, "%-10s(%02d) %-7i", vmarg_tToString(instructions[i].arg1.type), instructions[i].arg1.type, instructions[i].arg1.val);
        if(instructions[i].arg2.type != 11)
            fprintf(stream, "%-10s(%02d) %-7i", vmarg_tToString(instructions[i].arg2.type), instructions[i].arg2.type, instructions[i].arg2.val);
        fprintf(stream, "\n");
    }
}

void print_bc(FILE* stream){
    int i;
    size_t size;
    unsigned inst_size = currInst;
    /* magic number */
    long long magic_number = 384838503862;
    fwrite(&magic_number, sizeof(long long), 1, stream);

    /* string consts array*/
    fwrite(&totalStringConsts, sizeof(unsigned), 1, stream);
    for(i = 0; i < totalStringConsts; i++){
        size = strlen(stringConsts[i]);
        fwrite(&size, sizeof(size_t), 1, stream);
        fwrite(stringConsts[i], sizeof(char), size, stream);
    }

    /* numConsts array */
    fwrite(&totalNumConsts, sizeof(unsigned), 1, stream);
    for(i = 0; i < totalNumConsts; i++){
        fwrite(&numConst[i], sizeof(double), 1, stream);
    }

    /* userFuncs array */
    fwrite(&totalUserFuncs, sizeof(unsigned), 1, stream);
    for(i = 0; i < totalUserFuncs; i++){
        size = strlen(userFuncs[i].id);
        fwrite(&(userFuncs[i].address), sizeof(unsigned), 1, stream);
        fwrite(&(userFuncs[i].localSize), sizeof(unsigned), 1, stream);
        fwrite(&size, sizeof(size_t), 1, stream);
        fwrite(userFuncs[i].id, sizeof(char), size, stream);
    }
    /* lib funcs */
    fwrite(&totalnamedLibFuncs, sizeof(unsigned), 1, stream);
    for(i = 0; i < totalnamedLibFuncs; i++){
        size = strlen(namedLibfuncs[i]);
        fwrite(&size, sizeof(size_t), 1, stream);
        fwrite(namedLibfuncs[i], sizeof(char), size, stream);
    }    
    
    /* instructions */
    fwrite(&inst_size, sizeof(unsigned), 1, stream);
    for (i = 0; i < currInst; i++) {
        /* opcode */
        fwrite(&(instructions[i].opcode), sizeof(unsigned int), 1, stream);
        /* res */
        fwrite(&(instructions[i].result.type), sizeof(unsigned int), 1, stream);
        fwrite(&(instructions[i].result.val), sizeof(unsigned), 1, stream);
        /* arg1 */
        fwrite(&(instructions[i].arg1.type), sizeof(unsigned int), 1, stream);
        fwrite(&(instructions[i].arg1.val), sizeof(unsigned), 1, stream);
        /* arg2 */
        fwrite(&(instructions[i].arg2.type), sizeof(unsigned int), 1, stream);
        fwrite(&(instructions[i].arg2.val), sizeof(unsigned), 1, stream);

    }


}