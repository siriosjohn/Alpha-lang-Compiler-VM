#include "libfuncs.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../avm/execution_enviroment.h"
#include "../avm/vm.h"

int inside_functions(unsigned pc) {
    int funcenter_cnt = 0, funcexit_cnt = 0;
    int i;
    for (i = pc; i >= 0; i--) {
        if (instructions[i].opcode == funcexit_v)
            funcexit_cnt++;
        if (instructions[i].opcode == funcenter_v)
            funcenter_cnt++;
    }
    if (funcenter_cnt == funcexit_cnt)
        return 0;

    return 1;
}

void libfunc_print(void) {
    unsigned i;
    unsigned n = avm_totalactuals();
    for (i = 0; i < n; ++i) {
        char* s = avm_tostring(avm_getactual(i));
        printf("%s", s);
        free(s);
    }
}
void libfunc_input(void) {
    int num_flag = 0;
    char c;
    char* str = (char*)malloc(420 * sizeof(char));
    int i = 0;
    double number;
    unsigned n = avm_totalactuals();
    if (n != 0) {
        avm_error("zero arguments (not %d) expected in \'input\'!", n);
        free(str);
    } else {
        while (c = getchar()) {
            if (c == '\n')
                break;
            if (isdigit(c) || c == '.')
                num_flag = 1;

            str[i++] = c;
        }
        str[i] = '\0';
        avm_memcellclear(&retval);
        if (num_flag == 1) {
            retval.type = number_m;
            retval.data.numVal = strtod(str, NULL);
        } else if (strcmp(str, "true") == 0) {
            retval.type = bool_m;
            retval.data.boolVal = 't';
        } else if (strcmp(str, "false") == 0) {
            retval.type = bool_m;
            retval.data.boolVal = 'f';
        } else if (strcmp(str, "nil") == 0) {
            retval.type = nil_m;
        } else {
            retval.type = string_m;
            retval.data.strVal = str;
        }
    }
}

void libfunc_objecttotalmembers(void) {
    unsigned n = avm_totalactuals();
    avm_memcellclear(&retval);
    if (n != 1)
        avm_error("one argument (not %d) expected in \'objecttotalmembers\'!", n);
    else {
        avm_memcell* arg = avm_getactual(0);
        if(arg->type != table_m){
            avm_error("objecttotalmembers expects a table as an argument");
            retval.type = nil_m;
        } else{
            retval.type = number_m;
            retval.data.numVal = (double)arg->data.tableVal->total;
        }
    }
}

void libfunc_objectmemberkeys(void) {}
void libfunc_objectcopy(void) {}

void libfunc_totalarguments(void) {
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if (inside_functions(pc) == 0) {
        avm_error("'totalarguments' called outside a function");
        retval.type = nil_m;
    } else {
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void libfunc_argument(void) {
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if (inside_functions(pc) == 0) {
        avm_error("'argument' called outside a function!");
        retval.type = nil_m;
    } else {
        unsigned n = avm_totalactuals();
        if (n != 1)
            avm_error("one argument (not %d) expected in \'argument\'!", n);
        else {
            avm_memcellclear(&retval);
            unsigned par = avm_getactual(n - 1)->data.numVal + 1;
            retval.type = program_stack[p_topsp + AVM_NUMACTUALS_OFFSET + par].type;
            retval.data = program_stack[p_topsp + AVM_NUMACTUALS_OFFSET + par].data;
        }
    }
}

void libfunc_typeof(void) {
    unsigned n = avm_totalactuals();
    if (n != 1)
        avm_error("one argument (not %d) expected in \'typeof\'!", n);
    else {
        avm_memcellclear(&retval);
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }
}

void libfunc_strtonum(void) {
    unsigned n = avm_totalactuals();
    if (n != 1)
        avm_error("one argument (not %d) expected in \'strtonum\'!", n);
    else {
        avm_memcellclear(&retval);
        retval.data.numVal = strtod(avm_getactual(0)->data.strVal, NULL);
        retval.type = number_m;
    }
}

void libfunc_sqrt(void) {
    unsigned n = avm_totalactuals();
    if (n != 1)
        avm_error("one argument (not %d) expected in \'sqrt\'!", n);
    else {
        avm_memcellclear(&retval);
        retval.data.numVal = sqrt(avm_getactual(0)->data.numVal);
        retval.type = number_m;
        if (retval.data.numVal == NAN)
            retval.type = nil_m;
    }
}

void libfunc_cos(void) {
    unsigned n = avm_totalactuals();
    if (n != 1)
        avm_error("one argument (not %d) expected in \'cos\'!", n);
    else {
        avm_memcellclear(&retval);
        retval.data.numVal = cos(avm_getactual(0)->data.numVal);
        retval.type = number_m;
        if (retval.data.numVal == NAN)
            retval.type = nil_m;
    }
}

void libfunc_sin(void) {
    unsigned n = avm_totalactuals();
    if (n != 1)
        avm_error("one argument (not %d) expected in \'sin\'!", n);
    else {
        avm_memcellclear(&retval);
        retval.data.numVal = sin(avm_getactual(0)->data.numVal);
        retval.type = number_m;
        if (retval.data.numVal == NAN)
            retval.type = nil_m;
    }
}
