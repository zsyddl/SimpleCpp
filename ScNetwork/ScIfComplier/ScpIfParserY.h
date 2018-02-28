/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
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
