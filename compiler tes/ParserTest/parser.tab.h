/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    LITERAL_INTEGER = 259,         /* LITERAL_INTEGER  */
    LITERAL_FLOAT = 260,           /* LITERAL_FLOAT  */
    CLASS = 261,                   /* CLASS  */
    IMPLEMENT = 262,               /* IMPLEMENT  */
    ISA = 263,                     /* ISA  */
    PUBLIC = 264,                  /* PUBLIC  */
    PRIVATE = 265,                 /* PRIVATE  */
    LOCAL = 266,                   /* LOCAL  */
    FUNC = 267,                    /* FUNC  */
    CONSTRUCT = 268,               /* CONSTRUCT  */
    ATTRIBUTE = 269,               /* ATTRIBUTE  */
    IF = 270,                      /* IF  */
    THEN = 271,                    /* THEN  */
    ELSE = 272,                    /* ELSE  */
    WHILE = 273,                   /* WHILE  */
    READ = 274,                    /* READ  */
    WRITE = 275,                   /* WRITE  */
    RETURN = 276,                  /* RETURN  */
    NOT = 277,                     /* NOT  */
    OR = 278,                      /* OR  */
    AND = 279,                     /* AND  */
    SELF = 280,                    /* SELF  */
    INTEGER = 281,                 /* INTEGER  */
    FLOAT = 282,                   /* FLOAT  */
    VOID = 283,                    /* VOID  */
    ARROW_OP = 284,                /* ARROW_OP  */
    ASSIGN_OP = 285,               /* ASSIGN_OP  */
    EQ_OP = 286,                   /* EQ_OP  */
    NE_OP = 287,                   /* NE_OP  */
    LE_OP = 288,                   /* LE_OP  */
    GE_OP = 289,                   /* GE_OP  */
    LT = 290,                      /* LT  */
    GT = 291,                      /* GT  */
    PLUS = 292,                    /* PLUS  */
    MINUS = 293,                   /* MINUS  */
    MULT = 294,                    /* MULT  */
    DIV = 295,                     /* DIV  */
    L_PAREN = 296,                 /* L_PAREN  */
    R_PAREN = 297,                 /* R_PAREN  */
    L_BRACE = 298,                 /* L_BRACE  */
    R_BRACE = 299,                 /* R_BRACE  */
    L_BRACKET = 300,               /* L_BRACKET  */
    R_BRACKET = 301,               /* R_BRACKET  */
    SEMICOLON = 302,               /* SEMICOLON  */
    COMMA = 303,                   /* COMMA  */
    DOT = 304,                     /* DOT  */
    COLON = 305                    /* COLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "parser.y"

    int     ival;       /* For LITERAL_INTEGER */
    float   fval;       /* For LITERAL_FLOAT */
    char   *sval;       /* For ID */
    void   *node;       /* For AST Nodes */

#line 121 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
