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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "code_gen.h"

// Forward declarations
int yylex(void);
void yyerror(const char *s);
extern int yylineno;

// Global root for the AST
ASTNode *rootNode = NULL;

void print_lex_tokens(char *filename);

// Helper to handle unary logic consistently
ASTNode* apply_sign(ASTNode *signNode, ASTNode *exprNode) {
    if (strcmp(signNode->op, "-") == 0) {
        return createUnary("uminus", exprNode);
    }
    // '+' sign is identity, just return the expression
    return exprNode; 
}

#line 99 "parser.tab.c"

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
  YYSYMBOL_CLASS = 3,                      /* CLASS  */
  YYSYMBOL_IMPLEMENT = 4,                  /* IMPLEMENT  */
  YYSYMBOL_FUNC = 5,                       /* FUNC  */
  YYSYMBOL_ATTRIBUTE = 6,                  /* ATTRIBUTE  */
  YYSYMBOL_PUBLIC = 7,                     /* PUBLIC  */
  YYSYMBOL_PRIVATE = 8,                    /* PRIVATE  */
  YYSYMBOL_LOCAL = 9,                      /* LOCAL  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_THEN = 11,                      /* THEN  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_READ = 14,                      /* READ  */
  YYSYMBOL_WRITE = 15,                     /* WRITE  */
  YYSYMBOL_RETURN = 16,                    /* RETURN  */
  YYSYMBOL_ISA = 17,                       /* ISA  */
  YYSYMBOL_INTEGER = 18,                   /* INTEGER  */
  YYSYMBOL_FLOAT = 19,                     /* FLOAT  */
  YYSYMBOL_VOID = 20,                      /* VOID  */
  YYSYMBOL_SELF = 21,                      /* SELF  */
  YYSYMBOL_CONSTRUCT = 22,                 /* CONSTRUCT  */
  YYSYMBOL_ID = 23,                        /* ID  */
  YYSYMBOL_LITERAL_INTEGER = 24,           /* LITERAL_INTEGER  */
  YYSYMBOL_LITERAL_FLOAT = 25,             /* LITERAL_FLOAT  */
  YYSYMBOL_OR = 26,                        /* OR  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_ARROW_OP = 29,                  /* ARROW_OP  */
  YYSYMBOL_ASSIGN_OP = 30,                 /* ASSIGN_OP  */
  YYSYMBOL_EQ_OP = 31,                     /* EQ_OP  */
  YYSYMBOL_NE_OP = 32,                     /* NE_OP  */
  YYSYMBOL_LE_OP = 33,                     /* LE_OP  */
  YYSYMBOL_GE_OP = 34,                     /* GE_OP  */
  YYSYMBOL_35_ = 35,                       /* '<'  */
  YYSYMBOL_36_ = 36,                       /* '>'  */
  YYSYMBOL_37_ = 37,                       /* '+'  */
  YYSYMBOL_38_ = 38,                       /* '-'  */
  YYSYMBOL_39_ = 39,                       /* '*'  */
  YYSYMBOL_40_ = 40,                       /* '/'  */
  YYSYMBOL_UMINUS = 41,                    /* UMINUS  */
  YYSYMBOL_42_ = 42,                       /* '{'  */
  YYSYMBOL_43_ = 43,                       /* '}'  */
  YYSYMBOL_44_ = 44,                       /* ';'  */
  YYSYMBOL_45_ = 45,                       /* ','  */
  YYSYMBOL_46_ = 46,                       /* '('  */
  YYSYMBOL_47_ = 47,                       /* ')'  */
  YYSYMBOL_48_ = 48,                       /* ':'  */
  YYSYMBOL_49_ = 49,                       /* '['  */
  YYSYMBOL_50_ = 50,                       /* ']'  */
  YYSYMBOL_51_ = 51,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_prog = 53,                      /* prog  */
  YYSYMBOL_classOrImplOrFunc_ZM = 54,      /* classOrImplOrFunc_ZM  */
  YYSYMBOL_classOrImplOrFunc = 55,         /* classOrImplOrFunc  */
  YYSYMBOL_classDecl = 56,                 /* classDecl  */
  YYSYMBOL_isa_Id_ZO = 57,                 /* isa_Id_ZO  */
  YYSYMBOL_commaID_ZM = 58,                /* commaID_ZM  */
  YYSYMBOL_visibility_memberDecl_ZM = 59,  /* visibility_memberDecl_ZM  */
  YYSYMBOL_implDef = 60,                   /* implDef  */
  YYSYMBOL_funcDef_ZM = 61,                /* funcDef_ZM  */
  YYSYMBOL_funcDef = 62,                   /* funcDef  */
  YYSYMBOL_funcHead = 63,                  /* funcHead  */
  YYSYMBOL_funcBody = 64,                  /* funcBody  */
  YYSYMBOL_visibility = 65,                /* visibility  */
  YYSYMBOL_memberDecl = 66,                /* memberDecl  */
  YYSYMBOL_funcDecl = 67,                  /* funcDecl  */
  YYSYMBOL_attributeDecl = 68,             /* attributeDecl  */
  YYSYMBOL_localVarDecl = 69,              /* localVarDecl  */
  YYSYMBOL_varDecl = 70,                   /* varDecl  */
  YYSYMBOL_arraySize_ZM = 71,              /* arraySize_ZM  */
  YYSYMBOL_arraySize = 72,                 /* arraySize  */
  YYSYMBOL_varDeclOrStmt_ZM = 73,          /* varDeclOrStmt_ZM  */
  YYSYMBOL_varDeclOrStmt = 74,             /* varDeclOrStmt  */
  YYSYMBOL_statement = 75,                 /* statement  */
  YYSYMBOL_matchedStmt = 76,               /* matchedStmt  */
  YYSYMBOL_unmatchedStmt = 77,             /* unmatchedStmt  */
  YYSYMBOL_statement_ZM = 78,              /* statement_ZM  */
  YYSYMBOL_expr = 79,                      /* expr  */
  YYSYMBOL_relExpr = 80,                   /* relExpr  */
  YYSYMBOL_arithExpr = 81,                 /* arithExpr  */
  YYSYMBOL_sign = 82,                      /* sign  */
  YYSYMBOL_term = 83,                      /* term  */
  YYSYMBOL_safe_factor = 84,               /* safe_factor  */
  YYSYMBOL_factor = 85,                    /* factor  */
  YYSYMBOL_variable = 86,                  /* variable  */
  YYSYMBOL_primaryVar = 87,                /* primaryVar  */
  YYSYMBOL_idOrSelf = 88,                  /* idOrSelf  */
  YYSYMBOL_suffix_ZM = 89,                 /* suffix_ZM  */
  YYSYMBOL_suffix = 90,                    /* suffix  */
  YYSYMBOL_indice_ZM = 91,                 /* indice_ZM  */
  YYSYMBOL_indice = 92,                    /* indice  */
  YYSYMBOL_functionCall = 93,              /* functionCall  */
  YYSYMBOL_fParams = 94,                   /* fParams  */
  YYSYMBOL_fParamsTail_ZM = 95,            /* fParamsTail_ZM  */
  YYSYMBOL_fParamsTail = 96,               /* fParamsTail  */
  YYSYMBOL_aParams = 97,                   /* aParams  */
  YYSYMBOL_aParamsTail_ZM = 98,            /* aParamsTail_ZM  */
  YYSYMBOL_aParamsTail = 99,               /* aParamsTail  */
  YYSYMBOL_type = 100,                     /* type  */
  YYSYMBOL_returnType = 101,               /* returnType  */
  YYSYMBOL_assignOp = 102,                 /* assignOp  */
  YYSYMBOL_relOp = 103,                    /* relOp  */
  YYSYMBOL_addOp = 104,                    /* addOp  */
  YYSYMBOL_multOp = 105                    /* multOp  */
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
#define YYLAST   221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  206

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


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
      46,    47,    39,    37,    45,    38,    51,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    48,    44,
      35,     2,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    49,     2,    50,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
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
      41
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    83,    83,    88,    95,    99,   100,   101,   107,   118,
     124,   128,   135,   139,   147,   153,   162,   167,   173,   189,
     202,   213,   217,   218,   222,   223,   227,   234,   241,   245,
     255,   257,   261,   266,   276,   278,   282,   283,   287,   288,
     292,   298,   299,   306,   312,   317,   322,   327,   335,   342,
     349,   358,   360,   366,   370,   372,   376,   378,   382,   383,
     387,   389,   400,   401,   402,   403,   404,   405,   409,   411,
     415,   427,   439,   443,   444,   448,   450,   454,   462,   472,
     473,   477,   489,   500,   508,   512,   513,   517,   527,   533,
     537,   538,   542,   552,   553,   554,   558,   559,   563,   567,
     568,   569,   570,   571,   572,   576,   577,   581,   582,   583
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
  "\"end of file\"", "error", "\"invalid token\"", "CLASS", "IMPLEMENT",
  "FUNC", "ATTRIBUTE", "PUBLIC", "PRIVATE", "LOCAL", "IF", "THEN", "ELSE",
  "WHILE", "READ", "WRITE", "RETURN", "ISA", "INTEGER", "FLOAT", "VOID",
  "SELF", "CONSTRUCT", "ID", "LITERAL_INTEGER", "LITERAL_FLOAT", "OR",
  "AND", "NOT", "ARROW_OP", "ASSIGN_OP", "EQ_OP", "NE_OP", "LE_OP",
  "GE_OP", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "UMINUS", "'{'",
  "'}'", "';'", "','", "'('", "')'", "':'", "'['", "']'", "'.'", "$accept",
  "prog", "classOrImplOrFunc_ZM", "classOrImplOrFunc", "classDecl",
  "isa_Id_ZO", "commaID_ZM", "visibility_memberDecl_ZM", "implDef",
  "funcDef_ZM", "funcDef", "funcHead", "funcBody", "visibility",
  "memberDecl", "funcDecl", "attributeDecl", "localVarDecl", "varDecl",
  "arraySize_ZM", "arraySize", "varDeclOrStmt_ZM", "varDeclOrStmt",
  "statement", "matchedStmt", "unmatchedStmt", "statement_ZM", "expr",
  "relExpr", "arithExpr", "sign", "term", "safe_factor", "factor",
  "variable", "primaryVar", "idOrSelf", "suffix_ZM", "suffix", "indice_ZM",
  "indice", "functionCall", "fParams", "fParamsTail_ZM", "fParamsTail",
  "aParams", "aParamsTail_ZM", "aParamsTail", "type", "returnType",
  "assignOp", "relOp", "addOp", "multOp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-134)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      29,    22,    37,    41,   -23,    75,  -134,    29,  -134,  -134,
    -134,    31,    60,    69,    64,   102,  -134,  -134,    40,  -134,
     104,    81,    54,   102,    80,    92,   117,    95,    96,    98,
      99,   103,  -134,  -134,    73,  -134,   105,    40,  -134,  -134,
    -134,   116,    57,  -134,   106,   107,    23,   108,    54,   109,
       6,  -134,   111,  -134,    76,    76,    14,    76,    76,    73,
     110,  -134,  -134,  -134,    76,    76,    76,  -134,    63,   112,
     113,  -134,   124,  -134,  -134,  -134,   118,    52,  -134,  -134,
     125,  -134,  -134,  -134,   115,     6,  -134,  -134,    76,  -134,
    -134,    76,   119,   100,    76,    68,  -134,  -134,  -134,  -134,
     120,   121,    63,   122,  -134,   123,  -134,  -134,   127,   128,
     129,     2,    76,  -134,    14,  -134,   107,   130,   117,   131,
      23,  -134,  -134,   101,    -3,   132,   115,   115,  -134,    55,
     144,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,    76,
      76,    44,  -134,  -134,  -134,  -134,    76,    73,   134,   135,
     136,  -134,    76,  -134,   128,   114,  -134,   137,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,   133,  -134,   149,
    -134,   132,  -134,   141,  -134,    73,    79,    68,  -134,  -134,
     142,   143,  -134,  -134,  -134,  -134,  -134,    14,   114,  -134,
     140,  -134,  -134,  -134,   148,  -134,  -134,  -134,     6,    73,
     115,   145,   146,  -134,  -134,  -134
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     0,     0,     0,     2,     4,     5,     6,
       7,     0,    10,     0,     0,    84,     1,     3,    35,    18,
       0,     0,    17,    84,     0,     0,     0,     0,     0,     0,
       0,     0,    74,    73,    52,    36,     0,    35,    37,    38,
      39,     0,    80,    72,     0,    12,    14,     0,    17,     0,
       0,    20,     0,    28,     0,     0,     0,     0,     0,    52,
       0,    21,    34,    98,     0,    89,     0,    71,    80,    76,
      80,    41,     0,     9,    22,    23,     0,     0,    15,    16,
       0,    93,    94,    95,    31,     0,    64,    65,     0,    58,
      59,     0,     0,    55,     0,    57,    68,    61,    62,    63,
       0,     0,    80,     0,    53,     0,    51,    47,     0,    91,
       0,     0,    89,    75,     0,    79,    12,     0,     0,     0,
      14,    24,    25,     0,     0,    86,    31,    31,    67,     0,
       0,   106,    99,   102,   103,   104,   100,   101,   105,     0,
       0,     0,    69,   109,   107,   108,     0,     0,     0,     0,
       0,    40,     0,    88,    91,    82,    81,     0,    77,    11,
       8,    27,    26,    13,    97,    96,    19,     0,    33,     0,
      83,    86,    30,     0,    66,     0,    54,    56,    70,    60,
       0,     0,    44,    45,    46,    92,    90,     0,     0,    32,
       0,    85,    29,    48,    38,    43,    50,    78,     0,     0,
      31,     0,     0,    87,    42,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,   184,  -134,  -134,  -134,    77,    72,  -134,   147,
       0,   126,  -134,  -134,  -134,  -134,  -134,  -134,    78,  -120,
    -134,   157,  -134,   -32,  -129,  -133,   138,   -54,    24,   -55,
     -85,    58,   -79,   -75,   -18,   150,  -102,   139,  -134,   151,
    -134,   -17,   176,    30,  -134,    88,    48,  -134,   -80,  -134,
    -134,  -134,  -134,  -134
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,    21,    73,    76,     9,    47,
      10,    11,    19,    77,   120,   121,   122,    35,    53,   125,
     126,    36,    37,    38,    39,    40,    60,   109,   104,    93,
      94,    95,    96,    97,    98,    42,    43,    67,    68,    69,
      70,    99,    25,   170,   171,   110,   153,   154,    84,   166,
      64,   139,   140,   146
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      41,    44,    59,   103,   105,   127,   172,   173,   138,   141,
     108,   111,   158,   128,   181,   142,    41,    44,   180,    41,
      44,   167,    48,    15,    81,    82,   138,    59,   131,    83,
      74,    75,     1,     2,     3,    32,   129,    33,   101,    89,
      90,    41,    44,   165,   138,    12,   194,   168,    48,    26,
      27,     4,   156,    28,    29,    30,    31,     3,   118,     3,
      13,    32,   178,    33,    14,    32,   202,    33,    86,    87,
     201,   179,    88,    18,     4,    16,     4,    20,    92,   100,
     203,   131,    34,    27,   176,   197,    28,    29,    30,    31,
      91,   138,    89,    90,    32,   143,    33,    32,   185,    33,
      86,    87,   174,    65,    88,   131,    66,   144,   145,   112,
      23,    22,    66,    89,    90,    34,    89,    90,   200,    81,
      82,   164,    91,    46,    83,    24,   131,    45,    50,    41,
      44,   132,   133,   134,   135,   136,   137,    89,    90,    51,
      52,    54,    55,   193,    56,    57,    63,   116,    61,    58,
      71,    78,    72,   107,   123,   175,    80,    41,    44,    85,
     199,   117,    66,   114,   124,   187,   130,   147,   148,   149,
     150,   151,   190,   152,   160,   162,   155,   169,   182,   183,
     184,    41,    44,   189,   188,   192,   195,   196,   198,   204,
     205,    17,   163,   159,    62,    79,   161,   106,   177,    49,
     157,   191,   186,   119,     0,     0,   102,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115
};

static const yytype_int16 yycheck[] =
{
      18,    18,    34,    57,    58,    85,   126,   127,    93,    94,
      64,    66,   114,    88,   147,    94,    34,    34,   147,    37,
      37,    24,    22,    46,    18,    19,   111,    59,    26,    23,
       7,     8,     3,     4,     5,    21,    91,    23,    56,    37,
      38,    59,    59,   123,   129,    23,   175,    50,    48,     9,
      10,    22,    50,    13,    14,    15,    16,     5,     6,     5,
      23,    21,   141,    23,    23,    21,   199,    23,    24,    25,
     199,   146,    28,    42,    22,     0,    22,    17,    54,    55,
     200,    26,    42,    10,   139,   187,    13,    14,    15,    16,
      46,   176,    37,    38,    21,    27,    23,    21,   152,    23,
      24,    25,    47,    46,    28,    26,    49,    39,    40,    46,
      46,    42,    49,    37,    38,    42,    37,    38,   198,    18,
      19,    20,    46,    42,    23,    23,    26,    23,    48,   147,
     147,    31,    32,    33,    34,    35,    36,    37,    38,    47,
      23,    46,    46,   175,    46,    46,    30,    23,    43,    46,
      44,    43,    45,    43,    29,    11,    47,   175,   175,    48,
      12,    43,    49,    51,    49,    51,    47,    47,    47,    47,
      47,    44,    23,    45,    44,    44,    47,    45,    44,    44,
      44,   199,   199,    50,    47,    44,    44,    44,    48,    44,
      44,     7,   120,   116,    37,    48,   118,    59,   140,    23,
     112,   171,   154,    77,    -1,    -1,    56,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    22,    53,    54,    55,    56,    60,
      62,    63,    23,    23,    23,    46,     0,    54,    42,    64,
      17,    57,    42,    46,    23,    94,     9,    10,    13,    14,
      15,    16,    21,    23,    42,    69,    73,    74,    75,    76,
      77,    86,    87,    88,    93,    23,    42,    61,    62,    94,
      48,    47,    23,    70,    46,    46,    46,    46,    46,    75,
      78,    43,    73,    30,   102,    46,    49,    89,    90,    91,
      92,    44,    45,    58,     7,     8,    59,    65,    43,    61,
      47,    18,    19,    23,   100,    48,    24,    25,    28,    37,
      38,    46,    80,    81,    82,    83,    84,    85,    86,    93,
      80,    86,    87,    79,    80,    79,    78,    43,    79,    79,
      97,    81,    46,    89,    51,    91,    23,    43,     6,    63,
      66,    67,    68,    29,    49,    71,    72,   100,    85,    81,
      47,    26,    31,    32,    33,    34,    35,    36,    82,   103,
     104,    82,    84,    27,    39,    40,   105,    47,    47,    47,
      47,    44,    45,    98,    99,    47,    50,    97,    88,    58,
      44,    70,    44,    59,    20,   100,   101,    24,    50,    45,
      95,    96,    71,    71,    47,    11,    81,    83,    84,    85,
      76,    77,    44,    44,    44,    79,    98,    51,    47,    50,
      23,    95,    44,    75,    76,    44,    44,    88,    48,    12,
     100,    76,    77,    71,    44,    44
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    55,    55,    56,    57,
      57,    58,    58,    59,    59,    60,    61,    61,    62,    63,
      63,    64,    65,    65,    66,    66,    67,    68,    69,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    75,
      76,    76,    76,    76,    76,    76,    76,    76,    77,    77,
      77,    78,    78,    79,    80,    80,    81,    81,    82,    82,
      83,    83,    84,    84,    84,    84,    84,    84,    85,    85,
      85,    86,    87,    88,    88,    89,    89,    90,    90,    91,
      91,    92,    93,    94,    94,    95,    95,    96,    97,    97,
      98,    98,    99,   100,   100,   100,   101,   101,   102,   103,
     103,   103,   103,   103,   103,   104,   104,   105,   105,   105
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
       3,     1,     1,     1,     1,     1,     3,     2,     1,     2,
       3,     2,     1,     1,     1,     2,     1,     3,     5,     2,
       0,     3,     4,     5,     0,     2,     0,     5,     2,     0,
       2,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
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
#line 84 "parser.y"
      { rootNode = (yyvsp[0].node); }
#line 1341 "parser.tab.c"
    break;

  case 3: /* classOrImplOrFunc_ZM: classOrImplOrFunc classOrImplOrFunc_ZM  */
#line 89 "parser.y"
      { 
        (yyval.node) = (yyvsp[-1].node); 
        if ((yyval.node) != NULL) (yyval.node)->next = (yyvsp[0].node); 
        else (yyval.node) = (yyvsp[0].node); 
      }
#line 1351 "parser.tab.c"
    break;

  case 4: /* classOrImplOrFunc_ZM: %empty  */
#line 95 "parser.y"
      { (yyval.node) = NULL; }
#line 1357 "parser.tab.c"
    break;

  case 5: /* classOrImplOrFunc: classDecl  */
#line 99 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1363 "parser.tab.c"
    break;

  case 6: /* classOrImplOrFunc: implDef  */
#line 100 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1369 "parser.tab.c"
    break;

  case 7: /* classOrImplOrFunc: funcDef  */
#line 101 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1375 "parser.tab.c"
    break;

  case 8: /* classDecl: CLASS ID isa_Id_ZO '{' visibility_memberDecl_ZM '}' ';'  */
#line 108 "parser.y"
      {
        (yyval.node) = createNode(NODE_CLASS_DECL);
        (yyval.node)->name = (yyvsp[-5].sval);
        (yyval.node)->left = (yyvsp[-4].node);  
        (yyval.node)->right = (yyvsp[-2].node); 
        (yyval.node)->type_name = strdup((yyvsp[-5].sval));
      }
#line 1387 "parser.tab.c"
    break;

  case 9: /* isa_Id_ZO: ISA ID commaID_ZM  */
#line 119 "parser.y"
      {
         (yyval.node) = createNode(NODE_LIST); 
         (yyval.node)->name = (yyvsp[-1].sval);
         (yyval.node)->next = (yyvsp[0].node);
      }
#line 1397 "parser.tab.c"
    break;

  case 10: /* isa_Id_ZO: %empty  */
#line 124 "parser.y"
               { (yyval.node) = NULL; }
#line 1403 "parser.tab.c"
    break;

  case 11: /* commaID_ZM: ',' ID commaID_ZM  */
#line 129 "parser.y"
      {
         ASTNode *n = createNode(NODE_LIST);
         n->name = (yyvsp[-1].sval);
         n->next = (yyvsp[0].node);
         (yyval.node) = n;
      }
#line 1414 "parser.tab.c"
    break;

  case 12: /* commaID_ZM: %empty  */
#line 135 "parser.y"
               { (yyval.node) = NULL; }
#line 1420 "parser.tab.c"
    break;

  case 13: /* visibility_memberDecl_ZM: visibility memberDecl visibility_memberDecl_ZM  */
#line 140 "parser.y"
      {
        (yyval.node) = (yyvsp[-1].node);
        if ((yyval.node)) {
             (yyval.node)->visibility = (yyvsp[-2].node)->name; 
             (yyval.node)->next = (yyvsp[0].node);
        }
      }
#line 1432 "parser.tab.c"
    break;

  case 14: /* visibility_memberDecl_ZM: %empty  */
#line 147 "parser.y"
               { (yyval.node) = NULL; }
#line 1438 "parser.tab.c"
    break;

  case 15: /* implDef: IMPLEMENT ID '{' funcDef_ZM '}'  */
#line 154 "parser.y"
      {
        (yyval.node) = createNode(NODE_IMPL_DEF);
        (yyval.node)->name = (yyvsp[-3].sval);
        (yyval.node)->right = (yyvsp[-1].node); 
      }
#line 1448 "parser.tab.c"
    break;

  case 16: /* funcDef_ZM: funcDef funcDef_ZM  */
#line 163 "parser.y"
      {
        (yyval.node) = (yyvsp[-1].node);
        if((yyval.node)) (yyval.node)->next = (yyvsp[0].node);
      }
#line 1457 "parser.tab.c"
    break;

  case 17: /* funcDef_ZM: %empty  */
#line 167 "parser.y"
               { (yyval.node) = NULL; }
#line 1463 "parser.tab.c"
    break;

  case 18: /* funcDef: funcHead funcBody  */
#line 174 "parser.y"
      {
        (yyval.node) = createNode(NODE_FUNC_DEF);
        (yyval.node)->name = (yyvsp[-1].node)->name;      
        (yyval.node)->left = (yyvsp[-1].node);            
        (yyval.node)->right = (yyvsp[0].node);            

        if ((yyvsp[-1].node)->type_name) {
            (yyval.node)->type_name = strdup((yyvsp[-1].node)->type_name);
        } else {
            (yyval.node)->type_name = strdup("unknown");
        }
      }
#line 1480 "parser.tab.c"
    break;

  case 19: /* funcHead: FUNC ID '(' fParams ')' ARROW_OP returnType  */
#line 190 "parser.y"
      {
        (yyval.node) = createNode(NODE_FUNC_HEAD);
        (yyval.node)->name = (yyvsp[-5].sval);
        (yyval.node)->left = (yyvsp[-3].node); 
        (yyval.node)->right = (yyvsp[0].node); 

        if ((yyvsp[0].node)) {
            (yyval.node)->type_name = strdup((yyvsp[0].node)->name); 
        } else {
             (yyval.node)->type_name = strdup("void");
        }
      }
#line 1497 "parser.tab.c"
    break;

  case 20: /* funcHead: CONSTRUCT '(' fParams ')'  */
#line 203 "parser.y"
      {
        (yyval.node) = createNode(NODE_FUNC_HEAD);
        (yyval.node)->name = strdup("construct");
        (yyval.node)->left = (yyvsp[-1].node);
        (yyval.node)->right = NULL; 
        (yyval.node)->type_name = strdup("void");
      }
#line 1509 "parser.tab.c"
    break;

  case 21: /* funcBody: '{' varDeclOrStmt_ZM '}'  */
#line 213 "parser.y"
                               { (yyval.node) = (yyvsp[-1].node); }
#line 1515 "parser.tab.c"
    break;

  case 22: /* visibility: PUBLIC  */
#line 217 "parser.y"
              { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->name = strdup("public"); }
#line 1521 "parser.tab.c"
    break;

  case 23: /* visibility: PRIVATE  */
#line 218 "parser.y"
              { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->name = strdup("private"); }
#line 1527 "parser.tab.c"
    break;

  case 24: /* memberDecl: funcDecl  */
#line 222 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1533 "parser.tab.c"
    break;

  case 25: /* memberDecl: attributeDecl  */
#line 223 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1539 "parser.tab.c"
    break;

  case 26: /* funcDecl: funcHead ';'  */
#line 228 "parser.y"
      {
         (yyval.node) = (yyvsp[-1].node); 
      }
#line 1547 "parser.tab.c"
    break;

  case 27: /* attributeDecl: ATTRIBUTE varDecl  */
#line 235 "parser.y"
      { 
         (yyval.node) = (yyvsp[0].node); 
      }
#line 1555 "parser.tab.c"
    break;

  case 28: /* localVarDecl: LOCAL varDecl  */
#line 241 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1561 "parser.tab.c"
    break;

  case 29: /* varDecl: ID ':' type arraySize_ZM ';'  */
#line 246 "parser.y"
      {
        (yyval.node) = createNode(NODE_VAR_DECL);
        (yyval.node)->name = (yyvsp[-4].sval);
        (yyval.node)->type_name = (yyvsp[-2].node)->name; 
        (yyval.node)->left = (yyvsp[-1].node); 
      }
#line 1572 "parser.tab.c"
    break;

  case 30: /* arraySize_ZM: arraySize arraySize_ZM  */
#line 256 "parser.y"
      { (yyval.node) = (yyvsp[-1].node); (yyval.node)->next = (yyvsp[0].node); }
#line 1578 "parser.tab.c"
    break;

  case 31: /* arraySize_ZM: %empty  */
#line 257 "parser.y"
               { (yyval.node) = NULL; }
#line 1584 "parser.tab.c"
    break;

  case 32: /* arraySize: '[' LITERAL_INTEGER ']'  */
#line 262 "parser.y"
      {
        (yyval.node) = createNode(NODE_LITERAL_INT);
        (yyval.node)->int_val = (yyvsp[-1].ival);
      }
#line 1593 "parser.tab.c"
    break;

  case 33: /* arraySize: '[' ']'  */
#line 267 "parser.y"
      { 
        (yyval.node) = createNode(NODE_LITERAL_INT); 
        (yyval.node)->int_val = -1; 
      }
#line 1602 "parser.tab.c"
    break;

  case 34: /* varDeclOrStmt_ZM: varDeclOrStmt varDeclOrStmt_ZM  */
#line 277 "parser.y"
      { (yyval.node) = (yyvsp[-1].node); if((yyval.node)) (yyval.node)->next = (yyvsp[0].node); }
#line 1608 "parser.tab.c"
    break;

  case 35: /* varDeclOrStmt_ZM: %empty  */
#line 278 "parser.y"
               { (yyval.node) = NULL; }
#line 1614 "parser.tab.c"
    break;

  case 36: /* varDeclOrStmt: localVarDecl  */
#line 282 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1620 "parser.tab.c"
    break;

  case 37: /* varDeclOrStmt: statement  */
#line 283 "parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1626 "parser.tab.c"
    break;

  case 38: /* statement: matchedStmt  */
#line 287 "parser.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1632 "parser.tab.c"
    break;

  case 39: /* statement: unmatchedStmt  */
#line 288 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1638 "parser.tab.c"
    break;

  case 40: /* matchedStmt: variable assignOp expr ';'  */
#line 293 "parser.y"
      {
        (yyval.node) = createNode(NODE_ASSIGN);
        (yyval.node)->left = (yyvsp[-3].node);
        (yyval.node)->right = (yyvsp[-1].node);
      }
#line 1648 "parser.tab.c"
    break;

  case 41: /* matchedStmt: functionCall ';'  */
#line 298 "parser.y"
                       { (yyval.node) = (yyvsp[-1].node); }
#line 1654 "parser.tab.c"
    break;

  case 42: /* matchedStmt: IF '(' relExpr ')' THEN matchedStmt ELSE matchedStmt ';'  */
#line 300 "parser.y"
      {
        (yyval.node) = createNode(NODE_IF);
        (yyval.node)->left = (yyvsp[-6].node);  
        (yyval.node)->right = (yyvsp[-3].node); 
        (yyval.node)->extra = (yyvsp[-1].node); 
      }
#line 1665 "parser.tab.c"
    break;

  case 43: /* matchedStmt: WHILE '(' relExpr ')' matchedStmt ';'  */
#line 307 "parser.y"
      {
        (yyval.node) = createNode(NODE_WHILE);
        (yyval.node)->left = (yyvsp[-3].node);
        (yyval.node)->right = (yyvsp[-1].node);
      }
#line 1675 "parser.tab.c"
    break;

  case 44: /* matchedStmt: READ '(' variable ')' ';'  */
#line 313 "parser.y"
      {
        (yyval.node) = createNode(NODE_READ);
        (yyval.node)->left = (yyvsp[-2].node);
      }
#line 1684 "parser.tab.c"
    break;

  case 45: /* matchedStmt: WRITE '(' expr ')' ';'  */
#line 318 "parser.y"
      {
        (yyval.node) = createNode(NODE_WRITE);
        (yyval.node)->left = (yyvsp[-2].node);
      }
#line 1693 "parser.tab.c"
    break;

  case 46: /* matchedStmt: RETURN '(' expr ')' ';'  */
#line 323 "parser.y"
      {
        (yyval.node) = createNode(NODE_RETURN);
        (yyval.node)->left = (yyvsp[-2].node);
      }
#line 1702 "parser.tab.c"
    break;

  case 47: /* matchedStmt: '{' statement_ZM '}'  */
#line 328 "parser.y"
      {
        (yyval.node) = createNode(NODE_BLOCK);
        (yyval.node)->left = (yyvsp[-1].node);
      }
#line 1711 "parser.tab.c"
    break;

  case 48: /* unmatchedStmt: IF '(' relExpr ')' THEN statement  */
#line 336 "parser.y"
      {
        (yyval.node) = createNode(NODE_IF);
        (yyval.node)->left = (yyvsp[-3].node);
        (yyval.node)->right = (yyvsp[0].node);
        (yyval.node)->extra = NULL; 
      }
#line 1722 "parser.tab.c"
    break;

  case 49: /* unmatchedStmt: IF '(' relExpr ')' THEN matchedStmt ELSE unmatchedStmt ';'  */
#line 343 "parser.y"
      {
        (yyval.node) = createNode(NODE_IF);
        (yyval.node)->left = (yyvsp[-6].node);
        (yyval.node)->right = (yyvsp[-3].node);
        (yyval.node)->extra = (yyvsp[-1].node);
      }
#line 1733 "parser.tab.c"
    break;

  case 50: /* unmatchedStmt: WHILE '(' relExpr ')' unmatchedStmt ';'  */
#line 350 "parser.y"
      {
        (yyval.node) = createNode(NODE_WHILE);
        (yyval.node)->left = (yyvsp[-3].node);
        (yyval.node)->right = (yyvsp[-1].node);
      }
#line 1743 "parser.tab.c"
    break;

  case 51: /* statement_ZM: statement statement_ZM  */
#line 359 "parser.y"
      { (yyval.node) = (yyvsp[-1].node); if((yyval.node)) (yyval.node)->next = (yyvsp[0].node); }
#line 1749 "parser.tab.c"
    break;

  case 52: /* statement_ZM: %empty  */
#line 360 "parser.y"
               { (yyval.node) = NULL; }
#line 1755 "parser.tab.c"
    break;

  case 53: /* expr: relExpr  */
#line 366 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1761 "parser.tab.c"
    break;

  case 54: /* relExpr: arithExpr relOp arithExpr  */
#line 371 "parser.y"
      { (yyval.node) = createBinary((yyvsp[-1].node)->op, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1767 "parser.tab.c"
    break;

  case 55: /* relExpr: arithExpr  */
#line 372 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1773 "parser.tab.c"
    break;

  case 56: /* arithExpr: arithExpr addOp term  */
#line 377 "parser.y"
      { (yyval.node) = createBinary((yyvsp[-1].node)->op, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1779 "parser.tab.c"
    break;

  case 57: /* arithExpr: term  */
#line 378 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1785 "parser.tab.c"
    break;

  case 58: /* sign: '+'  */
#line 382 "parser.y"
          { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("+"); }
#line 1791 "parser.tab.c"
    break;

  case 59: /* sign: '-'  */
#line 383 "parser.y"
          { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("-"); }
#line 1797 "parser.tab.c"
    break;

  case 60: /* term: term multOp factor  */
#line 388 "parser.y"
      { (yyval.node) = createBinary((yyvsp[-1].node)->op, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1803 "parser.tab.c"
    break;

  case 61: /* term: factor  */
#line 389 "parser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1809 "parser.tab.c"
    break;

  case 62: /* safe_factor: variable  */
#line 400 "parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1815 "parser.tab.c"
    break;

  case 63: /* safe_factor: functionCall  */
#line 401 "parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1821 "parser.tab.c"
    break;

  case 64: /* safe_factor: LITERAL_INTEGER  */
#line 402 "parser.y"
                      { (yyval.node) = createLiteralInt((yyvsp[0].ival)); }
#line 1827 "parser.tab.c"
    break;

  case 65: /* safe_factor: LITERAL_FLOAT  */
#line 403 "parser.y"
                      { (yyval.node) = createLiteralFloat((yyvsp[0].fval)); }
#line 1833 "parser.tab.c"
    break;

  case 66: /* safe_factor: '(' arithExpr ')'  */
#line 404 "parser.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1839 "parser.tab.c"
    break;

  case 67: /* safe_factor: NOT factor  */
#line 405 "parser.y"
                      { (yyval.node) = createUnary("not", (yyvsp[0].node)); }
#line 1845 "parser.tab.c"
    break;

  case 68: /* factor: safe_factor  */
#line 410 "parser.y"
      { (yyval.node) = (yyvsp[0].node); }
#line 1851 "parser.tab.c"
    break;

  case 69: /* factor: sign safe_factor  */
#line 412 "parser.y"
      { 
        (yyval.node) = apply_sign((yyvsp[-1].node), (yyvsp[0].node)); 
      }
#line 1859 "parser.tab.c"
    break;

  case 70: /* factor: sign sign safe_factor  */
#line 416 "parser.y"
      {
        // Apply inner sign first
        ASTNode *inner = apply_sign((yyvsp[-1].node), (yyvsp[0].node));
        // Apply outer sign
        (yyval.node) = apply_sign((yyvsp[-2].node), inner);
      }
#line 1870 "parser.tab.c"
    break;

  case 71: /* variable: primaryVar suffix_ZM  */
#line 428 "parser.y"
      {
        if ((yyvsp[0].node) == NULL) {
            (yyval.node) = (yyvsp[-1].node); 
        } else {
            (yyval.node) = (yyvsp[-1].node);
            (yyval.node)->right = (yyvsp[0].node); 
        }
      }
#line 1883 "parser.tab.c"
    break;

  case 72: /* primaryVar: idOrSelf  */
#line 439 "parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1889 "parser.tab.c"
    break;

  case 73: /* idOrSelf: ID  */
#line 443 "parser.y"
           { (yyval.node) = createID((yyvsp[0].sval)); }
#line 1895 "parser.tab.c"
    break;

  case 74: /* idOrSelf: SELF  */
#line 444 "parser.y"
           { (yyval.node) = createID("self"); }
#line 1901 "parser.tab.c"
    break;

  case 75: /* suffix_ZM: suffix suffix_ZM  */
#line 449 "parser.y"
      { (yyval.node) = (yyvsp[-1].node); (yyval.node)->next = (yyvsp[0].node); }
#line 1907 "parser.tab.c"
    break;

  case 76: /* suffix_ZM: indice_ZM  */
#line 450 "parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1913 "parser.tab.c"
    break;

  case 77: /* suffix: indice_ZM '.' idOrSelf  */
#line 455 "parser.y"
      {
        ASTNode *dot = createBinary(".", NULL, (yyvsp[0].node));
        if ((yyvsp[-2].node)) {
             dot->left = (yyvsp[-2].node); 
        }
        (yyval.node) = dot;
      }
#line 1925 "parser.tab.c"
    break;

  case 78: /* suffix: '(' aParams ')' '.' idOrSelf  */
#line 463 "parser.y"
      {
        ASTNode *call = createNode(NODE_FUNC_CALL);
        call->left = (yyvsp[-3].node); 
        ASTNode *dot = createBinary(".", call, (yyvsp[0].node));
        (yyval.node) = dot;
      }
#line 1936 "parser.tab.c"
    break;

  case 79: /* indice_ZM: indice indice_ZM  */
#line 472 "parser.y"
                       { (yyval.node) = (yyvsp[-1].node); (yyval.node)->next = (yyvsp[0].node); }
#line 1942 "parser.tab.c"
    break;

  case 80: /* indice_ZM: %empty  */
#line 473 "parser.y"
               { (yyval.node) = NULL; }
#line 1948 "parser.tab.c"
    break;

  case 81: /* indice: '[' arithExpr ']'  */
#line 478 "parser.y"
      {
        ASTNode *idx = createNode(NODE_INDICE);
        idx->op = strdup("[]");
        idx->left = (yyvsp[-1].node); 
        (yyval.node) = idx;
      }
#line 1959 "parser.tab.c"
    break;

  case 82: /* functionCall: primaryVar '(' aParams ')'  */
#line 490 "parser.y"
      {
        (yyval.node) = createNode(NODE_FUNC_CALL);
        (yyval.node)->name = (yyvsp[-3].node)->name; 
        (yyval.node)->left = (yyvsp[-1].node);       
      }
#line 1969 "parser.tab.c"
    break;

  case 83: /* fParams: ID ':' type arraySize_ZM fParamsTail_ZM  */
#line 501 "parser.y"
      {
         (yyval.node) = createNode(NODE_PARAM);
         (yyval.node)->name = (yyvsp[-4].sval);
         (yyval.node)->type_name = (yyvsp[-2].node)->name;
         (yyval.node)->left = (yyvsp[-1].node); 
         (yyval.node)->next = (yyvsp[0].node); 
      }
#line 1981 "parser.tab.c"
    break;

  case 84: /* fParams: %empty  */
#line 508 "parser.y"
               { (yyval.node) = NULL; }
#line 1987 "parser.tab.c"
    break;

  case 85: /* fParamsTail_ZM: fParamsTail fParamsTail_ZM  */
#line 512 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); (yyval.node)->next = (yyvsp[0].node); }
#line 1993 "parser.tab.c"
    break;

  case 86: /* fParamsTail_ZM: %empty  */
#line 513 "parser.y"
               { (yyval.node) = NULL; }
#line 1999 "parser.tab.c"
    break;

  case 87: /* fParamsTail: ',' ID ':' type arraySize_ZM  */
#line 518 "parser.y"
      {
         (yyval.node) = createNode(NODE_PARAM);
         (yyval.node)->name = (yyvsp[-3].sval);
         (yyval.node)->type_name = (yyvsp[-1].node)->name;
         (yyval.node)->left = (yyvsp[0].node);
      }
#line 2010 "parser.tab.c"
    break;

  case 88: /* aParams: expr aParamsTail_ZM  */
#line 528 "parser.y"
      { 
        (yyval.node) = createNode(NODE_LIST); 
        (yyval.node)->left = (yyvsp[-1].node);
        (yyval.node)->next = (yyvsp[0].node); 
      }
#line 2020 "parser.tab.c"
    break;

  case 89: /* aParams: %empty  */
#line 533 "parser.y"
               { (yyval.node) = NULL; }
#line 2026 "parser.tab.c"
    break;

  case 90: /* aParamsTail_ZM: aParamsTail aParamsTail_ZM  */
#line 537 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); (yyval.node)->next = (yyvsp[0].node); }
#line 2032 "parser.tab.c"
    break;

  case 91: /* aParamsTail_ZM: %empty  */
#line 538 "parser.y"
               { (yyval.node) = NULL; }
#line 2038 "parser.tab.c"
    break;

  case 92: /* aParamsTail: ',' expr  */
#line 543 "parser.y"
      {
        (yyval.node) = createNode(NODE_LIST);
        (yyval.node)->left = (yyvsp[0].node);
      }
#line 2047 "parser.tab.c"
    break;

  case 93: /* type: INTEGER  */
#line 552 "parser.y"
              { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->name = strdup("integer"); }
#line 2053 "parser.tab.c"
    break;

  case 94: /* type: FLOAT  */
#line 553 "parser.y"
              { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->name = strdup("float"); }
#line 2059 "parser.tab.c"
    break;

  case 95: /* type: ID  */
#line 554 "parser.y"
              { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->name = (yyvsp[0].sval); }
#line 2065 "parser.tab.c"
    break;

  case 96: /* returnType: type  */
#line 558 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2071 "parser.tab.c"
    break;

  case 97: /* returnType: VOID  */
#line 559 "parser.y"
           { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->name = strdup("void"); }
#line 2077 "parser.tab.c"
    break;

  case 98: /* assignOp: ASSIGN_OP  */
#line 563 "parser.y"
                { (yyval.node) = NULL; }
#line 2083 "parser.tab.c"
    break;

  case 99: /* relOp: EQ_OP  */
#line 567 "parser.y"
            { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("=="); }
#line 2089 "parser.tab.c"
    break;

  case 100: /* relOp: '<'  */
#line 568 "parser.y"
            { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("<"); }
#line 2095 "parser.tab.c"
    break;

  case 101: /* relOp: '>'  */
#line 569 "parser.y"
            { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup(">"); }
#line 2101 "parser.tab.c"
    break;

  case 102: /* relOp: NE_OP  */
#line 570 "parser.y"
            { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("<>"); }
#line 2107 "parser.tab.c"
    break;

  case 103: /* relOp: LE_OP  */
#line 571 "parser.y"
            { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("<="); }
#line 2113 "parser.tab.c"
    break;

  case 104: /* relOp: GE_OP  */
#line 572 "parser.y"
            { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup(">="); }
#line 2119 "parser.tab.c"
    break;

  case 105: /* addOp: sign  */
#line 576 "parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2125 "parser.tab.c"
    break;

  case 106: /* addOp: OR  */
#line 577 "parser.y"
           { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("or"); }
#line 2131 "parser.tab.c"
    break;

  case 107: /* multOp: '*'  */
#line 581 "parser.y"
          { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("*"); }
#line 2137 "parser.tab.c"
    break;

  case 108: /* multOp: '/'  */
#line 582 "parser.y"
          { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("/"); }
#line 2143 "parser.tab.c"
    break;

  case 109: /* multOp: AND  */
#line 583 "parser.y"
          { (yyval.node) = createNode(NODE_TYPE); (yyval.node)->op = strdup("and"); }
#line 2149 "parser.tab.c"
    break;


#line 2153 "parser.tab.c"

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

#line 586 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, s);
}

int main(void) {
    printf("Running Lexical Debug pass...\n");
    print_lex_tokens("lexer_output.txt"); 
    printf("Lexical Debug pass complete. Check 'lexer_output.txt'.\n");

    printf("Starting Parser...\n");

    if (yyparse() == 0) {
        printf("Parsing Successful.\n");
        
        ast_print(rootNode, "ast.txt");
        semantic_analysis(rootNode); 
        sym_print("symbol_table.txt");
        
        FILE *asmFile = fopen("output.asm", "w");
        if (!asmFile) {
            perror("Failed to open output.asm");
            return 1;
        }

        code_gen_init(asmFile);
        
        ASTNode *curr = rootNode;
        while(curr) {
            code_gen(curr, asmFile);
            curr = curr->next;
        }
        
        code_gen_finalize(asmFile);
        fclose(asmFile);
        
        printf("Compilation Complete. \n");
        printf(" - AST: ast.txt\n");
        printf(" - Symbols: symbol_table.txt\n");
        printf(" - Assembly: output.asm\n");
        
    } else {
        printf("Parsing Failed.\n");
    }
    return 0;
}
