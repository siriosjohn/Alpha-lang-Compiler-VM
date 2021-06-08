%{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../icode/quads.h"
#include "../parser/parser.h"

void yyerror (char *s);

int no_of_tokens = 1;

int embedded_comments = 0;
int comment_starting_line = 0;
int comment_finish_line = 0;

typedef struct alpha_token{
    unsigned int line_no;
    unsigned int token_no;
    char *value;
    char *category;

    struct alpha_token *next;

}alpha_token_t; 

void remove_quotes(char*);
void fix_esc_char(char*);

alpha_token_t *head_ptr;
alpha_token_t* store_token(alpha_token_t *head, char *cat);
%}

%option yylineno
%option noyywrap

NEWLINE         [\n]|[\r\n]

IF              if
ELSE            else
WHILE           while 
FOR             for 
FUNCTION        function 
RETURN          return 
BREAK           break 
CONTINUE        continue 
AND             and 
OR              or 
NOT             not 
LOCAL           local 
TRUE            true 
FALSE           false 
NIL             nil 

ASSIGN          "="
PLUS            "+"
MINUS           "-"
MULTIPLY        "*"
DIVIDE          "/"
MOD             "%"
EQ              "=="
NOT_EQ          "!="
INCREMENT       "++"
DECREMENT       "--"
LESS            "<"
GREATER         ">"
GREATER_EQ      ">="
LOWER_EQ        "<="

INT_CONST       [0-9][0-9]*
REAL_CONST      [0-9]*\.[0-9][0-9]*

OPEN_BRACKET    "{"
CLOSE_BRACKET   "}"
OPEN_SUBSCRIPT  "["
CLOSE_SUBSCRIPT "]"
OPEN_PAR        "("
CLOSE_PAR       ")"
SEMICOLON       ";"
COMMA           ","
COLON           ":"
DOUBLE_COLON    "::"
DOUBLE_DOT      ".." 
DOT             "."

ID              [a-zA-Z][a-zA-Z_0-9]*

STRING          \"(\\.|[^"\\])*\"
STERR           (\"[^\n\"]?+)

COMMENT2        "//"*


%x  BLOCK_COMMENT
%x  LINE_COMMENT


%%


"/*"                {comment_starting_line = yylineno; BEGIN(BLOCK_COMMENT);}
<BLOCK_COMMENT>{
.                   {;}
<<EOF>>             {fprintf(stderr, "Error, non-terminated comment block\n"); return INF_COMMENT;}
{NEWLINE}           {}
"/*"                {embedded_comments++;}
"*"+"/"             {
                        if(embedded_comments != 0) 
                            embedded_comments--; 
                        else {
                            comment_finish_line = yylineno;
                            head_ptr = store_token(head_ptr, "BLOCK_COMMENT");
                            BEGIN(INITIAL);
                        }
                    }
}

<LINE_COMMENT>{
.                   {;}
{NEWLINE}           {BEGIN(INITIAL);}
}



{NEWLINE}           {}

{COMMENT2}          {BEGIN(LINE_COMMENT);}

{IF}                {return IF;}
{ELSE}              {return ELSE;}
{WHILE}             {return WHILE;}
{FOR}               {return FOR;}
{FUNCTION}          {return FUNCTION;}
{RETURN}            {return RETURN;}
{BREAK}             {return BREAK;}
{CONTINUE}          {return CONTINUE;}
{AND}               {return AND;}
{OR}                {return OR;}
{NOT}               {return NOT;}
{LOCAL}             {return LOCAL;}
{TRUE}              {yylval.character = 't'; return TRUE;}
{FALSE}             {yylval.character = 'f'; return FALSE;}
{NIL}               {return NIL;}

{ASSIGN}            {return ASSIGN;}
{PLUS}              {return PLUS;}
{MINUS}             {return MINUS;}
{MULTIPLY}          {return MULTIPLY;}
{DIVIDE}            {return DIVIDE;}
{MOD}               {return MOD;}
{EQ}                {return EQ;}
{NOT_EQ}            {return NOT_EQ;}
{INCREMENT}         {return INCREMENT;}
{DECREMENT}         {return DECREMENT;}
{LESS}              {return LESS;}
{GREATER}           {return GREATER;}
{GREATER_EQ}        {return GREATER_EQ;}
{LOWER_EQ}          {return LESS_EQ;}

{INT_CONST}         {yylval.num = atoi(yytext); return INT_CONST;}
{REAL_CONST}        {yylval.num = atof(yytext); return REAL_CONST;}

{OPEN_BRACKET}      {return OPEN_BRACKET;}
{CLOSE_BRACKET}     {return CLOSE_BRACKET;}
{OPEN_SUBSCRIPT}    {return OPEN_SUBSCRIPT;}
{CLOSE_SUBSCRIPT}   {return CLOSE_SUBSCRIPT;}
{OPEN_PAR}          {return OPEN_PAR;}
{CLOSE_PAR}         {return CLOSE_PAR;}
{SEMICOLON}         {return SEMICOLON;}
{COMMA}             {return COMMA;}
{COLON}             {return COLON;}
{DOUBLE_COLON}      {return DOUBLE_COLON;}
{DOUBLE_DOT}        {return DOUBLE_DOT;}
{DOT}               {return DOT;}

{ID}                {yylval.str = strdup(yytext); return ID;}

[ \t]               {;}
[^ \t]              {fprintf(stderr, "Warning, line %d: Undefined input: %s\n", yylineno, yytext);}

{STRING}            {char* str = strdup(yytext);  remove_quotes(str); fix_esc_char(str); yylval.str = strdup(str); return STRING;}
{STERR}             {fprintf(stderr, "ERROR, line %d: Non terminated String\n", yylineno); return INF_STRING;}


%%


void remove_quotes(char* ptr){
    char *tmp = ptr;
    char *next = ptr + 1;
    char *last = ptr + strlen(ptr) - 1;

    *last = '\0';

    while(*tmp != '\0'){
        *tmp = *next;
        tmp++;
        next++;
    }
}

void fix_esc_char(char* ptr){
    char *tmp;
    char *next_char;
    while(*ptr != '\0'){
        if(*ptr == '\\'){
            tmp = ptr;
            next_char = ptr + 1;
            if(*next_char == 'n')
                *ptr = '\n';

            else if(*next_char == 't')
                *ptr = '\t';
            
            else if(*next_char == '\\')
                *ptr = '\\';
            
            else if(*next_char == '\"')
                *ptr = '\"';
            
            else{
                fprintf(stderr, "Warning, string at line %d contains unknown escaped sequence: \'\\%c\'.\n", yylineno, *next_char);
                *ptr = *(ptr + 1);
            }

            tmp++;
            next_char++;
            while(*tmp != '\0'){
                *tmp = *next_char;
                tmp++;
                next_char++;
            }
            
        }
        
        ptr++;
    }
}

alpha_token_t* store_token(alpha_token_t *head, char *cat){

    char from[3];
    char to[3];
    char whole[10];

    alpha_token_t *new = (alpha_token_t *)malloc(sizeof(alpha_token_t));
    alpha_token_t *tmp = head;

    new->line_no = yylineno;
    new->token_no = no_of_tokens;
    
    if(strcmp(cat, "STRING") == 0){
        remove_quotes(yytext);
        fix_esc_char(yytext);
        new->value = strdup(yytext);
    }

    else if(strcmp(cat, "BLOCK_COMMENT") == 0){
        memset(from,0,strlen(from));
        memset(to,0,strlen(to));
        memset(whole,0,strlen(whole));

        sprintf(from, "%d", comment_starting_line);
        sprintf(to, "%d", comment_finish_line);

        strcat(whole, from);
        strcat(whole, " - ");
        strcat(whole, to);


        new->value = strdup(whole);
    }
        
    
    else if(strcmp(cat, "LINE_COMMENT") == 0)
        new->value = strdup("");
    
    else
        new->value = strdup(yytext);
    
    
    new->category = strdup(cat);

    if(head == NULL){
        head = new;
    }
    else{
        while(tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = new;
    }

    no_of_tokens++;

    return head;
}

void yyerror (char *s) {
    fprintf (stderr, "%s\n", s);
} 