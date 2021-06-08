#include <assert.h>

#include "execute.h"

void execute_jeq(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("'undef' involved in equality!");
    else if (rv1->type == nil_m || rv2->type == nil_m)
        result = rv1->type == nil_m && rv2->type == nil_m;
    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    else if (rv1->type != rv2->type)
        avm_error("%s == %s is illegal!",
                  typeStrings[rv1->type],
                  typeStrings[rv2->type]);
    else {
        /*GEMISMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA */
        if(rv1->type == number_m)
        result = rv1->data.numVal == rv2->data.numVal;
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jne(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("'undef' involved in equality!");
    else if (rv1->type == nil_m || rv2->type == nil_m)
        result = rv1->type == nil_m && rv2->type == nil_m;
    else if (rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) != avm_tobool(rv2));
    else if (rv1->type != rv2->type)
        avm_error("%s == %s is illegal!",
                  typeStrings[rv1->type],
                  typeStrings[rv2->type]);
    else {
            /*GEMISMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA */
        result = rv1->data.numVal != rv2->data.numVal;
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jle(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char result = 0;

    if (rv1->type != number_m || rv2->type != number_m)
        avm_error("comparison between not numbers is illegal");
    else if (rv1->type == number_m && rv2->type == number_m) {
        if (rv1->data.numVal <= rv2->data.numVal)
            result = 1;
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jge(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char result = 0;

    if (rv1->type != number_m || rv2->type != number_m)
        avm_error("comparison between not numbers is illegal");
    else if (rv1->type == number_m && rv2->type == number_m) {
        if (rv1->data.numVal >= rv2->data.numVal)
            result = 1;
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jlt(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char result = 0;

    if (rv1->type != number_m || rv2->type != number_m)
        avm_error("comparison between not numbers is illegal");
    else if (rv1->type == number_m && rv2->type == number_m) {
        if (rv1->data.numVal < rv2->data.numVal)
            result = 1;
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jgt(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    unsigned char result = 0;

    if (rv1->type != number_m || rv2->type != number_m)
        avm_error("comparison between not numbers is illegal");
    else if (rv1->type == number_m && rv2->type == number_m) {
        if (rv1->data.numVal > rv2->data.numVal)
            result = 1;
    }

    if (!executionFinished && result)
        pc = instr->result.val;
}

void execute_jmp(instruction* instr) {
    if (!executionFinished)
        pc = instr->result.val;
}
