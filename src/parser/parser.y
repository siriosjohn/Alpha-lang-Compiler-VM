%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../icode/quads.h"
#include "../symboltable/symtable.h"
#include "../symboltable/symboltype.h"
#include "../symboltable/Variable.h"
#include "../symboltable/Function.h"
#include "../targetcode/alpha_bc.h"

#define ERROR_OUT stderr
#define RULES_STREAM rules_out
#define SYMTAB_SC_OUT sym_sc_out
#define SYMTAB_BUC_OUT sym_buc_out
#define QUAD_STREAM quad_out
#define TARGET_STREAM target_out
#define BIN_STREAM binary_out

#define MAX_INNER_FUNC 50

int yydebug = 1;

char DIR_BUFF[100];

typedef enum error_t{
    error,
    warning
}error_t;
char error_msg[100];

struct forpr_s{
    int test;
    int enter;
};

struct break_s{
    struct list_s* breaklist;
};

struct cont_s{
    struct list_s* contlist;
};

int getCurrScope();
int getCurrLine();
void print_rules(char* rule1, char* rule2);
void print_error(error_t eror_type, char* msg);
void funcjumpsReset(int offset);

int yylex();
void yyerror();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
extern FILE *yyout;

FILE* rules_out;
FILE* sym_sc_out;
FILE* sym_buc_out;
FILE* quad_out;
FILE* target_out;
FILE* binary_out;

int curr_scope = 0;
bool isLocal = false;

SymTable *table;

/* Variables used for funcdef */
struct func_node{
    int local_offset;
    struct func_node* next;
};

struct func_local_stack{
    struct func_node* tail;
    struct func_node* head;
    int nodes;
};
struct func_local_stack* functionLocalsStack;
struct func_local_stack* create_func_stack();
void func_push(struct func_local_stack* s, int cur_local_offset);
int func_pop(struct func_local_stack* s);
int func_top(struct func_local_stack* s);

quad *funcjumpQuad[MAX_INNER_FUNC];
quad *retjumpQuad[MAX_INNER_FUNC];
int functionOffset = -1;

/* Flags and variables for function calls */
int parserFlag = 0;
int libreturnCnt = 0;

/* return - break - continue flags */
struct stack_s* loopstack;
int returnCnt = 0;

/* -=-=-=- for/while flags and vars -=-=-=- */
quad* whileQuad;
int loopcounter = 0;

%}

%union{
    int integer;
    double num;
    char *str;
    char character;
    bool boolean;
    struct expr* exp;
    struct arguments_s* arg;
    struct call_s* call_type;
    struct indexed_elem* ind_elem;
    struct elements* elems;
    struct SymbolTableEntry* symbol;
    struct forpr_s* forpr_t;
    struct stmt_s* stmt_t;
    struct break_s* break_t;
    struct cont_s* cont_t;
}

%start program

%token <num> INT_CONST
%token <num> REAL_CONST
%token <str> ID 
%token KEYWORD OPERATOR PUNCTATION
%token <str> STRING 
%token <character> TRUE FALSE


%type <integer> M K L whilestart whilecond ifprefix elserpefix 
%type <exp> lvalue expr assignexpr const term primary returnstmt call objectdef member funcdef 
%type <symbol> funcprefix 
%type <ind_elem> indexedelem
%type <elems> indexed 
%type <forpr_t> forprefix
%type <stmt_t> loopstmt stmt stmts block ifstmt elsestmt
%type <str> funcname
%type <integer> funcbody
%type <call_type> callsuffix methodcall normalcall
%type <arg> elist idlist
%type <cont_t> continuest
%type <break_t> breakst


%expect 1

%token  AND OR NOT
%token  ASSIGN PLUS MINUS MULTIPLY DIVIDE MOD GREATER GREATER_EQ LESS LESS_EQ EQ NOT_EQ UMINUS
%token  INCREMENT DECREMENT
%token  OPEN_BRACKET CLOSE_BRACKET OPEN_SUBSCRIPT CLOSE_SUBSCRIPT OPEN_PAR CLOSE_PAR SEMICOLON COMMA COLON DOUBLE_COLON DOUBLE_DOT DOT            
%token  WHILE FOR BREAK CONTINUE RETURN
%token  IF ELSE
%token  LOCAL NIL FUNCTION
%token  END_OF_FILE 
%token  INF_COMMENT INF_STRING


%right      ASSIGN
%left       OR
%left       AND
%nonassoc   EQ NOT_EQ
%nonassoc   GREATER GREATER_EQ LESS LESS_EQ 
%left       PLUS MINUS
%left       MULTIPLY DIVIDE MOD
%right      NOT INCREMENT DECREMENT UMINUS
%left       DOT DOUBLE_DOT
%left       OPEN_SUBSCRIPT CLOSE_SUBSCRIPT
%left       OPEN_PAR CLOSE_PAR
%left       OPEN_BRACKET CLOSE_BRACKET

%%

program     :   stmts                   {print_rules("program","stmts");}
            ;

stmt        :   expr SEMICOLON          {
                                            print_rules("stmt","expr;");
                                            if($1->type == boolexpr_e)
                                                $1 = bool_expr($1);
                                            $$ = new_stmt();
                                            $$->exp = $1;
                                            
                                        }
            |   ifstmt                  {
                                            print_rules("stmt","ifstmt");
                                            $$ = new_stmt();
                                            $$->breaklist = $1->breaklist; 
                                            $$->contlist = $1->contlist;
                                        }
            |   whilestmt               {
                                            print_rules("stmt","whilestmt");
                                            $$ = new_stmt();
                                        }
            |   forstmt                 {
                                            print_rules("stmt","forstmt");
                                            $$ = new_stmt(); 
                                        }
            |   returnstmt              {
                                            print_rules("stmt","returnstmt;");
                                            $$ = new_stmt();
                                            $$->exp = $1;
                                            if(returnCnt == 0){
                                                fprintf(stderr, "Error, return statement at line <%d>, outside of function\n", yylineno);
                                                exit_flag = 1;
                                            }
                                        }
            |   breakst                 {
                                            print_rules("stmt","break;");
                                            $$ = new_stmt();
                                            $$->breaklist = $1->breaklist;                                            
                                            if(loop_top(loopstack) == false){
                                                fprintf(stderr, "Error, break statement at line <%d>, outside of loop\n", yylineno);
                                                exit_flag = 1;
                                            }
                                                
                                        }
            |   continuest              {
                                            print_rules("stmt","continue;");
                                            $$ = new_stmt();
                                            $$->contlist = $1->contlist;
                                            if(loop_top(loopstack) == false){
                                                fprintf(stderr, "Error, continue statement at line <%d>, outside of loop\n", yylineno);
                                                exit_flag = 1;
                                            }
                                                
                                        }
            |   block                   {
                                            print_rules("stmt","block");
                                            $$ = new_stmt(); 
                                            $$->breaklist = $1->breaklist; 
                                            $$->contlist = $1->contlist; 
                                        }
            |   funcdef                 {
                                            print_rules("stmt","funcdef;");
                                            $$ = new_stmt();
                                        }
            |   SEMICOLON               {
                                            print_rules("stmt",";");
                                            $$ = new_stmt(); 
                                        }
            ;
           
stmts       :   stmts stmt  {
                                print_rules("stmt","stmts stmt");
                                $$ = new_stmt();
                                $$->breaklist = merge_list($$->breaklist, $1->breaklist, $2->breaklist);
                                $$->contlist = merge_list($$->contlist, $1->contlist, $2->contlist);
                                /*hide_tmp_var(table, curr_scope);*/
                                resetTempVar();
                            }
            |    /* empty */    {$$ = new_stmt();}
            ;

M           :   /* empty */     {$$ = nextQuad();}
            ;

expr        :   assignexpr  {
                                print_rules("expr","assignexpr");
                                $$ = $1;
                            }
            |   expr OR {$1 = boolQuads($1);} M expr        
                                                    {
                                                        print_rules("expr","expr or expr");
                                                        $5 = boolQuads($5);

                                                        $$ = newExpr(boolexpr_e);
                                                        backpatch($1->falselist_head, $4);
                                                        $$->truelist_head = merge_list($$->truelist_head, $1->truelist_head, $5->truelist_head);
                                                        $$->falselist_head = $5->falselist_head;
                                                    }    
            |   expr AND {$1 = boolQuads($1);} M expr        
                                                    {
                                                        print_rules("expr","expr and expr");
                                                        
                                                        $5 = boolQuads($5);

                                                        $$ = newExpr(boolexpr_e);
                                                        backpatch($1->truelist_head, $4);
                                                        $$->truelist_head = $5->truelist_head;
                                                        $$->falselist_head = merge_list($$->falselist_head, $1->falselist_head, $5->falselist_head);
                                                    }
            |   expr EQ M   {
                                if($1->type == boolexpr_e)
                                    $1 = bool_expr($1);
                            }
                expr        {
                                print_rules("expr","expr == expr");
                                if($5->type == boolexpr_e)
                                    $5 = bool_expr($5);
                 
                                $$ = releational_ops($1, if_eq, $5);  
                            }
            |   expr NOT_EQ M   {
                                    if($1->type == boolexpr_e)
                                        $1 = bool_expr($1);
                                }
                expr        {
                                print_rules("expr","expr == expr");
                                if($5->type == boolexpr_e)
                                    $5 = bool_expr($5);
                                
                                $$ = releational_ops($1, if_noteq, $5);  
                            }
            |   expr GREATER expr       {
                                            print_rules("expr","expr > expr");
                                            $$ = releational_ops($1, if_greater, $3);  
                                        }
            |   expr GREATER_EQ expr    {
                                            print_rules("expr","expr >= expr");
                                            $$ = releational_ops($1, if_greatereq, $3); 
                                        }
            |   expr LESS expr          {
                                            print_rules("expr","expr < expr");
                                            $$ = releational_ops($1, if_less, $3);
                                        }
            |   expr LESS_EQ expr       {
                                            print_rules("expr","expr <= expr");
                                            $$ = releational_ops($1, if_lesseq, $3); 
                                        }

            |   expr PLUS expr          {   /*                  PLUS(+)                    */
                                            print_rules("expr","expr + expr");  
                                            $$ = arithmetic_ops($1, add, $3);
                                        }
            |   expr MINUS expr         {   /*                  MINUS(-)                     */
                                            print_rules("expr","expr - expr");
                                            $$ = arithmetic_ops($1, sub, $3);
                                        }
            |   expr MULTIPLY expr      {   /*                  MULTIPLY(*)                     */
                                            print_rules("expr","expr * expr");
                                            $$ = arithmetic_ops($1, mul, $3);
                                        }
            |   expr DIVIDE expr        {   /*                  DIVIDE(/)                     */
                                            print_rules("expr","expr / expr");
                                            $$ = arithmetic_ops($1, divide, $3);
                                        }
            |   expr MOD expr           {   /*                  MOD(%)                     */
                                            print_rules("expr","expr % expr");
                                            $$ = arithmetic_ops($1, mod, $3);
                                        }
            |   term                    {
                                            print_rules("expr","term");
                                            $$ = $1;
                                        }
            ;

term        :   OPEN_PAR expr CLOSE_PAR  {
                                            print_rules("term","( expr )");
                                            $$ = $2;
                                        }
            |   MINUS expr %prec UMINUS {
                                            print_rules("term","- expr");
                                            allowed_operations($2, NULL);
                                            $$ = newExpr(arithexpr_e);
                                            $$->sym = istempexpr($2) ? $2->sym : newTempVar();

                                            $$->numConst = -$2->numConst;
                                            emit(uminus, $$, $2, NULL, 0, yylineno);

                                        }
            |   NOT expr                {
                                            print_rules("term","not expr");
                                            $2 = boolQuads($2);

                                            $$ = newExpr(boolexpr_e);
                                            $$->truelist_head = $2->falselist_head;
                                            $$->falselist_head = $2->truelist_head;
                                        }
            |   INCREMENT lvalue        {

                                            if(isLocal == false && $2->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, $2->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", $2->sym->value.varVal->name, yylineno);
                                                    parserFlag = 1;
                                                    exit_flag = 1;
                                                }
                                            }

                                            libreturnCnt = 0;
                                            print_rules("term","++lvalue");
                                            if(parserFlag == 0){
                                                if($2->type == tableitem_e){
                                                    expr* tmp_exp = emit_iftableitem($2);
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(add, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, $2, $2->index, tmp_exp, 0, yylineno);
                                                    $$ = newExpr(var_e);
                                                    $$->sym = newTempVar();
                                                    emit(assign, $$, tmp_exp, NULL, 0, yylineno);
                                                }else{
                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    $2->numConst += temp_expr->numConst; /* ?? */
                                                    emit(add, $2, $2, temp_expr, 0, yylineno);
                                                    SymbolTableEntry *temp = newTempVar();
                                                    $$ = newExpr(var_e);
                                                    $$->sym = temp;
                                                    $$->numConst = $2->numConst; /* ?? */
                                                    emit(assign, $$, $2, NULL, 0, yylineno);
                                                }
                                            }
                                            
                                            parserFlag = 0;

                                        }
            |   lvalue INCREMENT        {
                                            if(isLocal == false && $1->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, $1->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", $1->sym->value.varVal->name, yylineno);
                                                    exit_flag = 1;
                                                    parserFlag = 1;
                                                }
                                            }
                                            libreturnCnt = 0;

                                            print_rules("term","lvalue++");

                                            if(parserFlag == 0){
                                                
                                                if($1->type == tableitem_e){
                                                    $$ = newExpr(var_e);
                                                    $$->sym = newTempVar();
                                                    expr* tmp_exp = emit_iftableitem($1);
                                                    emit(assign, $$, tmp_exp, NULL, 0, yylineno);
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(add, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, $1, $1->index, tmp_exp, 0, yylineno);
                                                }else{
                                                    SymbolTableEntry *temp = newTempVar();
                                                    $$ = newExpr(var_e);
                                                    $$->sym = temp;
                                                    $$->numConst = $1->numConst; 
                                                    emit(assign, $$, $1, NULL, 0, yylineno);

                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    $1->numConst += temp_expr->numConst;
                                                    emit(add, $1, $1, temp_expr, 0, yylineno);
                                                }
                                            }
                                            parserFlag = 0;

                                            
                                        }
            |   DECREMENT lvalue        {
                                            if(isLocal == false && $2->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, $2->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", $2->sym->value.varVal->name, yylineno);
                                                    exit_flag = 1;
                                                    parserFlag = 1;
                                                }
                                            }
                                            libreturnCnt = 0;
                                            print_rules("term","--lvalue");

                                            if(parserFlag == 0){
                                                if($2->type == tableitem_e){
                                                    expr* tmp_exp = emit_iftableitem($2);
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(sub, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, $2, $2->index, tmp_exp, 0, yylineno);
                                                    $$ = newExpr(var_e);
                                                    $$->sym = newTempVar();
                                                    emit(assign, $$, tmp_exp, NULL, 0, yylineno);
                                                }else{
                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    $2->numConst -= temp_expr->numConst; /* ?? */
                                                    emit(add, $2, $2, temp_expr, 0, yylineno);
                                                    SymbolTableEntry *temp = newTempVar();
                                                    $$ = newExpr(var_e);
                                                    $$->sym = temp;
                                                    $$->numConst = $2->numConst; /* ?? */
                                                    emit(assign, $$, $2, NULL, 0, yylineno);
                                                }
                                            }
                                            parserFlag = 0;
                                        }
            |   lvalue DECREMENT        {
                                            print_rules("term","lvalue--");

                                            if(isLocal == false && $1->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, $1->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", $1->sym->value.varVal->name, yylineno);
                                                    exit_flag = 1;
                                                    parserFlag = 1;
                                                }
                                            }
                                            libreturnCnt = 0;
                                            

                                            if(parserFlag == 0){
                                                if($1->type == tableitem_e){
                                                    $$ = newExpr(var_e);
                                                    $$->sym = newTempVar();
                                                    expr* tmp_exp = emit_iftableitem($1);
                                                    emit(assign, $$, tmp_exp, NULL, 0, yylineno);
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(sub, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, $1, $1->index, tmp_exp, 0, yylineno);
                                                }else{
                                                    SymbolTableEntry *temp = newTempVar();
                                                    $$ = newExpr(var_e);
                                                    $$->sym = temp;
                                                    $$->numConst = $1->numConst; 
                                                    emit(assign, $$, $1, NULL, 0, yylineno);

                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    $1->numConst += temp_expr->numConst;
                                                    emit(sub, $1, $1, temp_expr, 0, yylineno);
                                                }
                                            }
                                            parserFlag = 0;
                                        }
            |   primary                 {$$ = $1;print_rules("term","primary");}
            ;

assignexpr  :   lvalue  
                ASSIGN  {
                            if(isLocal == false && $1->sym->value.varVal->name != NULL){
                                if(distinguish_lookup(table, $1->sym->value.varVal->name, curr_scope) != GENERAL){
                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", $1->sym->value.varVal->name, yylineno);
                                    exit_flag = 1;
                                    parserFlag = 1;
                                }
                            }
                            
                            libreturnCnt = 0;
                        }
                expr    {
                            print_rules("assignexpr","lvalue = expr");
                            if($4->type == boolexpr_e)
                                $4 = bool_expr($4);
                            if(parserFlag == 0){
                                if($1->type == tableitem_e){
                                    emit(tablesetelem, $1, $1->index, $4, 0, yylineno);
                                    $$ = newExpr(tableitem_e);
                                    $$->sym = newTempVar();
                                    emit(tablegetelem, $$, $1, $1->index, 0, yylineno); 

                                }else {
                                    $1->numConst = $4->numConst;
                                    $1->strConst = $4->strConst;
                                    $1->boolConst = $4->boolConst;
                                    emit(assign, $1, $4, NULL, 0, yylineno);

                                    SymbolTableEntry *temp = newTempVar();
                                    $$ = newExpr(var_e);
                                    $$->sym = temp;
                                    $$->numConst = $1->numConst;
                                    $$->strConst = $1->strConst;
                                    $$->boolConst = $1->boolConst;
                                    emit(assign, $$, $1, NULL, 0, yylineno);
                                    /*printf("%s = %f\n", $$->sym->value.varVal->name, $$->numConst);*/ /* print_tag */
                                }
                            }
                            parserFlag = 0;

                            
                        }
            ;

primary     :   lvalue                          {
                                                    print_rules("primary","lvalue");
                                                    $$ = emit_iftableitem($1);
                                                }
            |   call                            {
                                                    $$ = $1;
                                                    print_rules("primary","call");
                                                }
            |   objectdef                       {
                                                    print_rules("primary","objectdef");
                                                    $$ = $1;
                                                }
            |   OPEN_PAR funcdef CLOSE_PAR      {   
                                                    $$ = $2;
                                                    print_rules("primary","( funcdef )");
                                                    restoretmpvarOffset(curr_scope);
                                                }
            |   const                           {
                                                    $$ = $1;
                                                    print_rules("primary","const");
                                                }
            ;

lvalue      :   ID              {
                                    isLocal = false;
                                    if(distinguish_lookup(table, yytext, curr_scope) == GENERAL){
                                        /*fprintf(stderr, "variable ID: %s\n", yytext);*/
                                        $$ = newExpr(var_e);
                                        $$->sym = variable_lookup(table, create_variable(yytext, curr_scope, yylineno), GENERAL, curr_scope);

                                    }
                                    else{
                                        /*fprintf(stderr, "func call ID: %s\n", yytext);*/
                                        SymbolTableEntry *temp = function_lookup(table, create_function(yytext, curr_scope, yylineno), GLOBAL, curr_scope, true);
                                        if(temp != NULL){
                                            $$ = newExpr(programmfunc_e);
                                            $$->sym = temp;
                                        }
                                        else{
                                            $$ = newExpr(libraryfunc_e);
                                            $$->sym = libfunc_lookup(table, yytext);
                                            libreturnCnt = 1;
                                        }
                                    }
                                    print_rules("lvalue","ID");
                                }

            |   LOCAL ID        {
                                    isLocal = true;
                                    /*fprintf(stderr, "local ID: %s\n", yytext);*/
                                    $$ = newExpr(var_e);
                                    $$->sym = variable_lookup(table, create_variable(yytext, curr_scope, yylineno), LOCAL_t, curr_scope);
                                    print_rules("lvalue","local ID");
                                }        
            
            |   DOUBLE_COLON ID {
                                    isLocal = false;
                                    if(distinguish_lookup(table, yytext, curr_scope) == GENERAL){
                                        /*fprintf(stderr, "global ID: %s\n", yytext);*/
                                        SymbolTableEntry *temp = variable_lookup(table, create_variable(yytext, curr_scope, yylineno), GLOBAL, curr_scope);
                                        $$ = exprLookup(temp);
                                        if($$ == NULL){
                                            exit(0);
                                        }

                                    }
                                    else{
                                        /*fprintf(stderr, "func call ID: %s\n", yytext);*/
                                        SymbolTableEntry *temp = function_lookup(table, create_function(yytext, curr_scope, yylineno), GLOBAL, curr_scope, true);
                                        if(temp != NULL){
                                            $$ = newExpr(programmfunc_e);
                                            $$->sym = temp;
                                        }
                                        else{
                                            $$ = newExpr(libraryfunc_e);
                                            $$->sym = libfunc_lookup(table, yytext);
                                            libreturnCnt = 1;
                                        }

                                    }
                                    print_rules("lvalue","::ID");
                                }

            |   member          {
                                    print_rules("lvalue","member");
                                    $$ = $1;                                    
                                }
            ;

member      :   lvalue DOT ID   {
                                    print_rules("member",".ID");
                                    $$ = member_item($1, $3);

                                }
            |   lvalue OPEN_SUBSCRIPT expr CLOSE_SUBSCRIPT  {
                                                                print_rules("member","lvalue [ expr ]");
                                                                $1 = emit_iftableitem($1);
                                                                $$ = newExpr(tableitem_e);
                                                                $$->sym = $1->sym;
                                                                $$->index = (expr*) malloc(sizeof(expr));
                                                                $$->index = $3;
                                                            }
            |   call DOT ID {
                                print_rules("member","call.ID");
                                $$ = member_item($1, $3);
                            }
            |   call OPEN_SUBSCRIPT expr CLOSE_SUBSCRIPT    {
                                                                print_rules("member","call[ expr ]");
                                                                $1 = emit_iftableitem($1);
                                                                $$ = newExpr(tableitem_e);
                                                                $$->sym = $1->sym;
                                                                $$->index = (expr*) malloc(sizeof(expr));
                                                                $$->index = $3;
                                                            }
            ;

call        :   call OPEN_PAR elist CLOSE_PAR   {
                                                    print_rules("call","call ( elist )");
                                                    if($1->sym->isVariable == true){
                                                        $$ = make_call($1, $3->arguments, $3->nargs);
                                                    }
                                                    else{
                                                        if($3->nargs >= $1->sym->value.funcVal->totalArgs){
                                                            $$ = make_call($1, $3->arguments, $3->nargs);
                                                        }
                                                        else{
                                                            fprintf(stderr, "Error, too few arguments to function \"%s\", at line <%d>\n", $1->sym->value.funcVal->name, yylineno);
                                                            exit_flag = 1;
                                                        }
                                                    }

                                                }
            |   lvalue callsuffix   {
                                        print_rules("call","lvalue callsuffix");
                                        if($2->isMethod == true){
                                            expr* self = $1;
                                            $1 = emit_iftableitem(member_item(self, $2->name));
                                            int i;
                                            for(i = $2->nargs; i >= 0; i--)
                                                $2->arguments[i+1] = $2->arguments[i]; 
                                            
                                            $2->arguments[0] = self;
                                            $2->nargs++;
                                        }
                                        if($1->sym->isVariable == true){
                                            $$ = make_call($1, $2->arguments, $2->nargs);
                                        }
                                        else{
                                            if($2->nargs >= $1->sym->value.funcVal->totalArgs){
                                                $$ = make_call($1, $2->arguments, $2->nargs);
                                            } else {
                                                printf("%s : %d\n", $1->sym->value.funcVal->name, $1->sym->value.funcVal->totalArgs);
                                                fprintf(stderr, "Error, too few arguments to function \"%s\", at line <%d>\n", $1->sym->value.funcVal->name, yylineno);
                                                exit_flag = 1;
                                            }
                                        }

                                    }
            |   OPEN_PAR funcdef CLOSE_PAR {restoretmpvarOffset(curr_scope);}OPEN_PAR elist CLOSE_PAR     {   
                                                                            print_rules("call","( funcdef ) ( elist )");
                                                                            
                                                                            if($6->nargs >= $2->sym->value.funcVal->totalArgs){
                                                                                expr* func = newExpr(programmfunc_e);
                                                                                func->sym = $2->sym;
                                                                                $$ = make_call(func, $6->arguments, $6->nargs);
                                                                            }
                                                                            else{
                                                                                fprintf(stderr, "Error, too few arguments to function \"%s\", at line <%d>\n", $2->sym->value.funcVal->name, yylineno);
                                                                                exit_flag = 1;
                                                                            }
                                                                        }
            ;

callsuffix  :   normalcall  {$$ = $1; print_rules("callsuffix","normalcall");}
            |   methodcall  {$$ = $1; print_rules("callsuffix","methodcall");}
            ;

normalcall  :   OPEN_PAR elist CLOSE_PAR    {
                                                print_rules("normalcall","( elist )");
                                                $$ = (call_t*)malloc(sizeof(call_t));
                                                $$->nargs = 0;
                                                int i;
                                                for(i = 0; i < 100; i++){
                                                    $$->arguments[i] = $2->arguments[i];
                                                }
                                                $$->nargs = $2->nargs;
                                                $$->isMethod = false;                                           
                                                $$->name = NULL;
                                            }
            ;

methodcall  :   DOUBLE_DOT ID OPEN_PAR elist CLOSE_PAR  {
                                                            print_rules("methodcall","..ID ( elist )");
                                                            $$ = (call_t*)malloc(sizeof(call_t));
                                                            int i;
                                                            for(i = 0; i < 100; i++){
                                                                $$->arguments[i] = $4->arguments[i];
                                                            }
                                                            $$->nargs = $4->nargs;
                                                            $$->isMethod = true;
                                                            $$->name = strdup($2);
                                                        }
            ;

elist       :   expr        {
                                print_rules("elist","expr");

                                if($1->type == boolexpr_e)
                                    $1 = bool_expr($1);
                                $$ = (arguments_t*)malloc(sizeof(arguments_t));
                                $$->nargs = 0;
                                $$->arguments[$$->nargs++] = $1;
                            }
            |   elist COMMA expr    {
                                        print_rules("elist",", expr");
                                        if($3->type == boolexpr_e)
                                            $3 = bool_expr($3);
                                        $1->arguments[$1->nargs++] = $3;
                                    }
            |   /* empty */ {
                                $$ = (arguments_t*)malloc(sizeof(arguments_t));
                                $$->nargs = 0;
                                print_rules("elist"," ");
                            }
            ;



objectdef   :   OPEN_SUBSCRIPT elist CLOSE_SUBSCRIPT    {
                                                                print_rules("objectdef","[ elist ]");
                                                                expr *t = newExpr(newtable_e);
                                                                t->sym = newTempVar();
                                                                emit(tablecreate, t, NULL, NULL, 0, yylineno);
                                                                int i;
                                                                for(i = 0; i < $2->nargs; i++){
                                                                    expr *num_tmp = (expr*)malloc(sizeof(expr));
                                                                    num_tmp->numConst = i;
                                                                    num_tmp->type = constint_e;
                                                                    emit(tablesetelem, t, num_tmp, $2->arguments[i], 0, yylineno);
                                                                }
                                                                $$ = t;
                                                            }
            |   OPEN_SUBSCRIPT indexed CLOSE_SUBSCRIPT      {
                                                                print_rules("objectdef","[ indexed ]");
                                                                expr *t = newExpr(newtable_e);
                                                                t->sym = newTempVar();
                                                                emit(tablecreate, t, NULL, NULL, 0, yylineno);
                                                                int i;
                                                                for(i = 0; i < $2->no_of_elems; i++){
                                                                    emit(tablesetelem, t, $2->elems[i]->index, $2->elems[i]->element, 0, yylineno);
                                                                }
                                                                $$ = t;
                                                            }
            ;

indexedelem :   OPEN_BRACKET{curr_scope++;} 
                expr        {
                                if($3->type == boolexpr_e)
                                    $3 = bool_expr($3);
                            }
                COLON expr CLOSE_BRACKET    {
                                                print_rules("indexedelem","{ expr; expr }");
                                                curr_scope--;
                                                if($6->type == boolexpr_e)
                                                    $6 = bool_expr($6);

                                                $$ = (indexed_elem*)malloc(sizeof(indexed_elem));
                                                $$->index = $3;
                                                $$->element = $6;
                                                /*
                                                table_elem_index[table_elem_offset] = $2;
                                                table_elems[table_elem_offset++] = $5;
                                                */
                                            }
            ;

indexed     :   indexedelem                 {
                                                $$ = (elements*)malloc(sizeof(elements));
                                                $$->no_of_elems = 0;
                                                $$->elems[$$->no_of_elems++] = $1;
                                            }
            |   indexed COMMA indexedelem   {
                                                $1->elems[$1->no_of_elems++] = $3;
                                            }
            ;

block       :   OPEN_BRACKET    {savetmpvarOffset(curr_scope); curr_scope++;} 
                stmts 
                CLOSE_BRACKET   {
                                    print_rules("block","{ stmts }");

                                    $$ = new_stmt();

                                    if($3->breaklist != NULL)
                                        $$->breaklist = merge_list($$->breaklist, $3->breaklist, NULL);

                                    if($3->contlist != NULL)
                                        $$->contlist = merge_list($$->contlist, $3->contlist, NULL);
                                        
                                    hide(table, curr_scope);
                                    curr_scope--;
                                }
            ;

funcdef     :   funcprefix funcargs funcbody    {
                                                    exitscopespace();
                                                    $1->value.funcVal->totalLocals = functionLocalOffset;
                                                    functionLocalOffset = func_pop(functionLocalsStack);
                                                    $1->value.funcVal->totalArgs = formalArgOffset;
                                                    $$ = newExpr(programmfunc_e);
                                                    $$->sym = $1;
                                                    emit(funcend, $$, NULL, NULL, 0, yylineno);

                                                    if(retjumpQuad[functionOffset+1] != NULL)
                                                        retjumpQuad[functionOffset+1]->label = $3;

                                                    funcjumpQuad[functionOffset+1]->label = $3 + 1;

                                                    funcjumpsReset(functionOffset+1);
                                                    pop(loopstack);
                                                    $$->sym->isActive = false;
                                                }
            ;

funcprefix  :   FUNCTION funcname   {
                                        push(loopstack);
                                        functionOffset++;
                                        funcjumpQuad[functionOffset] = emit(jump, NULL, NULL, NULL, 0, yylineno);
                                        if($2 != NULL){
                                            $$ = function_lookup(table, create_function($2, curr_scope, yylineno), USERFUNC, curr_scope, false);
                                            if($$ == NULL){
                                                exit(0);
                                            }
                                        }
                                        else{
                                            $$ = newTempFuncVar();
                                        }
                                        $$->value.funcVal->funcAddress = nextQuad();    
                                        expr *func_expr = newExpr(programmfunc_e);
                                        func_expr->sym = $$;
                                        emit(funcstart, func_expr, NULL, NULL, 0, yylineno);
                                        func_push(functionLocalsStack, functionLocalOffset);
                                        enterscopespace();
                                        resetformalargsOffset();
                                        returnCnt++;
                                    }
            ;

funcargs    :   OPEN_PAR    { curr_scope++; } 
                idlist 
                CLOSE_PAR   {
                                curr_scope--;
                                enterscopespace();
                                resetfunctionlocalsOffset();
                            }
            ;

funcbody    :   block   {
                            $$ = nextQuad();
                            exitscopespace();
                            returnCnt--;
                            functionOffset--;
                        }
            ;

funcname    :   ID          {$$ = $1;}
            |   /*empty*/   {$$ = NULL;}
            ;
            
idlist      :   ID                  {
                                        print_rules("idlist","ID");
                                        formal_lookup(table, create_variable($1, curr_scope, yylineno), FORMAL, curr_scope);
                                    }
            |   idlist COMMA ID     {
                                        print_rules("idlist","idlist COMMA ID");
                                        formal_lookup(table, create_variable($3, curr_scope, yylineno), FORMAL, curr_scope);
                                    }
            |   /* empty */         {print_rules("idlist"," ");}
            ;



const       :   REAL_CONST  {
                                print_rules("const","REAL_CONST");
                                $$ = newExpr(constdouble_e);
                                $$->numConst = $1;
                            }
            |   INT_CONST   {
                                print_rules("const","INT_CONST");
                                $$ = newExpr(constint_e);
                                $$->numConst = $1;
                            }
            |   STRING      {
                                print_rules("const","STRING");
                                $$ = newExpr(conststring_e);
                                $$->strConst = $1;
                            }
            |   NIL         {
                                print_rules("const","NIL");
                                $$ = newExpr(nil_e);
                            }
            |   TRUE        {
                                print_rules("const","TRUE");
                                $$ = newExpr(constbool_e);
                                $$->boolConst = $1;
                            }
            |   FALSE       {
                                print_rules("const","FALSE");
                                $$ = newExpr(constbool_e);
                                $$->boolConst = $1;
                            }
            ;


ifprefix    :   IF OPEN_PAR expr CLOSE_PAR  {
                                                if($3->type == boolexpr_e)
                                                    $3 = bool_expr($3);                                                    
                                                
                                                expr* true_expr = newExpr(constbool_e);
                                                true_expr->boolConst = 't';
                                                
                                                emit(if_eq, NULL, $3, true_expr, nextQuad()+2 , yylineno);
                                                $$ = nextQuad();
                                                emit(jump, NULL, NULL, NULL, -1, yylineno);
                                            }
            ;
ifstmt      :   ifprefix stmt   {
                                    patchlabel($1, nextQuad());
                                    resetTempVar();
                                }
                elsestmt        {
                                    if($4->else_quad != -2){
                                        patchlabel($1, $4->else_quad + 1);
                                        patchlabel($4->else_quad, nextQuad()); 
                                    }
                                    $$ = new_stmt();
                                    $$->breaklist = merge_list($$->breaklist, $2->breaklist, $4->breaklist);
                                    $$->contlist = merge_list($$->contlist, $2->contlist, $4->contlist);
                                }
            ;
elsestmt    :   elserpefix stmt {
                                    resetTempVar();
                                    $$ = new_stmt();
                                    $$->else_quad = $1;
                                    $$->breaklist = $2->breaklist;
                                    $$->contlist = $2->contlist;
                                }
            |   /* empty */     {
                                    $$ = new_stmt();
                                    $$->else_quad = -2;
                                }
            ;

elserpefix  :   ELSE    {
                            $$ = nextQuad();
                            emit(jump, NULL, NULL, NULL, -1, yylineno);
                        }
            ;

breakst     :   BREAK SEMICOLON {
                                    $$ = (struct break_s*)malloc(sizeof(struct break_s));
                                    $$->breaklist = insert_list($$->breaklist, nextQuad());
                                    emit(jump, NULL, NULL, NULL, -1, yylineno);
                                }
            ;

continuest  :   CONTINUE SEMICOLON  {
                                        $$ = (struct cont_s*)malloc(sizeof(struct cont_s));
                                        $$->contlist = insert_list($$->contlist, nextQuad());
                                        emit(jump, NULL, NULL, NULL, -1, yylineno);
                                    }
            ;

loopstart   :   /* empty */   {}
            ;

loopend     :   /* empty */   {}
            ;

loopstmt    :   loopstart stmt loopend {$$ = $2;}
            ;

whilestart  :   WHILE   {
                            loopcounter++;
                            $$ = nextQuad();
                            if(loopcounter > 1)
                                push(loopstack);
                            modify_tail(loopstack, true);
                        }
            ;

whilecond   :   OPEN_PAR expr CLOSE_PAR {
                                            if($2->type == boolexpr_e)
                                                $2 = bool_expr($2);

                                            expr* true_expr = newExpr(constbool_e);
                                            true_expr->boolConst = 't';
                                            emit(if_eq, NULL, $2, true_expr, nextQuad()+2, yylineno);
                                            $$ = nextQuad();
                                            whileQuad = emit(jump, NULL, NULL, NULL, -1, yylineno);
                                        }
            ;

whilestmt   :   whilestart whilecond loopstmt   {
                                                    print_rules("whilestmt","while(expr) stmt");
                                                    emit(jump, NULL, NULL, NULL, $1, yylineno);
                                                    patchlabel($2, nextQuad());

                                                    struct list_s* tmp;
                                                    tmp = $3->breaklist;
                                                    while(tmp != NULL){
                                                        patchlabel(tmp->quadNo, nextQuad());
                                                        tmp = tmp->next;
                                                    }
                                                    tmp = $3->contlist;
                                                    while(tmp != NULL){
                                                        patchlabel(tmp->quadNo, $1);
                                                        tmp = tmp->next;
                                                    }
                                                    modify_tail(loopstack, false);
                                                    if(loopcounter > 1)
                                                        pop(loopstack);
                                                    loopcounter--;
                                                }
            ;

K           :    /* empty */  {$$ = nextQuad(); emit(jump, NULL, NULL, NULL, -1, yylineno);}
            ;

L           :    /* empty */  {$$ = nextQuad();}
            ;

forprefix   :   FOR OPEN_PAR elist SEMICOLON L expr SEMICOLON   {
                                                                    loopcounter++;
                                                                    if(loopcounter > 1)
                                                                        push(loopstack);
                                                                    modify_tail(loopstack, true);
                                                                    if($6->type == boolexpr_e)
                                                                        $6 = bool_expr($6);

                                                                    $$ = (struct forpr_s*)malloc(sizeof(struct forpr_s));
                                                                    expr* true_expr = newExpr(constbool_e);
                                                                    true_expr->boolConst = 't';
                                                                    $$->test = $5;
                                                                    $$->enter = nextQuad();
                                                                    emit(if_eq, NULL, $6, true_expr, -1, yylineno);
                                                                }
forstmt     :   forprefix K elist CLOSE_PAR K loopstmt K        {
                                                                    print_rules("forstmt","for(elist; expr; elist) stmt");
                                                                    patchlabel($1->enter, $5+1);
                                                                    patchlabel($2, nextQuad());
                                                                    patchlabel($5, $1->test);
                                                                    patchlabel($7, $2+1);

                                                                    struct list_s* tmp;
                                                                    tmp = $6->breaklist;
                                                                    while(tmp != NULL){
                                                                        patchlabel(tmp->quadNo, nextQuad());
                                                                        tmp = tmp->next;
                                                                    }
                                                                    tmp = $6->contlist;
                                                                    while(tmp != NULL){
                                                                        patchlabel(tmp->quadNo, $2+1);
                                                                        tmp = tmp->next;
                                                                    }
                                                                    modify_tail(loopstack, false);
                                                                    if(loopcounter > 1)
                                                                        pop(loopstack);
                                                                    loopcounter--;
                                                                }
            ;

returnstmt  :   RETURN expr SEMICOLON   {
                                            print_rules("returnstmt","return expr;");
                                            if($2->type == boolexpr_e)
                                                $2 = bool_expr($2);
                                            
                                            if(returnCnt != 0){
                                                $$ = $2;
                                                emit(ret, $2, NULL, NULL, 0, yylineno);
                                                retjumpQuad[functionOffset] = emit(jump, NULL, NULL, NULL, 0, yylineno);
                                            }
                                        }
            |   RETURN SEMICOLON    {
                                        print_rules("returnstmt","return;");
                                        if(returnCnt != 0){
                                            expr* nil = newExpr(nil_e);
                                            emit(ret, nil, NULL, NULL, 0, yylineno);
                                            retjumpQuad[functionOffset] = emit(jump, NULL, NULL, NULL, 0, yylineno);
                                        }
                                    }
            ;

%%

int getCurrScope(){return curr_scope;}

int getCurrLine(){return yylineno;}

void print_rules(char* rule1, char* rule2){
    size_t length = strlen(rule1) + 4 + strlen(rule2) + 1 + 1;
    char* final_rule = (char *)malloc(length*sizeof(char));
    memset(final_rule,'0',length);

    strcpy(final_rule, rule1);
    strcat(final_rule, " -> ");
    strcat(final_rule, rule2);
    strcat(final_rule, "\n");

    fputs(final_rule, rules_out);
    free(final_rule);
}

void print_error(error_t type, char* msg){
    if(type == error)
        fprintf(ERROR_OUT, "\033[0;31mError:\033[0m %s\n", msg);
    else if(type == warning)
        fprintf(ERROR_OUT, "\033[0;35mWarning:\033[0m %s\n", msg);
}

struct func_local_stack* create_func_stack(){
    struct func_local_stack* ret = (struct func_local_stack*)malloc(sizeof(struct func_local_stack));
    ret->head = NULL;
    ret->tail = NULL;
    ret->nodes = 0;

    return ret;
}

void func_push(struct func_local_stack* s, int cur_local_offset){
    struct func_node* new = (struct func_node *)malloc(sizeof(struct func_node));
    new->local_offset = cur_local_offset;
    new->next = NULL;
    
    if(s->head == NULL){
        s->head = new;
        s->tail = new;
    }   
    else{
        s->tail->next = new;
        s->tail = new;
    }
    s->nodes++;
}

int func_pop(struct func_local_stack* s){
    int ret = s->tail->local_offset;
    struct func_node* tmp = s->head;

    if(s->nodes == 1){
        free(s->head);
        s->head = NULL;
        s->tail = NULL;
        s->nodes--;
        return ret;
    }

    while(tmp->next != s->tail){
        tmp = tmp->next;
    }
    s->tail = tmp;
    free(tmp->next);
    s->tail->next = NULL;
    s->nodes--;
    return ret;
}

int func_top(struct func_local_stack* s){
    return s->tail->local_offset;
}


void funcjumpsReset(int offset){
    funcjumpQuad[offset] = NULL;
    retjumpQuad[offset] = NULL;
}

int main(int argc, char *argv[]){
    table = create_table();
    loopstack = new_stack();
    functionLocalsStack = create_func_stack();

    if(argv[1] != NULL)
        yyin = fopen(argv[1], "r");

    rules_out = fopen("output/rules.out","w+");
    sym_sc_out = fopen("output/symtab_scopes.out","w+");
    sym_buc_out = fopen("output/symtab_buckets.out","w+");
    quad_out = fopen("output/quads.aic", "w+");
    target_out = fopen("output/target_code.atc", "w+");
    binary_out = fopen("alpha_binary.abc", "wb");

    push(loopstack);

    yyparse();
    patch_return_jumps();
    fclose(rules_out);

    if(exit_flag == 1){
        fprintf(stderr, "Intermediate Code generation failed\n");
        print_table_scopes(table, sym_sc_out);
        print_table_buckets(table, sym_buc_out);
        exit(1);
    }

    generate_tc();

    print_table_buckets(table, sym_buc_out);
    fclose(sym_buc_out);

    print_table_scopes(table, sym_sc_out);
    fclose(sym_sc_out);

    printQuads(quad_out);
    fclose(quad_out);

    print_tc(target_out);
    fclose(target_out);

    print_bc(binary_out);
    fclose(binary_out);
 

    return 1;
}