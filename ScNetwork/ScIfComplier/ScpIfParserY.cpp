/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 6 "ScpIfParser.y.h"

#include <stdio.h>

//#define YYSTYPE CTag*
#include "tag.h"
#include "FileTags.h"


//#include "TestParserY.h"

    #define  __STDC__   0 
    void yyerror(char*);
    int yylex(void);

/* Line 371 of yacc.c  */
#line 83 "ScpIfParserY.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "ScpIfParserY.h".  */
#ifndef YY_YY_SCPIFPARSERY_H_INCLUDED
# define YY_YY_SCPIFPARSERY_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_BRANCES_LEFT = 258,
     TK_BRANCES_RIGHT = 259,
     TK_BRACKET_LEFT = 260,
     TK_BRACKET_RIGHT = 261,
     TK_PARENTHESES_LEFT = 262,
     TK_PARENTHESES_RIGHT = 263,
     TK_ANGLE_BRACKET_LEFT = 264,
     TK_ANGLE_BRACKET_RIGHT = 265,
     TK_SEMICOLON = 266,
     TK_COMMA = 267,
     TK_COLON = 268,
     TK_QUOTATION = 269,
     TK_POUND_SIGN = 270,
     TK_INVERSE = 271,
     TK_STAR = 272,
     TK_EQU = 273,
     TK_AND = 274,
     TK_COMMENT_LINE = 275,
     TK_COMMENT_BLOCK = 276,
     NUM = 277,
     TK_KW_INCLUDE = 278,
     TK_PRAGMA = 279,
     TK_ONCE = 280,
     TK_USING_NAME_SPACE = 281,
     TK_KW_DEFINE = 282,
     TK_KW_IFDEF = 283,
     TK_KW_IFNDEF = 284,
     TK_KW_ENDIF = 285,
     TK_KW_ELSE = 286,
     TK_KW_CLASS = 287,
     TK_KW_STRUCT = 288,
     TK_KW_ENUM = 289,
     TK_KW_UNION = 290,
     TK_KW_CHAR = 291,
     TK_KW_UCHAR = 292,
     TK_KW_SHORT = 293,
     TK_KW_USHORT = 294,
     TK_KW_INT = 295,
     TK_KW_UINT = 296,
     TK_KW_LONG = 297,
     TK_KW_ULONG = 298,
     TK_KW_LONGLONG = 299,
     TK_KW_ULONGLONG = 300,
     TK_KW_FLOAT = 301,
     TK_KW_DOUBLE = 302,
     TK_KW_INT8 = 303,
     TK_KW_UINT8 = 304,
     TK_KW_INT16 = 305,
     TK_KW_UINT16 = 306,
     TK_KW_INT32 = 307,
     TK_KW_UINT32 = 308,
     TK_KW_INT64 = 309,
     TK_KW_UINT64 = 310,
     TK_KW_PUBLIC = 311,
     TK_KW_PROTECTED = 312,
     TK_KW_PRIVATE = 313,
     TK_KW_VIRTUAL = 314,
     TK_KW_VOID = 315,
     TK_KW_TYPEDEF = 316,
     TK_TYPE = 317,
     WORD = 318,
     TK_LINE_OTHER = 319,
     TK_KW_IF_FUN = 320,
     TK_KW_IF_SLOT = 321,
     TK_KW_OUT_P = 322,
     TK_KW_ONE_WAY = 323
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_SCPIFPARSERY_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 217 "ScpIfParserY.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   201

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNRULES -- Number of states.  */
#define YYNSTATES  156

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   323

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    36,    42,    48,
      56,    65,    66,    69,    73,    75,    78,    79,    82,    84,
      86,    89,    93,    96,    98,   100,   104,   108,   110,   112,
     115,   118,   123,   125,   130,   136,   143,   151,   158,   161,
     164,   168,   171,   175,   176,   178,   182,   183,   185,   188,
     192,   194,   196,   198,   200,   202,   211,   212,   214,   218,
     223,   227,   229,   233,   236,   239,   242,   245,   247,   249,
     251,   253,   255,   256,   259,   262,   265
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      70,     0,    -1,    71,    -1,    72,    -1,    71,    72,    -1,
      73,    -1,    74,    -1,    26,    -1,    30,    -1,    87,    -1,
      75,    -1,    88,    -1,    91,    -1,    94,    -1,    95,    -1,
      96,    -1,    15,    24,    25,    -1,    15,    23,     9,    63,
      10,    -1,    15,    23,    14,    63,    14,    -1,    32,    63,
      76,     3,    78,     4,    11,    -1,    32,    63,    63,    76,
       3,    78,     4,    11,    -1,    -1,    13,    77,    -1,    76,
      12,    77,    -1,    63,    -1,    86,    63,    -1,    -1,    86,
      13,    -1,    79,    -1,    87,    -1,    78,    79,    -1,    78,
      86,    13,    -1,    78,    87,    -1,    80,    -1,    83,    -1,
      81,    82,    11,    -1,    63,    63,    11,    -1,    63,    -1,
      60,    -1,    81,    17,    -1,    81,    19,    -1,    81,     9,
      81,    10,    -1,    63,    -1,    63,     5,    63,     6,    -1,
      63,     7,    84,     8,    11,    -1,    16,    63,     7,    84,
       8,    11,    -1,    59,    16,    63,     7,    84,     8,    11,
      -1,    81,    63,     7,    84,     8,    11,    -1,    59,    83,
      -1,    65,    83,    -1,    68,    65,    83,    -1,    66,    83,
      -1,    68,    66,    83,    -1,    -1,    85,    -1,    84,    12,
      85,    -1,    -1,    60,    -1,    81,    82,    -1,    67,    81,
      82,    -1,    56,    -1,    57,    -1,    58,    -1,    20,    -1,
      21,    -1,    61,    34,    63,     3,    89,     4,    63,    11,
      -1,    -1,    90,    -1,    89,    12,    90,    -1,    89,    12,
      90,    12,    -1,    63,    18,    63,    -1,    63,    -1,    27,
      63,    93,    -1,    27,    66,    -1,    27,    65,    -1,    27,
      67,    -1,    27,    68,    -1,    63,    -1,     3,    -1,     4,
      -1,     7,    -1,     8,    -1,    -1,    93,    92,    -1,    29,
      63,    -1,    28,    63,    -1,    31,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    98,    98,   101,   102,   105,   106,   107,   120,   125,
     126,   127,   128,   129,   130,   131,   134,   145,   158,   174,
     202,   232,   239,   248,   259,   267,   283,   289,   298,   308,
     315,   324,   331,   340,   346,   354,   367,   390,   403,   415,
     424,   435,   453,   458,   471,   495,   524,   555,   582,   598,
     615,   635,   652,   673,   678,   685,   695,   696,   706,   716,
     731,   738,   744,   755,   760,   767,   788,   794,   801,   808,
     817,   828,   850,   862,   873,   884,   895,   920,   921,   922,
     923,   924,   927,   934,   941,   954,   967
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_BRANCES_LEFT", "TK_BRANCES_RIGHT",
  "TK_BRACKET_LEFT", "TK_BRACKET_RIGHT", "TK_PARENTHESES_LEFT",
  "TK_PARENTHESES_RIGHT", "TK_ANGLE_BRACKET_LEFT",
  "TK_ANGLE_BRACKET_RIGHT", "TK_SEMICOLON", "TK_COMMA", "TK_COLON",
  "TK_QUOTATION", "TK_POUND_SIGN", "TK_INVERSE", "TK_STAR", "TK_EQU",
  "TK_AND", "TK_COMMENT_LINE", "TK_COMMENT_BLOCK", "NUM", "TK_KW_INCLUDE",
  "TK_PRAGMA", "TK_ONCE", "TK_USING_NAME_SPACE", "TK_KW_DEFINE",
  "TK_KW_IFDEF", "TK_KW_IFNDEF", "TK_KW_ENDIF", "TK_KW_ELSE",
  "TK_KW_CLASS", "TK_KW_STRUCT", "TK_KW_ENUM", "TK_KW_UNION", "TK_KW_CHAR",
  "TK_KW_UCHAR", "TK_KW_SHORT", "TK_KW_USHORT", "TK_KW_INT", "TK_KW_UINT",
  "TK_KW_LONG", "TK_KW_ULONG", "TK_KW_LONGLONG", "TK_KW_ULONGLONG",
  "TK_KW_FLOAT", "TK_KW_DOUBLE", "TK_KW_INT8", "TK_KW_UINT8",
  "TK_KW_INT16", "TK_KW_UINT16", "TK_KW_INT32", "TK_KW_UINT32",
  "TK_KW_INT64", "TK_KW_UINT64", "TK_KW_PUBLIC", "TK_KW_PROTECTED",
  "TK_KW_PRIVATE", "TK_KW_VIRTUAL", "TK_KW_VOID", "TK_KW_TYPEDEF",
  "TK_TYPE", "WORD", "TK_LINE_OTHER", "TK_KW_IF_FUN", "TK_KW_IF_SLOT",
  "TK_KW_OUT_P", "TK_KW_ONE_WAY", "$accept", "Program", "Tags", "Tag",
  "Pragma", "Include", "Class", "BaseClasses", "BaseClass", "Members",
  "Member", "MemberVariant", "VarType", "VarName", "MemberFunction",
  "Params", "Param", "Access", "Comment", "Enum", "EnumMembers",
  "EnumMember", "Define", "DefineContent", "DefineContents", "Ifndef",
  "Ifdef", "Ifelse", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    69,    70,    71,    71,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    73,    74,    74,    75,
      75,    76,    76,    76,    77,    77,    78,    78,    78,    78,
      78,    78,    78,    79,    79,    80,    80,    81,    81,    81,
      81,    81,    82,    82,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    84,    84,    84,    85,    85,    85,    85,
      86,    86,    86,    87,    87,    88,    89,    89,    89,    89,
      90,    90,    91,    91,    91,    91,    91,    92,    92,    92,
      92,    92,    93,    93,    94,    95,    96
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     7,
       8,     0,     2,     3,     1,     2,     0,     2,     1,     1,
       2,     3,     2,     1,     1,     3,     3,     1,     1,     2,
       2,     4,     1,     4,     5,     6,     7,     6,     2,     2,
       3,     2,     3,     0,     1,     3,     0,     1,     2,     3,
       1,     1,     1,     1,     1,     8,     0,     1,     3,     4,
       3,     1,     3,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     0,     2,     2,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    63,    64,     7,     0,     0,     0,     8,    86,
       0,     0,     0,     2,     3,     5,     6,    10,     9,    11,
      12,    13,    14,    15,     0,     0,    82,    74,    73,    75,
      76,    85,    84,    21,     0,     1,     4,     0,     0,    16,
      72,     0,    21,     0,     0,     0,     0,    78,    79,    80,
      81,    77,    83,    60,    61,    62,    24,    22,     0,     0,
      26,     0,    66,    17,    18,    25,    26,     0,     0,    38,
      37,     0,     0,     0,     0,    28,    33,     0,    34,     0,
      29,    23,    71,     0,    67,     0,     0,     0,    37,     0,
      48,    53,     0,    49,    51,     0,     0,     0,    30,     0,
      32,     0,    39,    40,    42,     0,    27,     0,     0,     0,
       0,    53,     0,     0,    38,    37,     0,     0,     0,    54,
      36,    50,    52,    19,    31,     0,     0,    53,    35,    70,
       0,    68,    20,     0,    53,     0,    42,    58,     0,    56,
      41,     0,     0,    65,    69,     0,     0,    59,    44,    55,
      43,     0,    45,     0,    47,    45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    43,    57,    74,
      75,    76,    89,   105,    78,   118,   119,    58,    18,    19,
      83,    84,    20,    52,    40,    21,    22,    23
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -116
static const yytype_int16 yypact[] =
{
     118,   147,  -116,  -116,  -116,   100,   -53,   -30,  -116,  -116,
     -25,    36,    48,   118,  -116,  -116,  -116,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,     2,    40,  -116,  -116,  -116,  -116,
    -116,  -116,  -116,   -10,     9,  -116,  -116,    12,    44,  -116,
       1,    99,    73,    31,   108,   111,   110,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,    63,    97,
      57,    99,    67,  -116,  -116,  -116,    57,    80,    38,  -116,
      -6,    69,    69,    76,     3,  -116,  -116,    13,  -116,   140,
    -116,  -116,   154,    98,  -116,    24,   167,   112,   169,    30,
    -116,    45,   166,  -116,  -116,    69,    69,   170,  -116,   165,
    -116,    -5,  -116,  -116,    10,   171,  -116,   117,   120,    67,
     173,    45,   178,   179,   119,  -116,    -5,    33,   128,  -116,
    -116,  -116,  -116,  -116,  -116,   142,   124,    45,  -116,  -116,
     177,   180,  -116,   146,    45,    33,   184,  -116,   182,    45,
    -116,   185,   152,  -116,  -116,   183,   161,  -116,  -116,  -116,
    -116,   186,  -116,   187,  -116,  -116
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -116,  -116,  -116,   188,  -116,  -116,  -116,   148,   134,   130,
      14,  -116,   -60,  -115,    23,   -98,    60,   -48,   -39,  -116,
    -116,    91,  -116,  -116,  -116,  -116,  -116,  -116
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -58
static const yytype_int16 yytable[] =
{
      77,    91,   137,    41,    47,    48,    77,    97,    49,    50,
      31,    37,    79,   133,    77,   126,    38,   127,    79,    67,
     147,    80,   101,     2,     3,    77,    99,    80,   110,   142,
     102,   117,   103,    32,    60,   100,   146,    99,    33,   101,
      67,   125,   101,    61,     2,     3,   100,   102,    35,   103,
     102,   117,   103,    42,    87,    69,   135,    92,   115,    53,
      54,    55,    68,    69,    51,    39,    70,   117,    71,    72,
      34,    73,    44,    67,   117,    45,   104,     2,     3,   117,
      53,    54,    55,    68,    69,    67,    41,    70,    98,    71,
      72,    90,    73,   113,    93,    94,   136,    68,    69,    98,
      66,    88,   108,    71,    72,   114,    73,    46,   115,    61,
     109,    62,   116,    53,    54,    55,    68,    69,   121,   122,
      70,    63,    71,    72,    64,    73,    65,   -57,    68,    69,
      82,   -57,    88,     1,    71,    72,   138,    73,     2,     3,
     139,    95,    96,    86,     4,     5,     6,     7,     8,     9,
      10,   101,   140,   106,   145,    53,    54,    55,   139,   102,
     151,   103,    56,    26,   139,    27,    28,    29,    30,   153,
      24,    25,   107,   139,   111,   112,    91,   120,   124,    11,
     129,   123,   128,   130,   132,   134,   127,   141,   143,   126,
      59,   150,   144,   148,   152,    81,    85,   154,   155,   149,
     131,    36
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-116)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
      60,     7,   117,    13,     3,     4,    66,     4,     7,     8,
      63,     9,    60,   111,    74,     5,    14,     7,    66,    16,
     135,    60,     9,    20,    21,    85,    74,    66,     4,   127,
      17,    91,    19,    63,     3,    74,   134,    85,    63,     9,
      16,   101,     9,    12,    20,    21,    85,    17,     0,    19,
      17,   111,    19,    63,    16,    60,   116,    63,    63,    56,
      57,    58,    59,    60,    63,    25,    63,   127,    65,    66,
      34,    68,    63,    16,   134,    63,    63,    20,    21,   139,
      56,    57,    58,    59,    60,    16,    13,    63,    74,    65,
      66,    68,    68,    63,    71,    72,    63,    59,    60,    85,
       3,    63,     4,    65,    66,    60,    68,    63,    63,    12,
      12,     3,    67,    56,    57,    58,    59,    60,    95,    96,
      63,    10,    65,    66,    14,    68,    63,     8,    59,    60,
      63,    12,    63,    15,    65,    66,     8,    68,    20,    21,
      12,    65,    66,    63,    26,    27,    28,    29,    30,    31,
      32,     9,    10,    13,     8,    56,    57,    58,    12,    17,
       8,    19,    63,    63,    12,    65,    66,    67,    68,     8,
      23,    24,    18,    12,     7,    63,     7,    11,    13,    61,
      63,    11,    11,    63,    11,     7,     7,    63,    11,     5,
      42,     6,    12,    11,    11,    61,    66,    11,    11,   139,
     109,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    20,    21,    26,    27,    28,    29,    30,    31,
      32,    61,    70,    71,    72,    73,    74,    75,    87,    88,
      91,    94,    95,    96,    23,    24,    63,    65,    66,    67,
      68,    63,    63,    63,    34,     0,    72,     9,    14,    25,
      93,    13,    63,    76,    63,    63,    63,     3,     4,     7,
       8,    63,    92,    56,    57,    58,    63,    77,    86,    76,
       3,    12,     3,    10,    14,    63,     3,    16,    59,    60,
      63,    65,    66,    68,    78,    79,    80,    81,    83,    86,
      87,    77,    63,    89,    90,    78,    63,    16,    63,    81,
      83,     7,    63,    83,    83,    65,    66,     4,    79,    86,
      87,     9,    17,    19,    63,    82,    13,    18,     4,    12,
       4,     7,    63,    63,    60,    63,    67,    81,    84,    85,
      11,    83,    83,    11,    13,    81,     5,     7,    11,    63,
      63,    90,    11,    84,     7,    81,    63,    82,     8,    12,
      10,    63,    84,    11,    12,     8,    84,    82,    11,    85,
       6,     8,    11,     8,    11,    11
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = YYLEX;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

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
/* Line 1792 of yacc.c  */
#line 98 "ScpIfParser.y.h"
    { ; }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 101 "ScpIfParser.y.h"
    { CFileTags::m_pFileTags->AppendTopTag((yyvsp[(1) - (1)]));; }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 102 "ScpIfParser.y.h"
    { CFileTags::m_pFileTags->AppendTopTag((yyvsp[(2) - (2)]));; }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 106 "ScpIfParser.y.h"
    { }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 107 "ScpIfParser.y.h"
    {
							(yyvsp[(1) - (1)])->m_nType = eTagType_UsingNamespace;
							(yyval) = (yyvsp[(1) - (1)]);
							printf("tag using namespace\n");
						}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 120 "ScpIfParser.y.h"
    {
		(yyvsp[(1) - (1)])->m_nType = eTagType_Endif;
		(yyval) = (yyvsp[(1) - (1)]);
		printf("tag #endif\n");
	}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 125 "ScpIfParser.y.h"
    {printf("tag comment\n");}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 126 "ScpIfParser.y.h"
    {printf("tag class\n");}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 127 "ScpIfParser.y.h"
    {printf("tag enum \n");}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 128 "ScpIfParser.y.h"
    {printf("tag define\n");}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 129 "ScpIfParser.y.h"
    {printf("tag ifndef\n");}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 130 "ScpIfParser.y.h"
    {printf("tag ifdef\n");}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 131 "ScpIfParser.y.h"
    {printf("tag ifelse\n");}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 134 "ScpIfParser.y.h"
    {
		//
		(yyvsp[(2) - (3)])->m_lstProperty.push_back((yyvsp[(1) - (3)]));
		(yyvsp[(2) - (3)])->m_lstProperty.push_back((yyvsp[(3) - (3)]));
		(yyval) = (yyvsp[(2) - (3)]);
		
		printf("#pragram once: property %d\n", (yyval)->m_lstProperty.size()); 
	}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 145 "ScpIfParser.y.h"
    {
		//
		CTag* pTag = new CTagInclude();
		pTag->m_nType = eTagType_Include;
		pTag->m_lstProperty.push_back((yyvsp[(1) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(3) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(4) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(5) - (5)]));
		
		(yyval) = pTag;
		printf("include 1\n");
	}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 158 "ScpIfParser.y.h"
    {
		//
		CTag* pTag = new CTagInclude();
		pTag->m_nType = eTagType_Include;
		pTag->m_lstProperty.push_back((yyvsp[(1) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(3) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(4) - (5)]));
		pTag->m_lstProperty.push_back((yyvsp[(5) - (5)]));

		(yyval) = pTag;
		printf("include 2\n");
	}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 174 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTagClass();
		pTag->m_nType = eTagType_Class;
		pTag->m_strName = (yyvsp[(2) - (7)])->m_strName;
		//pTag->m_strBody = $1->m_strName + " "+ $2->m_strName + " \r\n" + $3->m_strName + "\r\n" + "\t";
		pTag->m_strBody = pTag->m_strName;//
		pTag->m_lstProperty.push_back((yyvsp[(1) - (7)]));
		//class TypeName
		(yyvsp[(2) - (7)])->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back((yyvsp[(2) - (7)]));
		//继承基类
		pTag->m_lstProperty.push_back((yyvsp[(3) - (7)]));
		//{
		pTag->m_lstProperty.push_back((yyvsp[(4) - (7)]));
		//members list
		(yyvsp[(5) - (7)])->m_pParent = pTag;
		pTag->m_lstProperty.push_back((yyvsp[(5) - (7)])); 
		pTag->m_bHasInterface = (yyvsp[(5) - (7)])->m_bHasInterface;
		//}
		pTag->m_lstProperty.push_back((yyvsp[(6) - (7)]));
		pTag->m_lstProperty.push_back((yyvsp[(7) - (7)]));
		
		
		(yyval) = pTag;
		
		printf("class \n");
	}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 202 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTagClass();
		pTag->m_nType = eTagType_Class;
		pTag->m_strName = (yyvsp[(3) - (8)])->m_strName;
		//pTag->m_strBody = $1->m_strName + " "+ $2->m_strName + " \r\n" + $3->m_strName + "\r\n" + "\t";
		pTag->m_strBody = pTag->m_strName;//
		pTag->m_lstProperty.push_back((yyvsp[(1) - (8)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (8)]));
		//class TypeName
		(yyvsp[(3) - (8)])->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back((yyvsp[(3) - (8)]));
		//继承基类
		pTag->m_lstProperty.push_back((yyvsp[(4) - (8)]));
		//{
		pTag->m_lstProperty.push_back((yyvsp[(5) - (8)]));
		//members list
		(yyvsp[(6) - (8)])->m_pParent = pTag;
		pTag->m_lstProperty.push_back((yyvsp[(6) - (8)])); 
		pTag->m_bHasInterface = (yyvsp[(6) - (8)])->m_bHasInterface;
		//}
		pTag->m_lstProperty.push_back((yyvsp[(7) - (8)]));
		pTag->m_lstProperty.push_back((yyvsp[(8) - (8)]));


		(yyval) = pTag;

		printf("class \n");
	}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 232 "ScpIfParser.y.h"
    {
			CTag* pTag = new CTagBaseClasses();
			pTag->m_nType = eTagType_BaseClasses;

			(yyval) = pTag;
	}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 239 "ScpIfParser.y.h"
    { //: pubic BaseClass
			CTag* pTag = new CTagBaseClasses();
			pTag->m_nType = eTagType_BaseClasses;
			
			pTag->m_lstChild.push_back((yyvsp[(2) - (2)]));

			(yyval) = pTag;
		;
	}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 248 "ScpIfParser.y.h"
    {
		CTag* pTag = (yyvsp[(1) - (3)]);
		pTag->m_nType = eTagType_BaseClasses;

		pTag->m_lstChild.push_back((yyvsp[(2) - (3)]));

		(yyval) = pTag;
		;
	}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 259 "ScpIfParser.y.h"
    {// CA
		CTag* pTag = new CTag;
		pTag->m_nType = eTagType_BaseClass;
		(yyvsp[(1) - (1)])->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back((yyvsp[(1) - (1)]));
		(yyval)=pTag;
		;
	}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 267 "ScpIfParser.y.h"
    {// public CA
		CTag* pTag = new CTag;
		pTag->m_nType = eTagType_BaseClass;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		(yyvsp[(2) - (2)])->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval)=pTag;

		printf("base classes: access name\n");
		//public CBase
		;}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 283 "ScpIfParser.y.h"
    {
			CTag *pTag = new CTag();
			pTag->m_nType = eTagType_Members;   //参数	
			
			(yyval) = pTag;
	}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 289 "ScpIfParser.y.h"
    { //public :
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Members;   //参数	
							(yyvsp[(1) - (2)])->m_pParent = pTag;
							pTag->m_lstChild.push_back((yyvsp[(1) - (2)]));
							delete (yyvsp[(2) - (2)]);

							(yyval) = pTag;
						}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 298 "ScpIfParser.y.h"
    {
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Members;   //参数	
							(yyvsp[(1) - (1)])->m_pParent = pTag;
							pTag->m_lstChild.push_back((yyvsp[(1) - (1)]));
							
							pTag->m_bHasInterface = (yyvsp[(1) - (1)])->m_bHasInterface;

							(yyval) = pTag;
						}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 308 "ScpIfParser.y.h"
    {
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Members;   //参数	
							(yyvsp[(1) - (1)])->m_pParent = pTag;
							pTag->m_lstChild.push_back((yyvsp[(1) - (1)]));
							(yyval) = pTag;
						}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 315 "ScpIfParser.y.h"
    {
							 
							(yyvsp[(2) - (2)])->m_pParent = (yyvsp[(1) - (2)]);
							(yyvsp[(1) - (2)])->m_lstChild.push_back((yyvsp[(2) - (2)]));

							(yyvsp[(1) - (2)])->m_bHasInterface = (yyvsp[(2) - (2)])->m_bHasInterface;

							(yyval) = (yyvsp[(1) - (2)]);
						}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 324 "ScpIfParser.y.h"
    {
							(yyvsp[(2) - (3)])->m_pParent = (yyvsp[(1) - (3)]);
							(yyvsp[(1) - (3)])->m_lstChild.push_back((yyvsp[(2) - (3)]));
							delete (yyvsp[(3) - (3)]);
							(yyval) = (yyvsp[(1) - (3)]);
						}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 331 "ScpIfParser.y.h"
    {
							(yyvsp[(2) - (2)])->m_pParent = (yyvsp[(1) - (2)]);
							(yyvsp[(1) - (2)])->m_lstChild.push_back((yyvsp[(2) - (2)]));
							(yyval) = (yyvsp[(1) - (2)]);
						}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 340 "ScpIfParser.y.h"
    {
							CTag* pTag = (yyvsp[(1) - (1)]);

							(yyval) = pTag;
							printf("member var\n");
						}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 346 "ScpIfParser.y.h"
    {
							CTag* pTag = (yyvsp[(1) - (1)]);

							(yyval) = pTag;
							printf("member function\n");
						}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 354 "ScpIfParser.y.h"
    { // int nCount;
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Variant;   //变量
							
							pTag->m_strName = (yyvsp[(2) - (3)])->m_strName;
							pTag->m_lstProperty.push_back((yyvsp[(1) - (3)]));
							pTag->m_lstProperty.push_back((yyvsp[(2) - (3)]));
							pTag->m_lstProperty.push_back((yyvsp[(3) - (3)]));
							
							(yyval) = pTag;			
							printf("member var0\n");
						}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 367 "ScpIfParser.y.h"
    {

		CTag *pTag = new CTag();
		pTag->m_nType = eTagType_Variant;   //变量

		pTag->m_strName = (yyvsp[(2) - (3)])->m_strName;

		CTagType* pTagType = new CTagType;
		pTagType->m_strName = (yyvsp[(1) - (3)])->m_strName;
		pTagType->m_strBody = (yyvsp[(1) - (3)])->m_strBody;
		pTag->m_lstProperty.push_back(pTagType);
		delete (yyvsp[(1) - (3)]);

		(yyvsp[(2) - (3)])->m_nType = eTagType_VarName;
		pTag->m_lstProperty.push_back((yyvsp[(2) - (3)]));
		pTag->m_lstProperty.push_back((yyvsp[(3) - (3)]));

		(yyval) = pTag;	
		printf("member var1\n");

	}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 390 "ScpIfParser.y.h"
    {
						CTag* pTag = new CTagType;
						pTag->m_nType = eTagType_VarType;
						pTag->m_strName = (yyvsp[(1) - (1)])->m_strName;
						pTag->m_strBody = (yyvsp[(1) - (1)])->m_strName;
						
						pTag->m_lstProperty.push_back((yyvsp[(1) - (1)]));
						(yyvsp[(1) - (1)])->m_nType = eTagType_VarType;
						
						(yyval) = pTag;
						
						printf("normal type \n");
					}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 403 "ScpIfParser.y.h"
    {
						CTag* pTag = new CTagType;
						pTag->m_nType = eTagType_VarType;
						pTag->m_strName = (yyvsp[(1) - (1)])->m_strName;
						
						pTag->m_lstProperty.push_back((yyvsp[(1) - (1)]));
						(yyvsp[(1) - (1)])->m_nType = eTagType_VarType;
						
						(yyval) = pTag;
						
						printf("void type \n");
					}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 415 "ScpIfParser.y.h"
    { //int m_nA;//指针类型  这会先匹配 普通类型 然后匹配指针
						CTag* pTag = (yyvsp[(1) - (2)]);
						 
						pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));
						pTag->m_strName = (yyvsp[(1) - (2)])->m_strName + (yyvsp[(2) - (2)])->m_strName;
						pTag->m_strBody = pTag->m_strName;
						(yyval) = pTag;
						printf("pointer type\n");
						}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 424 "ScpIfParser.y.h"
    { //int& m_nA;//引用  这会先匹配普通类型 然后匹配引用
						CTag* pTag = (yyvsp[(1) - (2)]);
						 
						pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));
						pTag->m_strName = (yyvsp[(1) - (2)])->m_strName + (yyvsp[(2) - (2)])->m_strName;
						pTag->m_strBody = pTag->m_strName;
						
						(yyval) = pTag;
						printf("pointer type\n");
					   }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 435 "ScpIfParser.y.h"
    {
						CTag* pTag = (yyvsp[(1) - (4)]);
						(yyvsp[(1) - (4)])->m_nType = eTagType_VarType;
						//<
						(yyvsp[(1) - (4)])->m_lstProperty.push_back((yyvsp[(2) - (4)]));
						// vartype
						(yyvsp[(1) - (4)])->m_lstProperty.push_back((yyvsp[(3) - (4)]));
						//>
						(yyvsp[(1) - (4)])->m_lstProperty.push_back((yyvsp[(4) - (4)]));
						
						(yyvsp[(1) - (4)])->m_strName = (yyvsp[(1) - (4)])->m_strName +  (yyvsp[(2) - (4)])->m_strName + (yyvsp[(3) - (4)])->m_strName + (yyvsp[(4) - (4)])->m_strName ;
						pTag->m_strBody = pTag->m_strName;

						(yyval) = (yyvsp[(1) - (4)]);
						printf("template type\n");
					}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 453 "ScpIfParser.y.h"
    {//普通名字
						(yyvsp[(1) - (1)])->m_nType = eTagType_VarName;
						(yyvsp[(1) - (1)])->m_strName = (yyvsp[(1) - (1)])->m_strBody;
						(yyval) = (yyvsp[(1) - (1)]);
					}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 459 "ScpIfParser.y.h"
    {//数组
						(yyvsp[(1) - (4)])->m_nType = eTagType_VarName;
						(yyvsp[(1) - (4)])->m_strBody = (yyvsp[(1) - (4)])->m_strBody + (yyvsp[(2) - (4)])->m_strBody + (yyvsp[(3) - (4)])->m_strBody + (yyvsp[(4) - (4)])->m_strBody;

						(yyvsp[(1) - (4)])->m_lstProperty.push_back((yyvsp[(2) - (4)]));
						(yyvsp[(1) - (4)])->m_lstProperty.push_back((yyvsp[(3) - (4)]));
						(yyvsp[(1) - (4)])->m_lstProperty.push_back((yyvsp[(4) - (4)]));
						
						(yyval) = (yyvsp[(1) - (4)]);
					}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 471 "ScpIfParser.y.h"
    {
						//构造函数  CAClass();
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Constructor;  	
												
						pTag->m_strName = (yyvsp[(1) - (5)])->m_strBody;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody + " " +  $3->m_strBody + " " +  $4->m_strBody;
						pTag->m_strBody = pTag->m_strName;
						
						(yyvsp[(1) - (5)])->m_nType = eTagType_FunctionName;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (5)]));
						
						(yyvsp[(2) - (5)])->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back((yyvsp[(2) - (5)]));
						
						(yyvsp[(3) - (5)])->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back((yyvsp[(3) - (5)]));
						
						(yyvsp[(4) - (5)])->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back((yyvsp[(4) - (5)]));
						
						(yyval) = pTag;
						printf("construct fun /no return function \n");
					}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 495 "ScpIfParser.y.h"
    {
						//析构函数 ~fun(void);
						
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Destructor;   //参数	
						
						pTag->m_strName = (yyvsp[(1) - (6)])->m_strBody + (yyvsp[(2) - (6)])->m_strBody;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody + " " +  $3->m_strBody +  " " + $4->m_strBody+  " " + $5->m_strBody;
						pTag->m_strBody = pTag->m_strName;
						
						(yyvsp[(1) - (6)])->m_nType = eTagType_FunctionName;
						(yyvsp[(1) - (6)])->m_strName =  (yyvsp[(1) - (6)])->m_strBody + (yyvsp[(2) - (6)])->m_strBody;
						(yyvsp[(1) - (6)])->m_strBody = (yyvsp[(1) - (6)])->m_strName;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (6)]));
						delete (yyvsp[(2) - (6)]);
						//pTag->m_lstProperty.push_back($2);
						//(
						(yyvsp[(3) - (6)])->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back((yyvsp[(3) - (6)]));
						//
						(yyvsp[(4) - (6)])->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back((yyvsp[(4) - (6)]));
						//)
						(yyvsp[(5) - (6)])->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back((yyvsp[(5) - (6)]));
						
						(yyval) = pTag;
						printf("distruct fun\n");
					}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 524 "ScpIfParser.y.h"
    {
						//析构函数 virtual ~fun(void);
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Destructor;   //参数	
						
						pTag->m_strName = (yyvsp[(2) - (7)])->m_strBody + (yyvsp[(3) - (7)])->m_strBody;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody + " " +  $3->m_strBody + " " +  $4->m_strBody + " " +  $5->m_strBody +  " " + $6->m_strBody;;
						pTag->m_strBody = pTag->m_strName;
						
						(yyvsp[(1) - (7)])->m_nType = eTagType_Virtual;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (7)]));
						//~Fun
						(yyvsp[(2) - (7)])->m_nType = eTagType_FunctionName;
						(yyvsp[(2) - (7)])->m_strName = (yyvsp[(2) - (7)])->m_strName + (yyvsp[(3) - (7)])->m_strName;
						(yyvsp[(2) - (7)])->m_strBody = (yyvsp[(2) - (7)])->m_strName ;;
						pTag->m_lstProperty.push_back((yyvsp[(2) - (7)]));
						delete (yyvsp[(3) - (7)]);
						//pTag->m_lstProperty.push_back($3);
						//(
						(yyvsp[(4) - (7)])->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back((yyvsp[(4) - (7)]));
						//params
						(yyvsp[(5) - (7)])->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back((yyvsp[(5) - (7)]));
						//;
						(yyvsp[(6) - (7)])->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back((yyvsp[(6) - (7)]));
						
						(yyval) = pTag;
						printf("virtual distruct fun\n");
					}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 555 "ScpIfParser.y.h"
    {
						//   void fun(void);
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Function;   //参数	
						
						pTag->m_strName = (yyvsp[(2) - (6)])->m_strBody ;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody +  " " + $3->m_strBody +  " " + $4->m_strBody +  " " + $5->m_strBody ;
						pTag->m_strBody = pTag->m_strName;
						//return
						(yyvsp[(1) - (6)])->m_nType = eTagType_Return;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (6)]));
						//fun name
						(yyvsp[(2) - (6)])->m_nType = eTagType_FunctionName;
						pTag->m_lstProperty.push_back((yyvsp[(2) - (6)]));
						//(
						(yyvsp[(3) - (6)])->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back((yyvsp[(3) - (6)]));
						//params
						(yyvsp[(4) - (6)])->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back((yyvsp[(4) - (6)]));
						//)
						(yyvsp[(5) - (6)])->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back((yyvsp[(5) - (6)]));
						
						(yyval) = pTag;
						printf("member  fun\n");
					}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 582 "ScpIfParser.y.h"
    {
						// 虚析构函数也走这里了
						//  virtual void fun(void);
						CTag *pTag = (yyvsp[(2) - (2)]);
						pTag->m_nType = (yyvsp[(2) - (2)])->m_nType;   //参数	
						
						//pTag->m_strName = $2->m_strBody;
						//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;;

						//
						(yyvsp[(1) - (2)])->m_nType = eTagType_Virtual;
						pTag->m_lstProperty.push_front((yyvsp[(1) - (2)]));
						
						(yyval) = pTag;
						printf("virtual member  fun\n");
					}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 598 "ScpIfParser.y.h"
    {
						 // IF_FUN virtual void fun(void);
						CTag *pTag = (yyvsp[(2) - (2)]);
						pTag->m_nType = eTagType_IfFunction;   	
						
						pTag->m_strName = (yyvsp[(2) - (2)])->m_strName;
						//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
						pTag->m_strBody = pTag->m_strName;
						
						pTag->m_lstProperty.push_front((yyvsp[(1) - (2)]));
						(yyvsp[(1) - (2)])->m_nType = eTagType_TokenIfFun;
						
						pTag->m_bHasInterface = true;

						(yyval) = pTag;
						printf("If Function\n");
					}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 615 "ScpIfParser.y.h"
    {
		// IF_FUN virtual void fun(void);
		CTag *pTag = (yyvsp[(3) - (3)]);
		pTag->m_nType = eTagType_IfFunction;   	

		//pTag->m_strName = $2->m_strName;
		//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
		pTag->m_strBody = pTag->m_strName;

		pTag->m_lstProperty.push_front((yyvsp[(2) - (3)]));
		pTag->m_lstProperty.push_front((yyvsp[(1) - (3)]));
		(yyvsp[(1) - (3)])->m_nType = eTagType_TokenIfFun;

		pTag->m_bHasInterface = true;
		pTag->m_bIsOneWay = true;

		(yyval) = pTag;
		printf("If Function one way\n");
	}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 635 "ScpIfParser.y.h"
    {
						// IF_SLOT virtual void fun(void);
						CTag *pTag = (yyvsp[(2) - (2)]);
						pTag->m_nType = eTagType_IfSlot;   //参数	
						
						pTag->m_strName = (yyvsp[(2) - (2)])->m_strName;
						//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
						pTag->m_strBody = pTag->m_strName;
					
						pTag->m_bHasInterface = true;

						pTag->m_lstProperty.push_front((yyvsp[(1) - (2)]));
						(yyvsp[(1) - (2)])->m_nType = eTagType_TokenIfSlot;

						(yyval) = pTag;
						printf(" IF Slot\n");
					}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 652 "ScpIfParser.y.h"
    {
		// IF_SLOT virtual void fun(void);
		CTag *pTag = (yyvsp[(3) - (3)]);
		pTag->m_nType = eTagType_IfSlot;   //参数	

		//pTag->m_strName = $3->m_strName;
		//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
		pTag->m_strBody = pTag->m_strName;

		pTag->m_bHasInterface = true;
		pTag->m_bIsOneWay = true;

		pTag->m_lstProperty.push_front((yyvsp[(2) - (3)]));
		pTag->m_lstProperty.push_front((yyvsp[(1) - (3)]));
		(yyvsp[(1) - (3)])->m_nType = eTagType_TokenIfSlot;

		(yyval) = pTag;
		printf(" IF Slot one way\n");
	}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 673 "ScpIfParser.y.h"
    {
								CTag *pTag = new CTagParams();
								pTag->m_nType = eTagType_Params;   //参数	
								(yyval) = pTag;
							}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 678 "ScpIfParser.y.h"
    { 
								CTag *pTag = new CTagParams();
								pTag->m_nType = eTagType_Params;   //参数	
								pTag->m_lstChild.push_back((yyvsp[(1) - (1)]));
								(yyval) = pTag;
								printf("params 1: \n");
							}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 685 "ScpIfParser.y.h"
    {
								CTag *pTag = (yyvsp[(1) - (3)]);
								pTag->m_lstChild.push_back((yyvsp[(3) - (3)]));
								
								delete (yyvsp[(2) - (3)]);
								
								(yyval) = pTag; 
								printf("params 2: \n");
							}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 696 "ScpIfParser.y.h"
    {	
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Param;   //参数	
						pTag->m_strName = (yyvsp[(1) - (1)])->m_strName;
						
						(yyvsp[(1) - (1)])->m_nType = eTagType_VarType;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (1)]));
						
						(yyval) = pTag;
					;}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 706 "ScpIfParser.y.h"
    {
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Param;   //参数	
						pTag->m_strName = (yyvsp[(2) - (2)])->m_strName;
						
						pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
						pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));
						
						(yyval) = pTag;	
					;}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 716 "ScpIfParser.y.h"
    {// OUT_P int& param;
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Param;   //参数	
						pTag->m_strName = (yyvsp[(3) - (3)])->m_strName;
						
						(yyvsp[(1) - (3)])->m_nType = eTagType_OutParam;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (3)]));
						pTag->m_lstProperty.push_back((yyvsp[(2) - (3)]));
						pTag->m_lstProperty.push_back((yyvsp[(3) - (3)]));
						
						(yyval) = pTag;	
					;}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 731 "ScpIfParser.y.h"
    {
							(yyvsp[(1) - (1)])->m_nType = eTagType_Access;
							(yyvsp[(1) - (1)])->m_strName = "public";
							//$1->m_strBody = "public:";
							(yyval) = (yyvsp[(1) - (1)]);
							printf("access public:\n");
						}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 738 "ScpIfParser.y.h"
    {
							(yyvsp[(1) - (1)])->m_nType = eTagType_Access;
							(yyvsp[(1) - (1)])->m_strName = "protected";
							//$1->m_strBody = "protected:";
							(yyval) = (yyvsp[(1) - (1)]);
						}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 744 "ScpIfParser.y.h"
    {
							(yyvsp[(1) - (1)])->m_nType = eTagType_Access;
							(yyvsp[(1) - (1)])->m_strName = "private";
							//$1->m_strBody = "private:";
							(yyval) = (yyvsp[(1) - (1)]);
						}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 755 "ScpIfParser.y.h"
    {
							
							printf("comment line\n");
							(yyval) = (yyvsp[(1) - (1)]);
						}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 760 "ScpIfParser.y.h"
    {
							printf("comment block\n");
							(yyval) = (yyvsp[(1) - (1)]);
						}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 767 "ScpIfParser.y.h"
    {
							CTag *pTag = new CTagEnum();
							pTag->m_nType = eTagType_Enum;
							pTag->m_strName = (yyvsp[(3) - (8)])->m_strName;
							pTag->m_lstProperty.push_back((yyvsp[(1) - (8)]));
							pTag->m_lstProperty.push_back((yyvsp[(2) - (8)]));
							//name
							pTag->m_lstProperty.push_back((yyvsp[(3) - (8)]));
							pTag->m_lstProperty.push_back((yyvsp[(4) - (8)]));
							//enum members;
							pTag->m_lstProperty.push_back((yyvsp[(5) - (8)]));
							pTag->m_lstProperty.push_back((yyvsp[(6) - (8)]));
							//typedef name
							pTag->m_lstProperty.push_back((yyvsp[(7) - (8)]));
							pTag->m_lstProperty.push_back((yyvsp[(8) - (8)]));

							(yyval) = pTag;
							printf("enum\n");
						}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 788 "ScpIfParser.y.h"
    {
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_EnumMembers;   //参数	
							
							(yyval) = pTag;
						}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 794 "ScpIfParser.y.h"
    {
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_EnumMembers;   //参数	
							pTag->m_lstChild.push_back((yyvsp[(1) - (1)]));
							
							(yyval) = pTag;
						}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 801 "ScpIfParser.y.h"
    {

							(yyvsp[(1) - (3)])->m_lstChild.push_back((yyvsp[(3) - (3)]));
							
							(yyval) = (yyvsp[(1) - (3)]);
						;
						}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 808 "ScpIfParser.y.h"
    {

							(yyvsp[(1) - (4)])->m_lstChild.push_back((yyvsp[(3) - (4)]));
							
							(yyval) = (yyvsp[(1) - (4)]);
						;
						}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 817 "ScpIfParser.y.h"
    {
						CTag* pTag = new CTag();
						pTag->m_nType = eTagType_EnumMember;
						pTag->m_strName = (yyvsp[(1) - (3)])->m_strName;

						pTag->m_lstProperty.push_back((yyvsp[(1) - (3)]));
						pTag->m_lstProperty.push_back((yyvsp[(2) - (3)]));
						pTag->m_lstProperty.push_back((yyvsp[(3) - (3)]));

						(yyval) = pTag;	
					}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 828 "ScpIfParser.y.h"
    {
						CTag* pTag = new CTag();
						pTag->m_nType = eTagType_EnumMember;
						pTag->m_strName = (yyvsp[(1) - (1)])->m_strName;
						pTag->m_lstProperty.push_back((yyvsp[(1) - (1)]));
							
						(yyval) = pTag;
					}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 851 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = (yyvsp[(1) - (3)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (3)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (3)]));
		pTag->m_lstProperty.push_back((yyvsp[(3) - (3)]));

		(yyval) = pTag;
	}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 863 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = (yyvsp[(1) - (2)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval) = pTag;
	}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 874 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = (yyvsp[(1) - (2)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval) = pTag;
	}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 885 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = (yyvsp[(1) - (2)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval) = pTag;
	}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 896 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = (yyvsp[(1) - (2)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval) = pTag;
	}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 927 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		//pTag->m_nType = eTagType_Define;
		pTag->m_nType = eTagType_DefineContents;
		pTag->m_strName = "define context";
		(yyval) = pTag;
	}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 934 "ScpIfParser.y.h"
    {
		CTag* pTag = (CTag* )(yyvsp[(1) - (2)]);
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));
		(yyval) = pTag;
	}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 942 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Ifndef;
		pTag->m_strName = (yyvsp[(1) - (2)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval) = pTag;
	}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 955 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Ifdef;
		pTag->m_strName = (yyvsp[(1) - (2)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (2)]));
		pTag->m_lstProperty.push_back((yyvsp[(2) - (2)]));

		(yyval) = pTag;
	}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 968 "ScpIfParser.y.h"
    {
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Ifelse;
		pTag->m_strName = (yyvsp[(1) - (1)])->m_strName;

		pTag->m_lstProperty.push_back((yyvsp[(1) - (1)]));
		
		//pTag->m_lstProperty.push_back($2);

		(yyval) = pTag;
	}
    break;


/* Line 1792 of yacc.c  */
#line 2728 "ScpIfParserY.cpp"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 980 "ScpIfParser.y.h"

void yyerror(char* s)
{
    printf("%s: %s\n", __FUNCTION__, s);
}
 