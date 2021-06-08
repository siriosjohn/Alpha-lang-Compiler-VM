/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SRC_PARSER_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_CONST = 258,
    REAL_CONST = 259,
    ID = 260,
    KEYWORD = 261,
    OPERATOR = 262,
    PUNCTATION = 263,
    STRING = 264,
    TRUE = 265,
    FALSE = 266,
    AND = 267,
    OR = 268,
    NOT = 269,
    ASSIGN = 270,
    PLUS = 271,
    MINUS = 272,
    MULTIPLY = 273,
    DIVIDE = 274,
    MOD = 275,
    GREATER = 276,
    GREATER_EQ = 277,
    LESS = 278,
    LESS_EQ = 279,
    EQ = 280,
    NOT_EQ = 281,
    UMINUS = 282,
    INCREMENT = 283,
    DECREMENT = 284,
    OPEN_BRACKET = 285,
    CLOSE_BRACKET = 286,
    OPEN_SUBSCRIPT = 287,
    CLOSE_SUBSCRIPT = 288,
    OPEN_PAR = 289,
    CLOSE_PAR = 290,
    SEMICOLON = 291,
    COMMA = 292,
    COLON = 293,
    DOUBLE_COLON = 294,
    DOUBLE_DOT = 295,
    DOT = 296,
    WHILE = 297,
    FOR = 298,
    BREAK = 299,
    CONTINUE = 300,
    RETURN = 301,
    IF = 302,
    ELSE = 303,
    LOCAL = 304,
    NIL = 305,
    FUNCTION = 306,
    END_OF_FILE = 307,
    INF_COMMENT = 308,
    INF_STRING = 309
  };
#endif
/* Tokens.  */
#define INT_CONST 258
#define REAL_CONST 259
#define ID 260
#define KEYWORD 261
#define OPERATOR 262
#define PUNCTATION 263
#define STRING 264
#define TRUE 265
#define FALSE 266
#define AND 267
#define OR 268
#define NOT 269
#define ASSIGN 270
#define PLUS 271
#define MINUS 272
#define MULTIPLY 273
#define DIVIDE 274
#define MOD 275
#define GREATER 276
#define GREATER_EQ 277
#define LESS 278
#define LESS_EQ 279
#define EQ 280
#define NOT_EQ 281
#define UMINUS 282
#define INCREMENT 283
#define DECREMENT 284
#define OPEN_BRACKET 285
#define CLOSE_BRACKET 286
#define OPEN_SUBSCRIPT 287
#define CLOSE_SUBSCRIPT 288
#define OPEN_PAR 289
#define CLOSE_PAR 290
#define SEMICOLON 291
#define COMMA 292
#define COLON 293
#define DOUBLE_COLON 294
#define DOUBLE_DOT 295
#define DOT 296
#define WHILE 297
#define FOR 298
#define BREAK 299
#define CONTINUE 300
#define RETURN 301
#define IF 302
#define ELSE 303
#define LOCAL 304
#define NIL 305
#define FUNCTION 306
#define END_OF_FILE 307
#define INF_COMMENT 308
#define INF_STRING 309

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 107 "src/parser/parser.y"

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

#line 183 "src/parser/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_PARSER_H_INCLUDED  */
