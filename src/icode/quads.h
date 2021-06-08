#ifndef ICODE_QUADS_H
#define ICODE_QUADS_H

#include <stdbool.h>

#include "../parser/parser.h"
#include "../symboltable/symtable.h"

#define EXPAND_SIZE 1024
#define CURR_SIZE (total * sizeof(quad))
#define NEW_SIZE (EXPAND_SIZE * sizeof(quad) + CURR_SIZE)

typedef enum iopcode {
    assign, 
    add, sub, mul, divide, mod, uminus,
    and, or, not, 
    if_eq, if_noteq,
    if_lesseq, if_greatereq, 
    if_less, if_greater, 
    jump, 
    call, param, 
    ret, getretval, funcstart, funcend,
    tablecreate, tablegetelem, tablesetelem,
    dummy
} iopcode;

static inline char* icodeToString(iopcode k) {
    static char* string[] = {
        "assign", 
        "add", "sub", "mul", "div", "mod", "uminus",
        "and", "or", "not",
        "if_eq", "if_noteq",
        "if_lesseq", "if_greatereq",
        "if_less", "if_greater",
        "jump", 
        "call", "param",
        "return", "getretval", "funcstart", "funcend", 
        "tablecreate", "tablegetelem", "tablesetelem", 
        ""
    };

    return string[k];
}

/* -=-=-=-=-=-=- Expressions -=-=-=-=-=-=- */
typedef enum expr_t {
    var_e,
    tableitem_e,

    programmfunc_e,
    libraryfunc_e,

    arithexpr_e,
    boolexpr_e,
    assignexpr_e,
    newtable_e,

    constint_e,
    constdouble_e,
    constbool_e,
    conststring_e,

    nil_e
} expr_t;

typedef struct list_s{
    unsigned int quadNo;
    struct list_s* next;

}list;

typedef struct expr {
    expr_t type;
    SymbolTableEntry* sym;
    struct expr* index;
    double numConst;
    char* strConst;
    unsigned char boolConst;

    struct expr* next;
    struct expr* prev;

    int falseQuad;
    int trueQuad;

    list* truelist_head;
    list* falselist_head;

} expr;

expr* expr_head;
expr* expr_tail;

expr* newExpr(expr_t expr);
expr* exprLookup(SymbolTableEntry* entry);
unsigned int istempname(const char *name);
unsigned int istempexpr(expr *e);
expr* member_item(expr* lvalue, char* name);

typedef struct indexed_elem{
    expr* index;
    expr* element;
}indexed_elem;

typedef struct elements{
    indexed_elem* elems[100];
    int no_of_elems;
}elements;


int allowed_operations(expr* e1, expr* e2);
expr* arithmetic_ops(expr *e1, iopcode operation, expr *e2);
expr* releational_ops(expr* e1, iopcode operation, expr* e2);

expr* boolQuads(expr* e);
void backpatch(list* l, int label);
expr* bool_expr(expr* e);

list* insert_list(list* lista, unsigned int quadNo);
list* merge_list(list* result, list* l1, list* l2);

struct stmt_s{
    expr *exp;
    int else_quad;
    struct list_s* breaklist;
    struct list_s* contlist;
};

struct stmt_s* new_stmt();
void patch_return_jumps();

typedef struct arguments_s {
    expr* arguments[100];
    int nargs;
} arguments_t;

typedef struct call_s {
    expr* arguments[100];
    int nargs;
    bool isMethod;
    char* name;
} call_t;

expr* make_call(expr* lvalue, expr* elist[], int nargs);

typedef struct quad {
    iopcode op;
    expr* result;
    expr* arg1;
    expr* arg2;
    unsigned label;
    unsigned line;
    unsigned taddress;
} quad;

quad* quads;
unsigned total;
unsigned int currQuad;

unsigned nextQuad();

void expand(void);
quad* emit(enum iopcode op, expr* result, expr* arg1, expr* arg2, unsigned label, unsigned line);
expr* emit_iftableitem(expr* exp);
void patchlabel(unsigned quadNo, unsigned label);

extern int getCurrScope();
extern int getCurrLine();
extern SymTable* table;

char* tempVarNameGen(char* prefix);
void savetmpvarOffset();
void restoretmpvarOffset();
SymbolTableEntry* newTempVar();
SymbolTableEntry* newTempFuncVar();
void resetTempVar();
void resetTempFunc();

void printQuads();

typedef struct stack_node{
    bool flag;
    struct stack_node* next;
}stack_node;

typedef struct stack_s{
    stack_node* head;
    stack_node* tail;
    int counter;
}stack;

stack* new_stack();
void push(stack* s);
bool pop(stack* s);
bool loop_top(stack* s);
void modify_tail(stack *s, bool flag);

#endif