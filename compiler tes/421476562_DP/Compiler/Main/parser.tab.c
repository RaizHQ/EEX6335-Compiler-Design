/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

/* ========================================================================== */
/* 1. Declarations Section                                                    */
/* ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* AST and Semantic Headers */
#include "ast.h"
#include "symbol_table.h"
#include "../ErrorHandlers/syntax_error_handler.h"
#include "../ErrorHandlers/semantic_error_handler.h"

/* Link to Lexer */
extern int yylex();
extern int yylineno;
extern char* yytext;

/* Global Root Node (Owned by main.c) */
extern ASTNode* rootNode;

/* Error Handling Wrapper */
void yyerror(const char *s);

/* ========================================================================== */
/* Semantic Helpers                                                           */
/* ========================================================================== */

DataType getNodeType(ASTNode* node) {
    if (!node) return TYPE_UNKNOWN;

    switch (node->type) {
        case NODE_INT:      return TYPE_INT;
        case NODE_FLOAT:    return TYPE_FLOAT;
        case NODE_VAR:      return lookupType(node->metaData); /* Symbol Table Lookup */
        case NODE_OP:       
            /* Simplified: Assume arithmetic results in INT unless operands are FLOAT. */
            return TYPE_INT; 
        default:            return TYPE_UNKNOWN;
    }
}

#line 116 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_LITERAL_INTEGER = 4,            /* LITERAL_INTEGER  */
  YYSYMBOL_LITERAL_FLOAT = 5,              /* LITERAL_FLOAT  */
  YYSYMBOL_CLASS = 6,                      /* CLASS  */
  YYSYMBOL_IMPLEMENT = 7,                  /* IMPLEMENT  */
  YYSYMBOL_ISA = 8,                        /* ISA  */
  YYSYMBOL_PUBLIC = 9,                     /* PUBLIC  */
  YYSYMBOL_PRIVATE = 10,                   /* PRIVATE  */
  YYSYMBOL_LOCAL = 11,                     /* LOCAL  */
  YYSYMBOL_FUNC = 12,                      /* FUNC  */
  YYSYMBOL_CONSTRUCT = 13,                 /* CONSTRUCT  */
  YYSYMBOL_ATTRIBUTE = 14,                 /* ATTRIBUTE  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_THEN = 16,                      /* THEN  */
  YYSYMBOL_ELSE = 17,                      /* ELSE  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_READ = 19,                      /* READ  */
  YYSYMBOL_WRITE = 20,                     /* WRITE  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_NOT = 22,                       /* NOT  */
  YYSYMBOL_OR = 23,                        /* OR  */
  YYSYMBOL_AND = 24,                       /* AND  */
  YYSYMBOL_SELF = 25,                      /* SELF  */
  YYSYMBOL_INTEGER = 26,                   /* INTEGER  */
  YYSYMBOL_FLOAT = 27,                     /* FLOAT  */
  YYSYMBOL_VOID = 28,                      /* VOID  */
  YYSYMBOL_ARROW_OP = 29,                  /* ARROW_OP  */
  YYSYMBOL_ASSIGN_OP = 30,                 /* ASSIGN_OP  */
  YYSYMBOL_EQ_OP = 31,                     /* EQ_OP  */
  YYSYMBOL_NE_OP = 32,                     /* NE_OP  */
  YYSYMBOL_LE_OP = 33,                     /* LE_OP  */
  YYSYMBOL_GE_OP = 34,                     /* GE_OP  */
  YYSYMBOL_LT = 35,                        /* LT  */
  YYSYMBOL_GT = 36,                        /* GT  */
  YYSYMBOL_PLUS = 37,                      /* PLUS  */
  YYSYMBOL_MINUS = 38,                     /* MINUS  */
  YYSYMBOL_MULT = 39,                      /* MULT  */
  YYSYMBOL_DIV = 40,                       /* DIV  */
  YYSYMBOL_L_PAREN = 41,                   /* L_PAREN  */
  YYSYMBOL_R_PAREN = 42,                   /* R_PAREN  */
  YYSYMBOL_L_BRACE = 43,                   /* L_BRACE  */
  YYSYMBOL_R_BRACE = 44,                   /* R_BRACE  */
  YYSYMBOL_L_BRACKET = 45,                 /* L_BRACKET  */
  YYSYMBOL_R_BRACKET = 46,                 /* R_BRACKET  */
  YYSYMBOL_SEMICOLON = 47,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 48,                     /* COMMA  */
  YYSYMBOL_DOT = 49,                       /* DOT  */
  YYSYMBOL_COLON = 50,                     /* COLON  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_prog = 52,                      /* prog  */
  YYSYMBOL_classOrImplOrFunc_ZM = 53,      /* classOrImplOrFunc_ZM  */
  YYSYMBOL_classOrImplOrFunc = 54,         /* classOrImplOrFunc  */
  YYSYMBOL_classDecl = 55,                 /* classDecl  */
  YYSYMBOL_isa_Id_ZO = 56,                 /* isa_Id_ZO  */
  YYSYMBOL_commaID_ZM = 57,                /* commaID_ZM  */
  YYSYMBOL_visibility_memberDecl_ZM = 58,  /* visibility_memberDecl_ZM  */
  YYSYMBOL_implDef = 59,                   /* implDef  */
  YYSYMBOL_funcDef_ZM = 60,                /* funcDef_ZM  */
  YYSYMBOL_funcDef = 61,                   /* funcDef  */
  YYSYMBOL_funcHead = 62,                  /* funcHead  */
  YYSYMBOL_funcBody = 63,                  /* funcBody  */
  YYSYMBOL_visibility = 64,                /* visibility  */
  YYSYMBOL_memberDecl = 65,                /* memberDecl  */
  YYSYMBOL_funcDecl = 66,                  /* funcDecl  */
  YYSYMBOL_attributeDecl = 67,             /* attributeDecl  */
  YYSYMBOL_localVarDecl = 68,              /* localVarDecl  */
  YYSYMBOL_varDecl = 69,                   /* varDecl  */
  YYSYMBOL_arraySize_ZM = 70,              /* arraySize_ZM  */
  YYSYMBOL_arraySize = 71,                 /* arraySize  */
  YYSYMBOL_varDeclOrStmt_ZM = 72,          /* varDeclOrStmt_ZM  */
  YYSYMBOL_varDeclOrStmt = 73,             /* varDeclOrStmt  */
  YYSYMBOL_statement = 74,                 /* statement  */
  YYSYMBOL_matchedStmt = 75,               /* matchedStmt  */
  YYSYMBOL_unmatchedStmt = 76,             /* unmatchedStmt  */
  YYSYMBOL_statement_ZM = 77,              /* statement_ZM  */
  YYSYMBOL_expr = 78,                      /* expr  */
  YYSYMBOL_relExpr = 79,                   /* relExpr  */
  YYSYMBOL_arithExpr = 80,                 /* arithExpr  */
  YYSYMBOL_sign = 81,                      /* sign  */
  YYSYMBOL_term = 82,                      /* term  */
  YYSYMBOL_factor = 83,                    /* factor  */
  YYSYMBOL_variable = 84,                  /* variable  */
  YYSYMBOL_primaryVar = 85,                /* primaryVar  */
  YYSYMBOL_idOrSelf = 86,                  /* idOrSelf  */
  YYSYMBOL_suffix_ZM = 87,                 /* suffix_ZM  */
  YYSYMBOL_suffix = 88,                    /* suffix  */
  YYSYMBOL_indice_ZM = 89,                 /* indice_ZM  */
  YYSYMBOL_indice = 90,                    /* indice  */
  YYSYMBOL_functionCall = 91,              /* functionCall  */
  YYSYMBOL_fParams = 92,                   /* fParams  */
  YYSYMBOL_fParamsTail_ZM = 93,            /* fParamsTail_ZM  */
  YYSYMBOL_fParamsTail = 94,               /* fParamsTail  */
  YYSYMBOL_aParams = 95,                   /* aParams  */
  YYSYMBOL_aParamsTail_ZM = 96,            /* aParamsTail_ZM  */
  YYSYMBOL_aParamsTail = 97,               /* aParamsTail  */
  YYSYMBOL_type = 98,                      /* type  */
  YYSYMBOL_returnType = 99,                /* returnType  */
  YYSYMBOL_assignOp = 100,                 /* assignOp  */
  YYSYMBOL_relOp = 101,                    /* relOp  */
  YYSYMBOL_addOp = 102,                    /* addOp  */
  YYSYMBOL_multOp = 103                    /* multOp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  203

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   120,   120,   127,   133,   137,   138,   139,   145,   153,
     154,   158,   159,   163,   164,   170,   178,   183,   189,   202,
     221,   231,   239,   240,   244,   245,   249,   254,   258,   262,
     278,   279,   283,   284,   290,   295,   299,   300,   304,   305,
     309,   315,   320,   326,   332,   337,   342,   358,   366,   372,
     378,   387,   392,   398,   402,   406,   410,   414,   418,   419,
     423,   427,   431,   432,   433,   434,   435,   436,   437,   453,
     460,   464,   465,   469,   470,   474,   475,   479,   480,   484,
     490,   501,   511,   515,   516,   520,   533,   538,   542,   547,
     551,   557,   558,   559,   563,   564,   568,   572,   573,   574,
     575,   576,   577,   581,   582,   586,   587,   588
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID",
  "LITERAL_INTEGER", "LITERAL_FLOAT", "CLASS", "IMPLEMENT", "ISA",
  "PUBLIC", "PRIVATE", "LOCAL", "FUNC", "CONSTRUCT", "ATTRIBUTE", "IF",
  "THEN", "ELSE", "WHILE", "READ", "WRITE", "RETURN", "NOT", "OR", "AND",
  "SELF", "INTEGER", "FLOAT", "VOID", "ARROW_OP", "ASSIGN_OP", "EQ_OP",
  "NE_OP", "LE_OP", "GE_OP", "LT", "GT", "PLUS", "MINUS", "MULT", "DIV",
  "L_PAREN", "R_PAREN", "L_BRACE", "R_BRACE", "L_BRACKET", "R_BRACKET",
  "SEMICOLON", "COMMA", "DOT", "COLON", "$accept", "prog",
  "classOrImplOrFunc_ZM", "classOrImplOrFunc", "classDecl", "isa_Id_ZO",
  "commaID_ZM", "visibility_memberDecl_ZM", "implDef", "funcDef_ZM",
  "funcDef", "funcHead", "funcBody", "visibility", "memberDecl",
  "funcDecl", "attributeDecl", "localVarDecl", "varDecl", "arraySize_ZM",
  "arraySize", "varDeclOrStmt_ZM", "varDeclOrStmt", "statement",
  "matchedStmt", "unmatchedStmt", "statement_ZM", "expr", "relExpr",
  "arithExpr", "sign", "term", "factor", "variable", "primaryVar",
  "idOrSelf", "suffix_ZM", "suffix", "indice_ZM", "indice", "functionCall",
  "fParams", "fParamsTail_ZM", "fParamsTail", "aParams", "aParamsTail_ZM",
  "aParamsTail", "type", "returnType", "assignOp", "relOp", "addOp",
  "multOp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-136)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     117,    21,    25,    42,    15,    76,  -136,   117,  -136,  -136,
    -136,    39,    77,    48,    38,    85,  -136,  -136,    11,  -136,
     111,    82,    53,    85,    81,    90,  -136,   123,    92,    93,
      94,    95,    96,  -136,    32,  -136,    99,    11,  -136,  -136,
    -136,   108,    -8,  -136,    97,    91,    64,   101,    53,   100,
      41,  -136,   102,  -136,    55,    55,    45,    55,    55,    32,
     103,  -136,  -136,  -136,    55,    55,    55,  -136,    59,   104,
     105,  -136,   137,  -136,  -136,  -136,   107,    98,  -136,  -136,
     119,  -136,  -136,  -136,   109,    41,  -136,  -136,    55,  -136,
    -136,    55,   115,    84,    55,    66,  -136,  -136,  -136,   116,
     118,    59,   120,  -136,   121,  -136,  -136,   112,   113,   122,
      49,    55,  -136,    45,  -136,    91,   124,   123,   125,    64,
    -136,  -136,    36,     9,   126,   109,   109,  -136,    61,   130,
    -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,    55,    55,
    -136,  -136,  -136,  -136,    55,    32,   128,   129,   133,  -136,
      55,  -136,   113,   132,  -136,   127,  -136,  -136,  -136,  -136,
    -136,  -136,  -136,  -136,  -136,   131,  -136,   146,  -136,   126,
    -136,   135,  -136,    32,    71,    66,  -136,   136,   138,  -136,
    -136,  -136,  -136,  -136,    45,   132,  -136,   134,  -136,  -136,
    -136,   148,  -136,  -136,  -136,    41,    32,   109,   139,   140,
    -136,  -136,  -136
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     0,     0,     0,     2,     4,     5,     6,
       7,     0,    10,     0,     0,    82,     1,     3,    35,    18,
       0,     0,    17,    82,     0,     0,    71,     0,     0,     0,
       0,     0,     0,    72,    52,    36,     0,    35,    37,    38,
      39,     0,    78,    70,     0,    12,    14,     0,    17,     0,
       0,    20,     0,    28,     0,     0,     0,     0,     0,    52,
       0,    21,    34,    96,     0,    87,     0,    69,    78,    74,
      78,    41,     0,     9,    22,    23,     0,     0,    15,    16,
       0,    93,    91,    92,    31,     0,    64,    65,     0,    58,
      59,     0,     0,    55,     0,    57,    61,    62,    63,     0,
       0,    78,     0,    53,     0,    51,    47,     0,    89,     0,
       0,    87,    73,     0,    77,    12,     0,     0,     0,    14,
      24,    25,     0,     0,    84,    31,    31,    67,     0,     0,
     104,    97,   100,   101,   102,    98,    99,   103,     0,     0,
      68,   107,   105,   106,     0,     0,     0,     0,     0,    40,
       0,    86,    89,    80,    79,     0,    75,    11,     8,    27,
      26,    13,    95,    94,    19,     0,    33,     0,    81,    84,
      30,     0,    66,     0,    54,    56,    60,     0,     0,    44,
      45,    46,    90,    88,     0,     0,    32,     0,    83,    29,
      48,    38,    43,    50,    76,     0,     0,    31,     0,     0,
      85,    42,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -136,  -136,   159,  -136,  -136,  -136,    52,    51,  -136,   141,
       1,   114,  -136,  -136,  -136,  -136,  -136,  -136,    56,  -119,
    -136,   151,  -136,   -32,  -127,  -135,   142,   -53,    47,   -57,
     -85,    29,   -73,   -18,   143,  -101,   144,  -136,   145,  -136,
     -17,   167,    23,  -136,    83,    43,  -136,   -82,  -136,  -136,
    -136,  -136,  -136
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,    21,    73,    76,     9,    47,
      10,    11,    19,    77,   119,   120,   121,    35,    53,   124,
     125,    36,    37,    38,    39,    40,    60,   108,   103,    93,
      94,    95,    96,    97,    42,    43,    67,    68,    69,    70,
      98,    25,   168,   169,   109,   151,   152,    84,   164,    64,
     138,   139,   144
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      41,    44,    59,   126,   102,   104,   170,   171,   137,   110,
     178,   107,   156,   165,    26,   127,    41,    44,   177,    41,
      44,   140,    27,    48,    12,   137,    28,    59,    13,    29,
      30,    31,    32,    65,   128,    26,    33,    66,   100,    81,
     163,    41,    44,   137,    81,    14,   191,    28,    26,    48,
      29,    30,    31,    32,    34,   166,    15,    33,    26,    86,
      87,   199,    82,    83,   162,     3,     4,    82,    83,   198,
      33,   176,   130,    74,    75,    34,    16,    88,   200,    23,
      33,   174,    18,   194,   130,    20,    89,    90,    24,   137,
     141,    22,    89,    90,   130,   154,    91,   182,    89,    90,
     111,    92,    99,   172,    66,   142,   143,   130,    89,    90,
       3,     4,   117,   197,    45,   131,   132,   133,   134,   135,
     136,    89,    90,     1,     2,    46,    52,    41,    44,     3,
       4,    50,    51,    54,    55,    56,    57,    58,    63,    72,
     115,   190,    80,    61,    71,    78,   173,   106,   122,   187,
      66,   116,    85,   113,   123,    41,    44,   129,   145,   149,
     146,   150,   147,   148,   153,   196,    17,   157,   175,   185,
     161,   158,   160,   159,   167,   179,   180,   186,    41,    44,
     181,   184,   189,   192,   195,   193,   201,   202,    62,    79,
      49,   118,   188,     0,   155,   183,     0,     0,     0,   101,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,     0,     0,   114
};

static const yytype_int16 yycheck[] =
{
      18,    18,    34,    85,    57,    58,   125,   126,    93,    66,
     145,    64,   113,     4,     3,    88,    34,    34,   145,    37,
      37,    94,    11,    22,     3,   110,    15,    59,     3,    18,
      19,    20,    21,    41,    91,     3,    25,    45,    56,     3,
     122,    59,    59,   128,     3,     3,   173,    15,     3,    48,
      18,    19,    20,    21,    43,    46,    41,    25,     3,     4,
       5,   196,    26,    27,    28,    12,    13,    26,    27,   196,
      25,   144,    23,     9,    10,    43,     0,    22,   197,    41,
      25,   138,    43,   184,    23,     8,    37,    38,     3,   174,
      24,    43,    37,    38,    23,    46,    41,   150,    37,    38,
      41,    54,    55,    42,    45,    39,    40,    23,    37,    38,
      12,    13,    14,   195,     3,    31,    32,    33,    34,    35,
      36,    37,    38,     6,     7,    43,     3,   145,   145,    12,
      13,    50,    42,    41,    41,    41,    41,    41,    30,    48,
       3,   173,    42,    44,    47,    44,    16,    44,    29,     3,
      45,    44,    50,    49,    45,   173,   173,    42,    42,    47,
      42,    48,    42,    42,    42,    17,     7,   115,   139,    42,
     119,    47,    47,   117,    48,    47,    47,    46,   196,   196,
      47,    49,    47,    47,    50,    47,    47,    47,    37,    48,
      23,    77,   169,    -1,   111,   152,    -1,    -1,    -1,    56,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    70
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,    12,    13,    52,    53,    54,    55,    59,
      61,    62,     3,     3,     3,    41,     0,    53,    43,    63,
       8,    56,    43,    41,     3,    92,     3,    11,    15,    18,
      19,    20,    21,    25,    43,    68,    72,    73,    74,    75,
      76,    84,    85,    86,    91,     3,    43,    60,    61,    92,
      50,    42,     3,    69,    41,    41,    41,    41,    41,    74,
      77,    44,    72,    30,   100,    41,    45,    87,    88,    89,
      90,    47,    48,    57,     9,    10,    58,    64,    44,    60,
      42,     3,    26,    27,    98,    50,     4,     5,    22,    37,
      38,    41,    79,    80,    81,    82,    83,    84,    91,    79,
      84,    85,    78,    79,    78,    77,    44,    78,    78,    95,
      80,    41,    87,    49,    89,     3,    44,    14,    62,    65,
      66,    67,    29,    45,    70,    71,    98,    83,    80,    42,
      23,    31,    32,    33,    34,    35,    36,    81,   101,   102,
      83,    24,    39,    40,   103,    42,    42,    42,    42,    47,
      48,    96,    97,    42,    46,    95,    86,    57,    47,    69,
      47,    58,    28,    98,    99,     4,    46,    48,    93,    94,
      70,    70,    42,    16,    80,    82,    83,    75,    76,    47,
      47,    47,    78,    96,    49,    42,    46,     3,    93,    47,
      74,    75,    47,    47,    86,    50,    17,    98,    75,    76,
      70,    47,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    54,    54,    55,    56,
      56,    57,    57,    58,    58,    59,    60,    60,    61,    62,
      62,    63,    64,    64,    65,    65,    66,    67,    68,    69,
      70,    70,    71,    71,    72,    72,    73,    73,    74,    74,
      75,    75,    75,    75,    75,    75,    75,    75,    76,    76,
      76,    77,    77,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    83,    83,    83,    83,    83,    84,
      85,    86,    86,    87,    87,    88,    88,    89,    89,    90,
      91,    92,    92,    93,    93,    94,    95,    95,    96,    96,
      97,    98,    98,    98,    99,    99,   100,   101,   101,   101,
     101,   101,   101,   102,   102,   103,   103,   103
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     7,     3,
       0,     3,     0,     3,     0,     5,     2,     0,     2,     7,
       4,     3,     1,     1,     1,     1,     2,     2,     2,     5,
       2,     0,     3,     2,     2,     0,     1,     1,     1,     1,
       4,     2,     9,     6,     5,     5,     5,     3,     6,     9,
       6,     2,     0,     1,     3,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     2,     2,     2,
       1,     1,     1,     2,     1,     3,     5,     2,     0,     3,
       4,     5,     0,     2,     0,     5,     2,     0,     2,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* prog: classOrImplOrFunc_ZM  */
#line 121 "parser.y"
      { 
        rootNode = (yyvsp[0].node); /* Final assignment to the external root */
      }
#line 1358 "parser.tab.c"
    break;

  case 3: /* classOrImplOrFunc_ZM: classOrImplOrFunc classOrImplOrFunc_ZM  */
#line 128 "parser.y"
      { 
        (yyval.node) = (yyvsp[-1].node); 
        addSibling((yyval.node), (yyvsp[0].node)); 
      }
#line 1367 "parser.tab.c"
    break;

  case 4: /* classOrImplOrFunc_ZM: %empty  */
#line 133 "parser.y"
      { (yyval.node) = NULL; }
#line 1373 "parser.tab.c"
    break;

  case 5: /* classOrImplOrFunc: classDecl  */
#line 137 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1379 "parser.tab.c"
    break;

  case 6: /* classOrImplOrFunc: implDef  */
#line 138 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1385 "parser.tab.c"
    break;

  case 7: /* classOrImplOrFunc: funcDef  */
#line 139 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1391 "parser.tab.c"
    break;

  case 8: /* classDecl: CLASS ID isa_Id_ZO L_BRACE visibility_memberDecl_ZM R_BRACE SEMICOLON  */
#line 146 "parser.y"
      {
          (yyval.node) = createNode(NODE_CLASS);
          (yyval.node)->metaData = (yyvsp[-5].sval); 
      }
#line 1400 "parser.tab.c"
    break;

  case 9: /* isa_Id_ZO: ISA ID commaID_ZM  */
#line 153 "parser.y"
                        { (yyval.node) = NULL; }
#line 1406 "parser.tab.c"
    break;

  case 10: /* isa_Id_ZO: %empty  */
#line 154 "parser.y"
               { (yyval.node) = NULL; }
#line 1412 "parser.tab.c"
    break;

  case 11: /* commaID_ZM: COMMA ID commaID_ZM  */
#line 158 "parser.y"
                          { (yyval.node) = NULL; }
#line 1418 "parser.tab.c"
    break;

  case 12: /* commaID_ZM: %empty  */
#line 159 "parser.y"
                 { (yyval.node) = NULL; }
#line 1424 "parser.tab.c"
    break;

  case 13: /* visibility_memberDecl_ZM: visibility memberDecl visibility_memberDecl_ZM  */
#line 163 "parser.y"
                                                     { (yyval.node) = NULL; }
#line 1430 "parser.tab.c"
    break;

  case 14: /* visibility_memberDecl_ZM: %empty  */
#line 164 "parser.y"
               { (yyval.node) = NULL; }
#line 1436 "parser.tab.c"
    break;

  case 15: /* implDef: IMPLEMENT ID L_BRACE funcDef_ZM R_BRACE  */
#line 171 "parser.y"
      {
          (yyval.node) = createNode(NODE_CLASS); 
          (yyval.node)->metaData = (yyvsp[-3].sval);
      }
#line 1445 "parser.tab.c"
    break;

  case 16: /* funcDef_ZM: funcDef funcDef_ZM  */
#line 179 "parser.y"
      {
          (yyval.node) = (yyvsp[-1].node);
          addSibling((yyval.node), (yyvsp[0].node));
      }
#line 1454 "parser.tab.c"
    break;

  case 17: /* funcDef_ZM: %empty  */
#line 183 "parser.y"
               { (yyval.node) = NULL; }
#line 1460 "parser.tab.c"
    break;

  case 18: /* funcDef: funcHead funcBody  */
#line 190 "parser.y"
      {
          (yyval.node) = createNode(NODE_FUNC);
          (yyval.node)->left = (yyvsp[-1].node);  /* Head */
          (yyval.node)->right = (yyvsp[0].node); /* Body */

          /* SEMANTICS: Exit Scope & Reset Return Type */
          exitScope(); 
          setGlobalReturnType(TYPE_UNKNOWN);
      }
#line 1474 "parser.tab.c"
    break;

  case 19: /* funcHead: FUNC ID L_PAREN fParams R_PAREN ARROW_OP returnType  */
#line 203 "parser.y"
      {
          DataType rType = TYPE_VOID;
          if ((yyvsp[0].node) && (yyvsp[0].node)->metaData) {
              if (strcmp((yyvsp[0].node)->metaData, "integer") == 0) rType = TYPE_INT;
              else if (strcmp((yyvsp[0].node)->metaData, "float") == 0) rType = TYPE_FLOAT;
          }

          /* 1. Insert Function Name (Global) */
          insertSymbol((yyvsp[-5].sval), rType, "FUNC");

          /* 2. Enter Scope (For Body) */
          enterScope((yyvsp[-5].sval));

          /* 3. Set Context */
          setGlobalReturnType(rType);

          (yyval.node) = createIDNode((yyvsp[-5].sval)); 
      }
#line 1497 "parser.tab.c"
    break;

  case 20: /* funcHead: CONSTRUCT L_PAREN fParams R_PAREN  */
#line 222 "parser.y"
      { 
          /* Constructors often imply a scope change too, treating simpler here */
          setGlobalReturnType(TYPE_VOID);
          enterScope("construct");
          (yyval.node) = createIDNode("construct"); 
      }
#line 1508 "parser.tab.c"
    break;

  case 21: /* funcBody: L_BRACE varDeclOrStmt_ZM R_BRACE  */
#line 232 "parser.y"
      { 
          (yyval.node) = createNode(NODE_BLOCK);
          (yyval.node)->left = (yyvsp[-1].node); 
      }
#line 1517 "parser.tab.c"
    break;

  case 22: /* visibility: PUBLIC  */
#line 239 "parser.y"
              { (yyval.node) = NULL; }
#line 1523 "parser.tab.c"
    break;

  case 23: /* visibility: PRIVATE  */
#line 240 "parser.y"
              { (yyval.node) = NULL; }
#line 1529 "parser.tab.c"
    break;

  case 24: /* memberDecl: funcDecl  */
#line 244 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1535 "parser.tab.c"
    break;

  case 25: /* memberDecl: attributeDecl  */
#line 245 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1541 "parser.tab.c"
    break;

  case 26: /* funcDecl: funcHead SEMICOLON  */
#line 249 "parser.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1547 "parser.tab.c"
    break;

  case 27: /* attributeDecl: ATTRIBUTE varDecl  */
#line 254 "parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1553 "parser.tab.c"
    break;

  case 28: /* localVarDecl: LOCAL varDecl  */
#line 258 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1559 "parser.tab.c"
    break;

  case 29: /* varDecl: ID COLON type arraySize_ZM SEMICOLON  */
#line 263 "parser.y"
    {
        DataType vType = TYPE_UNKNOWN;
        if ((yyvsp[-2].node) && (yyvsp[-2].node)->metaData) {
             if (strcmp((yyvsp[-2].node)->metaData, "integer") == 0) vType = TYPE_INT;
             else if (strcmp((yyvsp[-2].node)->metaData, "float") == 0) vType = TYPE_FLOAT;
        }
        insertSymbol((yyvsp[-4].sval), vType, "VAR");

        (yyval.node) = createNode(NODE_VAR_DECL);
        (yyval.node)->metaData = (yyvsp[-4].sval); 
        (yyval.node)->left = (yyvsp[-2].node);     
    }
#line 1576 "parser.tab.c"
    break;

  case 30: /* arraySize_ZM: arraySize arraySize_ZM  */
#line 278 "parser.y"
                             { (yyval.node) = NULL; }
#line 1582 "parser.tab.c"
    break;

  case 31: /* arraySize_ZM: %empty  */
#line 279 "parser.y"
               { (yyval.node) = NULL; }
#line 1588 "parser.tab.c"
    break;

  case 32: /* arraySize: L_BRACKET LITERAL_INTEGER R_BRACKET  */
#line 283 "parser.y"
                                          { (yyval.node) = NULL; }
#line 1594 "parser.tab.c"
    break;

  case 33: /* arraySize: L_BRACKET R_BRACKET  */
#line 284 "parser.y"
                          { (yyval.node) = NULL; }
#line 1600 "parser.tab.c"
    break;

  case 34: /* varDeclOrStmt_ZM: varDeclOrStmt varDeclOrStmt_ZM  */
#line 291 "parser.y"
      {
          (yyval.node) = (yyvsp[-1].node);
          addSibling((yyval.node), (yyvsp[0].node));
      }
#line 1609 "parser.tab.c"
    break;

  case 35: /* varDeclOrStmt_ZM: %empty  */
#line 295 "parser.y"
               { (yyval.node) = NULL; }
#line 1615 "parser.tab.c"
    break;

  case 36: /* varDeclOrStmt: localVarDecl  */
#line 299 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1621 "parser.tab.c"
    break;

  case 37: /* varDeclOrStmt: statement  */
#line 300 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1627 "parser.tab.c"
    break;

  case 38: /* statement: matchedStmt  */
#line 304 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1633 "parser.tab.c"
    break;

  case 39: /* statement: unmatchedStmt  */
#line 305 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1639 "parser.tab.c"
    break;

  case 40: /* matchedStmt: variable assignOp expr SEMICOLON  */
#line 310 "parser.y"
      { 
          (yyval.node) = createNode(NODE_ASSIGN);
          (yyval.node)->left = (yyvsp[-3].node); 
          (yyval.node)->right = (yyvsp[-1].node); 
      }
#line 1649 "parser.tab.c"
    break;

  case 41: /* matchedStmt: functionCall SEMICOLON  */
#line 316 "parser.y"
      { 
          (yyval.node) = createNode(NODE_FUNC_CALL);
          (yyval.node)->left = (yyvsp[-1].node);
      }
#line 1658 "parser.tab.c"
    break;

  case 42: /* matchedStmt: IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE matchedStmt SEMICOLON  */
#line 321 "parser.y"
      { 
           (yyval.node) = createNode(NODE_IF);
           (yyval.node)->left = (yyvsp[-6].node); 
           (yyval.node)->right = (yyvsp[-3].node); 
      }
#line 1668 "parser.tab.c"
    break;

  case 43: /* matchedStmt: WHILE L_PAREN relExpr R_PAREN matchedStmt SEMICOLON  */
#line 327 "parser.y"
      { 
           (yyval.node) = createNode(NODE_WHILE);
           (yyval.node)->left = (yyvsp[-3].node);
           (yyval.node)->right = (yyvsp[-1].node);
      }
#line 1678 "parser.tab.c"
    break;

  case 44: /* matchedStmt: READ L_PAREN variable R_PAREN SEMICOLON  */
#line 333 "parser.y"
      { 
           (yyval.node) = createNode(NODE_READ);
           (yyval.node)->left = (yyvsp[-2].node);
      }
#line 1687 "parser.tab.c"
    break;

  case 45: /* matchedStmt: WRITE L_PAREN expr R_PAREN SEMICOLON  */
#line 338 "parser.y"
      { 
           (yyval.node) = createNode(NODE_WRITE);
           (yyval.node)->left = (yyvsp[-2].node);
      }
#line 1696 "parser.tab.c"
    break;

  case 46: /* matchedStmt: RETURN L_PAREN expr R_PAREN SEMICOLON  */
#line 343 "parser.y"
      { 
           /* SEMANTICS: RETURN TYPE CHECK */
           DataType exprType = getNodeType((yyvsp[-2].node));
           
           if (currentFunctionReturnType != TYPE_UNKNOWN && exprType != TYPE_UNKNOWN) {
               if (currentFunctionReturnType != exprType) {
                   char msg[128];
                   sprintf(msg, "Return type mismatch. Expected type %d, got %d", currentFunctionReturnType, exprType);
                   handleSemanticError(yylineno, msg);
               }
           }

           (yyval.node) = createNode(NODE_RETURN);
           (yyval.node)->left = (yyvsp[-2].node);
      }
#line 1716 "parser.tab.c"
    break;

  case 47: /* matchedStmt: L_BRACE statement_ZM R_BRACE  */
#line 359 "parser.y"
      { 
           (yyval.node) = createNode(NODE_BLOCK);
           (yyval.node)->left = (yyvsp[-1].node);
      }
#line 1725 "parser.tab.c"
    break;

  case 48: /* unmatchedStmt: IF L_PAREN relExpr R_PAREN THEN statement  */
#line 367 "parser.y"
      { 
           (yyval.node) = createNode(NODE_IF);
           (yyval.node)->left = (yyvsp[-3].node);
           (yyval.node)->right = (yyvsp[0].node);
      }
#line 1735 "parser.tab.c"
    break;

  case 49: /* unmatchedStmt: IF L_PAREN relExpr R_PAREN THEN matchedStmt ELSE unmatchedStmt SEMICOLON  */
#line 373 "parser.y"
      { 
           (yyval.node) = createNode(NODE_IF);
           (yyval.node)->left = (yyvsp[-6].node);
           (yyval.node)->right = (yyvsp[-3].node);
      }
#line 1745 "parser.tab.c"
    break;

  case 50: /* unmatchedStmt: WHILE L_PAREN relExpr R_PAREN unmatchedStmt SEMICOLON  */
#line 379 "parser.y"
      { 
           (yyval.node) = createNode(NODE_WHILE);
           (yyval.node)->left = (yyvsp[-3].node);
           (yyval.node)->right = (yyvsp[-1].node);
      }
#line 1755 "parser.tab.c"
    break;

  case 51: /* statement_ZM: statement statement_ZM  */
#line 388 "parser.y"
      {
          (yyval.node) = (yyvsp[-1].node);
          addSibling((yyval.node), (yyvsp[0].node));
      }
#line 1764 "parser.tab.c"
    break;

  case 52: /* statement_ZM: %empty  */
#line 392 "parser.y"
               { (yyval.node) = NULL; }
#line 1770 "parser.tab.c"
    break;

  case 53: /* expr: relExpr  */
#line 398 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1776 "parser.tab.c"
    break;

  case 54: /* relExpr: arithExpr relOp arithExpr  */
#line 403 "parser.y"
      { 
          (yyval.node) = createOpNode("REL_OP", (yyvsp[-2].node), (yyvsp[0].node)); 
      }
#line 1784 "parser.tab.c"
    break;

  case 55: /* relExpr: arithExpr  */
#line 406 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1790 "parser.tab.c"
    break;

  case 56: /* arithExpr: arithExpr addOp term  */
#line 411 "parser.y"
      { 
          (yyval.node) = createOpNode((yyvsp[-1].node)->metaData, (yyvsp[-2].node), (yyvsp[0].node)); 
      }
#line 1798 "parser.tab.c"
    break;

  case 57: /* arithExpr: term  */
#line 414 "parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1804 "parser.tab.c"
    break;

  case 58: /* sign: PLUS  */
#line 418 "parser.y"
            { (yyval.node) = createOpNode("+", NULL, NULL); }
#line 1810 "parser.tab.c"
    break;

  case 59: /* sign: MINUS  */
#line 419 "parser.y"
            { (yyval.node) = createOpNode("-", NULL, NULL); }
#line 1816 "parser.tab.c"
    break;

  case 60: /* term: term multOp factor  */
#line 424 "parser.y"
      { 
          (yyval.node) = createOpNode((yyvsp[-1].node)->metaData, (yyvsp[-2].node), (yyvsp[0].node)); 
      }
#line 1824 "parser.tab.c"
    break;

  case 61: /* term: factor  */
#line 427 "parser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1830 "parser.tab.c"
    break;

  case 62: /* factor: variable  */
#line 431 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1836 "parser.tab.c"
    break;

  case 63: /* factor: functionCall  */
#line 432 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1842 "parser.tab.c"
    break;

  case 64: /* factor: LITERAL_INTEGER  */
#line 433 "parser.y"
                      { (yyval.node) = createIntNode((yyvsp[0].ival)); }
#line 1848 "parser.tab.c"
    break;

  case 65: /* factor: LITERAL_FLOAT  */
#line 434 "parser.y"
                     { (yyval.node) = createFloatNode((yyvsp[0].fval)); }
#line 1854 "parser.tab.c"
    break;

  case 66: /* factor: L_PAREN arithExpr R_PAREN  */
#line 435 "parser.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 1860 "parser.tab.c"
    break;

  case 67: /* factor: NOT factor  */
#line 436 "parser.y"
                 { (yyval.node) = createOpNode("NOT", (yyvsp[0].node), NULL); }
#line 1866 "parser.tab.c"
    break;

  case 68: /* factor: sign factor  */
#line 438 "parser.y"
      { 
          /* SEMANTICS: SIGN RECURSION CHECK (Max 2) */
          if ((yyvsp[0].node)->type == NODE_OP && ((yyvsp[0].node)->metaData[0] == '+' || (yyvsp[0].node)->metaData[0] == '-')) {
              ASTNode* child = (yyvsp[0].node)->left; 
              if (child && child->type == NODE_OP && (child->metaData[0] == '+' || child->metaData[0] == '-')) {
                   handleSemanticError(yylineno, "Too many consecutive signs (max 2 allowed)");
              }
          }
          (yyval.node) = createOpNode((yyvsp[-1].node)->metaData, (yyvsp[0].node), NULL); 
      }
#line 1881 "parser.tab.c"
    break;

  case 69: /* variable: primaryVar suffix_ZM  */
#line 454 "parser.y"
      { 
          (yyval.node) = (yyvsp[-1].node); 
      }
#line 1889 "parser.tab.c"
    break;

  case 70: /* primaryVar: idOrSelf  */
#line 460 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1895 "parser.tab.c"
    break;

  case 71: /* idOrSelf: ID  */
#line 464 "parser.y"
           { (yyval.node) = createIDNode((yyvsp[0].sval)); }
#line 1901 "parser.tab.c"
    break;

  case 72: /* idOrSelf: SELF  */
#line 465 "parser.y"
           { (yyval.node) = createIDNode("self"); }
#line 1907 "parser.tab.c"
    break;

  case 73: /* suffix_ZM: suffix suffix_ZM  */
#line 469 "parser.y"
                       { (yyval.node) = NULL; }
#line 1913 "parser.tab.c"
    break;

  case 74: /* suffix_ZM: indice_ZM  */
#line 470 "parser.y"
                { (yyval.node) = NULL; }
#line 1919 "parser.tab.c"
    break;

  case 75: /* suffix: indice_ZM DOT idOrSelf  */
#line 474 "parser.y"
                             { (yyval.node) = NULL; }
#line 1925 "parser.tab.c"
    break;

  case 76: /* suffix: L_PAREN aParams R_PAREN DOT idOrSelf  */
#line 475 "parser.y"
                                           { (yyval.node) = NULL; }
#line 1931 "parser.tab.c"
    break;

  case 77: /* indice_ZM: indice indice_ZM  */
#line 479 "parser.y"
                       { (yyval.node) = NULL; }
#line 1937 "parser.tab.c"
    break;

  case 78: /* indice_ZM: %empty  */
#line 480 "parser.y"
               { (yyval.node) = NULL; }
#line 1943 "parser.tab.c"
    break;

  case 79: /* indice: L_BRACKET arithExpr R_BRACKET  */
#line 484 "parser.y"
                                    { (yyval.node) = NULL; }
#line 1949 "parser.tab.c"
    break;

  case 80: /* functionCall: primaryVar L_PAREN aParams R_PAREN  */
#line 491 "parser.y"
      {
          (yyval.node) = createNode(NODE_FUNC_CALL);
          (yyval.node)->left = (yyvsp[-3].node); 
          (yyval.node)->right = (yyvsp[-1].node); 
      }
#line 1959 "parser.tab.c"
    break;

  case 81: /* fParams: ID COLON type arraySize_ZM fParamsTail_ZM  */
#line 502 "parser.y"
      {
          DataType pType = TYPE_UNKNOWN;
          if ((yyvsp[-2].node) && (yyvsp[-2].node)->metaData) {
               if (strcmp((yyvsp[-2].node)->metaData, "integer") == 0) pType = TYPE_INT;
               else if (strcmp((yyvsp[-2].node)->metaData, "float") == 0) pType = TYPE_FLOAT;
          }
          insertSymbol((yyvsp[-4].sval), pType, "PARAM");
          (yyval.node) = NULL; 
      }
#line 1973 "parser.tab.c"
    break;

  case 82: /* fParams: %empty  */
#line 511 "parser.y"
               { (yyval.node) = NULL; }
#line 1979 "parser.tab.c"
    break;

  case 83: /* fParamsTail_ZM: fParamsTail fParamsTail_ZM  */
#line 515 "parser.y"
                                  { (yyval.node) = NULL; }
#line 1985 "parser.tab.c"
    break;

  case 84: /* fParamsTail_ZM: %empty  */
#line 516 "parser.y"
               { (yyval.node) = NULL; }
#line 1991 "parser.tab.c"
    break;

  case 85: /* fParamsTail: COMMA ID COLON type arraySize_ZM  */
#line 521 "parser.y"
      {
          DataType pType = TYPE_UNKNOWN;
          if ((yyvsp[-1].node) && (yyvsp[-1].node)->metaData) {
               if (strcmp((yyvsp[-1].node)->metaData, "integer") == 0) pType = TYPE_INT;
               else if (strcmp((yyvsp[-1].node)->metaData, "float") == 0) pType = TYPE_FLOAT;
          }
          insertSymbol((yyvsp[-3].sval), pType, "PARAM");
          (yyval.node) = NULL;
      }
#line 2005 "parser.tab.c"
    break;

  case 86: /* aParams: expr aParamsTail_ZM  */
#line 534 "parser.y"
      { 
          (yyval.node) = (yyvsp[-1].node); 
          addSibling((yyval.node), (yyvsp[0].node));
      }
#line 2014 "parser.tab.c"
    break;

  case 87: /* aParams: %empty  */
#line 538 "parser.y"
               { (yyval.node) = NULL; }
#line 2020 "parser.tab.c"
    break;

  case 88: /* aParamsTail_ZM: aParamsTail aParamsTail_ZM  */
#line 543 "parser.y"
      { 
          (yyval.node) = (yyvsp[-1].node);
          addSibling((yyval.node), (yyvsp[0].node));
      }
#line 2029 "parser.tab.c"
    break;

  case 89: /* aParamsTail_ZM: %empty  */
#line 547 "parser.y"
                { (yyval.node) = NULL; }
#line 2035 "parser.tab.c"
    break;

  case 90: /* aParamsTail: COMMA expr  */
#line 551 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2041 "parser.tab.c"
    break;

  case 91: /* type: INTEGER  */
#line 557 "parser.y"
              { (yyval.node) = createIDNode("integer"); }
#line 2047 "parser.tab.c"
    break;

  case 92: /* type: FLOAT  */
#line 558 "parser.y"
             { (yyval.node) = createIDNode("float"); }
#line 2053 "parser.tab.c"
    break;

  case 93: /* type: ID  */
#line 559 "parser.y"
              { (yyval.node) = createIDNode((yyvsp[0].sval)); }
#line 2059 "parser.tab.c"
    break;

  case 94: /* returnType: type  */
#line 563 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2065 "parser.tab.c"
    break;

  case 95: /* returnType: VOID  */
#line 564 "parser.y"
           { (yyval.node) = createIDNode("void"); }
#line 2071 "parser.tab.c"
    break;

  case 96: /* assignOp: ASSIGN_OP  */
#line 568 "parser.y"
                { (yyval.node) = NULL; }
#line 2077 "parser.tab.c"
    break;

  case 97: /* relOp: EQ_OP  */
#line 572 "parser.y"
            { (yyval.node) = NULL; }
#line 2083 "parser.tab.c"
    break;

  case 98: /* relOp: LT  */
#line 573 "parser.y"
           { (yyval.node) = NULL; }
#line 2089 "parser.tab.c"
    break;

  case 99: /* relOp: GT  */
#line 574 "parser.y"
           { (yyval.node) = NULL; }
#line 2095 "parser.tab.c"
    break;

  case 100: /* relOp: NE_OP  */
#line 575 "parser.y"
            { (yyval.node) = NULL; }
#line 2101 "parser.tab.c"
    break;

  case 101: /* relOp: LE_OP  */
#line 576 "parser.y"
            { (yyval.node) = NULL; }
#line 2107 "parser.tab.c"
    break;

  case 102: /* relOp: GE_OP  */
#line 577 "parser.y"
            { (yyval.node) = NULL; }
#line 2113 "parser.tab.c"
    break;

  case 103: /* addOp: sign  */
#line 581 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2119 "parser.tab.c"
    break;

  case 104: /* addOp: OR  */
#line 582 "parser.y"
          { (yyval.node) = createOpNode("OR", NULL, NULL); }
#line 2125 "parser.tab.c"
    break;

  case 105: /* multOp: MULT  */
#line 586 "parser.y"
           { (yyval.node) = createOpNode("*", NULL, NULL); }
#line 2131 "parser.tab.c"
    break;

  case 106: /* multOp: DIV  */
#line 587 "parser.y"
          { (yyval.node) = createOpNode("/", NULL, NULL); }
#line 2137 "parser.tab.c"
    break;

  case 107: /* multOp: AND  */
#line 588 "parser.y"
          { (yyval.node) = createOpNode("AND", NULL, NULL); }
#line 2143 "parser.tab.c"
    break;


#line 2147 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 591 "parser.y"


void yyerror(const char *s) {
    handleSyntaxError(yylineno, s, yytext);
}
