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
    CLASS = 258,                   /* CLASS  */
    IMPLEMENT = 259,               /* IMPLEMENT  */
    FUNC = 260,                    /* FUNC  */
    ATTRIBUTE = 261,               /* ATTRIBUTE  */
    PUBLIC = 262,                  /* PUBLIC  */
    PRIVATE = 263,                 /* PRIVATE  */
    LOCAL = 264,                   /* LOCAL  */
    IF = 265,                      /* IF  */
    THEN = 266,                    /* THEN  */
    ELSE = 267,                    /* ELSE  */
    WHILE = 268,                   /* WHILE  */
    READ = 269,                    /* READ  */
    WRITE = 270,                   /* WRITE  */
    RETURN = 271,                  /* RETURN  */
    ISA = 272,                     /* ISA  */
    INTEGER = 273,                 /* INTEGER  */
    FLOAT = 274,                   /* FLOAT  */
    VOID = 275,                    /* VOID  */
    SELF = 276,                    /* SELF  */
    CONSTRUCT = 277,               /* CONSTRUCT  */
    ID = 278,                      /* ID  */
    LITERAL_INTEGER = 279,         /* LITERAL_INTEGER  */
    LITERAL_FLOAT = 280,           /* LITERAL_FLOAT  */
    OR = 281,                      /* OR  */
    AND = 282,                     /* AND  */
    NOT = 283,                     /* NOT  */
    ARROW_OP = 284,                /* ARROW_OP  */
    ASSIGN_OP = 285,               /* ASSIGN_OP  */
    EQ_OP = 286,                   /* EQ_OP  */
    NE_OP = 287,                   /* NE_OP  */
    LE_OP = 288,                   /* LE_OP  */
    GE_OP = 289,                   /* GE_OP  */
    UMINUS = 290                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "parser.y"

    int ival;
    float fval;
    char *sval;
    struct ASTNode *node;

#line 106 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
