#include "quads.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCOPES 100

unsigned total = 0;
unsigned int currQuad = 0;

unsigned int tmpvarOffset = 0;
unsigned int tmpOffsetSave[MAX_SCOPES] = {-1};
unsigned int tmpfuncOffset = 0;

expr* newExpr(expr_t type) {
    expr* new = (expr*)malloc(sizeof(expr));
    expr* tmp = expr_head;

    new->type = type;
    new->sym = NULL;
    new->numConst = 0;
    new->strConst = NULL;
    new->boolConst = 0;
    new->next = NULL;
    new->prev = NULL;

    new->truelist_head = NULL;
    new->falselist_head = NULL;
    
    if (expr_head == NULL) {
        expr_head = new;
        expr_tail = new;
    } else {
        expr_tail->next = new;
        new->prev = expr_tail;
        expr_tail = new;
    }

    return new;
}

expr* exprLookup(SymbolTableEntry* entry) {
    expr* tmp = expr_head;
    while (tmp != NULL) {
        if (tmp->type == var_e) {
            if (tmp->sym == entry) {
                return tmp;
            }
        }
        tmp = tmp->next;
    }

    return NULL;
}

unsigned int istempname(const char *name){
    return *name == '^';
}

unsigned int istempexpr(expr *e){
    return e->sym && istempname(e->sym->value.varVal->name);
}

expr* member_item(expr* lvalue, char* name) {
    lvalue = emit_iftableitem(lvalue);
    expr* item = newExpr(tableitem_e);
    item->sym = lvalue->sym;
    item->index = (expr*)malloc(sizeof(expr));
    item->index->strConst = strdup(name);
    item->index->type = conststring_e;

    return item;
}

int allowed_operations(expr* e1, expr* e2) {
    int flag = 1;
    switch (e1->type) {
        case programmfunc_e:
            printf("Error, Invalid use of operator on user function at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case libraryfunc_e:
            printf("Error, Invalid use of  operator on library function at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case newtable_e:
            printf("Error, Invalid use of operator on table at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case conststring_e:
            printf("Error, Invalid use of operator on string at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case constbool_e:
            printf("Error, Invalid use of operator on constant boolean at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case boolexpr_e:
            printf("Error, Invalid use of operator on boolean expression at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case nil_e:
            printf("Error, Invalid use of operator on nil at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        default:
            break;
    }

    if(e2 == NULL)
        return flag;

    switch (e2->type) {
        case programmfunc_e:
            printf("Error, Invalid use of operator on user function at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case libraryfunc_e:
            printf("Error, Invalid use of operator on library function at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case newtable_e:
            printf("Error, Invalid use of operator on table at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case conststring_e:
            printf("Error, Invalid use of operator on string at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case constbool_e:
            printf("Error, Invalid use of operator on constant boolean at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case boolexpr_e:
            printf("Error, Invalid use of operator on boolean expression at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        case nil_e:
            printf("Error, Invalid use of operator on nil at line <%d>\n", getCurrLine());
            flag = 0;
            break;
        default:
            break;
    }
    return flag;
}

expr* arithmetic_ops(expr* e1, iopcode operation, expr* e2) {
    if (allowed_operations(e1, e2) == 0)
        exit(0);

    expr* res = newExpr(arithexpr_e);
    if(istempexpr(e1)) {
        res->sym = e1->sym;
    }else if(istempexpr(e2)) {
        res->sym = e2->sym;
    }else {
        res->sym = newTempVar();
    }

    switch (operation) {
        case add:
            res->numConst = e1->numConst + e2->numConst;
            break;
        case sub:
            res->numConst = e1->numConst - e2->numConst;
            break;
        case mul:
            res->numConst = e1->numConst * e2->numConst;
            break;
        case divide:
            res->numConst = e1->numConst / e2->numConst;
            break;
        case mod:
            if(e2->numConst != 0)
                res->numConst = (int)e1->numConst % (int)e2->numConst;
            break;
        default:
            break;
    }
    emit(operation, res, e1, e2, 0, getCurrLine());

    return res;
}

expr* releational_ops(expr* e1, iopcode operation, expr* e2) {
    if(operation != if_eq && operation != if_noteq){
        if (allowed_operations(e1, e2) == 0){
            exit(0);
        }
    }
        

    expr* res = newExpr(boolexpr_e);

    expr* false_temp = newExpr(constbool_e);
    false_temp->boolConst = 'f';
    expr* true_temp = newExpr(constbool_e);
    true_temp->boolConst = 't';

    emit(operation, NULL, e1, e2, -1, getCurrLine());
    res->truelist_head = insert_list(res->truelist_head, currQuad - 1);
    emit(jump, NULL, NULL, NULL, -1, getCurrLine());
    res->falselist_head = insert_list(res->falselist_head, currQuad - 1);

    return res;
}

expr* boolQuads(expr* e) {
    expr* ret = e;
    expr* false_temp = newExpr(constbool_e);
    false_temp->boolConst = 'f';
    expr* true_temp = newExpr(constbool_e);

    true_temp->boolConst = 't';
    if (ret->type != boolexpr_e) {
        emit(if_eq, NULL, ret, true_temp, -1, getCurrLine());
        ret->truelist_head = insert_list(ret->truelist_head, currQuad - 1);
        emit(jump, NULL, NULL, NULL, -1, getCurrLine());
        ret->falselist_head = insert_list(ret->falselist_head, currQuad - 1);
    }

    return ret;
}

list* insert_list(list* lista, unsigned int quadNo) {
    list* head = lista;
    list* new = (list*)malloc(sizeof(list));
    new->quadNo = quadNo;
    new->next = NULL;
    if (lista == NULL)
        head = new;
    else {
        while (lista->next != NULL) {
            lista = lista->next;
        }
        lista->next = new;
    }

    return head;
}

list* merge_list(list* result, list* l1, list* l2) {
    list* head = result;
    list* res_l1 = result;
    list* l1_l2 = l1;

    if (result != NULL) {
        if (l1 == NULL && l2 == NULL)
            return head;

        while (res_l1->next != NULL)
            res_l1 = res_l1->next;

        if (l2 == NULL) {
            res_l1->next = l1;
            return head;
        } else if (l1 == NULL) {
            res_l1->next = l2;
            return head;
        } else {
            res_l1->next = l1;
            while (l1_l2->next != NULL)
                l1_l2 = l1_l2->next;

            l1_l2->next = l2;

            return head;
        }
    } else {
        if (l1 == NULL && l2 == NULL)
            return NULL;

        if (l1 == NULL) {
            head = l2;
            return head;
        } else if (l2 == NULL) {
            head = l1;
            return head;
        } else {
            while (l1_l2->next != NULL)
                l1_l2 = l1_l2->next;

            l1_l2->next = l2;
            head = l1;

            return head;
        }
    }

    return head;
}

void backpatch(list* l, int label) {
    list* tmp = l;
    while (tmp != NULL) {
        quads[tmp->quadNo].label = label;
        tmp = tmp->next;
    }
    return;
}

expr* bool_expr(expr* e) {
    e->sym = newTempVar();
    e->type = var_e;
    expr* false_temp = newExpr(constbool_e);
    false_temp->boolConst = 'f';
    expr* true_temp = newExpr(constbool_e);
    true_temp->boolConst = 't';

    emit(assign, e, true_temp, NULL, 0, getCurrLine());
    e->trueQuad = currQuad - 1;
    emit(jump, NULL, NULL, NULL, currQuad + 2, getCurrLine());
    emit(assign, e, false_temp, NULL, 0, getCurrLine()); 
    e->falseQuad = currQuad - 1;
    
    backpatch(e->truelist_head, e->trueQuad);
    backpatch(e->falselist_head, e->falseQuad);

    return e;
}

struct stmt_s* new_stmt(){
    struct stmt_s* ret = (struct stmt_s*)malloc(sizeof(struct stmt_s));
    ret->breaklist = NULL;
    ret->contlist = NULL;

    return ret;
}

void patch_return_jumps(){
    int i = currQuad;
    int inFunc = 0;
    int funcend_address = 0;
    while(i > 0){
        if(quads[i].op == funcend){
            inFunc = 1;
            funcend_address = i;
        }
        if(quads[i].op == jump && inFunc == 1){
            if(quads[i-1].op == ret)
                quads[i].label = funcend_address;
        }
        if(quads[i].op == funcstart){
            inFunc = 1;
            funcend_address = 0;
        }
        
        i--;
    }
}

expr* make_call(expr* lvalue, expr* elist[], int nargs) {
    expr* func = emit_iftableitem(lvalue);
    int i;
    for (i = nargs - 1; i >= 0; i--) {
        emit(param, elist[i], NULL, NULL, 0, getCurrLine());
    }
    emit(call, func, NULL, NULL, 0, getCurrLine());
    expr* result = newExpr(var_e);
    result->sym = newTempVar();
    emit(getretval, result, NULL, NULL, 0, getCurrLine());
    return result;
}

char* tempVarNameGen(char* prefix) {
    char* var = (char*)malloc(3 * sizeof(char));

    if (prefix[0] == '^') {
        sprintf(var, "%s%d", prefix, tmpvarOffset);
        tmpvarOffset++;
    } else if (prefix[0] == '$') {
        sprintf(var, "%s%d", prefix, tmpfuncOffset);
        tmpfuncOffset++;
    }

    return var;
}
void savetmpvarOffset(int scope){
    /*printf("line: %d, save offset: %d\n", getCurrLine(), tmpvarOffset);*/
    tmpOffsetSave[scope] = tmpvarOffset;
}

void restoretmpvarOffset(int scope){
    /*printf("line: %d, offset: %d restored to: %d\n", getCurrLine(), tmpvarOffset, tmpOffsetSave[scope]);*/
    tmpvarOffset = tmpOffsetSave[scope] ;
}

SymbolTableEntry* newTempVar() {
    Variable* var = create_variable(tempVarNameGen("^"), getCurrScope(), 0);
    SymbolTableEntry* new = variable_lookup(table, var, TEMP_VAR, getCurrScope());

    return new;
}

SymbolTableEntry* newTempFuncVar() {
    Function* tmp = create_function(tempVarNameGen("$"), getCurrScope(), getCurrLine());
    SymbolTableEntry* new;
    new = function_lookup(table, tmp, TEMP_VAR, getCurrScope(), false);

    return new;
}

void resetTempVar() {
    tmpvarOffset = 0;
}
void resetTempFunc() {
    tmpfuncOffset = 0;
}

unsigned nextQuad(){
    return currQuad;
}

void expand(void) {
    assert(total == currQuad);
    quad* p = (quad*)malloc(NEW_SIZE);
    if (quads) {
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }
    quads = p;
    total += EXPAND_SIZE;
}

quad* emit(enum iopcode op, expr* result, expr* arg1, expr* arg2, unsigned label, unsigned line) {
    if (currQuad == total) expand();

    quad* p = quads + currQuad;
    p->op = op;
    p->result = result;
    p->arg1 = arg1;
    p->arg2 = arg2;
    p->label = label;
    p->line = line;
    p->taddress = 0;

    currQuad++;

    return p;
}

expr* emit_iftableitem(expr* exp) {
    if (exp->type != tableitem_e)
        return exp;
    else {
        expr* res = newExpr(var_e);
        res->sym = newTempVar();
        emit(tablegetelem, res, exp, exp->index, 0, getCurrLine());
        return res;
    }
}

void patchlabel(unsigned quadNo, unsigned label){
    assert(quadNo < currQuad);
    quads[quadNo].label = label;
}

void printQuads(FILE* stream) {
    int i;
    for (i = 0; i < currQuad; i++) {
        fprintf(stream, "%d: ", i);

        fprintf(stream, "%s\t", icodeToString(quads[i].op));

        if (quads[i].result != NULL) {
            switch (quads[i].result->type) {
                case var_e:
                    fprintf(stream, "%s ", quads[i].result->sym->value.varVal->name);
                    break;
                case arithexpr_e:
                    fprintf(stream, "%s ", quads[i].result->sym->value.varVal->name);
                    break;
                case programmfunc_e:
                    fprintf(stream, "%s ", quads[i].result->sym->value.funcVal->name);
                    break;
                case libraryfunc_e:
                    fprintf(stream, "%s ", quads[i].result->sym->value.funcVal->name);
                    break;
                case newtable_e:
                    fprintf(stream, "%s ", quads[i].result->sym->value.varVal->name);
                    break;
                case tableitem_e:
                    fprintf(stream, "%s ", quads[i].result->sym->value.varVal->name);
                    break;
                case constdouble_e:
                    fprintf(stream, "%lf ", quads[i].result->numConst);
                    break;
                case constint_e:
                    fprintf(stream, "%d ", (int)quads[i].result->numConst);
                    break;
                case conststring_e:
                    fprintf(stream, "%s ", quads[i].result->strConst);
                    break;
                case constbool_e:
                    fprintf(stream, "%s ", quads[i].result->boolConst == 't' ? "true" : "false");
                    break;
                default:
                    break;
            }
        }

        /*Argument 1 print*/
        if (quads[i].arg1 != NULL) {
            switch (quads[i].arg1->type) {
                case constdouble_e:
                    fprintf(stream, "%lf ", quads[i].arg1->numConst);
                    break;
                case constint_e:
                    fprintf(stream, "%d ", (int)quads[i].arg1->numConst);
                    break;
                case conststring_e:
                    fprintf(stream, "%s ", quads[i].arg1->strConst);
                    break;
                case constbool_e:
                    fprintf(stream, "%s ", quads[i].arg1->boolConst == 't' ? "true" : "false");
                    break;
                case var_e:
                    fprintf(stream, "%s ", quads[i].arg1->sym->value.varVal->name);
                    break;
                case arithexpr_e:
                    fprintf(stream, "%s ", quads[i].arg1->sym->value.varVal->name);
                    break;

                case programmfunc_e:
                    fprintf(stream, "%s ", quads[i].arg1->sym->value.funcVal->name);
                    break;
                case newtable_e:
                    fprintf(stream, "%s ", quads[i].arg1->sym->value.varVal->name);
                    break;
                case tableitem_e:
                    fprintf(stream, "%s ", quads[i].arg1->sym->value.varVal->name);
                    break;

                default:
                    break;
            }
        }

        /*Argument 2 print*/
        if (quads[i].arg2 != NULL) {
            switch (quads[i].arg2->type) {
                case constdouble_e:
                    fprintf(stream, "%lf ", quads[i].arg2->numConst);
                    break;
                case constint_e:
                    fprintf(stream, "%d ", (int)quads[i].arg2->numConst);
                    break;
                case conststring_e:
                    fprintf(stream, "%s ", quads[i].arg2->strConst);
                    break;
                case constbool_e:
                    fprintf(stream, "%s ", quads[i].arg2->boolConst == 't' ? "true" : "false");
                    break;
                case var_e:
                    fprintf(stream, "%s ", quads[i].arg2->sym->value.varVal->name);
                    break;
                case arithexpr_e:
                    fprintf(stream, "%s ", quads[i].arg2->sym->value.varVal->name);
                    break;
                case programmfunc_e:
                    fprintf(stream, "%s ", quads[i].arg2->sym->value.funcVal->name);
                    break;
                case newtable_e:
                    fprintf(stream, "%s ", quads[i].arg2->sym->value.varVal->name);
                    break;
                case tableitem_e:
                    fprintf(stream, "%s ", quads[i].arg2->sym->value.varVal->name);
                    break;

                default:
                    break;
            }
        }
        switch (quads[i].op)
        {
        case jump:
            fprintf(stream, "%d", quads[i].label);
            break;
        case if_eq:
            fprintf(stream, "%d", quads[i].label);
            break;
        case if_noteq:
            fprintf(stream, "%d", quads[i].label);
            break;
        case if_lesseq:
            fprintf(stream, "%d", quads[i].label);
            break;
        case if_greatereq:
            fprintf(stream, "%d", quads[i].label);
            break;
        case if_less:
            fprintf(stream, "%d", quads[i].label);
            break;
        case if_greater:
            fprintf(stream, "%d", quads[i].label);
            break;
        default:
            break;
        }
        

        fprintf(stream, "[line %d]\n", quads[i].line);
    }
}


stack* new_stack(){
    stack* ret = (stack *)malloc(sizeof(stack));
    ret->head = NULL;
    ret->tail = NULL;
    ret->counter = 0;

    return ret;
}

void push(stack* s){
    stack_node* new = (stack_node *)malloc(sizeof(stack_node));
    new->flag = false;
    new->next = NULL;
    
    if(s->head == NULL){
        s->head = new;
        s->tail = new;
    }   
    else{
        s->tail->next = new;
        s->tail = new;
    }
        
}

bool pop(stack* s){
    bool ret = s->tail->flag;
    stack_node* tmp = s->head;

    while(tmp->next != s->tail){
        tmp = tmp->next;
    }
    s->tail = tmp;
    free(tmp->next);
    s->tail->next = NULL;

    return ret;
}

bool loop_top(stack* s){
    return s->tail->flag;
}

void modify_tail(stack *s, bool flag){
    s->tail->flag = flag;
}