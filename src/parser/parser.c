/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser/parser.y"

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


#line 176 "src/parser/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 354 "src/parser/parser.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_PARSER_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   557

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  198

#define YYUNDEFTOK  2
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   178,   178,   181,   189,   195,   199,   203,   212,   222,
     232,   238,   242,   248,   256,   259,   262,   266,   266,   276,
     276,   287,   287,   298,   298,   309,   313,   317,   321,   326,
     330,   334,   338,   342,   346,   352,   356,   366,   374,   412,
     452,   487,   525,   529,   528,   573,   577,   581,   585,   590,
     596,   620,   628,   656,   662,   667,   675,   679,   689,   705,
     731,   731,   746,   747,   750,   764,   777,   786,   792,   801,
     815,   828,   829,   828,   849,   854,   859,   859,   877,   897,
     921,   921,   930,   938,   939,   942,   946,   950,   955,   960,
     965,   970,   974,   979,   987,   999,   999,  1013,  1020,  1026,
    1032,  1039,  1046,  1049,  1052,  1055,  1064,  1076,  1099,  1102,
    1105,  1120,  1145,  1156
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_CONST", "REAL_CONST", "ID",
  "KEYWORD", "OPERATOR", "PUNCTATION", "STRING", "TRUE", "FALSE", "AND",
  "OR", "NOT", "ASSIGN", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MOD",
  "GREATER", "GREATER_EQ", "LESS", "LESS_EQ", "EQ", "NOT_EQ", "UMINUS",
  "INCREMENT", "DECREMENT", "OPEN_BRACKET", "CLOSE_BRACKET",
  "OPEN_SUBSCRIPT", "CLOSE_SUBSCRIPT", "OPEN_PAR", "CLOSE_PAR",
  "SEMICOLON", "COMMA", "COLON", "DOUBLE_COLON", "DOUBLE_DOT", "DOT",
  "WHILE", "FOR", "BREAK", "CONTINUE", "RETURN", "IF", "ELSE", "LOCAL",
  "NIL", "FUNCTION", "END_OF_FILE", "INF_COMMENT", "INF_STRING", "$accept",
  "program", "stmt", "stmts", "M", "expr", "$@1", "$@2", "$@3", "$@4",
  "term", "assignexpr", "$@5", "primary", "lvalue", "member", "call",
  "$@6", "callsuffix", "normalcall", "methodcall", "elist", "objectdef",
  "indexedelem", "$@7", "$@8", "indexed", "block", "$@9", "funcdef",
  "funcprefix", "funcargs", "$@10", "funcbody", "funcname", "idlist",
  "const", "ifprefix", "ifstmt", "$@11", "elsestmt", "elserpefix",
  "breakst", "continuest", "loopstart", "loopend", "loopstmt",
  "whilestart", "whilecond", "whilestmt", "K", "L", "forprefix", "forstmt",
  "returnstmt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309
};
# endif

#define YYPACT_NINF (-171)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-61)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -171,    10,   161,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
     284,   284,    20,    20,  -171,   210,   110,  -171,    -3,  -171,
     -16,    -7,     1,   247,    -8,    35,  -171,    38,  -171,   319,
    -171,  -171,  -171,   194,  -171,    31,  -171,  -171,  -171,    16,
    -171,   161,  -171,  -171,  -171,    21,  -171,  -171,  -171,  -171,
    -171,  -171,    22,    30,    31,    30,  -171,  -171,   494,   -10,
    -171,    50,   382,    70,  -171,   284,  -171,  -171,  -171,   340,
     284,  -171,  -171,  -171,  -171,  -171,   284,   284,   284,   284,
     284,   284,   284,   284,   284,  -171,  -171,  -171,  -171,  -171,
    -171,   284,   284,    96,   105,  -171,  -171,  -171,   284,   284,
     107,  -171,    86,  -171,   284,  -171,   284,    82,     2,   284,
    -171,   284,  -171,    88,  -171,    89,    24,  -171,   402,  -171,
    -171,    66,    66,  -171,  -171,  -171,   249,   249,   249,   249,
    -171,  -171,   284,   442,   -20,    92,  -171,   460,    53,  -171,
     117,  -171,  -171,    77,   422,   161,  -171,    56,  -171,  -171,
     494,   494,  -171,    94,  -171,  -171,   284,   284,   284,   284,
     494,  -171,  -171,   284,  -171,  -171,  -171,    57,  -171,  -171,
     161,  -171,  -171,  -171,    93,   284,   284,   520,   509,   531,
     531,    60,  -171,   124,  -171,  -171,  -171,   284,    63,   361,
    -171,  -171,  -171,   478,  -171,  -171,  -171,  -171
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      14,     0,     2,     1,    89,    88,    50,    90,    92,    93,
       0,     0,     0,     0,    76,    68,     0,    12,     0,   105,
       0,     0,     0,     0,     0,     0,    91,    84,    13,     0,
      34,    16,    42,    45,    53,    46,    47,    10,    11,     0,
      49,     0,     4,     8,     9,     0,     5,   108,     6,     7,
      37,    36,     0,    38,     0,    40,    14,    71,    66,     0,
      74,     0,     0,     0,    52,    68,   100,   101,   113,     0,
       0,    51,    83,    79,    19,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,    15,     3,    43,    39,
      41,     0,    68,     0,     0,    59,    62,    63,     0,    68,
       0,    80,     0,    95,     0,   102,    68,     0,     0,     0,
      69,     0,    70,     0,    35,    48,     0,   112,     0,    15,
      15,    29,    30,    31,    32,    33,    25,    26,    27,    28,
      21,    23,     0,     0,     0,     0,    54,     0,     0,    56,
      87,    82,    78,    98,     0,     0,   107,     0,    60,    77,
      72,    67,    75,     0,   109,    94,     0,     0,     0,     0,
      44,    55,    64,    68,    57,    58,    85,     0,    99,    96,
       0,   106,   103,   108,     0,    68,     0,    20,    18,    22,
      24,     0,    81,     0,    97,   104,   102,     0,     0,     0,
      65,    86,   108,     0,    61,   110,   111,    73
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -171,  -171,   -37,    76,   -62,    -2,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,    54,  -171,    91,  -171,  -171,  -171,
    -171,   -64,  -171,    23,  -171,  -171,  -171,    32,  -171,     4,
    -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,  -171,  -171,   -51,  -171,  -171,  -171,
    -170,  -171,  -171,  -171,  -171
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    28,     2,   130,    58,   120,   119,   158,   159,
      30,    31,   132,    32,    33,    34,    35,   153,    95,    96,
      97,    59,    36,    60,   109,   174,    61,    37,    56,    38,
      39,   102,   140,   142,    73,   167,    40,    41,    42,   143,
     169,   170,    43,    44,   145,   185,   146,    45,   105,    46,
     106,   176,    47,    48,    49
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      29,   116,    64,   186,   103,     4,     5,     6,    50,    51,
       3,     7,     8,     9,    62,   162,    10,   111,    65,    11,
      63,    69,   196,   110,   131,     6,    70,   111,   134,    66,
      12,    13,    14,   149,    15,   138,    16,    67,    17,    29,
      71,    18,   147,    72,    19,    20,    21,    22,    23,    24,
     101,    25,    26,    27,    52,   104,   107,   156,   157,    18,
     154,   111,    91,    98,    92,    99,    53,    55,   118,    25,
      93,    94,   100,    27,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   112,    78,    79,    80,   113,   165,   133,
     111,   173,   182,   111,   183,   190,   137,   111,   194,   181,
     111,   135,   144,    54,    54,   115,    29,   150,   172,   151,
     136,   188,   139,     4,     5,     6,    14,   148,    57,     7,
       8,     9,   166,   -60,    10,   168,   163,    11,   175,   191,
     160,   187,   108,   184,   141,   192,   152,     0,    12,    13,
       0,     0,    15,    29,    16,     0,     0,     0,     0,    18,
       0,     0,     0,     0,   177,   178,   179,   180,     0,    25,
      26,    27,     0,     0,     4,     5,     6,     0,    29,     0,
       7,     8,     9,     0,   189,    10,     0,     0,    11,     0,
       0,     0,     0,     0,     0,   193,     0,     0,     0,    12,
      13,    14,     0,    15,     0,    16,     0,    17,     0,     0,
      18,     0,     0,    19,    20,    21,    22,    23,    24,    88,
      25,    26,    27,     4,     5,     6,     0,     0,     0,     7,
       8,     9,    89,    90,    10,     0,    91,    11,    92,     0,
       0,     0,     0,     0,    93,    94,     0,     0,    12,    13,
      57,     0,    15,     0,    16,     0,     0,     0,     0,    18,
       4,     5,     6,     0,     0,     0,     7,     8,     9,    25,
      26,    10,     0,     0,    11,    76,    77,    78,    79,    80,
     -61,   -61,   -61,   -61,     0,    12,    13,     0,     0,    15,
       0,    16,     0,    68,     0,     0,    18,     4,     5,     6,
       0,     0,     0,     7,     8,     9,    25,    26,    10,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    12,    13,     0,     0,    15,     0,    16,     0,
       0,     0,     0,    18,     0,     0,     0,     0,     0,     0,
       0,    74,    75,    25,    26,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,     0,     0,     0,     0,
       0,     0,    74,    75,     0,    87,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,     0,    74,    75,     0,   117,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,     0,     0,
       0,     0,     0,     0,    74,    75,     0,   195,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,    74,    75,     0,   114,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,    74,    75,     0,   155,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,    74,    75,     0,   171,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,     0,    74,    75,     0,   161,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     0,     0,     0,
      74,    75,     0,   164,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,    74,    75,     0,   197,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    74,     0,     0,     0,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    76,    77,    78,
      79,    80,    81,    82,    83,    84,   -61,   -61
};

static const yytype_int16 yycheck[] =
{
       2,    65,     5,   173,    41,     3,     4,     5,    10,    11,
       0,     9,    10,    11,    16,    35,    14,    37,    34,    17,
      16,    23,   192,    33,    86,     5,    34,    37,    92,    36,
      28,    29,    30,    31,    32,    99,    34,    36,    36,    41,
       5,    39,   106,     5,    42,    43,    44,    45,    46,    47,
      34,    49,    50,    51,    34,    34,    52,   119,   120,    39,
      36,    37,    32,    32,    34,    34,    12,    13,    70,    49,
      40,    41,    41,    51,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    33,    18,    19,    20,    37,    35,    91,
      37,    35,    35,    37,    37,    35,    98,    37,    35,   163,
      37,     5,   104,    12,    13,    35,   108,   109,   145,   111,
       5,   175,     5,     3,     4,     5,    30,    35,    30,     9,
      10,    11,     5,    34,    14,    48,    34,    17,    34,     5,
     132,    38,    56,   170,   102,   186,   113,    -1,    28,    29,
      -1,    -1,    32,   145,    34,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   156,   157,   158,   159,    -1,    49,
      50,    51,    -1,    -1,     3,     4,     5,    -1,   170,    -1,
       9,    10,    11,    -1,   176,    14,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    28,
      29,    30,    -1,    32,    -1,    34,    -1,    36,    -1,    -1,
      39,    -1,    -1,    42,    43,    44,    45,    46,    47,    15,
      49,    50,    51,     3,     4,     5,    -1,    -1,    -1,     9,
      10,    11,    28,    29,    14,    -1,    32,    17,    34,    -1,
      -1,    -1,    -1,    -1,    40,    41,    -1,    -1,    28,    29,
      30,    -1,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,
       3,     4,     5,    -1,    -1,    -1,     9,    10,    11,    49,
      50,    14,    -1,    -1,    17,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    28,    29,    -1,    -1,    32,
      -1,    34,    -1,    36,    -1,    -1,    39,     3,     4,     5,
      -1,    -1,    -1,     9,    10,    11,    49,    50,    14,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    49,    50,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    -1,    36,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    -1,    36,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    36,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    35,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    35,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    35,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    12,    13,    -1,    33,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      12,    13,    -1,    33,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    12,    13,    -1,    31,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    12,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    56,    58,     0,     3,     4,     5,     9,    10,    11,
      14,    17,    28,    29,    30,    32,    34,    36,    39,    42,
      43,    44,    45,    46,    47,    49,    50,    51,    57,    60,
      65,    66,    68,    69,    70,    71,    77,    82,    84,    85,
      91,    92,    93,    97,    98,   102,   104,   107,   108,   109,
      60,    60,    34,    69,    71,    69,    83,    30,    60,    76,
      78,    81,    60,    84,     5,    34,    36,    36,    36,    60,
      34,     5,     5,    89,    12,    13,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    36,    15,    28,
      29,    32,    34,    40,    41,    73,    74,    75,    32,    34,
      41,    34,    86,    57,    34,   103,   105,    84,    58,    79,
      33,    37,    33,    37,    35,    35,    76,    36,    60,    62,
      61,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      59,    59,    67,    60,    76,     5,     5,    60,    76,     5,
      87,    82,    88,    94,    60,    99,   101,    76,    35,    31,
      60,    60,    78,    72,    36,    35,    59,    59,    63,    64,
      60,    33,    35,    34,    33,    35,     5,    90,    48,    95,
      96,    35,    57,    35,    80,    34,   106,    60,    60,    60,
      60,    76,    35,    37,    57,   100,   105,    38,    76,    60,
      35,     5,   101,    60,    35,    36,   105,    31
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    59,    60,    61,    60,    62,
      60,    63,    60,    64,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    65,    65,    65,    65,    65,
      65,    65,    65,    67,    66,    68,    68,    68,    68,    68,
      69,    69,    69,    69,    70,    70,    70,    70,    71,    71,
      72,    71,    73,    73,    74,    75,    76,    76,    76,    77,
      77,    79,    80,    78,    81,    81,    83,    82,    84,    85,
      87,    86,    88,    89,    89,    90,    90,    90,    91,    91,
      91,    91,    91,    91,    92,    94,    93,    95,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   109
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     0,     1,     0,     5,     0,
       5,     0,     5,     0,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     2,     2,     2,     2,
       2,     2,     1,     0,     4,     1,     1,     1,     3,     1,
       1,     2,     2,     1,     3,     4,     3,     4,     4,     2,
       0,     7,     1,     1,     3,     5,     1,     3,     0,     3,
       3,     0,     0,     7,     1,     3,     0,     4,     3,     2,
       0,     4,     1,     1,     0,     1,     3,     0,     1,     1,
       1,     1,     1,     1,     4,     0,     4,     2,     0,     1,
       2,     2,     0,     0,     3,     1,     3,     3,     0,     0,
       7,     7,     3,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 178 "src/parser/parser.y"
                                        {print_rules("program","stmts");}
#line 1752 "src/parser/parser.c"
    break;

  case 3:
#line 181 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","expr;");
                                            if((yyvsp[-1].exp)->type == boolexpr_e)
                                                (yyvsp[-1].exp) = bool_expr((yyvsp[-1].exp));
                                            (yyval.stmt_t) = new_stmt();
                                            (yyval.stmt_t)->exp = (yyvsp[-1].exp);
                                            
                                        }
#line 1765 "src/parser/parser.c"
    break;

  case 4:
#line 189 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","ifstmt");
                                            (yyval.stmt_t) = new_stmt();
                                            (yyval.stmt_t)->breaklist = (yyvsp[0].stmt_t)->breaklist; 
                                            (yyval.stmt_t)->contlist = (yyvsp[0].stmt_t)->contlist;
                                        }
#line 1776 "src/parser/parser.c"
    break;

  case 5:
#line 195 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","whilestmt");
                                            (yyval.stmt_t) = new_stmt();
                                        }
#line 1785 "src/parser/parser.c"
    break;

  case 6:
#line 199 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","forstmt");
                                            (yyval.stmt_t) = new_stmt(); 
                                        }
#line 1794 "src/parser/parser.c"
    break;

  case 7:
#line 203 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","returnstmt;");
                                            (yyval.stmt_t) = new_stmt();
                                            (yyval.stmt_t)->exp = (yyvsp[0].exp);
                                            if(returnCnt == 0){
                                                fprintf(stderr, "Error, return statement at line <%d>, outside of function\n", yylineno);
                                                exit_flag = 1;
                                            }
                                        }
#line 1808 "src/parser/parser.c"
    break;

  case 8:
#line 212 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","break;");
                                            (yyval.stmt_t) = new_stmt();
                                            (yyval.stmt_t)->breaklist = (yyvsp[0].break_t)->breaklist;                                            
                                            if(loop_top(loopstack) == false){
                                                fprintf(stderr, "Error, break statement at line <%d>, outside of loop\n", yylineno);
                                                exit_flag = 1;
                                            }
                                                
                                        }
#line 1823 "src/parser/parser.c"
    break;

  case 9:
#line 222 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","continue;");
                                            (yyval.stmt_t) = new_stmt();
                                            (yyval.stmt_t)->contlist = (yyvsp[0].cont_t)->contlist;
                                            if(loop_top(loopstack) == false){
                                                fprintf(stderr, "Error, continue statement at line <%d>, outside of loop\n", yylineno);
                                                exit_flag = 1;
                                            }
                                                
                                        }
#line 1838 "src/parser/parser.c"
    break;

  case 10:
#line 232 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","block");
                                            (yyval.stmt_t) = new_stmt(); 
                                            (yyval.stmt_t)->breaklist = (yyvsp[0].stmt_t)->breaklist; 
                                            (yyval.stmt_t)->contlist = (yyvsp[0].stmt_t)->contlist; 
                                        }
#line 1849 "src/parser/parser.c"
    break;

  case 11:
#line 238 "src/parser/parser.y"
                                        {
                                            print_rules("stmt","funcdef;");
                                            (yyval.stmt_t) = new_stmt();
                                        }
#line 1858 "src/parser/parser.c"
    break;

  case 12:
#line 242 "src/parser/parser.y"
                                        {
                                            print_rules("stmt",";");
                                            (yyval.stmt_t) = new_stmt(); 
                                        }
#line 1867 "src/parser/parser.c"
    break;

  case 13:
#line 248 "src/parser/parser.y"
                            {
                                print_rules("stmt","stmts stmt");
                                (yyval.stmt_t) = new_stmt();
                                (yyval.stmt_t)->breaklist = merge_list((yyval.stmt_t)->breaklist, (yyvsp[-1].stmt_t)->breaklist, (yyvsp[0].stmt_t)->breaklist);
                                (yyval.stmt_t)->contlist = merge_list((yyval.stmt_t)->contlist, (yyvsp[-1].stmt_t)->contlist, (yyvsp[0].stmt_t)->contlist);
                                /*hide_tmp_var(table, curr_scope);*/
                                resetTempVar();
                            }
#line 1880 "src/parser/parser.c"
    break;

  case 14:
#line 256 "src/parser/parser.y"
                                {(yyval.stmt_t) = new_stmt();}
#line 1886 "src/parser/parser.c"
    break;

  case 15:
#line 259 "src/parser/parser.y"
                                {(yyval.integer) = nextQuad();}
#line 1892 "src/parser/parser.c"
    break;

  case 16:
#line 262 "src/parser/parser.y"
                            {
                                print_rules("expr","assignexpr");
                                (yyval.exp) = (yyvsp[0].exp);
                            }
#line 1901 "src/parser/parser.c"
    break;

  case 17:
#line 266 "src/parser/parser.y"
                        {(yyvsp[-1].exp) = boolQuads((yyvsp[-1].exp));}
#line 1907 "src/parser/parser.c"
    break;

  case 18:
#line 267 "src/parser/parser.y"
                                                    {
                                                        print_rules("expr","expr or expr");
                                                        (yyvsp[0].exp) = boolQuads((yyvsp[0].exp));

                                                        (yyval.exp) = newExpr(boolexpr_e);
                                                        backpatch((yyvsp[-4].exp)->falselist_head, (yyvsp[-1].integer));
                                                        (yyval.exp)->truelist_head = merge_list((yyval.exp)->truelist_head, (yyvsp[-4].exp)->truelist_head, (yyvsp[0].exp)->truelist_head);
                                                        (yyval.exp)->falselist_head = (yyvsp[0].exp)->falselist_head;
                                                    }
#line 1921 "src/parser/parser.c"
    break;

  case 19:
#line 276 "src/parser/parser.y"
                         {(yyvsp[-1].exp) = boolQuads((yyvsp[-1].exp));}
#line 1927 "src/parser/parser.c"
    break;

  case 20:
#line 277 "src/parser/parser.y"
                                                    {
                                                        print_rules("expr","expr and expr");
                                                        
                                                        (yyvsp[0].exp) = boolQuads((yyvsp[0].exp));

                                                        (yyval.exp) = newExpr(boolexpr_e);
                                                        backpatch((yyvsp[-4].exp)->truelist_head, (yyvsp[-1].integer));
                                                        (yyval.exp)->truelist_head = (yyvsp[0].exp)->truelist_head;
                                                        (yyval.exp)->falselist_head = merge_list((yyval.exp)->falselist_head, (yyvsp[-4].exp)->falselist_head, (yyvsp[0].exp)->falselist_head);
                                                    }
#line 1942 "src/parser/parser.c"
    break;

  case 21:
#line 287 "src/parser/parser.y"
                            {
                                if((yyvsp[-2].exp)->type == boolexpr_e)
                                    (yyvsp[-2].exp) = bool_expr((yyvsp[-2].exp));
                            }
#line 1951 "src/parser/parser.c"
    break;

  case 22:
#line 291 "src/parser/parser.y"
                            {
                                print_rules("expr","expr == expr");
                                if((yyvsp[0].exp)->type == boolexpr_e)
                                    (yyvsp[0].exp) = bool_expr((yyvsp[0].exp));
                 
                                (yyval.exp) = releational_ops((yyvsp[-4].exp), if_eq, (yyvsp[0].exp));  
                            }
#line 1963 "src/parser/parser.c"
    break;

  case 23:
#line 298 "src/parser/parser.y"
                                {
                                    if((yyvsp[-2].exp)->type == boolexpr_e)
                                        (yyvsp[-2].exp) = bool_expr((yyvsp[-2].exp));
                                }
#line 1972 "src/parser/parser.c"
    break;

  case 24:
#line 302 "src/parser/parser.y"
                            {
                                print_rules("expr","expr == expr");
                                if((yyvsp[0].exp)->type == boolexpr_e)
                                    (yyvsp[0].exp) = bool_expr((yyvsp[0].exp));
                                
                                (yyval.exp) = releational_ops((yyvsp[-4].exp), if_noteq, (yyvsp[0].exp));  
                            }
#line 1984 "src/parser/parser.c"
    break;

  case 25:
#line 309 "src/parser/parser.y"
                                        {
                                            print_rules("expr","expr > expr");
                                            (yyval.exp) = releational_ops((yyvsp[-2].exp), if_greater, (yyvsp[0].exp));  
                                        }
#line 1993 "src/parser/parser.c"
    break;

  case 26:
#line 313 "src/parser/parser.y"
                                        {
                                            print_rules("expr","expr >= expr");
                                            (yyval.exp) = releational_ops((yyvsp[-2].exp), if_greatereq, (yyvsp[0].exp)); 
                                        }
#line 2002 "src/parser/parser.c"
    break;

  case 27:
#line 317 "src/parser/parser.y"
                                        {
                                            print_rules("expr","expr < expr");
                                            (yyval.exp) = releational_ops((yyvsp[-2].exp), if_less, (yyvsp[0].exp));
                                        }
#line 2011 "src/parser/parser.c"
    break;

  case 28:
#line 321 "src/parser/parser.y"
                                        {
                                            print_rules("expr","expr <= expr");
                                            (yyval.exp) = releational_ops((yyvsp[-2].exp), if_lesseq, (yyvsp[0].exp)); 
                                        }
#line 2020 "src/parser/parser.c"
    break;

  case 29:
#line 326 "src/parser/parser.y"
                                        {   /*                  PLUS(+)                    */
                                            print_rules("expr","expr + expr");  
                                            (yyval.exp) = arithmetic_ops((yyvsp[-2].exp), add, (yyvsp[0].exp));
                                        }
#line 2029 "src/parser/parser.c"
    break;

  case 30:
#line 330 "src/parser/parser.y"
                                        {   /*                  MINUS(-)                     */
                                            print_rules("expr","expr - expr");
                                            (yyval.exp) = arithmetic_ops((yyvsp[-2].exp), sub, (yyvsp[0].exp));
                                        }
#line 2038 "src/parser/parser.c"
    break;

  case 31:
#line 334 "src/parser/parser.y"
                                        {   /*                  MULTIPLY(*)                     */
                                            print_rules("expr","expr * expr");
                                            (yyval.exp) = arithmetic_ops((yyvsp[-2].exp), mul, (yyvsp[0].exp));
                                        }
#line 2047 "src/parser/parser.c"
    break;

  case 32:
#line 338 "src/parser/parser.y"
                                        {   /*                  DIVIDE(/)                     */
                                            print_rules("expr","expr / expr");
                                            (yyval.exp) = arithmetic_ops((yyvsp[-2].exp), divide, (yyvsp[0].exp));
                                        }
#line 2056 "src/parser/parser.c"
    break;

  case 33:
#line 342 "src/parser/parser.y"
                                        {   /*                  MOD(%)                     */
                                            print_rules("expr","expr % expr");
                                            (yyval.exp) = arithmetic_ops((yyvsp[-2].exp), mod, (yyvsp[0].exp));
                                        }
#line 2065 "src/parser/parser.c"
    break;

  case 34:
#line 346 "src/parser/parser.y"
                                        {
                                            print_rules("expr","term");
                                            (yyval.exp) = (yyvsp[0].exp);
                                        }
#line 2074 "src/parser/parser.c"
    break;

  case 35:
#line 352 "src/parser/parser.y"
                                         {
                                            print_rules("term","( expr )");
                                            (yyval.exp) = (yyvsp[-1].exp);
                                        }
#line 2083 "src/parser/parser.c"
    break;

  case 36:
#line 356 "src/parser/parser.y"
                                        {
                                            print_rules("term","- expr");
                                            allowed_operations((yyvsp[0].exp), NULL);
                                            (yyval.exp) = newExpr(arithexpr_e);
                                            (yyval.exp)->sym = istempexpr((yyvsp[0].exp)) ? (yyvsp[0].exp)->sym : newTempVar();

                                            (yyval.exp)->numConst = -(yyvsp[0].exp)->numConst;
                                            emit(uminus, (yyval.exp), (yyvsp[0].exp), NULL, 0, yylineno);

                                        }
#line 2098 "src/parser/parser.c"
    break;

  case 37:
#line 366 "src/parser/parser.y"
                                        {
                                            print_rules("term","not expr");
                                            (yyvsp[0].exp) = boolQuads((yyvsp[0].exp));

                                            (yyval.exp) = newExpr(boolexpr_e);
                                            (yyval.exp)->truelist_head = (yyvsp[0].exp)->falselist_head;
                                            (yyval.exp)->falselist_head = (yyvsp[0].exp)->truelist_head;
                                        }
#line 2111 "src/parser/parser.c"
    break;

  case 38:
#line 374 "src/parser/parser.y"
                                        {

                                            if(isLocal == false && (yyvsp[0].exp)->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, (yyvsp[0].exp)->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", (yyvsp[0].exp)->sym->value.varVal->name, yylineno);
                                                    parserFlag = 1;
                                                    exit_flag = 1;
                                                }
                                            }

                                            libreturnCnt = 0;
                                            print_rules("term","++lvalue");
                                            if(parserFlag == 0){
                                                if((yyvsp[0].exp)->type == tableitem_e){
                                                    expr* tmp_exp = emit_iftableitem((yyvsp[0].exp));
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(add, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, (yyvsp[0].exp), (yyvsp[0].exp)->index, tmp_exp, 0, yylineno);
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = newTempVar();
                                                    emit(assign, (yyval.exp), tmp_exp, NULL, 0, yylineno);
                                                }else{
                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    (yyvsp[0].exp)->numConst += temp_expr->numConst; /* ?? */
                                                    emit(add, (yyvsp[0].exp), (yyvsp[0].exp), temp_expr, 0, yylineno);
                                                    SymbolTableEntry *temp = newTempVar();
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = temp;
                                                    (yyval.exp)->numConst = (yyvsp[0].exp)->numConst; /* ?? */
                                                    emit(assign, (yyval.exp), (yyvsp[0].exp), NULL, 0, yylineno);
                                                }
                                            }
                                            
                                            parserFlag = 0;

                                        }
#line 2154 "src/parser/parser.c"
    break;

  case 39:
#line 412 "src/parser/parser.y"
                                        {
                                            if(isLocal == false && (yyvsp[-1].exp)->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, (yyvsp[-1].exp)->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", (yyvsp[-1].exp)->sym->value.varVal->name, yylineno);
                                                    exit_flag = 1;
                                                    parserFlag = 1;
                                                }
                                            }
                                            libreturnCnt = 0;

                                            print_rules("term","lvalue++");

                                            if(parserFlag == 0){
                                                
                                                if((yyvsp[-1].exp)->type == tableitem_e){
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = newTempVar();
                                                    expr* tmp_exp = emit_iftableitem((yyvsp[-1].exp));
                                                    emit(assign, (yyval.exp), tmp_exp, NULL, 0, yylineno);
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(add, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, (yyvsp[-1].exp), (yyvsp[-1].exp)->index, tmp_exp, 0, yylineno);
                                                }else{
                                                    SymbolTableEntry *temp = newTempVar();
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = temp;
                                                    (yyval.exp)->numConst = (yyvsp[-1].exp)->numConst; 
                                                    emit(assign, (yyval.exp), (yyvsp[-1].exp), NULL, 0, yylineno);

                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    (yyvsp[-1].exp)->numConst += temp_expr->numConst;
                                                    emit(add, (yyvsp[-1].exp), (yyvsp[-1].exp), temp_expr, 0, yylineno);
                                                }
                                            }
                                            parserFlag = 0;

                                            
                                        }
#line 2199 "src/parser/parser.c"
    break;

  case 40:
#line 452 "src/parser/parser.y"
                                        {
                                            if(isLocal == false && (yyvsp[0].exp)->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, (yyvsp[0].exp)->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", (yyvsp[0].exp)->sym->value.varVal->name, yylineno);
                                                    exit_flag = 1;
                                                    parserFlag = 1;
                                                }
                                            }
                                            libreturnCnt = 0;
                                            print_rules("term","--lvalue");

                                            if(parserFlag == 0){
                                                if((yyvsp[0].exp)->type == tableitem_e){
                                                    expr* tmp_exp = emit_iftableitem((yyvsp[0].exp));
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(sub, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, (yyvsp[0].exp), (yyvsp[0].exp)->index, tmp_exp, 0, yylineno);
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = newTempVar();
                                                    emit(assign, (yyval.exp), tmp_exp, NULL, 0, yylineno);
                                                }else{
                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    (yyvsp[0].exp)->numConst -= temp_expr->numConst; /* ?? */
                                                    emit(add, (yyvsp[0].exp), (yyvsp[0].exp), temp_expr, 0, yylineno);
                                                    SymbolTableEntry *temp = newTempVar();
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = temp;
                                                    (yyval.exp)->numConst = (yyvsp[0].exp)->numConst; /* ?? */
                                                    emit(assign, (yyval.exp), (yyvsp[0].exp), NULL, 0, yylineno);
                                                }
                                            }
                                            parserFlag = 0;
                                        }
#line 2239 "src/parser/parser.c"
    break;

  case 41:
#line 487 "src/parser/parser.y"
                                        {
                                            print_rules("term","lvalue--");

                                            if(isLocal == false && (yyvsp[-1].exp)->sym->value.varVal->name != NULL){
                                                if(distinguish_lookup(table, (yyvsp[-1].exp)->sym->value.varVal->name, curr_scope) != GENERAL){
                                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", (yyvsp[-1].exp)->sym->value.varVal->name, yylineno);
                                                    exit_flag = 1;
                                                    parserFlag = 1;
                                                }
                                            }
                                            libreturnCnt = 0;
                                            

                                            if(parserFlag == 0){
                                                if((yyvsp[-1].exp)->type == tableitem_e){
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = newTempVar();
                                                    expr* tmp_exp = emit_iftableitem((yyvsp[-1].exp));
                                                    emit(assign, (yyval.exp), tmp_exp, NULL, 0, yylineno);
                                                    expr* num_expr = newExpr(constint_e);
                                                    num_expr->numConst = 1;
                                                    emit(sub, tmp_exp, tmp_exp, num_expr, 0, yylineno);
                                                    emit(tablesetelem, (yyvsp[-1].exp), (yyvsp[-1].exp)->index, tmp_exp, 0, yylineno);
                                                }else{
                                                    SymbolTableEntry *temp = newTempVar();
                                                    (yyval.exp) = newExpr(var_e);
                                                    (yyval.exp)->sym = temp;
                                                    (yyval.exp)->numConst = (yyvsp[-1].exp)->numConst; 
                                                    emit(assign, (yyval.exp), (yyvsp[-1].exp), NULL, 0, yylineno);

                                                    expr* temp_expr = newExpr(constint_e);
                                                    temp_expr->numConst = 1;
                                                    (yyvsp[-1].exp)->numConst += temp_expr->numConst;
                                                    emit(sub, (yyvsp[-1].exp), (yyvsp[-1].exp), temp_expr, 0, yylineno);
                                                }
                                            }
                                            parserFlag = 0;
                                        }
#line 2282 "src/parser/parser.c"
    break;

  case 42:
#line 525 "src/parser/parser.y"
                                        {(yyval.exp) = (yyvsp[0].exp);print_rules("term","primary");}
#line 2288 "src/parser/parser.c"
    break;

  case 43:
#line 529 "src/parser/parser.y"
                        {
                            if(isLocal == false && (yyvsp[-1].exp)->sym->value.varVal->name != NULL){
                                if(distinguish_lookup(table, (yyvsp[-1].exp)->sym->value.varVal->name, curr_scope) != GENERAL){
                                    fprintf(stderr, "Error, cannot change value of \"%s\" at line <%d>, functions are constant\n", (yyvsp[-1].exp)->sym->value.varVal->name, yylineno);
                                    exit_flag = 1;
                                    parserFlag = 1;
                                }
                            }
                            
                            libreturnCnt = 0;
                        }
#line 2304 "src/parser/parser.c"
    break;

  case 44:
#line 540 "src/parser/parser.y"
                        {
                            print_rules("assignexpr","lvalue = expr");
                            if((yyvsp[0].exp)->type == boolexpr_e)
                                (yyvsp[0].exp) = bool_expr((yyvsp[0].exp));
                            if(parserFlag == 0){
                                if((yyvsp[-3].exp)->type == tableitem_e){
                                    emit(tablesetelem, (yyvsp[-3].exp), (yyvsp[-3].exp)->index, (yyvsp[0].exp), 0, yylineno);
                                    (yyval.exp) = newExpr(tableitem_e);
                                    (yyval.exp)->sym = newTempVar();
                                    emit(tablegetelem, (yyval.exp), (yyvsp[-3].exp), (yyvsp[-3].exp)->index, 0, yylineno); 

                                }else {
                                    (yyvsp[-3].exp)->numConst = (yyvsp[0].exp)->numConst;
                                    (yyvsp[-3].exp)->strConst = (yyvsp[0].exp)->strConst;
                                    (yyvsp[-3].exp)->boolConst = (yyvsp[0].exp)->boolConst;
                                    emit(assign, (yyvsp[-3].exp), (yyvsp[0].exp), NULL, 0, yylineno);

                                    SymbolTableEntry *temp = newTempVar();
                                    (yyval.exp) = newExpr(var_e);
                                    (yyval.exp)->sym = temp;
                                    (yyval.exp)->numConst = (yyvsp[-3].exp)->numConst;
                                    (yyval.exp)->strConst = (yyvsp[-3].exp)->strConst;
                                    (yyval.exp)->boolConst = (yyvsp[-3].exp)->boolConst;
                                    emit(assign, (yyval.exp), (yyvsp[-3].exp), NULL, 0, yylineno);
                                    /*printf("%s = %f\n", $$->sym->value.varVal->name, $$->numConst);*/ /* print_tag */
                                }
                            }
                            parserFlag = 0;

                            
                        }
#line 2340 "src/parser/parser.c"
    break;

  case 45:
#line 573 "src/parser/parser.y"
                                                {
                                                    print_rules("primary","lvalue");
                                                    (yyval.exp) = emit_iftableitem((yyvsp[0].exp));
                                                }
#line 2349 "src/parser/parser.c"
    break;

  case 46:
#line 577 "src/parser/parser.y"
                                                {
                                                    (yyval.exp) = (yyvsp[0].exp);
                                                    print_rules("primary","call");
                                                }
#line 2358 "src/parser/parser.c"
    break;

  case 47:
#line 581 "src/parser/parser.y"
                                                {
                                                    print_rules("primary","objectdef");
                                                    (yyval.exp) = (yyvsp[0].exp);
                                                }
#line 2367 "src/parser/parser.c"
    break;

  case 48:
#line 585 "src/parser/parser.y"
                                                {   
                                                    (yyval.exp) = (yyvsp[-1].exp);
                                                    print_rules("primary","( funcdef )");
                                                    restoretmpvarOffset(curr_scope);
                                                }
#line 2377 "src/parser/parser.c"
    break;

  case 49:
#line 590 "src/parser/parser.y"
                                                {
                                                    (yyval.exp) = (yyvsp[0].exp);
                                                    print_rules("primary","const");
                                                }
#line 2386 "src/parser/parser.c"
    break;

  case 50:
#line 596 "src/parser/parser.y"
                                {
                                    isLocal = false;
                                    if(distinguish_lookup(table, yytext, curr_scope) == GENERAL){
                                        /*fprintf(stderr, "variable ID: %s\n", yytext);*/
                                        (yyval.exp) = newExpr(var_e);
                                        (yyval.exp)->sym = variable_lookup(table, create_variable(yytext, curr_scope, yylineno), GENERAL, curr_scope);

                                    }
                                    else{
                                        /*fprintf(stderr, "func call ID: %s\n", yytext);*/
                                        SymbolTableEntry *temp = function_lookup(table, create_function(yytext, curr_scope, yylineno), GLOBAL, curr_scope, true);
                                        if(temp != NULL){
                                            (yyval.exp) = newExpr(programmfunc_e);
                                            (yyval.exp)->sym = temp;
                                        }
                                        else{
                                            (yyval.exp) = newExpr(libraryfunc_e);
                                            (yyval.exp)->sym = libfunc_lookup(table, yytext);
                                            libreturnCnt = 1;
                                        }
                                    }
                                    print_rules("lvalue","ID");
                                }
#line 2414 "src/parser/parser.c"
    break;

  case 51:
#line 620 "src/parser/parser.y"
                                {
                                    isLocal = true;
                                    /*fprintf(stderr, "local ID: %s\n", yytext);*/
                                    (yyval.exp) = newExpr(var_e);
                                    (yyval.exp)->sym = variable_lookup(table, create_variable(yytext, curr_scope, yylineno), LOCAL_t, curr_scope);
                                    print_rules("lvalue","local ID");
                                }
#line 2426 "src/parser/parser.c"
    break;

  case 52:
#line 628 "src/parser/parser.y"
                                {
                                    isLocal = false;
                                    if(distinguish_lookup(table, yytext, curr_scope) == GENERAL){
                                        /*fprintf(stderr, "global ID: %s\n", yytext);*/
                                        SymbolTableEntry *temp = variable_lookup(table, create_variable(yytext, curr_scope, yylineno), GLOBAL, curr_scope);
                                        (yyval.exp) = exprLookup(temp);
                                        if((yyval.exp) == NULL){
                                            exit(0);
                                        }

                                    }
                                    else{
                                        /*fprintf(stderr, "func call ID: %s\n", yytext);*/
                                        SymbolTableEntry *temp = function_lookup(table, create_function(yytext, curr_scope, yylineno), GLOBAL, curr_scope, true);
                                        if(temp != NULL){
                                            (yyval.exp) = newExpr(programmfunc_e);
                                            (yyval.exp)->sym = temp;
                                        }
                                        else{
                                            (yyval.exp) = newExpr(libraryfunc_e);
                                            (yyval.exp)->sym = libfunc_lookup(table, yytext);
                                            libreturnCnt = 1;
                                        }

                                    }
                                    print_rules("lvalue","::ID");
                                }
#line 2458 "src/parser/parser.c"
    break;

  case 53:
#line 656 "src/parser/parser.y"
                                {
                                    print_rules("lvalue","member");
                                    (yyval.exp) = (yyvsp[0].exp);                                    
                                }
#line 2467 "src/parser/parser.c"
    break;

  case 54:
#line 662 "src/parser/parser.y"
                                {
                                    print_rules("member",".ID");
                                    (yyval.exp) = member_item((yyvsp[-2].exp), (yyvsp[0].str));

                                }
#line 2477 "src/parser/parser.c"
    break;

  case 55:
#line 667 "src/parser/parser.y"
                                                            {
                                                                print_rules("member","lvalue [ expr ]");
                                                                (yyvsp[-3].exp) = emit_iftableitem((yyvsp[-3].exp));
                                                                (yyval.exp) = newExpr(tableitem_e);
                                                                (yyval.exp)->sym = (yyvsp[-3].exp)->sym;
                                                                (yyval.exp)->index = (expr*) malloc(sizeof(expr));
                                                                (yyval.exp)->index = (yyvsp[-1].exp);
                                                            }
#line 2490 "src/parser/parser.c"
    break;

  case 56:
#line 675 "src/parser/parser.y"
                            {
                                print_rules("member","call.ID");
                                (yyval.exp) = member_item((yyvsp[-2].exp), (yyvsp[0].str));
                            }
#line 2499 "src/parser/parser.c"
    break;

  case 57:
#line 679 "src/parser/parser.y"
                                                            {
                                                                print_rules("member","call[ expr ]");
                                                                (yyvsp[-3].exp) = emit_iftableitem((yyvsp[-3].exp));
                                                                (yyval.exp) = newExpr(tableitem_e);
                                                                (yyval.exp)->sym = (yyvsp[-3].exp)->sym;
                                                                (yyval.exp)->index = (expr*) malloc(sizeof(expr));
                                                                (yyval.exp)->index = (yyvsp[-1].exp);
                                                            }
#line 2512 "src/parser/parser.c"
    break;

  case 58:
#line 689 "src/parser/parser.y"
                                                {
                                                    print_rules("call","call ( elist )");
                                                    if((yyvsp[-3].exp)->sym->isVariable == true){
                                                        (yyval.exp) = make_call((yyvsp[-3].exp), (yyvsp[-1].arg)->arguments, (yyvsp[-1].arg)->nargs);
                                                    }
                                                    else{
                                                        if((yyvsp[-1].arg)->nargs >= (yyvsp[-3].exp)->sym->value.funcVal->totalArgs){
                                                            (yyval.exp) = make_call((yyvsp[-3].exp), (yyvsp[-1].arg)->arguments, (yyvsp[-1].arg)->nargs);
                                                        }
                                                        else{
                                                            fprintf(stderr, "Error, too few arguments to function \"%s\", at line <%d>\n", (yyvsp[-3].exp)->sym->value.funcVal->name, yylineno);
                                                            exit_flag = 1;
                                                        }
                                                    }

                                                }
#line 2533 "src/parser/parser.c"
    break;

  case 59:
#line 705 "src/parser/parser.y"
                                    {
                                        print_rules("call","lvalue callsuffix");
                                        if((yyvsp[0].call_type)->isMethod == true){
                                            expr* self = (yyvsp[-1].exp);
                                            (yyvsp[-1].exp) = emit_iftableitem(member_item(self, (yyvsp[0].call_type)->name));
                                            int i;
                                            for(i = (yyvsp[0].call_type)->nargs; i >= 0; i--)
                                                (yyvsp[0].call_type)->arguments[i+1] = (yyvsp[0].call_type)->arguments[i]; 
                                            
                                            (yyvsp[0].call_type)->arguments[0] = self;
                                            (yyvsp[0].call_type)->nargs++;
                                        }
                                        if((yyvsp[-1].exp)->sym->isVariable == true){
                                            (yyval.exp) = make_call((yyvsp[-1].exp), (yyvsp[0].call_type)->arguments, (yyvsp[0].call_type)->nargs);
                                        }
                                        else{
                                            if((yyvsp[0].call_type)->nargs >= (yyvsp[-1].exp)->sym->value.funcVal->totalArgs){
                                                (yyval.exp) = make_call((yyvsp[-1].exp), (yyvsp[0].call_type)->arguments, (yyvsp[0].call_type)->nargs);
                                            } else {
                                                printf("%s : %d\n", (yyvsp[-1].exp)->sym->value.funcVal->name, (yyvsp[-1].exp)->sym->value.funcVal->totalArgs);
                                                fprintf(stderr, "Error, too few arguments to function \"%s\", at line <%d>\n", (yyvsp[-1].exp)->sym->value.funcVal->name, yylineno);
                                                exit_flag = 1;
                                            }
                                        }

                                    }
#line 2564 "src/parser/parser.c"
    break;

  case 60:
#line 731 "src/parser/parser.y"
                                           {restoretmpvarOffset(curr_scope);}
#line 2570 "src/parser/parser.c"
    break;

  case 61:
#line 731 "src/parser/parser.y"
                                                                                                          {   
                                                                            print_rules("call","( funcdef ) ( elist )");
                                                                            
                                                                            if((yyvsp[-1].arg)->nargs >= (yyvsp[-5].exp)->sym->value.funcVal->totalArgs){
                                                                                expr* func = newExpr(programmfunc_e);
                                                                                func->sym = (yyvsp[-5].exp)->sym;
                                                                                (yyval.exp) = make_call(func, (yyvsp[-1].arg)->arguments, (yyvsp[-1].arg)->nargs);
                                                                            }
                                                                            else{
                                                                                fprintf(stderr, "Error, too few arguments to function \"%s\", at line <%d>\n", (yyvsp[-5].exp)->sym->value.funcVal->name, yylineno);
                                                                                exit_flag = 1;
                                                                            }
                                                                        }
#line 2588 "src/parser/parser.c"
    break;

  case 62:
#line 746 "src/parser/parser.y"
                            {(yyval.call_type) = (yyvsp[0].call_type); print_rules("callsuffix","normalcall");}
#line 2594 "src/parser/parser.c"
    break;

  case 63:
#line 747 "src/parser/parser.y"
                            {(yyval.call_type) = (yyvsp[0].call_type); print_rules("callsuffix","methodcall");}
#line 2600 "src/parser/parser.c"
    break;

  case 64:
#line 750 "src/parser/parser.y"
                                            {
                                                print_rules("normalcall","( elist )");
                                                (yyval.call_type) = (call_t*)malloc(sizeof(call_t));
                                                (yyval.call_type)->nargs = 0;
                                                int i;
                                                for(i = 0; i < 100; i++){
                                                    (yyval.call_type)->arguments[i] = (yyvsp[-1].arg)->arguments[i];
                                                }
                                                (yyval.call_type)->nargs = (yyvsp[-1].arg)->nargs;
                                                (yyval.call_type)->isMethod = false;                                           
                                                (yyval.call_type)->name = NULL;
                                            }
#line 2617 "src/parser/parser.c"
    break;

  case 65:
#line 764 "src/parser/parser.y"
                                                        {
                                                            print_rules("methodcall","..ID ( elist )");
                                                            (yyval.call_type) = (call_t*)malloc(sizeof(call_t));
                                                            int i;
                                                            for(i = 0; i < 100; i++){
                                                                (yyval.call_type)->arguments[i] = (yyvsp[-1].arg)->arguments[i];
                                                            }
                                                            (yyval.call_type)->nargs = (yyvsp[-1].arg)->nargs;
                                                            (yyval.call_type)->isMethod = true;
                                                            (yyval.call_type)->name = strdup((yyvsp[-3].str));
                                                        }
#line 2633 "src/parser/parser.c"
    break;

  case 66:
#line 777 "src/parser/parser.y"
                            {
                                print_rules("elist","expr");

                                if((yyvsp[0].exp)->type == boolexpr_e)
                                    (yyvsp[0].exp) = bool_expr((yyvsp[0].exp));
                                (yyval.arg) = (arguments_t*)malloc(sizeof(arguments_t));
                                (yyval.arg)->nargs = 0;
                                (yyval.arg)->arguments[(yyval.arg)->nargs++] = (yyvsp[0].exp);
                            }
#line 2647 "src/parser/parser.c"
    break;

  case 67:
#line 786 "src/parser/parser.y"
                                    {
                                        print_rules("elist",", expr");
                                        if((yyvsp[0].exp)->type == boolexpr_e)
                                            (yyvsp[0].exp) = bool_expr((yyvsp[0].exp));
                                        (yyvsp[-2].arg)->arguments[(yyvsp[-2].arg)->nargs++] = (yyvsp[0].exp);
                                    }
#line 2658 "src/parser/parser.c"
    break;

  case 68:
#line 792 "src/parser/parser.y"
                            {
                                (yyval.arg) = (arguments_t*)malloc(sizeof(arguments_t));
                                (yyval.arg)->nargs = 0;
                                print_rules("elist"," ");
                            }
#line 2668 "src/parser/parser.c"
    break;

  case 69:
#line 801 "src/parser/parser.y"
                                                        {
                                                                print_rules("objectdef","[ elist ]");
                                                                expr *t = newExpr(newtable_e);
                                                                t->sym = newTempVar();
                                                                emit(tablecreate, t, NULL, NULL, 0, yylineno);
                                                                int i;
                                                                for(i = 0; i < (yyvsp[-1].arg)->nargs; i++){
                                                                    expr *num_tmp = (expr*)malloc(sizeof(expr));
                                                                    num_tmp->numConst = i;
                                                                    num_tmp->type = constint_e;
                                                                    emit(tablesetelem, t, num_tmp, (yyvsp[-1].arg)->arguments[i], 0, yylineno);
                                                                }
                                                                (yyval.exp) = t;
                                                            }
#line 2687 "src/parser/parser.c"
    break;

  case 70:
#line 815 "src/parser/parser.y"
                                                            {
                                                                print_rules("objectdef","[ indexed ]");
                                                                expr *t = newExpr(newtable_e);
                                                                t->sym = newTempVar();
                                                                emit(tablecreate, t, NULL, NULL, 0, yylineno);
                                                                int i;
                                                                for(i = 0; i < (yyvsp[-1].elems)->no_of_elems; i++){
                                                                    emit(tablesetelem, t, (yyvsp[-1].elems)->elems[i]->index, (yyvsp[-1].elems)->elems[i]->element, 0, yylineno);
                                                                }
                                                                (yyval.exp) = t;
                                                            }
#line 2703 "src/parser/parser.c"
    break;

  case 71:
#line 828 "src/parser/parser.y"
                            {curr_scope++;}
#line 2709 "src/parser/parser.c"
    break;

  case 72:
#line 829 "src/parser/parser.y"
                            {
                                if((yyvsp[0].exp)->type == boolexpr_e)
                                    (yyvsp[0].exp) = bool_expr((yyvsp[0].exp));
                            }
#line 2718 "src/parser/parser.c"
    break;

  case 73:
#line 833 "src/parser/parser.y"
                                            {
                                                print_rules("indexedelem","{ expr; expr }");
                                                curr_scope--;
                                                if((yyvsp[-1].exp)->type == boolexpr_e)
                                                    (yyvsp[-1].exp) = bool_expr((yyvsp[-1].exp));

                                                (yyval.ind_elem) = (indexed_elem*)malloc(sizeof(indexed_elem));
                                                (yyval.ind_elem)->index = (yyvsp[-4].exp);
                                                (yyval.ind_elem)->element = (yyvsp[-1].exp);
                                                /*
                                                table_elem_index[table_elem_offset] = $2;
                                                table_elems[table_elem_offset++] = $5;
                                                */
                                            }
#line 2737 "src/parser/parser.c"
    break;

  case 74:
#line 849 "src/parser/parser.y"
                                            {
                                                (yyval.elems) = (elements*)malloc(sizeof(elements));
                                                (yyval.elems)->no_of_elems = 0;
                                                (yyval.elems)->elems[(yyval.elems)->no_of_elems++] = (yyvsp[0].ind_elem);
                                            }
#line 2747 "src/parser/parser.c"
    break;

  case 75:
#line 854 "src/parser/parser.y"
                                            {
                                                (yyvsp[-2].elems)->elems[(yyvsp[-2].elems)->no_of_elems++] = (yyvsp[0].ind_elem);
                                            }
#line 2755 "src/parser/parser.c"
    break;

  case 76:
#line 859 "src/parser/parser.y"
                                {savetmpvarOffset(curr_scope); curr_scope++;}
#line 2761 "src/parser/parser.c"
    break;

  case 77:
#line 861 "src/parser/parser.y"
                                {
                                    print_rules("block","{ stmts }");

                                    (yyval.stmt_t) = new_stmt();

                                    if((yyvsp[-1].stmt_t)->breaklist != NULL)
                                        (yyval.stmt_t)->breaklist = merge_list((yyval.stmt_t)->breaklist, (yyvsp[-1].stmt_t)->breaklist, NULL);

                                    if((yyvsp[-1].stmt_t)->contlist != NULL)
                                        (yyval.stmt_t)->contlist = merge_list((yyval.stmt_t)->contlist, (yyvsp[-1].stmt_t)->contlist, NULL);
                                        
                                    hide(table, curr_scope);
                                    curr_scope--;
                                }
#line 2780 "src/parser/parser.c"
    break;

  case 78:
#line 877 "src/parser/parser.y"
                                                {
                                                    exitscopespace();
                                                    (yyvsp[-2].symbol)->value.funcVal->totalLocals = functionLocalOffset;
                                                    functionLocalOffset = func_pop(functionLocalsStack);
                                                    (yyvsp[-2].symbol)->value.funcVal->totalArgs = formalArgOffset;
                                                    (yyval.exp) = newExpr(programmfunc_e);
                                                    (yyval.exp)->sym = (yyvsp[-2].symbol);
                                                    emit(funcend, (yyval.exp), NULL, NULL, 0, yylineno);

                                                    if(retjumpQuad[functionOffset+1] != NULL)
                                                        retjumpQuad[functionOffset+1]->label = (yyvsp[0].integer);

                                                    funcjumpQuad[functionOffset+1]->label = (yyvsp[0].integer) + 1;

                                                    funcjumpsReset(functionOffset+1);
                                                    pop(loopstack);
                                                    (yyval.exp)->sym->isActive = false;
                                                }
#line 2803 "src/parser/parser.c"
    break;

  case 79:
#line 897 "src/parser/parser.y"
                                    {
                                        push(loopstack);
                                        functionOffset++;
                                        funcjumpQuad[functionOffset] = emit(jump, NULL, NULL, NULL, 0, yylineno);
                                        if((yyvsp[0].str) != NULL){
                                            (yyval.symbol) = function_lookup(table, create_function((yyvsp[0].str), curr_scope, yylineno), USERFUNC, curr_scope, false);
                                            if((yyval.symbol) == NULL){
                                                exit(0);
                                            }
                                        }
                                        else{
                                            (yyval.symbol) = newTempFuncVar();
                                        }
                                        (yyval.symbol)->value.funcVal->funcAddress = nextQuad();    
                                        expr *func_expr = newExpr(programmfunc_e);
                                        func_expr->sym = (yyval.symbol);
                                        emit(funcstart, func_expr, NULL, NULL, 0, yylineno);
                                        func_push(functionLocalsStack, functionLocalOffset);
                                        enterscopespace();
                                        resetformalargsOffset();
                                        returnCnt++;
                                    }
#line 2830 "src/parser/parser.c"
    break;

  case 80:
#line 921 "src/parser/parser.y"
                            { curr_scope++; }
#line 2836 "src/parser/parser.c"
    break;

  case 81:
#line 923 "src/parser/parser.y"
                            {
                                curr_scope--;
                                enterscopespace();
                                resetfunctionlocalsOffset();
                            }
#line 2846 "src/parser/parser.c"
    break;

  case 82:
#line 930 "src/parser/parser.y"
                        {
                            (yyval.integer) = nextQuad();
                            exitscopespace();
                            returnCnt--;
                            functionOffset--;
                        }
#line 2857 "src/parser/parser.c"
    break;

  case 83:
#line 938 "src/parser/parser.y"
                            {(yyval.str) = (yyvsp[0].str);}
#line 2863 "src/parser/parser.c"
    break;

  case 84:
#line 939 "src/parser/parser.y"
                            {(yyval.str) = NULL;}
#line 2869 "src/parser/parser.c"
    break;

  case 85:
#line 942 "src/parser/parser.y"
                                    {
                                        print_rules("idlist","ID");
                                        formal_lookup(table, create_variable((yyvsp[0].str), curr_scope, yylineno), FORMAL, curr_scope);
                                    }
#line 2878 "src/parser/parser.c"
    break;

  case 86:
#line 946 "src/parser/parser.y"
                                    {
                                        print_rules("idlist","idlist COMMA ID");
                                        formal_lookup(table, create_variable((yyvsp[0].str), curr_scope, yylineno), FORMAL, curr_scope);
                                    }
#line 2887 "src/parser/parser.c"
    break;

  case 87:
#line 950 "src/parser/parser.y"
                                    {print_rules("idlist"," ");}
#line 2893 "src/parser/parser.c"
    break;

  case 88:
#line 955 "src/parser/parser.y"
                            {
                                print_rules("const","REAL_CONST");
                                (yyval.exp) = newExpr(constdouble_e);
                                (yyval.exp)->numConst = (yyvsp[0].num);
                            }
#line 2903 "src/parser/parser.c"
    break;

  case 89:
#line 960 "src/parser/parser.y"
                            {
                                print_rules("const","INT_CONST");
                                (yyval.exp) = newExpr(constint_e);
                                (yyval.exp)->numConst = (yyvsp[0].num);
                            }
#line 2913 "src/parser/parser.c"
    break;

  case 90:
#line 965 "src/parser/parser.y"
                            {
                                print_rules("const","STRING");
                                (yyval.exp) = newExpr(conststring_e);
                                (yyval.exp)->strConst = (yyvsp[0].str);
                            }
#line 2923 "src/parser/parser.c"
    break;

  case 91:
#line 970 "src/parser/parser.y"
                            {
                                print_rules("const","NIL");
                                (yyval.exp) = newExpr(nil_e);
                            }
#line 2932 "src/parser/parser.c"
    break;

  case 92:
#line 974 "src/parser/parser.y"
                            {
                                print_rules("const","TRUE");
                                (yyval.exp) = newExpr(constbool_e);
                                (yyval.exp)->boolConst = (yyvsp[0].character);
                            }
#line 2942 "src/parser/parser.c"
    break;

  case 93:
#line 979 "src/parser/parser.y"
                            {
                                print_rules("const","FALSE");
                                (yyval.exp) = newExpr(constbool_e);
                                (yyval.exp)->boolConst = (yyvsp[0].character);
                            }
#line 2952 "src/parser/parser.c"
    break;

  case 94:
#line 987 "src/parser/parser.y"
                                            {
                                                if((yyvsp[-1].exp)->type == boolexpr_e)
                                                    (yyvsp[-1].exp) = bool_expr((yyvsp[-1].exp));                                                    
                                                
                                                expr* true_expr = newExpr(constbool_e);
                                                true_expr->boolConst = 't';
                                                
                                                emit(if_eq, NULL, (yyvsp[-1].exp), true_expr, nextQuad()+2 , yylineno);
                                                (yyval.integer) = nextQuad();
                                                emit(jump, NULL, NULL, NULL, -1, yylineno);
                                            }
#line 2968 "src/parser/parser.c"
    break;

  case 95:
#line 999 "src/parser/parser.y"
                                {
                                    patchlabel((yyvsp[-1].integer), nextQuad());
                                    resetTempVar();
                                }
#line 2977 "src/parser/parser.c"
    break;

  case 96:
#line 1003 "src/parser/parser.y"
                                {
                                    if((yyvsp[0].stmt_t)->else_quad != -2){
                                        patchlabel((yyvsp[-3].integer), (yyvsp[0].stmt_t)->else_quad + 1);
                                        patchlabel((yyvsp[0].stmt_t)->else_quad, nextQuad()); 
                                    }
                                    (yyval.stmt_t) = new_stmt();
                                    (yyval.stmt_t)->breaklist = merge_list((yyval.stmt_t)->breaklist, (yyvsp[-2].stmt_t)->breaklist, (yyvsp[0].stmt_t)->breaklist);
                                    (yyval.stmt_t)->contlist = merge_list((yyval.stmt_t)->contlist, (yyvsp[-2].stmt_t)->contlist, (yyvsp[0].stmt_t)->contlist);
                                }
#line 2991 "src/parser/parser.c"
    break;

  case 97:
#line 1013 "src/parser/parser.y"
                                {
                                    resetTempVar();
                                    (yyval.stmt_t) = new_stmt();
                                    (yyval.stmt_t)->else_quad = (yyvsp[-1].integer);
                                    (yyval.stmt_t)->breaklist = (yyvsp[0].stmt_t)->breaklist;
                                    (yyval.stmt_t)->contlist = (yyvsp[0].stmt_t)->contlist;
                                }
#line 3003 "src/parser/parser.c"
    break;

  case 98:
#line 1020 "src/parser/parser.y"
                                {
                                    (yyval.stmt_t) = new_stmt();
                                    (yyval.stmt_t)->else_quad = -2;
                                }
#line 3012 "src/parser/parser.c"
    break;

  case 99:
#line 1026 "src/parser/parser.y"
                        {
                            (yyval.integer) = nextQuad();
                            emit(jump, NULL, NULL, NULL, -1, yylineno);
                        }
#line 3021 "src/parser/parser.c"
    break;

  case 100:
#line 1032 "src/parser/parser.y"
                                {
                                    (yyval.break_t) = (struct break_s*)malloc(sizeof(struct break_s));
                                    (yyval.break_t)->breaklist = insert_list((yyval.break_t)->breaklist, nextQuad());
                                    emit(jump, NULL, NULL, NULL, -1, yylineno);
                                }
#line 3031 "src/parser/parser.c"
    break;

  case 101:
#line 1039 "src/parser/parser.y"
                                    {
                                        (yyval.cont_t) = (struct cont_s*)malloc(sizeof(struct cont_s));
                                        (yyval.cont_t)->contlist = insert_list((yyval.cont_t)->contlist, nextQuad());
                                        emit(jump, NULL, NULL, NULL, -1, yylineno);
                                    }
#line 3041 "src/parser/parser.c"
    break;

  case 102:
#line 1046 "src/parser/parser.y"
                              {}
#line 3047 "src/parser/parser.c"
    break;

  case 103:
#line 1049 "src/parser/parser.y"
                              {}
#line 3053 "src/parser/parser.c"
    break;

  case 104:
#line 1052 "src/parser/parser.y"
                                       {(yyval.stmt_t) = (yyvsp[-1].stmt_t);}
#line 3059 "src/parser/parser.c"
    break;

  case 105:
#line 1055 "src/parser/parser.y"
                        {
                            loopcounter++;
                            (yyval.integer) = nextQuad();
                            if(loopcounter > 1)
                                push(loopstack);
                            modify_tail(loopstack, true);
                        }
#line 3071 "src/parser/parser.c"
    break;

  case 106:
#line 1064 "src/parser/parser.y"
                                        {
                                            if((yyvsp[-1].exp)->type == boolexpr_e)
                                                (yyvsp[-1].exp) = bool_expr((yyvsp[-1].exp));

                                            expr* true_expr = newExpr(constbool_e);
                                            true_expr->boolConst = 't';
                                            emit(if_eq, NULL, (yyvsp[-1].exp), true_expr, nextQuad()+2, yylineno);
                                            (yyval.integer) = nextQuad();
                                            whileQuad = emit(jump, NULL, NULL, NULL, -1, yylineno);
                                        }
#line 3086 "src/parser/parser.c"
    break;

  case 107:
#line 1076 "src/parser/parser.y"
                                                {
                                                    print_rules("whilestmt","while(expr) stmt");
                                                    emit(jump, NULL, NULL, NULL, (yyvsp[-2].integer), yylineno);
                                                    patchlabel((yyvsp[-1].integer), nextQuad());

                                                    struct list_s* tmp;
                                                    tmp = (yyvsp[0].stmt_t)->breaklist;
                                                    while(tmp != NULL){
                                                        patchlabel(tmp->quadNo, nextQuad());
                                                        tmp = tmp->next;
                                                    }
                                                    tmp = (yyvsp[0].stmt_t)->contlist;
                                                    while(tmp != NULL){
                                                        patchlabel(tmp->quadNo, (yyvsp[-2].integer));
                                                        tmp = tmp->next;
                                                    }
                                                    modify_tail(loopstack, false);
                                                    if(loopcounter > 1)
                                                        pop(loopstack);
                                                    loopcounter--;
                                                }
#line 3112 "src/parser/parser.c"
    break;

  case 108:
#line 1099 "src/parser/parser.y"
                              {(yyval.integer) = nextQuad(); emit(jump, NULL, NULL, NULL, -1, yylineno);}
#line 3118 "src/parser/parser.c"
    break;

  case 109:
#line 1102 "src/parser/parser.y"
                              {(yyval.integer) = nextQuad();}
#line 3124 "src/parser/parser.c"
    break;

  case 110:
#line 1105 "src/parser/parser.y"
                                                                {
                                                                    loopcounter++;
                                                                    if(loopcounter > 1)
                                                                        push(loopstack);
                                                                    modify_tail(loopstack, true);
                                                                    if((yyvsp[-1].exp)->type == boolexpr_e)
                                                                        (yyvsp[-1].exp) = bool_expr((yyvsp[-1].exp));

                                                                    (yyval.forpr_t) = (struct forpr_s*)malloc(sizeof(struct forpr_s));
                                                                    expr* true_expr = newExpr(constbool_e);
                                                                    true_expr->boolConst = 't';
                                                                    (yyval.forpr_t)->test = (yyvsp[-2].integer);
                                                                    (yyval.forpr_t)->enter = nextQuad();
                                                                    emit(if_eq, NULL, (yyvsp[-1].exp), true_expr, -1, yylineno);
                                                                }
#line 3144 "src/parser/parser.c"
    break;

  case 111:
#line 1120 "src/parser/parser.y"
                                                                {
                                                                    print_rules("forstmt","for(elist; expr; elist) stmt");
                                                                    patchlabel((yyvsp[-6].forpr_t)->enter, (yyvsp[-2].integer)+1);
                                                                    patchlabel((yyvsp[-5].integer), nextQuad());
                                                                    patchlabel((yyvsp[-2].integer), (yyvsp[-6].forpr_t)->test);
                                                                    patchlabel((yyvsp[0].integer), (yyvsp[-5].integer)+1);

                                                                    struct list_s* tmp;
                                                                    tmp = (yyvsp[-1].stmt_t)->breaklist;
                                                                    while(tmp != NULL){
                                                                        patchlabel(tmp->quadNo, nextQuad());
                                                                        tmp = tmp->next;
                                                                    }
                                                                    tmp = (yyvsp[-1].stmt_t)->contlist;
                                                                    while(tmp != NULL){
                                                                        patchlabel(tmp->quadNo, (yyvsp[-5].integer)+1);
                                                                        tmp = tmp->next;
                                                                    }
                                                                    modify_tail(loopstack, false);
                                                                    if(loopcounter > 1)
                                                                        pop(loopstack);
                                                                    loopcounter--;
                                                                }
#line 3172 "src/parser/parser.c"
    break;

  case 112:
#line 1145 "src/parser/parser.y"
                                        {
                                            print_rules("returnstmt","return expr;");
                                            if((yyvsp[-1].exp)->type == boolexpr_e)
                                                (yyvsp[-1].exp) = bool_expr((yyvsp[-1].exp));
                                            
                                            if(returnCnt != 0){
                                                (yyval.exp) = (yyvsp[-1].exp);
                                                emit(ret, (yyvsp[-1].exp), NULL, NULL, 0, yylineno);
                                                retjumpQuad[functionOffset] = emit(jump, NULL, NULL, NULL, 0, yylineno);
                                            }
                                        }
#line 3188 "src/parser/parser.c"
    break;

  case 113:
#line 1156 "src/parser/parser.y"
                                    {
                                        print_rules("returnstmt","return;");
                                        if(returnCnt != 0){
                                            expr* nil = newExpr(nil_e);
                                            emit(ret, nil, NULL, NULL, 0, yylineno);
                                            retjumpQuad[functionOffset] = emit(jump, NULL, NULL, NULL, 0, yylineno);
                                        }
                                    }
#line 3201 "src/parser/parser.c"
    break;


#line 3205 "src/parser/parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1166 "src/parser/parser.y"


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
