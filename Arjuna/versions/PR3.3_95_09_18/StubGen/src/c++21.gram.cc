
/*  A Bison parser, made from c++21.gram.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	IDENTIFIER	258
#define	CHARACTERCONSTANT	259
#define	INTEGERCONSTANT	260
#define	FLOATINGCONSTANT	261
#define	STRING	262
#define	ASM	263
#define	BREAK	264
#define	CASE	265
#define	CONTINUE	266
#define	DEFAULT	267
#define	DO	268
#define	ELSE	269
#define	FOR	270
#define	GOTO	271
#define	IF	272
#define	RETURN	273
#define	SWITCH	274
#define	WHILE	275
#define	CONST_CAST	276
#define	DYNAMIC_CAST	277
#define	REINTERPRET_CAST	278
#define	STATIC_CAST	279
#define	NAMESPACE	280
#define	USING	281
#define	TYPEID	282
#define	STAREQUAL	283
#define	SLASHEQUAL	284
#define	PERCENTEQUAL	285
#define	PLUSEQUAL	286
#define	MINUSEQUAL	287
#define	RSHIFTEQUAL	288
#define	LSHIFTEQUAL	289
#define	ANDEQUAL	290
#define	XOREQUAL	291
#define	OREQUAL	292
#define	COLONCOLON	293
#define	ARROW	294
#define	DOTSTAR	295
#define	ARROWSTAR	296
#define	PLUSPLUS	297
#define	MINUSMINUS	298
#define	OROR	299
#define	ANDAND	300
#define	EQUALEQUAL	301
#define	NOTEQUAL	302
#define	LESSEQUAL	303
#define	MOREEQUAL	304
#define	LSHIFT	305
#define	RSHIFT	306
#define	ELLIPSIS	307
#define	NEW	308
#define	DELETE	309
#define	OPERATOR	310
#define	SIZEOF	311
#define	THIS	312
#define	TEMPLATE	313
#define	INLINE	314
#define	VIRTUAL	315
#define	CHAR	316
#define	SHORT	317
#define	INT	318
#define	LONG	319
#define	SIGNED	320
#define	UNSIGNED	321
#define	FLOAT	322
#define	DOUBLE	323
#define	VOID	324
#define	CLASS	325
#define	STRUCT	326
#define	UNION	327
#define	ENUM	328
#define	WCHAR	329
#define	BOOL_TYPE	330
#define	BOOL_TRUE	331
#define	BOOL_FALSE	332
#define	AUTO	333
#define	REGISTER	334
#define	STATIC	335
#define	EXTERN	336
#define	FRIEND	337
#define	TYPEDEF	338
#define	PRIVATE	339
#define	PROTECTED	340
#define	PUBLIC	341
#define	CONST	342
#define	VOLATILE	343
#define	MUTABLE	344
#define	CATCH	345
#define	THROW	346
#define	TRY	347
#define	CLASSTYPE	348
#define	ENUMTYPE	349
#define	TYPEDEFTYPE	350
#define	STUB_CMD	351
#define	DIRECTIVE	352

#line 12 "c++21.gram.y"

/*
 * simple yacc parser for C++ for tatsu stub generator. Does not parse all
 * of the language, only a limited subset. Grammer based on the syntax 
 * summary in Appendix A of the AT&T 2.1 product reference manual and the
 * ARM
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <strstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef TEMPLATECT_H_
#  include "TemplateCT.h"
#endif

#ifndef INSTCLASST_H_
#  include "InstClassT.h"
#endif

#ifndef CVT_H_
#  include "CvT.h"
#endif

#ifndef FUNCT_H_
#  include "FuncT.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef CHART_H_
#  include "CharT.h"
#endif

#ifndef BOOLT_H_
#  include "BoolT.h"
#endif

#ifndef FLOATT_H_
#  include "FloatT.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef VOIDT_H_
#  include "VoidT.h"
#endif

#ifndef ENUMT_H_
#  include "EnumT.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
#endif

#ifndef VECT_H_
#  include "VecT.h"
#endif

#ifndef QUALT_H_
#  include "QualT.h"
#endif

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

#ifndef DUMMYT_H_
#  include "DummyT.h"
#endif

#ifndef DUMMYARGT_H_
#  include "DummyArgT.h"
#endif

#ifndef BASECLASS_H_
#  include "BaseClass.h"
#endif

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
#endif

#ifndef CLNKAGE_H_
#  include "CLnkage.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif

#ifndef DECLSPEC_H_
#  include "DeclSpec.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

#ifndef ALLOCEXPR_H_
#  include "AllocExpr.h"
#endif

#ifndef ARRAYEXPR_H_
#  include "ArrayExpr.h"
#endif

#ifndef BINEXPR_H_
#  include "BinExpr.h"
#endif

#ifndef LITERAL_H_
#  include "Literal.h"
#endif

#ifndef NESTEXPR_H_
#  include "NestExpr.h"
#endif

#ifndef TMPLCNTXT_H_
#  include "TmplCntxt.h"
#endif

extern String tokenText;
    
extern String *collectInlineDef ();
extern int getLookahead ();
extern "C" int yylex ();
extern "C" yyless (int);
extern "C" int yyparse ();
extern "C" int yywrap ();

#ifdef NONCONST_YYERROR
extern "C" void yyerror (char *);
#else
extern "C" void yyerror (const char *);
#endif

static ClassType *declareClass (const String&, ClassSpecifier, BaseClass *,
				DeclContext * = 0, Boolean = TRUE);
static void popClassLevel ();
static void pushClassLevel (ClassType *);
static void exitTemporaryNest ();

static const char RCSid[] = "$Id: c++21.gram.y,v 1.23 1995/07/11 15:12:37 ngdp Exp $";

static Boolean isCtor = FALSE;
static int templateNest = 0;
static TemplateContext *templateArgs = 0;
static Boolean declIsFriend = FALSE;
static const int MAXCLASSNEST = 50;
static int classNestLevel = 0;
static int temporaryNest = 0;
static ClassType *currentClassType = 0;
static ClassType *classStack[MAXCLASSNEST];

static String equal("= ");

static Declaration *currentCEDecl = 0;

#line 227 "c++21.gram.y"
typedef union
{
    long longconst;
    BaseClass *bclass;
    Declaration *decl;
    DeclarationList *decllist;
    DeclSpecifier *declspec;
    Expression *expr;
    FuncModifier funcspec;
    FctType *func;
    StorageSpecifier storagespec;
    CvQualifier cvq;
    String *text;
    TypeDescriptor *typedesc;
    VisibilitySpec vspec;
    ClassSpecifier clSpec;
    Linkage *linkspec;
    StubModifier stubcommand;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		711
#define	YYFLAG		-32768
#define	YYNTBASE	122

#define YYTRANSLATE(x) ((unsigned)(x) <= 352 ? yytranslate[x] : 281)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,   114,     2,     2,     2,   111,   104,     2,   112,
   113,   109,   107,    98,   108,   118,   110,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,   101,   119,   105,
    99,   106,   100,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
   116,     2,   117,   103,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,   120,   102,   121,   115,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     3,     5,     8,    10,    12,    14,    16,    18,
    20,    24,    26,    30,    32,    34,    36,    38,    40,    42,
    44,    46,    48,    50,    52,    54,    60,    62,    66,    68,
    72,    74,    78,    80,    84,    86,    90,    92,    96,   100,
   102,   106,   110,   114,   118,   120,   124,   128,   130,   134,
   138,   140,   144,   148,   152,   154,   158,   162,   164,   169,
   171,   174,   177,   180,   183,   188,   190,   192,   194,   196,
   198,   200,   202,   204,   210,   218,   220,   221,   223,   224,
   228,   231,   233,   234,   238,   244,   249,   251,   252,   256,
   258,   259,   263,   269,   271,   276,   281,   286,   290,   294,
   297,   300,   302,   303,   305,   309,   311,   313,   316,   319,
   322,   326,   328,   330,   332,   335,   337,   340,   343,   347,
   349,   351,   353,   355,   357,   359,   364,   369,   375,   377,
   378,   380,   383,   385,   386,   392,   394,   397,   400,   403,
   407,   411,   414,   416,   417,   419,   421,   423,   425,   427,
   429,   432,   434,   436,   438,   440,   443,   445,   448,   450,
   453,   455,   458,   460,   463,   465,   468,   470,   472,   475,
   478,   480,   482,   484,   486,   488,   490,   492,   494,   496,
   498,   500,   503,   506,   508,   510,   512,   514,   516,   518,
   520,   522,   524,   526,   528,   530,   532,   534,   536,   538,
   540,   542,   545,   547,   550,   553,   555,   556,   558,   560,
   563,   566,   569,   571,   573,   575,   578,   580,   583,   589,
   591,   592,   594,   595,   597,   601,   603,   607,   609,   614,
   618,   621,   624,   626,   630,   632,   636,   639,   641,   643,
   645,   648,   650,   653,   657,   661,   663,   664,   666,   670,
   675,   677,   681,   686,   689,   691,   693,   696,   698,   701,
   705,   708,   715,   719,   721,   724,   727,   730,   732,   733,
   735,   736,   738,   741,   744,   747,   752,   754,   757,   759,
   761,   763,   765,   767,   769,   772,   774,   777,   779,   782,
   784,   785,   788,   790,   792,   793,   796,   801,   805,   807,
   810,   814,   816,   817,   819,   823,   826,   831,   834,   839,
   842,   847,   850,   855,   857,   860,   866,   871,   876,   880,
   884,   888,   893,   897,   901,   905,   908,   911,   916,   920,
   924,   928,   930,   931,   933,   938,   941,   947,   949,   950,
   952,   956,   961,   963,   965,   970,   974,   977,   981,   982,
   988,   990,   994,   996,   998,  1000,  1001,  1003,  1006,  1010,
  1013,  1015,  1018,  1021,  1024,  1027,  1031,  1035,  1038,  1041,
  1049,  1054,  1059,  1062,  1066,  1068,  1069,  1071,  1075,  1077,
  1079,  1081,  1083,  1087,  1089,  1093,  1095,  1097,  1099,  1102,
  1103,  1110,  1113,  1115,  1119,  1121,  1125,  1128,  1132,  1135,
  1137,  1139,  1141,  1143,  1146,  1148,  1149,  1152,  1154,  1158,
  1163,  1167,  1172,  1176,  1179,  1181,  1183,  1185,  1187,  1189,
  1191,  1193,  1195,  1197,  1199,  1201,  1203,  1205,  1207,  1209,
  1211,  1213,  1215,  1217,  1219,  1221,  1223,  1225,  1227,  1229,
  1231,  1233,  1235,  1237,  1239,  1241,  1243,  1245,  1247,  1249,
  1251,  1253,  1255,  1258,  1261,  1262,  1266,  1271,  1272,  1274,
  1275,  1277
};

static const short yyrhs[] = {    -1,
   123,     0,   124,     0,   123,   124,     0,   167,     0,   236,
     0,   200,     0,   161,     0,   237,     0,   126,     0,   125,
    98,   126,     0,   128,     0,   141,   127,   126,     0,    99,
     0,    28,     0,    29,     0,    30,     0,    31,     0,    32,
     0,    33,     0,    34,     0,    35,     0,    36,     0,    37,
     0,   129,     0,   129,   100,   125,   101,   128,     0,   130,
     0,   129,    44,   130,     0,   131,     0,   130,    45,   131,
     0,   132,     0,   131,   102,   132,     0,   133,     0,   132,
   103,   133,     0,   134,     0,   133,   104,   134,     0,   135,
     0,   134,    46,   135,     0,   134,    47,   135,     0,   136,
     0,   135,   105,   136,     0,   135,   106,   136,     0,   135,
    48,   136,     0,   135,    49,   136,     0,   137,     0,   136,
    50,   137,     0,   136,    51,   137,     0,   138,     0,   137,
   107,   138,     0,   137,   108,   138,     0,   139,     0,   138,
   109,   139,     0,   138,   110,   139,     0,   138,   111,   139,
     0,   140,     0,   139,    40,   140,     0,   139,    41,   140,
     0,   141,     0,   112,   226,   113,   140,     0,   154,     0,
    42,   141,     0,    43,   141,     0,   142,   140,     0,    56,
   141,     0,    56,   112,   226,   113,     0,   143,     0,   153,
     0,   109,     0,   104,     0,   107,     0,   108,     0,   114,
     0,   115,     0,   144,    53,   145,   147,   150,     0,   144,
    53,   145,   112,   226,   113,   150,     0,    38,     0,     0,
   146,     0,     0,   112,   156,   113,     0,   227,   148,     0,
   149,     0,     0,   109,   219,   148,     0,   186,    38,   109,
   219,   148,     0,   148,   116,   125,   117,     0,   151,     0,
     0,   112,   152,   113,     0,   244,     0,     0,   144,    54,
   140,     0,   144,    54,   116,   117,   140,     0,   157,     0,
   154,   116,   125,   117,     0,   154,   112,   155,   113,     0,
   185,   112,   155,   113,     0,   154,   118,   158,     0,   154,
    39,   158,     0,   154,    42,     0,   154,    43,     0,   156,
     0,     0,   126,     0,   156,    98,   126,     0,   160,     0,
    57,     0,    38,     3,     0,    38,   274,     0,    38,   159,
     0,   112,   125,   113,     0,   158,     0,     3,     0,   274,
     0,   115,    93,     0,   159,     0,   193,     3,     0,   193,
   274,     0,   193,   115,    93,     0,     5,     0,     4,     0,
     6,     0,     7,     0,    76,     0,    77,     0,   162,   165,
   241,   119,     0,   162,   165,   190,   119,     0,   162,   165,
   169,   204,   119,     0,   163,     0,     0,   164,     0,   163,
   164,     0,   183,     0,     0,    58,   105,   166,   216,   106,
     0,   119,     0,   203,   119,     0,   176,   119,     0,   175,
   119,     0,   169,   202,   119,     0,   170,   168,   119,     0,
     1,   119,     0,   203,     0,     0,   173,     0,   172,     0,
   175,     0,   176,     0,   171,     0,   163,     0,   170,   171,
     0,   181,     0,   182,     0,   223,     0,   174,     0,   170,
   174,     0,   177,     0,   177,   178,     0,   185,     0,   174,
   223,     0,   190,     0,   170,   190,     0,   189,     0,   170,
   189,     0,   179,     0,   170,   179,     0,   179,     0,   171,
     0,   178,   179,     0,   178,   171,     0,    61,     0,    74,
     0,    62,     0,    63,     0,    64,     0,    65,     0,    66,
     0,    67,     0,    68,     0,    69,     0,    75,     0,   193,
    95,     0,   193,    94,     0,    78,     0,    79,     0,    80,
     0,    81,     0,    82,     0,    83,     0,    59,     0,    60,
     0,    96,     0,   185,     0,   179,     0,   190,     0,   189,
     0,   186,     0,    94,     0,    95,     0,   180,     0,   245,
     0,    38,   245,     0,   187,     0,    38,   187,     0,   193,
   245,     0,   265,     0,     0,   246,     0,   194,     0,   191,
     3,     0,   191,   245,     0,    73,    94,     0,    70,     0,
    71,     0,    72,     0,   245,    38,     0,   192,     0,   193,
   192,     0,    73,   195,   120,   196,   121,     0,     3,     0,
     0,   197,     0,     0,   198,     0,   197,    98,   198,     0,
     3,     0,     3,    99,   199,     0,   128,     0,   201,   120,
   123,   121,     0,   201,   120,   121,     0,   201,   124,     0,
    81,     7,     0,   204,     0,   202,    98,   204,     0,   206,
     0,   203,    98,   204,     0,   205,   209,     0,   207,     0,
   214,     0,   212,     0,   214,   209,     0,   207,     0,   211,
   208,     0,   220,   211,   208,     0,   112,   156,   113,     0,
   242,     0,     0,   225,     0,   112,   205,   113,     0,   210,
   116,   218,   117,     0,   224,     0,   112,   214,   113,     0,
   211,   116,   218,   117,     0,   220,   213,     0,   213,     0,
   210,     0,   210,   215,     0,   211,     0,   220,   211,     0,
   220,   211,   215,     0,   211,   215,     0,   112,   232,   113,
   219,   278,   263,     0,   216,    98,   217,     0,   217,     0,
   191,     3,     0,   191,    93,     0,   169,   205,     0,   199,
     0,     0,   222,     0,     0,   221,     0,   220,   221,     0,
   109,   219,     0,   104,   219,     0,   186,    38,   109,   219,
     0,   223,     0,   222,   223,     0,    87,     0,    88,     0,
   158,     0,   186,     0,   180,     0,    95,     0,   227,   228,
     0,   184,     0,   227,   184,     0,   223,     0,   227,   223,
     0,   229,     0,     0,   220,   230,     0,   231,     0,   231,
     0,     0,   230,   215,     0,   230,   116,   218,   117,     0,
   112,   229,   113,     0,   233,     0,   233,    52,     0,   234,
    98,    52,     0,   234,     0,     0,   235,     0,   234,    98,
   235,     0,   169,   205,     0,   169,   205,    99,   126,     0,
   169,   228,     0,   169,   228,    99,   126,     0,   170,   214,
     0,   170,   214,    99,   126,     0,   170,   228,     0,   170,
   228,    99,   126,     0,     1,     0,   214,   240,     0,   170,
   193,   255,   239,   240,     0,   193,   255,   239,   240,     0,
   170,   193,   256,   240,     0,   193,   256,   240,     0,   169,
   205,   240,     0,   170,   214,   240,     0,   170,   193,   264,
   240,     0,   193,   264,   240,     0,   162,   165,   236,     0,
   255,   239,   240,     0,   256,   240,     0,   264,   240,     0,
   170,   255,   239,   240,     0,   170,   256,   240,     0,   170,
   264,   240,     0,   169,   260,   240,     0,   271,     0,     0,
   276,     0,   247,   120,   252,   121,     0,    99,   126,     0,
    99,   120,   244,   243,   121,     0,    98,     0,     0,   125,
     0,   244,    98,   125,     0,   120,   244,   243,   121,     0,
    93,     0,   248,     0,   247,   120,   252,   121,     0,   191,
     3,   188,     0,   191,   188,     0,   191,    93,   188,     0,
     0,    93,   105,   249,   250,   106,     0,   251,     0,   250,
    98,   251,     0,   226,     0,   126,     0,   253,     0,     0,
   254,     0,   268,   101,     0,   253,   268,   101,     0,   253,
   254,     0,   119,     0,     1,   119,     0,   261,   119,     0,
   176,   119,     0,   175,   119,     0,   169,   259,   119,     0,
   170,   261,   119,     0,   238,   258,     0,   159,   119,     0,
    93,   112,   233,   113,   219,   278,   263,     0,   115,    93,
   257,   278,     0,   115,     3,   257,   278,     0,   112,   113,
     0,   112,    69,   113,     0,   119,     0,     0,   260,     0,
   259,    98,   260,     0,   205,     0,   255,     0,   256,     0,
   264,     0,   195,   101,   199,     0,   262,     0,   261,    98,
   262,     0,   255,     0,   256,     0,   264,     0,    99,     5,
     0,     0,    55,   269,   257,   219,   278,   263,     0,   101,
   266,     0,   267,     0,   266,    98,   267,     0,   245,     0,
    60,   268,   245,     0,    60,   245,     0,   268,    60,   245,
     0,   268,   245,     0,     1,     0,    84,     0,    85,     0,
    86,     0,   227,   270,     0,   221,     0,     0,   101,   272,
     0,   273,     0,   272,    98,   273,     0,   245,   112,   156,
   113,     0,   245,   112,   113,     0,     3,   112,   156,   113,
     0,     3,   112,   113,     0,    55,   275,     0,    53,     0,
    54,     0,   107,     0,   108,     0,   109,     0,   110,     0,
   111,     0,   103,     0,   104,     0,   102,     0,   115,     0,
   114,     0,    99,     0,   105,     0,   106,     0,    31,     0,
    32,     0,    28,     0,    29,     0,    30,     0,    36,     0,
    35,     0,    37,     0,    50,     0,    51,     0,    34,     0,
    33,     0,    46,     0,    47,     0,    48,     0,    49,     0,
    45,     0,    44,     0,    42,     0,    43,     0,    98,     0,
    41,     0,    39,     0,   112,   113,     0,   116,   117,     0,
     0,   120,   277,   121,     0,    91,   112,   279,   113,     0,
     0,   280,     0,     0,   226,     0,   280,    98,   226,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   448,   450,   459,   461,   476,   478,   480,   481,   482,   486,
   488,   492,   494,   498,   501,   503,   505,   507,   509,   511,
   513,   515,   517,   519,   523,   525,   531,   533,   537,   539,
   543,   545,   549,   551,   555,   557,   561,   563,   565,   569,
   571,   573,   575,   577,   581,   583,   585,   589,   591,   593,
   597,   599,   601,   603,   607,   609,   611,   615,   617,   631,
   633,   635,   637,   639,   641,   655,   656,   659,   662,   664,
   666,   668,   670,   674,   681,   694,   697,   701,   703,   707,
   712,   725,   727,   731,   744,   756,   769,   771,   775,   780,
   782,   786,   794,   802,   804,   806,   808,   818,   820,   822,
   824,   828,   830,   834,   836,   840,   842,   844,   846,   848,
   850,   852,   856,   858,   859,   861,   864,   871,   877,   884,
   887,   889,   891,   893,   895,   899,   917,   936,   958,   960,
   965,   967,   971,   976,   979,   983,   986,  1002,  1011,  1022,
  1063,  1086,  1090,  1092,  1096,  1098,  1099,  1100,  1103,  1105,
  1106,  1110,  1113,  1115,  1119,  1121,  1125,  1127,  1131,  1134,
  1138,  1141,  1145,  1148,  1153,  1156,  1160,  1163,  1164,  1166,
  1170,  1173,  1175,  1177,  1179,  1181,  1183,  1185,  1187,  1189,
  1191,  1195,  1201,  1208,  1211,  1213,  1215,  1217,  1219,  1223,
  1226,  1230,  1235,  1237,  1238,  1239,  1242,  1244,  1246,  1248,
  1251,  1254,  1256,  1257,  1261,  1269,  1271,  1275,  1284,  1294,
  1310,  1320,  1324,  1327,  1329,  1333,  1350,  1352,  1358,  1372,
  1374,  1379,  1381,  1385,  1392,  1400,  1403,  1407,  1411,  1418,
  1424,  1441,  1463,  1466,  1469,  1472,  1476,  1479,  1482,  1484,
  1487,  1490,  1493,  1496,  1510,  1515,  1517,  1521,  1523,  1527,
  1537,  1539,  1543,  1552,  1563,  1566,  1568,  1581,  1583,  1595,
  1606,  1618,  1630,  1633,  1638,  1654,  1669,  1678,  1680,  1684,
  1686,  1690,  1692,  1699,  1702,  1704,  1712,  1715,  1721,  1724,
  1728,  1733,  1736,  1738,  1742,  1757,  1760,  1762,  1764,  1768,
  1770,  1774,  1785,  1788,  1790,  1794,  1805,  1817,  1821,  1823,
  1834,  1838,  1840,  1844,  1847,  1856,  1864,  1872,  1882,  1895,
  1902,  1910,  1921,  1933,  1937,  1945,  1968,  1986,  1999,  2008,
  2015,  2022,  2031,  2040,  2059,  2079,  2086,  2092,  2111,  2120,
  2128,  2138,  2140,  2144,  2149,  2161,  2164,  2169,  2172,  2176,
  2178,  2180,  2184,  2186,  2189,  2201,  2207,  2211,  2218,  2224,
  2240,  2245,  2250,  2255,  2261,  2263,  2267,  2269,  2271,  2276,
  2290,  2293,  2295,  2317,  2327,  2338,  2381,  2404,  2406,  2413,
  2452,  2471,  2491,  2494,  2498,  2501,  2505,  2510,  2516,  2518,
  2520,  2522,  2523,  2527,  2532,  2538,  2541,  2543,  2546,  2553,
  2558,  2578,  2583,  2585,  2599,  2602,  2604,  2606,  2608,  2610,
  2617,  2620,  2622,  2626,  2640,  2642,  2646,  2651,  2653,  2657,
  2662,  2664,  2668,  2672,  2682,  2685,  2687,  2689,  2691,  2693,
  2695,  2697,  2699,  2701,  2703,  2705,  2707,  2709,  2711,  2713,
  2715,  2717,  2719,  2721,  2723,  2725,  2727,  2729,  2731,  2733,
  2735,  2737,  2739,  2741,  2743,  2745,  2747,  2749,  2751,  2753,
  2755,  2757,  2759,  2761,  2765,  2767,  2770,  2773,  2777,  2779,
  2783,  2786
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","IDENTIFIER",
"CHARACTERCONSTANT","INTEGERCONSTANT","FLOATINGCONSTANT","STRING","ASM","BREAK",
"CASE","CONTINUE","DEFAULT","DO","ELSE","FOR","GOTO","IF","RETURN","SWITCH",
"WHILE","CONST_CAST","DYNAMIC_CAST","REINTERPRET_CAST","STATIC_CAST","NAMESPACE",
"USING","TYPEID","STAREQUAL","SLASHEQUAL","PERCENTEQUAL","PLUSEQUAL","MINUSEQUAL",
"RSHIFTEQUAL","LSHIFTEQUAL","ANDEQUAL","XOREQUAL","OREQUAL","COLONCOLON","ARROW",
"DOTSTAR","ARROWSTAR","PLUSPLUS","MINUSMINUS","OROR","ANDAND","EQUALEQUAL","NOTEQUAL",
"LESSEQUAL","MOREEQUAL","LSHIFT","RSHIFT","ELLIPSIS","NEW","DELETE","OPERATOR",
"SIZEOF","THIS","TEMPLATE","INLINE","VIRTUAL","CHAR","SHORT","INT","LONG","SIGNED",
"UNSIGNED","FLOAT","DOUBLE","VOID","CLASS","STRUCT","UNION","ENUM","WCHAR","BOOL_TYPE",
"BOOL_TRUE","BOOL_FALSE","AUTO","REGISTER","STATIC","EXTERN","FRIEND","TYPEDEF",
"PRIVATE","PROTECTED","PUBLIC","CONST","VOLATILE","MUTABLE","CATCH","THROW",
"TRY","CLASSTYPE","ENUMTYPE","TYPEDEFTYPE","STUB_CMD","DIRECTIVE","','","'='",
"'?'","':'","'|'","'^'","'&'","'<'","'>'","'+'","'-'","'*'","'/'","'%'","'('",
"')'","'!'","'~'","'['","']'","'.'","';'","'{'","'}'","program","external.defs",
"top.level.declaration","expression","assignment.expression","assignment.operator",
"conditional.expression","logical.or.expression","logical.and.expression","inclusive.or.expression",
"exclusive.or.expression","and.expression","equality.expression","relational.expression",
"shift.expression","additive.expression","multiplicative.expression","pm.expression",
"cast.expression","unary.expression","unary.operator","allocation.expression",
"clcl.opt","placement.opt","placement","new.type.name","new.declarator.opt",
"new.declarator","new.initializer.opt","new.initializer","initializer.list.opt",
"deallocation.expression","postfix.expression","expression.list.opt","expression.list",
"primary.expression","name","qualified.name","literal","template.declaration",
"stub.decl.specifiers.opt","stub.decl.specifiers","stub.decl.specifier","template.head",
"@1","declaration","id.declarator.list.opt","typed.decl.specifiers","untyped.decl.specifiers",
"untyped.decl.specifier","typedef.type.specifiers","basic.type.specifiers","typedef.type.specifier",
"elaborated.type.specifiers","class.or.enum.specifiers","basic.type.specifier",
"more.basic.type.specifiers","basic.type","qualified.type","sc.specifier","fct.specifier",
"stub.command.specifier","type.specifier","simple.type","complete.class.type",
"qualified.class.type","base.spec.opt","class.or.enum.specifier","elaborated.type.specifier",
"class.key","class.scope","nested.scope","enum.specifier","identifier.opt","enum.list.opt",
"enum.list","enumerator","constant.expression","linkage.specification","linkage",
"declarator.list","id.declarator.list","init.declarator","declarator","id.init.declarator",
"constructed.id.declarator","paren.exp.list","initializer.opt","nonf.direct.declarator",
"nonf.id.direct.declarator","typedef.declarator","direct.declarator","id.declarator",
"param.type.list","template.decl.arglist","template.decl.arg","constant.expression.opt",
"cv.qualifier.list.opt","ptr.operator.list","ptr.operator","cv.qualifier.list",
"cv.qualifier","id.dname","dname","type.name","type.specifier.list","abstract.declarator.opt",
"abstract.declarator","direct.abstract.declarator.opt","direct.abstract.declarator",
"argument.declaration.list","arg.declaration.list.opt","arg.declaration.list",
"argument.declaration","function.definition","template.definition","member.function.definition",
"ctor.initializer.opt","function.body","class.template.body","initializer","comma.opt",
"initializer.list","class.type","class.specifier","class.head","template.class.id",
"@2","template.inst.arglist","template.inst.arg","member.list.opt","member.list",
"member.declaration","constructor","destructor","void.parameter.list","semi.opt",
"member.declarator.list","member.declarator","nospec.member.declarator.list",
"nospec.member.declarator","pure.specifier.opt","conversion.operator","base.spec",
"base.list","base.specifier","access.specifier","conversion.type.name","ptr.operator.opt",
"ctor.initializer","mem.initializer.list","mem.initializer","operator.function.name",
"operator","compound.statement","@3","exception.spec.opt","exception.list.opt",
"exception.list", NULL
};
#endif

static const short yyr1[] = {     0,
   122,   122,   123,   123,   124,   124,   124,   124,   124,   125,
   125,   126,   126,   127,   127,   127,   127,   127,   127,   127,
   127,   127,   127,   127,   128,   128,   129,   129,   130,   130,
   131,   131,   132,   132,   133,   133,   134,   134,   134,   135,
   135,   135,   135,   135,   136,   136,   136,   137,   137,   137,
   138,   138,   138,   138,   139,   139,   139,   140,   140,   141,
   141,   141,   141,   141,   141,   141,   141,   142,   142,   142,
   142,   142,   142,   143,   143,   144,   144,   145,   145,   146,
   147,   148,   148,   149,   149,   149,   150,   150,   151,   152,
   152,   153,   153,   154,   154,   154,   154,   154,   154,   154,
   154,   155,   155,   156,   156,   157,   157,   157,   157,   157,
   157,   157,   158,   158,   158,   158,   159,   159,   159,   160,
   160,   160,   160,   160,   160,   161,   161,   161,   162,   162,
   163,   163,   164,   166,   165,   167,   167,   167,   167,   167,
   167,   167,   168,   168,   169,   169,   169,   169,   170,   170,
   170,   171,   171,   171,   172,   172,   173,   173,   174,   174,
   175,   175,   176,   176,   177,   177,   178,   178,   178,   178,
   179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
   179,   180,   180,   181,   181,   181,   181,   181,   181,   182,
   182,   183,   184,   184,   184,   184,   185,   185,   185,   185,
   186,   186,   186,   186,   187,   188,   188,   189,   189,   190,
   190,   190,   191,   191,   191,   192,   193,   193,   194,   195,
   195,   196,   196,   197,   197,   198,   198,   199,   200,   200,
   200,   201,   202,   202,   203,   203,   204,   204,   205,   205,
   206,   206,   207,   207,   208,   209,   209,   210,   210,   210,
   211,   211,   211,   212,   212,   213,   213,   214,   214,   214,
   214,   215,   216,   216,   217,   217,   217,   218,   218,   219,
   219,   220,   220,   221,   221,   221,   222,   222,   223,   223,
   224,   225,   225,   225,   226,   227,   227,   227,   227,   228,
   228,   229,   229,   230,   230,   231,   231,   231,   232,   232,
   232,   233,   233,   234,   234,   235,   235,   235,   235,   235,
   235,   235,   235,   235,   236,   236,   236,   236,   236,   236,
   236,   236,   236,   237,   238,   238,   238,   238,   238,   238,
   238,   239,   239,   240,   241,   242,   242,   243,   243,   244,
   244,   244,   245,   245,   246,   247,   247,   247,   249,   248,
   250,   250,   251,   251,   252,   252,   253,   253,   253,   253,
   254,   254,   254,   254,   254,   254,   254,   254,   254,   255,
   256,   256,   257,   257,   258,   258,   259,   259,   260,   260,
   260,   260,   260,   261,   261,   262,   262,   262,   263,   263,
   264,   265,   266,   266,   267,   267,   267,   267,   267,   267,
   268,   268,   268,   269,   270,   270,   271,   272,   272,   273,
   273,   273,   273,   274,   275,   275,   275,   275,   275,   275,
   275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
   275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
   275,   275,   275,   275,   275,   275,   275,   275,   275,   275,
   275,   275,   275,   275,   277,   276,   278,   278,   279,   279,
   280,   280
};

static const short yyr2[] = {     0,
     0,     1,     1,     2,     1,     1,     1,     1,     1,     1,
     3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     5,     1,     3,     1,     3,
     1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
     3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
     1,     3,     3,     3,     1,     3,     3,     1,     4,     1,
     2,     2,     2,     2,     4,     1,     1,     1,     1,     1,
     1,     1,     1,     5,     7,     1,     0,     1,     0,     3,
     2,     1,     0,     3,     5,     4,     1,     0,     3,     1,
     0,     3,     5,     1,     4,     4,     4,     3,     3,     2,
     2,     1,     0,     1,     3,     1,     1,     2,     2,     2,
     3,     1,     1,     1,     2,     1,     2,     2,     3,     1,
     1,     1,     1,     1,     1,     4,     4,     5,     1,     0,
     1,     2,     1,     0,     5,     1,     2,     2,     2,     3,
     3,     2,     1,     0,     1,     1,     1,     1,     1,     1,
     2,     1,     1,     1,     1,     2,     1,     2,     1,     2,
     1,     2,     1,     2,     1,     2,     1,     1,     2,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     2,     1,     2,     2,     1,     0,     1,     1,     2,
     2,     2,     1,     1,     1,     2,     1,     2,     5,     1,
     0,     1,     0,     1,     3,     1,     3,     1,     4,     3,
     2,     2,     1,     3,     1,     3,     2,     1,     1,     1,
     2,     1,     2,     3,     3,     1,     0,     1,     3,     4,
     1,     3,     4,     2,     1,     1,     2,     1,     2,     3,
     2,     6,     3,     1,     2,     2,     2,     1,     0,     1,
     0,     1,     2,     2,     2,     4,     1,     2,     1,     1,
     1,     1,     1,     1,     2,     1,     2,     1,     2,     1,
     0,     2,     1,     1,     0,     2,     4,     3,     1,     2,
     3,     1,     0,     1,     3,     2,     4,     2,     4,     2,
     4,     2,     4,     1,     2,     5,     4,     4,     3,     3,
     3,     4,     3,     3,     3,     2,     2,     4,     3,     3,
     3,     1,     0,     1,     4,     2,     5,     1,     0,     1,
     3,     4,     1,     1,     4,     3,     2,     3,     0,     5,
     1,     3,     1,     1,     1,     0,     1,     2,     3,     2,
     1,     2,     2,     2,     2,     3,     3,     2,     2,     7,
     4,     4,     2,     3,     1,     0,     1,     3,     1,     1,
     1,     1,     3,     1,     3,     1,     1,     1,     2,     0,
     6,     2,     1,     3,     1,     3,     2,     3,     2,     1,
     1,     1,     1,     2,     1,     0,     2,     1,     3,     4,
     3,     4,     3,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     2,     2,     0,     3,     4,     0,     1,     0,
     1,     3
};

static const short yydefact[] = {     0,
     0,   113,     0,     0,   190,   191,   171,   173,   174,   175,
   176,   177,   178,   179,   180,   213,   214,   215,   221,   172,
   181,   184,   185,   186,   187,   188,   189,   279,   280,   343,
   198,   199,   192,   271,   271,     0,     0,   136,     0,     3,
   281,   116,     8,     0,   150,   131,     5,     0,   144,   149,
   146,   145,   155,   147,   148,   157,   165,   200,   152,   153,
   133,   159,   197,   203,   163,   161,   207,   217,     0,   209,
     7,     0,     0,   235,   242,   258,   247,     0,   272,   154,
   251,     6,     9,   201,   208,     0,   344,   114,   142,   204,
     0,   202,   432,   433,   434,   430,   431,   441,   440,   436,
   435,   437,   452,   451,   448,   449,   447,   446,   442,   443,
   444,   445,   438,   439,   415,   416,   450,   427,   424,   422,
   423,   428,   429,   417,   418,   419,   420,   421,     0,   426,
   425,     0,   414,   220,   212,     0,   232,   349,   275,   270,
   277,   274,     0,     0,   258,     0,     0,   115,     4,     0,
     0,   132,   284,     0,   283,   282,     0,     0,   233,   247,
   238,   256,   240,   255,   239,     0,   248,   187,     0,   151,
   156,   166,   164,   162,     0,   143,   247,   160,   139,   138,
   168,   158,   167,     0,   210,   343,     0,   347,   211,   206,
   117,     0,   343,   183,   182,     0,   218,   205,   333,     0,
     0,   118,     0,   231,     0,   137,     0,    77,   243,   261,
    77,   455,   241,   315,   246,   334,   259,   273,   216,     0,
   453,   454,   223,    77,   278,     0,     0,   252,   259,   134,
   150,     0,     0,   147,   148,   161,     0,   324,     0,     0,
     0,     0,     0,     0,   140,   237,   320,    77,   257,   254,
   141,   333,     0,     0,   321,   170,   169,   271,   346,   348,
   400,     0,   401,   402,   403,   395,   392,   393,     0,   194,
   286,   193,   197,   196,   195,     0,   288,   406,     0,     0,
     0,   119,     0,     0,   332,   319,   323,   230,     0,   236,
   247,   314,   121,   120,   122,   123,    76,    77,    77,    77,
   107,   124,   125,    69,    70,    71,    68,    77,    72,    73,
   104,    12,    25,    27,    29,    31,    33,    35,    37,    40,
    45,    48,    51,    55,    58,    77,    66,     0,    67,    60,
     0,    94,   112,   106,   291,   291,   159,     0,   299,   302,
   304,   228,    58,     0,   268,     0,    77,   336,     0,   244,
   260,     0,     0,     0,   361,     0,   221,     0,   147,   148,
   376,     0,     0,   357,   386,   387,     0,   384,   388,     0,
   226,     0,   222,   224,   354,   193,   353,   291,     0,   351,
   119,     0,     0,     0,   127,   126,     0,   249,   234,     0,
     0,   318,   322,   276,   397,     0,     0,     0,   399,   287,
   405,   289,   404,     0,   271,     0,   302,   458,   458,     0,
     0,   407,   408,   317,   229,   108,   110,   109,    77,    61,
    62,    77,    64,     0,    10,     0,    77,    77,    77,    77,
    77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
    77,    77,    77,    77,    77,    77,    77,    15,    16,    17,
    18,    19,    20,    21,    22,    23,    24,    14,    77,    63,
    79,    77,     0,   100,   101,    77,    77,     0,    77,   245,
   295,   306,   295,   308,   290,     0,   293,   295,   310,   295,
   312,    77,   271,   300,     0,   253,    77,   340,   339,   456,
   362,     0,   369,   113,     0,     0,   379,   380,   381,     0,
   377,   382,   386,   387,     0,   388,   365,   364,   375,   368,
   345,   360,     0,     0,   326,     0,   363,   327,   358,    77,
   219,     0,   295,   295,   285,    77,   350,     0,     0,   207,
     0,   264,   128,     0,   250,   316,   396,   394,   398,     0,
   373,   458,   271,     0,     0,   372,   371,    77,    77,     0,
     0,    77,   111,    77,    28,     0,    30,    32,    34,    36,
    38,    39,    43,    44,    41,    42,    46,    47,    49,    50,
    52,    53,    54,    56,    57,    13,    77,     0,    78,     0,
    92,    99,     0,     0,     0,   102,     0,    98,   105,     0,
    77,   292,   294,    77,    77,   296,    77,    77,     0,   458,
   301,   305,   339,    77,     0,   115,    77,   221,   366,   331,
     0,   329,   367,   330,   359,   325,     0,   386,   387,   385,
   388,   227,   225,   352,   267,   210,   343,     0,   135,   345,
   374,   390,   458,   460,   413,     0,   411,     0,   409,    65,
    11,    59,    77,     0,     0,    88,    83,    77,    96,    95,
   298,   307,   309,     0,   311,   313,    97,   390,     0,   341,
   337,   383,   378,   328,   263,     0,   391,   390,   461,     0,
   459,   412,   410,    26,    80,     0,    77,    74,    87,   271,
    81,    82,   197,    93,   297,   262,   342,   389,   370,   457,
     0,    88,     0,    90,    83,    77,     0,   462,    75,    89,
    77,    84,     0,     0,   271,    86,    83,    85,     0,     0,
     0
};

static const short yydefgoto[] = {   709,
    39,    40,   424,   425,   459,   312,   313,   314,   315,   316,
   317,   318,   319,   320,   321,   322,   323,   324,   343,   326,
   327,   328,   578,   579,   646,   681,   682,   678,   679,   693,
   329,   330,   585,   586,   332,   333,    42,   334,    43,    44,
   231,    46,   151,   382,    47,   169,    48,    49,    50,    51,
    52,    53,   234,   235,    56,   182,    57,    58,    59,    60,
    61,   271,   344,   273,    64,   188,    65,    66,    67,    68,
   157,    70,   496,   372,   373,   374,   345,    71,    72,   158,
    73,   159,   160,    74,    75,   209,   213,   162,   145,   163,
   164,   165,   210,   531,   532,   346,   139,    78,    79,   140,
    80,    81,   167,   377,   378,   474,   475,   476,   477,   338,
   339,   340,   341,    82,    83,   361,   284,   214,   239,   215,
   605,   489,    84,    85,    86,    87,   224,   379,   380,   362,
   363,   364,   365,   366,   409,   510,   500,   501,   367,   368,
   667,   369,   190,   267,   268,   269,   279,   403,   285,   412,
   413,    88,   133,   216,   349,   546,   670,   671
};

static const short yypact[] = {  1417,
   -51,-32768,    17,  2984,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    31,-32768,
-32768,-32768,-32768,-32768,    77,-32768,-32768,-32768,-32768,    -4,
-32768,-32768,-32768,   281,   281,   329,    26,-32768,  1506,-32768,
-32768,-32768,-32768,    66,   118,-32768,-32768,   814,  2379,-32768,
-32768,-32768,   281,    16,    25,  1980,-32768,-32768,-32768,-32768,
-32768,-32768,   152,-32768,-32768,-32768,    14,-32768,    45,-32768,
-32768,  1589,   -45,-32768,-32768,   155,   -41,   329,-32768,-32768,
-32768,-32768,-32768,   199,-32768,   126,-32768,-32768,-32768,-32768,
    17,   199,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   218,-32768,
-32768,   160,-32768,-32768,-32768,   227,-32768,-32768,-32768,   281,
-32768,-32768,   152,    82,   271,   225,   329,-32768,-32768,   247,
  2306,-32768,-32768,   814,-32768,   152,   703,    91,-32768,   -41,
-32768,   305,-32768,-32768,-32768,   814,-32768,-32768,   272,-32768,
   281,-32768,-32768,-32768,    45,   320,   -41,-32768,-32768,-32768,
-32768,  1980,-32768,   294,    29,   311,   376,-32768,-32768,-32768,
-32768,  2895,   189,-32768,-32768,    52,-32768,   199,   319,   317,
   317,-32768,  1090,-32768,   814,-32768,  1920,  2014,-32768,-32768,
  1802,-32768,-32768,-32768,-32768,-32768,   155,-32768,-32768,  1253,
-32768,-32768,   437,  2231,-32768,   350,   895,-32768,   271,-32768,
   375,   814,  2379,-32768,-32768,   327,   317,-32768,   339,   359,
   370,   225,   814,   814,-32768,-32768,-32768,  2014,-32768,-32768,
-32768,   319,   317,   317,-32768,-32768,-32768,   281,-32768,-32768,
-32768,   388,-32768,-32768,-32768,-32768,   392,-32768,    10,-32768,
-32768,-32768,-32768,-32768,-32768,   438,-32768,  3188,   385,  2454,
   385,   385,    78,   317,-32768,-32768,-32768,-32768,  1178,-32768,
   406,-32768,-32768,-32768,-32768,-32768,    72,  2637,  2637,  2662,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2231,-32768,    26,
-32768,-32768,    -9,   465,   410,   411,   413,   429,   182,   517,
   466,   502,   556,-32768,  1022,  2730,-32768,   548,-32768,   476,
   -25,-32768,-32768,-32768,   747,  2075,   408,   422,   474,   439,
-32768,-32768,-32768,   408,-32768,   425,  1825,-32768,   424,-32768,
-32768,   428,  1039,    92,-32768,   431,   827,  3098,   435,   451,
   458,   459,  1328,-32768,    71,   317,   119,-32768,   317,   486,
   490,   470,   524,-32768,-32768,   408,-32768,  3040,   107,-32768,
-32768,  3290,   518,   317,-32768,-32768,  1253,-32768,-32768,   521,
   317,-32768,-32768,-32768,-32768,    17,   376,    17,-32768,-32768,
-32768,-32768,-32768,    -2,   281,   523,   545,   558,   558,   547,
   549,   565,-32768,-32768,-32768,-32768,-32768,-32768,  2730,-32768,
-32768,  2231,-32768,    85,-32768,   551,  2730,  2730,  2730,  2730,
  2730,  2730,  2730,  2730,  2730,  2730,  2730,  2730,  2730,  2730,
  2730,  2730,  2730,  2730,  2730,  2730,  2730,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2730,-32768,
   554,  2168,   198,-32768,-32768,  2501,  2730,   198,  2730,-32768,
   977,   568,   977,   569,-32768,   313,   340,   697,   571,   697,
   573,  2501,   281,-32768,  2788,-32768,  1825,   576,   583,-32768,
-32768,   385,-32768,   584,    93,   585,-32768,-32768,-32768,   124,
   317,-32768,    71,   317,   137,   317,-32768,-32768,-32768,-32768,
-32768,-32768,   588,   317,-32768,    11,-32768,-32768,-32768,  2730,
-32768,   437,    19,    19,-32768,  2231,-32768,   814,  2986,    53,
   283,-32768,-32768,   577,-32768,-32768,-32768,-32768,-32768,   581,
-32768,   558,   281,  2826,   589,-32768,-32768,  2569,  2594,    78,
   598,  2730,-32768,  2730,   465,   334,   410,   411,   413,   429,
   182,   182,   517,   517,   517,   517,   466,   466,   502,   502,
   556,   556,   556,-32768,-32768,-32768,  2730,  3136,-32768,   580,
-32768,-32768,    82,   199,   599,   616,   164,-32768,-32768,   602,
  2730,   313,-32768,  2730,  2014,-32768,  2730,  2730,   603,   558,
-32768,-32768,   583,  1683,   596,   385,  2730,   827,-32768,-32768,
   317,-32768,-32768,-32768,-32768,-32768,   606,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   520,   401,  3290,-32768,   604,
-32768,   622,   558,  1039,-32768,   141,-32768,   170,-32768,-32768,
-32768,-32768,  2730,   235,  1039,   612,  3239,  2730,-32768,-32768,
-32768,-32768,-32768,   609,-32768,-32768,-32768,   622,   608,   576,
-32768,-32768,-32768,-32768,-32768,   725,-32768,   622,-32768,   619,
   636,-32768,-32768,-32768,-32768,   623,  1707,-32768,-32768,   281,
   621,-32768,   700,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1039,   612,   627,   648,   103,  2730,   638,-32768,-32768,-32768,
  2730,   621,   700,   202,   281,-32768,   103,   621,   748,   751,
-32768
};

static const short yypgoto[] = {-32768,
   552,    -7,  -331,   469,-32768,  -196,-32768,   330,   325,   331,
   332,   328,   173,   120,   180,   183,   187,  -304,   445,-32768,
-32768,-32768,-32768,-32768,-32768,  -423,-32768,    75,-32768,-32768,
-32768,-32768,   280,  -206,-32768,   536,  -176,-32768,-32768,-32768,
    60,   -32,-32768,-32768,-32768,-32768,   -57,   114,   -29,-32768,
-32768,   -31,   177,   362,-32768,-32768,   -18,   260,-32768,-32768,
-32768,  -245,    37,    15,    23,  -177,   131,     1,  -352,   -67,
     0,-32768,   752,-32768,-32768,   252,  -473,-32768,-32768,-32768,
   728,   -50,   -83,-32768,   -37,   564,  -132,-32768,   561,-32768,
  -145,   605,  -137,-32768,   154,  -229,   -30,    70,    79,-32768,
     8,-32768,-32768,  -249,  -186,  -296,  -409,  -188,  -117,-32768,
   503,   504,  -439,   635,-32768,-32768,  -248,   519,-32768,-32768,
   184,  -458,   307,-32768,   640,-32768,-32768,-32768,   263,   407,
-32768,   432,   -62,   -59,   129,-32768,-32768,   191,   442,   291,
  -417,   -46,-32768,-32768,   414,  -179,-32768,-32768,-32768,-32768,
   264,   -55,-32768,-32768,-32768,  -364,-32768,-32768
};


#define	YYLAST		3386


static const short yytable[] = {    69,
   331,   197,    91,   391,   142,   278,   199,   259,   260,   200,
   161,   342,   152,   202,    63,   488,   185,   171,   390,   170,
   250,   460,   201,   197,   249,    90,   181,   246,   603,   530,
   172,   149,   400,   134,   427,   144,    62,   183,    69,   481,
   370,   141,   141,   356,   547,   602,   622,   191,   175,   174,
   143,   342,   205,    63,   281,   626,     3,   211,   426,    45,
   178,   590,   156,    63,   204,   353,   540,    89,   590,   398,
   241,    69,   469,   206,   416,    62,   197,   144,   212,   351,
   410,   525,   396,   137,   191,    62,    63,   470,   202,   197,
   428,   351,   143,   232,   281,   281,   556,   250,    45,   192,
   138,   202,    30,   617,   602,   147,   186,   197,    62,    30,
   541,    30,   252,   590,   187,   253,   514,   166,   148,   202,
   417,   291,    34,   150,   135,   354,     4,    35,   254,   187,
   523,    45,   400,   662,   179,   587,     4,   193,   194,   195,
     3,   574,   575,   180,   282,   627,   144,   225,  -207,   335,
    69,   236,   256,   187,   290,   488,   218,   581,   246,   196,
   291,   143,   357,   257,    30,    63,   278,   161,   156,   335,
    30,   283,   551,   270,    30,  -129,    54,   632,   178,   173,
   156,   383,   552,   513,   492,   606,   356,    62,   244,   184,
  -333,   276,   275,   389,   161,    30,   226,   553,   152,   277,
     2,   171,    69,   170,   526,   270,   161,   370,   197,   245,
   356,   680,   527,    33,   172,    54,   516,    63,   694,   156,
   147,   608,   335,   243,   275,   218,   276,   394,   272,   435,
   436,   277,   175,   174,   516,   658,   219,   517,   469,    62,
   686,   418,   609,   337,   218,   220,   156,    63,    54,   642,
   689,   472,     4,   672,   611,   613,    62,   156,   156,   270,
   376,   552,    45,    62,   233,   141,   207,   469,   668,    62,
   208,   702,   660,   497,   166,   530,   222,   276,   275,   276,
   650,   149,   673,   708,   592,   402,   437,   438,    69,   270,
    30,   592,    63,   138,   498,   503,   144,   499,   504,   552,
   280,   166,   147,    63,   171,   357,   170,   155,   275,    92,
   502,   506,    37,   166,   272,   277,    62,   172,   706,    90,
   336,   218,   274,   342,   528,    62,   171,   250,   170,   357,
   221,     2,   469,   358,   270,   592,   174,   228,   596,   172,
   336,   636,   638,   684,   376,   488,   223,   675,    45,   156,
    63,   230,   276,   275,   274,   593,   401,   276,   174,   270,
   277,    55,   593,   173,   704,   654,     3,    28,    29,   660,
   644,   156,    62,   189,   542,   198,   261,   276,   275,    54,
   628,   276,   227,     4,   669,   402,   208,   241,   629,   272,
   251,   647,    63,   336,    62,   676,   359,   198,   342,    62,
    55,   400,   258,   270,   473,   480,   593,   405,   274,   408,
   342,   187,   141,   155,   272,   138,   227,   205,    62,   283,
   248,    30,   275,    62,   227,   155,   243,   335,   595,   277,
  -207,   552,    34,    55,   643,   262,   212,    35,   274,   371,
    36,   698,   381,    37,   625,   385,   674,   524,   259,   260,
   198,  -294,   600,   618,   596,  -294,   619,   386,   376,   263,
   264,   265,   583,   198,   155,    54,   173,   583,    30,   621,
    33,   263,   264,   265,   433,   434,   358,   144,   387,   144,
    30,   198,   388,   274,   276,   156,   335,   156,   173,   397,
   141,   155,   143,   266,   143,   529,   404,   171,  -266,   170,
   358,   187,   155,   155,   211,   138,  -266,   270,   274,   429,
   172,   430,   633,   431,   463,   197,   432,   464,   465,   482,
  -207,    62,    91,    91,   497,   484,   275,   202,   276,   174,
    30,   194,   195,   277,   483,    41,   485,   143,   143,   359,
   473,   486,   156,   276,   490,   498,   491,   480,   499,   493,
   141,   218,   274,   507,   563,   564,   565,   566,   218,   270,
    76,   502,   376,   359,    55,    62,   439,   440,   395,   508,
   528,    41,   441,   442,    41,   399,   509,   276,   275,   511,
    62,   360,   198,    41,    41,   277,   519,   466,   520,   411,
   521,   467,   524,   468,   155,   446,   447,   243,   336,    76,
   461,   462,   218,    92,    77,   561,   562,    41,    76,    76,
   443,   444,   445,    41,   272,   270,   155,  -265,   567,   568,
   187,   522,   156,   569,   570,  -265,   270,   276,   270,   571,
   572,   573,    76,   276,   275,   543,   533,   535,   217,  -207,
   146,   277,   544,    77,   276,   275,   276,   275,   545,   695,
    55,   325,   277,   177,   402,   325,   274,   336,   548,   173,
   549,   683,   550,   554,    62,   577,   591,   594,   325,   597,
   272,   598,   270,   552,   707,   311,    77,   243,   247,   348,
   604,   272,    41,   272,  -220,   607,    41,   141,   615,    41,
   276,   275,   375,   631,    91,   255,   648,   630,   277,     2,
   634,    41,   537,   266,   539,   191,    91,   229,   274,   703,
   640,   649,   141,   469,   651,   657,   661,   280,   286,   287,
   666,   703,  -335,   677,   360,   685,   217,   272,   687,   688,
   155,   690,   155,   691,     3,   692,   696,   697,    41,   700,
    41,   529,   420,   421,   423,   701,   705,   710,   360,     2,
   711,     4,   325,   557,   289,   237,   555,     4,   242,   560,
   558,   599,   559,    76,   274,    76,   699,    41,    41,   584,
   136,   392,   393,   623,   584,   274,   176,   274,    41,    41,
   350,   665,   406,   407,     3,   238,   659,   155,   624,    30,
   240,   325,    76,   534,   512,    30,   194,   195,   663,   505,
    34,     4,   414,   229,    76,    35,   620,    77,   478,     0,
   538,    37,     0,   639,     0,     0,     2,   226,     0,     0,
     0,   274,     0,     0,    41,     0,     0,     0,     0,   494,
     0,     0,     0,     0,     0,     0,   189,   384,     0,    30,
     0,   153,     0,     0,     0,     0,     0,     0,     0,    76,
    34,     3,     0,     0,     0,    35,   411,     0,   471,     0,
     0,    37,  -295,   325,     3,     0,   325,   155,     4,     0,
    41,    41,   325,     0,     0,     0,     0,     0,     0,     0,
     0,   192,     0,     0,   515,     0,     0,   518,     0,   584,
     0,     0,    41,    77,     0,   292,     0,     0,     0,     0,
     0,     0,   255,   325,     0,     0,    30,     0,   153,   536,
   325,   325,     0,   325,     0,     0,     0,    34,     0,   193,
     0,   153,    35,     0,     0,   154,   325,   576,    37,     0,
    34,   325,     3,     0,   311,    35,     0,   589,   154,     0,
   479,   495,     0,     0,     0,     0,  -303,     0,     0,     0,
   311,     0,     0,     5,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
   325,     0,    22,    23,    24,   168,    26,    27,     0,     2,
     0,    28,    29,     0,     0,     0,     0,    30,    31,    32,
    33,     0,   325,   325,   375,     0,   325,     0,   582,     0,
     0,     0,     0,   588,     0,     0,    41,  -303,    41,     0,
     0,     0,     0,    41,     3,    41,   311,   311,     0,   610,
   641,   325,   612,     0,   614,     0,     0,     0,     0,     0,
     0,     4,   616,   229,     0,   325,     0,     0,   325,     0,
   229,   325,   325,     0,     0,   311,     0,     0,   325,   448,
   449,   450,   451,   452,   453,   454,   455,   456,   457,   652,
     0,     0,   653,    41,     0,   655,   656,     0,     0,    30,
     0,   153,     0,     0,     0,   242,     3,     0,     0,     0,
    34,     0,   146,     0,     0,    35,     0,     0,   471,     0,
     1,    37,     2,     0,     0,     0,     0,     0,     0,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,     0,     0,     0,     0,     0,     0,
   458,   325,     0,     0,     0,    28,    29,     3,     0,   664,
     0,    30,    31,    32,     0,     0,     0,     0,     0,     0,
   325,     0,     0,    41,     4,   325,     0,  -130,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,     0,     0,    22,    23,    24,
    25,    26,    27,     0,     0,     0,    28,    29,     1,     0,
     2,     0,    30,    31,    32,    33,     0,     0,     0,     0,
     0,     0,     0,    34,     0,     0,     0,     0,    35,     0,
     0,    36,     0,     0,    37,     0,     0,     0,    38,     0,
   288,     0,     0,     0,     0,     3,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     4,     0,     0,  -130,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,   352,     0,    22,    23,    24,    25,    26,
    27,     0,     0,     0,    28,    29,     0,     0,     0,     0,
    30,    31,    32,    33,     0,     0,     0,     0,     0,     0,
     0,    34,     0,     0,     0,     0,    35,     0,     0,    36,
     3,     0,    37,     0,     0,     0,    38,     0,   415,     0,
     0,     0,     0,     0,     0,     0,     0,   353,     0,     0,
     0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    16,    17,    18,    19,    20,    21,   352,     0,
    22,    23,    24,   168,    26,    27,   263,   264,   265,    28,
    29,     0,     0,     0,     0,   193,    31,    32,    33,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     3,     0,   354,     0,     0,
     0,   355,     0,  -356,     0,     0,     0,     0,     0,     0,
     0,     0,   353,     0,     0,     0,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,     0,     0,    22,    23,    24,   168,    26,
    27,   263,   264,   265,    28,    29,    -1,     1,     0,     2,
   193,    31,    32,    33,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   354,     0,     0,     0,   355,     0,  -355,     0,
     0,     0,     0,     0,     3,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     4,     0,     0,  -130,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,     0,     0,    22,    23,    24,    25,    26,    27,
     0,     0,     0,    28,    29,    -2,     1,     0,     2,    30,
    31,    32,    33,     0,     0,     0,     0,     0,     0,     0,
    34,     0,     0,     0,     0,    35,     0,     0,    36,     0,
     0,    37,     0,     0,     0,    38,     0,     0,     0,     0,
     0,     0,     0,     3,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     4,     0,     0,  -130,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,     0,     0,    22,    23,    24,    25,    26,    27,     1,
     0,     2,    28,    29,     0,     0,     0,     0,    30,    31,
    32,    33,     0,     0,     0,     0,     0,     0,     0,    34,
     0,     0,     0,     0,    35,     0,     0,    36,     0,     0,
    37,     0,     0,     0,    38,     0,     3,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     4,     0,     0,  -130,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,     0,     0,    22,    23,    24,    25,
    26,    27,     0,     0,     0,    28,    29,     0,     0,     0,
     0,    30,    31,    32,    33,     2,   293,   294,   295,   296,
     0,     0,    34,     0,     0,     0,     0,    35,     0,     0,
    36,     0,     0,    37,     0,     0,     0,    38,   203,     2,
   293,   294,   295,   296,     0,     0,     0,     0,     0,     0,
   297,     0,     0,     0,   298,   299,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     4,   300,   301,
     0,     0,     0,     0,   297,     0,     0,     0,   298,   299,
     0,     0,     0,     0,     0,     0,     0,     0,   302,   303,
     0,     4,   300,   301,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    30,    31,    32,     0,     0,
     0,     0,   302,   303,     0,     0,   304,     0,     0,   305,
   306,   307,     0,     0,   308,     0,   309,   310,     0,    30,
    31,    32,     0,  -338,     2,   293,   294,   295,   296,     0,
   304,     0,     0,   305,   306,   307,     0,     0,   308,   -91,
   309,   310,     0,     0,     0,     0,   487,     2,   293,   294,
   295,   296,     0,     0,     0,     0,     0,     0,     0,   297,
     0,     0,     0,   298,   299,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     4,   300,   301,     0,
     0,     0,   297,     0,     0,     0,   298,   299,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   302,   303,     4,
   300,   301,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    30,    31,    32,     0,     0,     0,
   302,   303,     0,     0,     0,   304,     0,     0,   305,   306,
   307,     0,     0,   308,     0,   309,   310,    30,    31,    32,
   292,   347,     2,   293,   294,   295,   296,     0,   304,     0,
     0,   305,   306,   307,     0,     0,   308,     0,   309,   310,
     0,     0,     0,     0,   487,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   297,     0,     0,
     0,   298,   299,     0,     0,     0,     0,     0,     0,     0,
     0,  -303,   -77,   -77,     4,   300,   301,     0,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,   302,   303,    22,    23,    24,
   168,    26,    27,     0,     0,     0,    28,    29,     0,     0,
     0,     0,    30,    31,    32,    33,     2,   293,   294,   295,
   296,     0,     0,   304,     0,     0,   305,   306,   307,     0,
     0,   308,  -303,   309,   310,     0,     0,     0,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
     0,   297,     0,    20,    21,   298,   299,    22,    23,    24,
   168,    26,    27,     0,     0,     0,    28,    29,     4,   300,
   301,     0,     0,     0,     0,     0,     0,     2,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
   303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    30,    31,    32,     0,
     0,     0,     3,     0,     0,     0,     0,   304,     0,     0,
   305,   306,   307,     0,     0,   308,     0,   309,   310,     4,
  -269,     0,     0,     5,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     0,     0,    22,    23,    24,   168,    26,    27,     0,     0,
     0,    28,    29,     0,     0,     0,     0,    30,    31,    32,
     2,   293,   294,   295,   296,     0,     0,     0,    34,     0,
     0,     0,     0,    35,     0,     0,   478,     0,     0,    37,
  -295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   297,     0,     0,     0,   298,
   299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     4,   300,   301,     0,     0,     0,     0,     0,
     0,     0,     0,     2,   293,   294,   295,   296,     0,     0,
     0,     0,     0,   302,   303,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    30,    31,    32,     0,     0,     0,     0,     0,   297,     0,
     0,   304,   298,   299,   305,   306,   307,     0,     0,   308,
     0,   309,   310,   580,     0,     4,   300,   301,     0,     0,
     0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,   302,   303,     2,     0,
     0,     0,     0,     0,     0,     0,     0,    28,    29,     0,
     0,     0,     0,    30,    31,    32,     0,     0,     0,     0,
     0,     0,     0,     0,   304,     0,     0,   305,   306,   307,
     0,     0,   308,     3,   309,   310,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     4,     0,     0,     0,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,     2,     0,    22,    23,    24,   168,    26,    27,     0,
     0,     0,    28,    29,     0,     0,     0,     0,    30,    31,
    32,    33,     0,     0,     0,     0,     0,     0,     0,    34,
     0,     0,     0,     0,    35,     0,     3,    36,     0,     0,
    37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     4,     0,     0,     0,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,   292,     0,    22,    23,    24,   168,
    26,    27,     0,     0,     0,    28,    29,     0,     0,     0,
     0,    30,    31,    32,     0,     0,     0,     0,     0,     0,
     0,     0,    34,     0,     0,     0,     0,    35,     0,     0,
    36,     3,     0,    37,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     2,   293,   294,   295,   296,     0,     0,
     0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
    13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
     0,    22,    23,    24,   168,    26,    27,     0,   297,     0,
    28,    29,   298,   299,     0,     0,    30,    31,    32,    33,
     0,     0,     0,     0,     0,     4,   300,   301,     0,     0,
     0,     0,     0,     0,     0,     0,  -303,     0,     0,     0,
     0,     2,   293,   294,   295,   296,   302,   303,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    30,    31,    32,     2,   293,   294,   295,
   296,     0,     0,     0,   304,     0,   297,   305,   306,   307,
   298,   299,   308,  -103,   309,   310,     0,     0,     0,     0,
     0,     0,     0,     4,   300,   301,     0,     0,     0,     0,
     0,   297,     0,     0,     0,   298,   299,     0,     0,     2,
   293,   294,   295,   296,   302,   303,     0,     0,     4,   300,
   301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    30,    31,    32,     2,   293,   294,   295,   296,   302,
   303,     0,   304,     0,   297,   305,   306,   307,   298,   299,
   308,   635,   309,   310,     0,     0,    30,    31,    32,     0,
     0,     4,   300,   301,     0,     0,     0,   304,     0,   297,
   305,   306,   307,   298,   299,   308,   637,   309,   310,     0,
     0,     0,   302,   303,     0,     0,     4,   300,   301,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
    31,    32,     2,   293,   294,   295,   296,   302,   303,     0,
   304,     0,     0,   305,   306,   307,     0,     0,   419,     0,
   309,   310,     0,     0,    30,    31,    32,     0,     0,     0,
     0,     0,     0,     0,     0,   304,     0,   297,   305,   306,
   307,   298,   299,   422,     0,   309,   310,     0,     0,     0,
     0,     0,     0,     0,     4,   300,   301,     0,   292,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   302,   303,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    30,    31,    32,     3,   292,     0,     0,     0,
     0,     0,     0,   304,     0,     0,   305,   306,   307,   601,
     0,   308,     0,   309,   310,     0,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,     3,     0,    22,    23,    24,   168,    26,
    27,     0,     0,     0,    28,    29,     0,     0,     0,     0,
    30,    31,    32,    33,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,     0,     0,    22,    23,    24,   168,    26,    27,     0,
     0,     0,    28,    29,     0,     0,     0,     0,    30,    31,
    32,    33,    93,    94,    95,    96,    97,    98,    99,   100,
   101,   102,     3,   103,     0,   104,   105,   106,   107,   108,
   109,   110,   111,   112,   113,   114,     0,   115,   116,     0,
     0,     0,     0,     0,     0,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    28,    29,     0,     0,     0,     0,    30,    31,    32,
     0,     0,   117,   118,     0,     0,   119,   120,   121,   122,
   123,   124,   125,   126,   127,   128,   129,     0,   130,   131,
   132,    93,    94,    95,    96,    97,    98,    99,   100,   101,
   102,     0,   103,     3,   104,   105,   106,   107,   108,   109,
   110,   111,   112,   113,   114,     0,   115,   116,     0,     0,
     0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,     0,     0,    22,    23,    24,   168,    26,    27,     0,
     0,     0,    28,    29,     0,     0,     0,     3,    30,    31,
    32,   117,   118,     0,     0,   119,   120,   121,   122,   123,
   124,   125,   126,   127,   128,   129,     0,   130,   131,   132,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    28,    29,     0,     0,
     0,     0,    30,    31,    32,     3,     0,     0,     0,     0,
     0,     0,     0,    34,     0,     0,     0,     0,    35,     0,
     0,   523,   353,     0,     0,  -295,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,     3,     0,    22,    23,    24,   168,    26,
    27,     0,     0,     0,    28,    29,     0,     0,     0,     0,
   193,    31,    32,     0,     0,     0,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,     0,   354,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    28,    29,     0,     3,     0,     0,    30,    31,
    32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   645,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    28,    29,     3,     0,     0,     0,
    30,    31,    32,     0,     0,     0,     0,     0,     0,     0,
     0,    34,     0,     0,     0,     0,    35,     0,     0,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,    20,    21,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    28,    29,     3,     0,     0,
     0,    30,    31,    32,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   680,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,     0,     0,    22,    23,    24,
   168,    26,    27,     0,     0,     0,    28,    29,     0,     0,
     0,     0,    30,    31,    32,    33
};

static const short yycheck[] = {     0,
   207,    69,     3,   252,    35,   192,    69,   185,   186,    69,
    48,   208,    45,    69,     0,   347,     3,    49,   248,    49,
   166,   326,    69,    91,   162,     3,    56,   160,   487,   382,
    49,    39,   278,     3,    44,    36,     0,    56,    39,   336,
   220,    34,    35,   220,   409,   485,   520,     3,    49,    49,
    36,   248,    98,    39,     3,     3,    38,    99,   308,     0,
    53,   471,    48,    49,    72,    55,    69,   119,   478,    60,
   154,    72,    98,   119,     3,    39,   144,    78,   120,   217,
     3,   378,   262,     7,     3,    49,    72,   113,   144,   157,
   100,   229,    78,   151,     3,     3,   428,   243,    39,    55,
   105,   157,    93,    93,   544,    36,    93,   175,    72,    93,
   113,    93,   175,   523,   101,   175,   365,    48,    93,   175,
   297,   205,   104,    58,    94,   115,    55,   109,   175,   101,
   112,    72,   378,   607,   119,   467,    55,    93,    94,    95,
    38,   446,   447,   119,    93,    93,   147,   140,   120,   207,
   151,   151,   182,   101,   205,   487,    78,   462,   291,   115,
   244,   147,   220,   182,    93,   151,   353,   205,   154,   227,
    93,   101,   422,   192,    93,    58,     0,   542,   171,    49,
   166,   232,    98,   363,    93,    93,   363,   151,    98,    38,
   120,   192,   192,   244,   232,    93,   115,   113,   231,   192,
     3,   233,   203,   233,    98,   224,   244,   387,   276,   119,
   387,   109,   106,    96,   233,    39,    98,   203,   677,   205,
   151,    98,   280,   154,   224,   147,   227,   258,   192,    48,
    49,   224,   233,   233,    98,   600,    38,   119,    98,   203,
   658,   297,   119,   207,   166,   120,   232,   233,    72,   554,
   668,   335,    55,   113,   503,   119,   220,   243,   244,   278,
   224,    98,   203,   227,   151,   258,   112,    98,   633,   233,
   116,   695,   604,   357,   205,   628,   117,   278,   278,   280,
   117,   289,   113,   707,   473,   278,   105,   106,   289,   308,
    93,   480,   278,   105,   357,   358,   297,   357,   358,    98,
   112,   232,   233,   289,   336,   363,   336,    48,   308,     3,
   357,   358,   115,   244,   278,   308,   280,   336,   117,   297,
   207,   243,   192,   520,   382,   289,   358,   473,   358,   387,
   113,     3,    98,   220,   353,   524,   336,   113,   476,   358,
   227,   548,   549,   648,   308,   677,   120,   113,   289,   335,
   336,   105,   353,   353,   224,   473,   278,   358,   358,   378,
   353,     0,   480,   233,   696,   595,    38,    87,    88,   701,
   577,   357,   336,    67,   405,    69,     1,   378,   378,   203,
    98,   382,   112,    55,   634,   378,   116,   471,   106,   353,
   119,   578,   378,   280,   358,   645,   220,    91,   595,   363,
    39,   647,   109,   422,   335,   336,   524,   279,   278,   281,
   607,   101,   405,   154,   378,   105,   112,    98,   382,   101,
   116,    93,   422,   387,   112,   166,   357,   485,   116,   422,
   120,    98,   104,    72,   101,    60,   120,   109,   308,     3,
   112,   691,    93,   115,   528,   119,   643,   378,   626,   627,
   144,   112,   483,   516,   592,   116,   516,   119,   422,    84,
    85,    86,   463,   157,   205,   289,   336,   468,    93,   516,
    96,    84,    85,    86,    46,    47,   363,   478,   120,   480,
    93,   175,   113,   353,   485,   471,   544,   473,   358,    98,
   483,   232,   478,   187,   480,   382,   112,   529,    98,   529,
   387,   101,   243,   244,    99,   105,   106,   526,   378,    45,
   529,   102,   543,   103,    39,   583,   104,    42,    43,   112,
   120,   485,   523,   524,   608,    52,   526,   583,   529,   529,
    93,    94,    95,   526,   113,     0,    98,   523,   524,   363,
   471,   117,   528,   544,   121,   608,   119,   478,   608,   119,
   543,   473,   422,   119,   435,   436,   437,   438,   480,   578,
     0,   608,   526,   387,   203,   529,    50,    51,   262,   119,
   628,    36,   107,   108,    39,   269,   119,   578,   578,   121,
   544,   220,   276,    48,    49,   578,   101,   112,    99,   283,
   121,   116,   523,   118,   335,    40,    41,   528,   485,    39,
    53,    54,   524,   297,     0,   433,   434,    72,    48,    49,
   109,   110,   111,    78,   578,   634,   357,    98,   439,   440,
   101,    98,   608,   441,   442,   106,   645,   628,   647,   443,
   444,   445,    72,   634,   634,   113,   119,   117,    78,   120,
    36,   634,    98,    39,   645,   645,   647,   647,    91,   680,
   289,   207,   645,    49,   647,   211,   526,   544,   112,   529,
   112,   647,    98,   113,   628,   112,    99,    99,   224,    99,
   634,    99,   691,    98,   705,   207,    72,   608,   160,   211,
    98,   645,   147,   647,   101,   101,   151,   680,   101,   154,
   691,   691,   224,   113,   695,   177,   117,   121,   691,     3,
   112,   166,   396,   397,   398,     3,   707,   147,   578,   695,
   113,   113,   705,    98,   113,   113,   121,   112,   200,   201,
    99,   707,   119,   112,   363,   117,   166,   691,   121,     5,
   471,   113,   473,    98,    38,   113,   116,    38,   203,   113,
   205,   628,   298,   299,   300,    98,   109,     0,   387,     3,
     0,    55,   308,   429,   203,   151,   427,    55,   154,   432,
   430,   482,   431,   203,   634,   205,   692,   232,   233,   463,
    19,   253,   254,   522,   468,   645,    49,   647,   243,   244,
   217,   628,   280,   280,    38,   151,   603,   528,   526,    93,
   151,   347,   232,   387,   363,    93,    94,    95,   608,   358,
   104,    55,   284,   243,   244,   109,   516,   203,   112,    -1,
   397,   115,    -1,   550,    -1,    -1,     3,   115,    -1,    -1,
    -1,   691,    -1,    -1,   289,    -1,    -1,    -1,    -1,     3,
    -1,    -1,    -1,    -1,    -1,    -1,   530,   233,    -1,    93,
    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   289,
   104,    38,    -1,    -1,    -1,   109,   550,    -1,   112,    -1,
    -1,   115,   116,   419,    38,    -1,   422,   608,    55,    -1,
   335,   336,   428,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    55,    -1,    -1,   366,    -1,    -1,   369,    -1,   583,
    -1,    -1,   357,   289,    -1,     1,    -1,    -1,    -1,    -1,
    -1,    -1,   384,   459,    -1,    -1,    93,    -1,    95,   391,
   466,   467,    -1,   469,    -1,    -1,    -1,   104,    -1,    93,
    -1,    95,   109,    -1,    -1,   112,   482,   459,   115,    -1,
   104,   487,    38,    -1,   466,   109,    -1,   469,   112,    -1,
   336,   115,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,
   482,    -1,    -1,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
   526,    -1,    78,    79,    80,    81,    82,    83,    -1,     3,
    -1,    87,    88,    -1,    -1,    -1,    -1,    93,    94,    95,
    96,    -1,   548,   549,   526,    -1,   552,    -1,   463,    -1,
    -1,    -1,    -1,   468,    -1,    -1,   471,   113,   473,    -1,
    -1,    -1,    -1,   478,    38,   480,   548,   549,    -1,   501,
   552,   577,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,
    -1,    55,   514,   473,    -1,   591,    -1,    -1,   594,    -1,
   480,   597,   598,    -1,    -1,   577,    -1,    -1,   604,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,   591,
    -1,    -1,   594,   528,    -1,   597,   598,    -1,    -1,    93,
    -1,    95,    -1,    -1,    -1,   471,    38,    -1,    -1,    -1,
   104,    -1,   478,    -1,    -1,   109,    -1,    -1,   112,    -1,
     1,   115,     3,    -1,    -1,    -1,    -1,    -1,    -1,    61,
    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,
    99,   677,    -1,    -1,    -1,    87,    88,    38,    -1,   611,
    -1,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
   696,    -1,    -1,   608,    55,   701,    -1,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    -1,    -1,    78,    79,    80,
    81,    82,    83,    -1,    -1,    -1,    87,    88,     1,    -1,
     3,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,
    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,
   121,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    55,    -1,    -1,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,     1,    -1,    78,    79,    80,    81,    82,
    83,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
    93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,
    38,    -1,   115,    -1,    -1,    -1,   119,    -1,   121,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
    -1,    59,    60,    61,    62,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    74,    75,     1,    -1,
    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    38,    -1,   115,    -1,    -1,
    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    55,    -1,    -1,    -1,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    -1,    -1,    78,    79,    80,    81,    82,
    83,    84,    85,    86,    87,    88,     0,     1,    -1,     3,
    93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,   121,    -1,
    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    55,    -1,    -1,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
    74,    75,    -1,    -1,    78,    79,    80,    81,    82,    83,
    -1,    -1,    -1,    87,    88,     0,     1,    -1,     3,    93,
    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    55,    -1,    -1,    58,    59,    60,    61,    62,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    -1,    -1,    78,    79,    80,    81,    82,    83,     1,
    -1,     3,    87,    88,    -1,    -1,    -1,    -1,    93,    94,
    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
   115,    -1,    -1,    -1,   119,    -1,    38,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    55,    -1,    -1,    58,    59,    60,    61,
    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,    -1,    -1,    78,    79,    80,    81,
    82,    83,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
    -1,    93,    94,    95,    96,     3,     4,     5,     6,     7,
    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
   112,    -1,    -1,   115,    -1,    -1,    -1,   119,   120,     3,
     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
    38,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    43,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
    -1,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    -1,    -1,
    -1,    -1,    76,    77,    -1,    -1,   104,    -1,    -1,   107,
   108,   109,    -1,    -1,   112,    -1,   114,   115,    -1,    93,
    94,    95,    -1,   121,     3,     4,     5,     6,     7,    -1,
   104,    -1,    -1,   107,   108,   109,    -1,    -1,   112,   113,
   114,   115,    -1,    -1,    -1,    -1,   120,     3,     4,     5,
     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,
    -1,    -1,    38,    -1,    -1,    -1,    42,    43,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    55,
    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    93,    94,    95,    -1,    -1,    -1,
    76,    77,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,
   109,    -1,    -1,   112,    -1,   114,   115,    93,    94,    95,
     1,   120,     3,     4,     5,     6,     7,    -1,   104,    -1,
    -1,   107,   108,   109,    -1,    -1,   112,    -1,   114,   115,
    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    -1,    -1,    -1,    87,    88,    -1,    -1,
    -1,    -1,    93,    94,    95,    96,     3,     4,     5,     6,
     7,    -1,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
    -1,   112,   113,   114,   115,    -1,    -1,    -1,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    -1,
    -1,    38,    -1,    74,    75,    42,    43,    78,    79,    80,
    81,    82,    83,    -1,    -1,    -1,    87,    88,    55,    56,
    57,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    -1,
    -1,    -1,    38,    -1,    -1,    -1,    -1,   104,    -1,    -1,
   107,   108,   109,    -1,    -1,   112,    -1,   114,   115,    55,
   117,    -1,    -1,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    -1,    -1,    78,    79,    80,    81,    82,    83,    -1,    -1,
    -1,    87,    88,    -1,    -1,    -1,    -1,    93,    94,    95,
     3,     4,     5,     6,     7,    -1,    -1,    -1,   104,    -1,
    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,
    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    38,    -1,
    -1,   104,    42,    43,   107,   108,   109,    -1,    -1,   112,
    -1,   114,   115,   116,    -1,    55,    56,    57,    -1,    -1,
    -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,     3,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
    -1,    -1,    -1,    93,    94,    95,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,   109,
    -1,    -1,   112,    38,   114,   115,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    55,    -1,    -1,    -1,    59,    60,    61,    62,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,     3,    -1,    78,    79,    80,    81,    82,    83,    -1,
    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    93,    94,
    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
    -1,    -1,    -1,    -1,   109,    -1,    38,   112,    -1,    -1,
   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    55,    -1,    -1,    -1,    59,    60,    61,
    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,     1,    -1,    78,    79,    80,    81,
    82,    83,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
    -1,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
   112,    38,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
    -1,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    -1,
    -1,    78,    79,    80,    81,    82,    83,    -1,    38,    -1,
    87,    88,    42,    43,    -1,    -1,    93,    94,    95,    96,
    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
    -1,     3,     4,     5,     6,     7,    76,    77,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    93,    94,    95,     3,     4,     5,     6,
     7,    -1,    -1,    -1,   104,    -1,    38,   107,   108,   109,
    42,    43,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    55,    56,    57,    -1,    -1,    -1,    -1,
    -1,    38,    -1,    -1,    -1,    42,    43,    -1,    -1,     3,
     4,     5,     6,     7,    76,    77,    -1,    -1,    55,    56,
    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    93,    94,    95,     3,     4,     5,     6,     7,    76,
    77,    -1,   104,    -1,    38,   107,   108,   109,    42,    43,
   112,   113,   114,   115,    -1,    -1,    93,    94,    95,    -1,
    -1,    55,    56,    57,    -1,    -1,    -1,   104,    -1,    38,
   107,   108,   109,    42,    43,   112,   113,   114,   115,    -1,
    -1,    -1,    76,    77,    -1,    -1,    55,    56,    57,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
    94,    95,     3,     4,     5,     6,     7,    76,    77,    -1,
   104,    -1,    -1,   107,   108,   109,    -1,    -1,   112,    -1,
   114,   115,    -1,    -1,    93,    94,    95,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   104,    -1,    38,   107,   108,
   109,    42,    43,   112,    -1,   114,   115,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    55,    56,    57,    -1,     1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    93,    94,    95,    38,     1,    -1,    -1,    -1,
    -1,    -1,    -1,   104,    -1,    -1,   107,   108,   109,    52,
    -1,   112,    -1,   114,   115,    -1,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    38,    -1,    78,    79,    80,    81,    82,
    83,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
    93,    94,    95,    96,    59,    60,    61,    62,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    -1,    -1,    78,    79,    80,    81,    82,    83,    -1,
    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    93,    94,
    95,    96,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    -1,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    -1,    53,    54,    -1,
    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    87,    88,    -1,    -1,    -1,    -1,    93,    94,    95,
    -1,    -1,    98,    99,    -1,    -1,   102,   103,   104,   105,
   106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
   116,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    -1,    39,    38,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    -1,    53,    54,    -1,    -1,
    -1,    -1,    -1,    -1,    59,    60,    61,    62,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    -1,    -1,    78,    79,    80,    81,    82,    83,    -1,
    -1,    -1,    87,    88,    -1,    -1,    -1,    38,    93,    94,
    95,    98,    99,    -1,    -1,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
    -1,    -1,    93,    94,    95,    38,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,
    -1,   112,    55,    -1,    -1,   116,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    38,    -1,    78,    79,    80,    81,    82,
    83,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
    93,    94,    95,    -1,    -1,    -1,    61,    62,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    87,    88,    -1,    38,    -1,    -1,    93,    94,
    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    87,    88,    38,    -1,    -1,    -1,
    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    61,
    62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    87,    88,    38,    -1,    -1,
    -1,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    -1,    -1,    78,    79,    80,
    81,    82,    83,    -1,    -1,    -1,    87,    88,    -1,    -1,
    -1,    -1,    93,    94,    95,    96
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 449 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 2:
#line 451 "c++21.gram.y"
{ 
		    if (yyvsp[0].decllist)
		    {
			delete yyvsp[0].decllist;
		    }
		;
    break;}
case 4:
#line 462 "c++21.gram.y"
{ 
		    if (yyvsp[-1].decllist)
		    {
			if (yyvsp[0].decllist)
			{
			    yyvsp[-1].decllist->appendList(yyvsp[0].decllist);
			    delete yyvsp[0].decllist;
			}
		    }
		    else
			yyval.decllist = yyvsp[0].decllist;
		;
    break;}
case 6:
#line 479 "c++21.gram.y"
{ yyvsp[0].decl->addStubModifier(STUB_NOMARSHALL);GlobalInfo::putIn(yyvsp[0].decl); yyval.decllist = new DeclarationList(yyvsp[0].decl); ;
    break;}
case 9:
#line 483 "c++21.gram.y"
{ yyval.decllist = new DeclarationList(yyvsp[0].decl); ;
    break;}
case 11:
#line 489 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(",", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 13:
#line 495 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(*yyvsp[-1].text, yyvsp[-2].expr, yyvsp[0].expr); delete yyvsp[-1].text; ;
    break;}
case 14:
#line 500 "c++21.gram.y"
{ yyval.text = new String("="); ;
    break;}
case 15:
#line 502 "c++21.gram.y"
{ yyval.text = new String("*="); ;
    break;}
case 16:
#line 504 "c++21.gram.y"
{ yyval.text = new String("/="); ;
    break;}
case 17:
#line 506 "c++21.gram.y"
{ yyval.text = new String("%="); ;
    break;}
case 18:
#line 508 "c++21.gram.y"
{ yyval.text = new String("+="); ;
    break;}
case 19:
#line 510 "c++21.gram.y"
{ yyval.text = new String("-="); ;
    break;}
case 20:
#line 512 "c++21.gram.y"
{ yyval.text = new String(">>="); ;
    break;}
case 21:
#line 514 "c++21.gram.y"
{ yyval.text = new String("<<="); ;
    break;}
case 22:
#line 516 "c++21.gram.y"
{ yyval.text = new String("&="); ;
    break;}
case 23:
#line 518 "c++21.gram.y"
{ yyval.text = new String("^="); ;
    break;}
case 24:
#line 520 "c++21.gram.y"
{ yyval.text = new String("|="); ;
    break;}
case 26:
#line 526 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("?", yyvsp[-4].expr,
					    new BinaryExpression(":", yyvsp[-2].expr, yyvsp[0].expr));
	        ;
    break;}
case 28:
#line 534 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("||", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 30:
#line 540 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("&&", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 32:
#line 546 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("|", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 34:
#line 552 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("^", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 36:
#line 558 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("&", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 38:
#line 564 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("==", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 39:
#line 566 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("!=", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 41:
#line 572 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("<", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 42:
#line 574 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(">", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 43:
#line 576 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("<=", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 44:
#line 578 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(">=", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 46:
#line 584 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("<<", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 47:
#line 586 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(">>", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 49:
#line 592 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("+", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 50:
#line 594 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("-", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 52:
#line 600 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("*", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 53:
#line 602 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("/", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 54:
#line 604 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("%", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 56:
#line 610 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(".*", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 57:
#line 612 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("->*", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 59:
#line 618 "c++21.gram.y"
{ 
		    TypeDescriptor *tp = 0;
		    String sp;

		    if (yyvsp[-2].decl && ((tp = yyvsp[-2].decl->getType()) != 0))
			sp = tp->typeName(NORMAL_DECL);

		    yyval.expr = new BinaryExpression("",
					      new NestedExpression(new Literal(sp)),
					      yyvsp[0].expr);
		;
    break;}
case 61:
#line 634 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("++", 0, yyvsp[0].expr); ;
    break;}
case 62:
#line 636 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("--", 0, yyvsp[0].expr); ;
    break;}
case 63:
#line 638 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(*yyvsp[-1].text, 0, yyvsp[0].expr); delete yyvsp[-1].text; ;
    break;}
case 64:
#line 640 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("sizeof", 0, yyvsp[0].expr); ;
    break;}
case 65:
#line 642 "c++21.gram.y"
{ 
		    TypeDescriptor *tp = 0;
		    String sp;

		    if (yyvsp[-1].decl && ((tp = yyvsp[-1].decl->getType()) != 0))
			sp = tp->typeName(NORMAL_DECL);


		    yyval.expr = new BinaryExpression("sizeof", 0,
					      new NestedExpression(new Literal(sp)));
		    if (yyvsp[-1].decl)
			delete yyvsp[-1].decl;
		;
    break;}
case 68:
#line 661 "c++21.gram.y"
{ yyval.text = new String("*"); ;
    break;}
case 69:
#line 663 "c++21.gram.y"
{ yyval.text = new String("&"); ;
    break;}
case 70:
#line 665 "c++21.gram.y"
{ yyval.text = new String("+"); ;
    break;}
case 71:
#line 667 "c++21.gram.y"
{ yyval.text = new String("-"); ;
    break;}
case 72:
#line 669 "c++21.gram.y"
{ yyval.text = new String("!"); ;
    break;}
case 73:
#line 671 "c++21.gram.y"
{ yyval.text = new String("~"); ;
    break;}
case 74:
#line 676 "c++21.gram.y"
{ 
		    Boolean useGlobal = yyvsp[-4].expr ? TRUE : FALSE;
		
		    yyval.expr = new AllocExpression(useGlobal, yyvsp[-2].expr, new Literal(*yyvsp[-1].text), yyvsp[0].expr);
		;
    break;}
case 75:
#line 682 "c++21.gram.y"
{ 
		    Boolean useGlobal = yyvsp[-6].expr ? TRUE : FALSE;
		    TypeDescriptor *tp = 0;
		    String sp;

		    if (yyvsp[-2].decl && ((tp = yyvsp[-2].decl->getType()) != 0))
			sp = tp->typeName(NORMAL_DECL);

		    yyval.expr = new AllocExpression(useGlobal, yyvsp[-4].expr, new Literal(sp), yyvsp[0].expr);
		;
    break;}
case 76:
#line 696 "c++21.gram.y"
{ yyval.expr = new Literal("::"); ;
    break;}
case 77:
#line 698 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 79:
#line 704 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 80:
#line 709 "c++21.gram.y"
{ yyval.expr = new NestedExpression(yyvsp[-1].expr); ;
    break;}
case 81:
#line 714 "c++21.gram.y"
{
		    yyvsp[-1].declspec->finalise();
		    if (yyvsp[0].decl)
			yyvsp[0].decl->attachBasicSpec(yyvsp[-1].declspec);
		    else
			yyvsp[0].decl = new Declaration(NullString, yyvsp[-1].declspec->getType());
		    delete yyvsp[-1].declspec;
		    yyval.text = new String(yyvsp[0].decl->getType()->typeName(NORMAL_DECL));
		;
    break;}
case 83:
#line 728 "c++21.gram.y"
{ yyval.decl = 0; ;
    break;}
case 84:
#line 733 "c++21.gram.y"
{
		    PointerType *pt = new PointerType(POINTER_TYPE, 0, yyvsp[-1].typedesc);

		    if (yyvsp[0].decl)
		    {
			yyvsp[0].decl->adjustType(pt);
			yyval.decl = yyvsp[0].decl;
		    }
		    else
			yyval.decl = new Declaration(NullString, pt);
		;
    break;}
case 85:
#line 745 "c++21.gram.y"
{
		    PointerType *pt = new PointerType(POINTER_TYPE, 0, yyvsp[-1].typedesc);
		    QualifiedType *qt = new QualifiedType(yyvsp[-4].typedesc, pt);
		    if (yyvsp[0].decl)
		    {
			yyvsp[0].decl->adjustType(qt);
			yyval.decl = yyvsp[0].decl;
		    }
		    else
			yyval.decl = new Declaration(NullString, qt);
		;
    break;}
case 86:
#line 757 "c++21.gram.y"
{
                    VecType *vp = new VecType(yyvsp[-1].expr, 0);

		    if (yyvsp[-3].decl)
		    {
			yyvsp[-3].decl->adjustType(vp);
			yyval.decl = yyvsp[-3].decl;
		    } else
			yyval.decl = new Declaration(NullString, vp);
                ;
    break;}
case 88:
#line 772 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 89:
#line 777 "c++21.gram.y"
{ yyval.expr = new NestedExpression(yyvsp[-1].expr); ;
    break;}
case 91:
#line 783 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 92:
#line 788 "c++21.gram.y"
{
		    String op(yyvsp[-2].expr ? "::" : "");

		    op += "delete ";
		    yyval.expr = new BinaryExpression(op, 0, yyvsp[0].expr);
                ;
    break;}
case 93:
#line 795 "c++21.gram.y"
{
		    String op(yyvsp[-4].expr ? "::" : "");

		    op += "delete []";
		    yyval.expr = new BinaryExpression(op, 0, yyvsp[0].expr);
                ;
    break;}
case 95:
#line 805 "c++21.gram.y"
{  yyval.expr = new ArrayExpression(yyvsp[-3].expr, yyvsp[-1].expr); ;
    break;}
case 96:
#line 807 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("", yyvsp[-3].expr, new NestedExpression(yyvsp[-1].expr)); ;
    break;}
case 97:
#line 809 "c++21.gram.y"
{
		    String tn;

		    if (yyvsp[-3].typedesc)
			tn = yyvsp[-3].typedesc->typeName(NORMAL_DECL);

		    yyval.expr = new BinaryExpression("", new Literal(tn),
					      new NestedExpression(yyvsp[-1].expr));
		;
    break;}
case 98:
#line 819 "c++21.gram.y"
{  yyval.expr = new BinaryExpression(".", yyvsp[-2].expr, new Literal(*yyvsp[0].text)); ;
    break;}
case 99:
#line 821 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("->", yyvsp[-2].expr, new Literal(*yyvsp[0].text)); ;
    break;}
case 100:
#line 823 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("++", yyvsp[-1].expr, 0); ;
    break;}
case 101:
#line 825 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("--", yyvsp[-1].expr, 0); ;
    break;}
case 103:
#line 831 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 105:
#line 837 "c++21.gram.y"
{  yyval.expr = new BinaryExpression(",", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 107:
#line 843 "c++21.gram.y"
{ yyval.expr = new Literal("this"); ;
    break;}
case 108:
#line 845 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("::", 0, new Literal(*yyvsp[0].text)); delete yyvsp[0].text;;
    break;}
case 109:
#line 847 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("::", 0, new Literal(*yyvsp[0].text)); ;
    break;}
case 110:
#line 849 "c++21.gram.y"
{  yyval.expr = new BinaryExpression("::", 0, new Literal(*yyvsp[0].text)); ;
    break;}
case 111:
#line 851 "c++21.gram.y"
{ yyval.expr = new NestedExpression(yyvsp[-1].expr); ;
    break;}
case 112:
#line 853 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[0].text); delete yyvsp[0].text; ;
    break;}
case 115:
#line 860 "c++21.gram.y"
{ yyval.text = new String(String("~") + yyvsp[0].decl->getName()); ;
    break;}
case 117:
#line 866 "c++21.gram.y"
{ 
		    yyval.text = new String(yyvsp[-1].typedesc->typeName(NORMAL_DECL) + "::" + *yyvsp[0].text);
                    exitTemporaryNest();
		    delete yyvsp[0].text;
		;
    break;}
case 118:
#line 872 "c++21.gram.y"
{ 
		    yyval.text = new String(yyvsp[-1].typedesc->typeName(NORMAL_DECL) + "::" + *yyvsp[0].text);
                    exitTemporaryNest();
		    delete yyvsp[0].text;
		;
    break;}
case 119:
#line 878 "c++21.gram.y"
{ 
		    yyval.text = new String(yyvsp[-2].typedesc->typeName(NORMAL_DECL) + "::~" + yyvsp[0].decl->getName());
                    exitTemporaryNest();
		;
    break;}
case 120:
#line 886 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[0].text); delete yyvsp[0].text; ;
    break;}
case 121:
#line 888 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[0].text); delete yyvsp[0].text; ;
    break;}
case 122:
#line 890 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[0].text); delete yyvsp[0].text; ;
    break;}
case 123:
#line 892 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[0].text); delete yyvsp[0].text; ;
    break;}
case 124:
#line 894 "c++21.gram.y"
{ yyval.expr = new Literal("true"); ;
    break;}
case 125:
#line 896 "c++21.gram.y"
{ yyval.expr = new Literal("false"); ;
    break;}
case 126:
#line 901 "c++21.gram.y"
{ 
		    Declaration *d = new Declaration(yyvsp[-1].typedesc->simpleTypeName(NORMAL_DECL), yyvsp[-1].typedesc);

                    if (yyvsp[-3].declspec)
                    {
                        yyvsp[-3].declspec->finalise();
                        d->attachBasicSpec(yyvsp[-3].declspec);
		        delete yyvsp[-3].declspec;
                    }

		    d->makeTypedef();
		    GlobalInfo::popContext();
		    GlobalInfo::putIn(d);
		    yyval.decllist = 0; templateNest--;templateArgs = 0;
		    currentCEDecl = d;
		;
    break;}
case 127:
#line 918 "c++21.gram.y"
{ 
		    Declaration *d = new Declaration(NullString, yyvsp[-1].typedesc);
 
                    if (yyvsp[-3].declspec)
                    {
                        yyvsp[-3].declspec->finalise();
                        d->attachBasicSpec(yyvsp[-3].declspec);
		        delete yyvsp[-3].declspec;
                    }

	            d->makeElaborated();
	            d->makeTypedef();
		    GlobalInfo::popContext();
		    GlobalInfo::putIn(d);

		    yyval.decllist = 0; templateNest--;templateArgs = 0;
		    currentCEDecl = d;
		;
    break;}
case 128:
#line 937 "c++21.gram.y"
{
		    if (yyvsp[-4].declspec)
                    {
                        yyvsp[-4].declspec->mergeSpecInto(yyvsp[-2].declspec);
                        delete yyvsp[-4].declspec;
                    }

		    yyvsp[-2].declspec->finalise();
		    GlobalInfo::popContext();
		    if (yyvsp[-1].decl)
		    {
			    yyvsp[-1].decl->attachBasicSpec(yyvsp[-2].declspec);
			    yyvsp[-1].decl->addTemplateArgs(templateArgs);
			    GlobalInfo::putIn(yyvsp[-1].decl);
		    }
		    templateNest--;templateArgs = 0;
		    delete yyvsp[-2].declspec;
		    yyval.decllist = 0;
		;
    break;}
case 130:
#line 961 "c++21.gram.y"
{ yyval.declspec = 0; ;
    break;}
case 132:
#line 968 "c++21.gram.y"
{ yyval.declspec = yyvsp[-1].declspec->mergeSpecInto(yyvsp[0].declspec); delete yyvsp[-1].declspec; ;
    break;}
case 133:
#line 973 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].stubcommand); ;
    break;}
case 134:
#line 978 "c++21.gram.y"
{ templateArgs = new TemplateContext(GlobalInfo::getContext()); yyval.decllist = 0; GlobalInfo::pushContext(templateArgs);;
    break;}
case 135:
#line 980 "c++21.gram.y"
{ yyval.decllist = 0; templateNest++; templateArgs->freeze();;
    break;}
case 136:
#line 985 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 137:
#line 987 "c++21.gram.y"
{ 
		    DeclarationListIterator nd(yyvsp[-1].decllist);
		    Declaration* d;
		    ostrstream errorStream;
		    
		    while ((d = nd()))
		    {
			errorStream << "type expected for ";
			errorStream << d->getName();
			error(errorStream);

			d->attachBasicSpec(0);
			GlobalInfo::putIn(d);
		    }
		;
    break;}
case 138:
#line 1003 "c++21.gram.y"
{
		    /* Current decl is in currentCEDecl and is in symtab */

		    yyvsp[-1].declspec->finalise();
		    currentCEDecl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		    yyval.decllist = new DeclarationList(currentCEDecl);
		;
    break;}
case 139:
#line 1012 "c++21.gram.y"
{
		    TypeDescriptor *t = yyvsp[-1].declspec->getType();
		    Declaration *d = new Declaration(NullString, t);

		    yyvsp[-1].declspec->finalise();
		    d->attachBasicSpec(yyvsp[-1].declspec);
		    GlobalInfo::putIn(d);
		    delete yyvsp[-1].declspec;
		    yyval.decllist = new DeclarationList(d);
		;
    break;}
case 140:
#line 1023 "c++21.gram.y"
{
		    yyvsp[-2].declspec->finalise();

		    if (yyvsp[-1].decllist)
		    {
			Declaration *d;
			DeclarationListIterator next(yyvsp[-1].decllist);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec(yyvsp[-2].declspec);
			    GlobalInfo::putIn(d);
			}
		    }
		    else
		    {
			TypeDescriptor *td = yyvsp[-2].declspec->getType();

			if (td == 0)
			    error("null declarator");
			else
			    if ((td->kind() != AGGREGATE_TYPE) &&
				(td->kind() != ENUMERATION_TYPE))
				error("null declarator");
			    else
			    {
				yyvsp[-2].declspec->propagateAttributes(td);
				if (yyvsp[-2].declspec->isElaborated())
				{
				    Declaration *d = new Declaration(NullString, td);

				    d->makeElaborated();
				    d->attachBasicSpec(0);
				    GlobalInfo::putIn(d);
				}
			    }
		    }
		    delete yyvsp[-2].declspec;
		    yyval.decllist = yyvsp[-1].decllist;
		;
    break;}
case 141:
#line 1064 "c++21.gram.y"
{ 

		    yyvsp[-2].declspec->finalise();

		    if (yyvsp[-1].decllist)
		    {
			Declaration *d;
			DeclarationListIterator next(yyvsp[-1].decllist);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec(yyvsp[-2].declspec);
			    GlobalInfo::putIn(d);
		        }
		    }
		    else
		    {
			    error("null declarator");
		    }
		    yyval.decllist = yyvsp[-1].decllist;
		    delete yyvsp[-2].declspec; 
		;
    break;}
case 142:
#line 1087 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 144:
#line 1093 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 151:
#line 1107 "c++21.gram.y"
{ yyval.declspec = yyvsp[-1].declspec->mergeSpecInto(yyvsp[0].declspec); delete yyvsp[-1].declspec; ;
    break;}
case 152:
#line 1112 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].storagespec); ;
    break;}
case 153:
#line 1114 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].funcspec); ;
    break;}
case 154:
#line 1116 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].cvq); ;
    break;}
case 156:
#line 1122 "c++21.gram.y"
{ yyval.declspec = yyvsp[-1].declspec->mergeSpecInto(yyvsp[0].declspec); delete yyvsp[-1].declspec; ;
    break;}
case 158:
#line 1128 "c++21.gram.y"
{ yyval.declspec = yyvsp[0].declspec->mergeSpecInto(yyvsp[-1].declspec); delete yyvsp[0].declspec; ;
    break;}
case 159:
#line 1133 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].typedesc); ;
    break;}
case 160:
#line 1135 "c++21.gram.y"
{ yyvsp[-1].declspec->addCvQualifier(yyvsp[0].cvq); ;
    break;}
case 161:
#line 1140 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].typedesc, TRUE); ;
    break;}
case 162:
#line 1142 "c++21.gram.y"
{ yyval.declspec = yyvsp[-1].declspec->mergeSpecInto(new DeclSpecifier(yyvsp[0].typedesc, TRUE)); delete yyvsp[-1].declspec; ;
    break;}
case 163:
#line 1147 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].typedesc, FALSE); ;
    break;}
case 164:
#line 1149 "c++21.gram.y"
{ yyval.declspec = yyvsp[-1].declspec->mergeSpecInto(new DeclSpecifier(yyvsp[0].typedesc, FALSE)); delete yyvsp[-1].declspec; ;
    break;}
case 165:
#line 1155 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].typedesc); ;
    break;}
case 166:
#line 1157 "c++21.gram.y"
{ yyvsp[-1].declspec->adjustBasicType(yyvsp[0].typedesc); yyval.declspec = yyvsp[-1].declspec; ;
    break;}
case 167:
#line 1162 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].typedesc); ;
    break;}
case 169:
#line 1165 "c++21.gram.y"
{ yyvsp[-1].declspec->adjustBasicType(yyvsp[0].typedesc); yyval.declspec = yyvsp[-1].declspec; ;
    break;}
case 170:
#line 1167 "c++21.gram.y"
{ yyval.declspec = yyvsp[0].declspec->mergeSpecInto(yyvsp[-1].declspec); delete yyvsp[0].declspec; ;
    break;}
case 171:
#line 1172 "c++21.gram.y"
{ yyval.typedesc = new CharType(); ;
    break;}
case 172:
#line 1174 "c++21.gram.y"
{ yyval.typedesc = new CharType(TRUE); ;
    break;}
case 173:
#line 1176 "c++21.gram.y"
{ yyval.typedesc = new ModifiedType(MOD_SHORT); ;
    break;}
case 174:
#line 1178 "c++21.gram.y"
{ yyval.typedesc = new IntType(); ;
    break;}
case 175:
#line 1180 "c++21.gram.y"
{ yyval.typedesc = new ModifiedType(MOD_LONG); ;
    break;}
case 176:
#line 1182 "c++21.gram.y"
{ yyval.typedesc = new ModifiedType(MOD_SIGNED); ;
    break;}
case 177:
#line 1184 "c++21.gram.y"
{ yyval.typedesc = new ModifiedType(MOD_UNSIGNED); ;
    break;}
case 178:
#line 1186 "c++21.gram.y"
{ yyval.typedesc = new FloatType(FALSE); ;
    break;}
case 179:
#line 1188 "c++21.gram.y"
{ yyval.typedesc = new FloatType(TRUE); ;
    break;}
case 180:
#line 1190 "c++21.gram.y"
{ yyval.typedesc = new VoidType(); ;
    break;}
case 181:
#line 1192 "c++21.gram.y"
{ yyval.typedesc = new BoolType(); ;
    break;}
case 182:
#line 1197 "c++21.gram.y"
{
		    yyval.typedesc = new QualifiedType(yyvsp[-1].typedesc, yyvsp[0].decl->getType());
                    exitTemporaryNest();
		;
    break;}
case 183:
#line 1202 "c++21.gram.y"
{
		    yyval.typedesc = new QualifiedType(yyvsp[-1].typedesc, yyvsp[0].decl->getType());
                    exitTemporaryNest();
		;
    break;}
case 184:
#line 1210 "c++21.gram.y"
{ yyval.storagespec = SC_AUTO; ;
    break;}
case 185:
#line 1212 "c++21.gram.y"
{ yyval.storagespec = SC_REGISTER; ;
    break;}
case 186:
#line 1214 "c++21.gram.y"
{ yyval.storagespec = SC_STATIC; ;
    break;}
case 187:
#line 1216 "c++21.gram.y"
{ yyval.storagespec = SC_EXTERN; ;
    break;}
case 188:
#line 1218 "c++21.gram.y"
{ yyval.storagespec = SC_FRIEND; declIsFriend = TRUE;;
    break;}
case 189:
#line 1220 "c++21.gram.y"
{ yyval.storagespec = SC_TYPEDEF; ;
    break;}
case 190:
#line 1225 "c++21.gram.y"
{ yyval.funcspec = FUNC_INLINE; ;
    break;}
case 191:
#line 1227 "c++21.gram.y"
{ yyval.funcspec = FUNC_VIRTUAL; ;
    break;}
case 192:
#line 1232 "c++21.gram.y"
{ yyval.stubcommand = yyvsp[0].stubcommand; ;
    break;}
case 198:
#line 1245 "c++21.gram.y"
{ yyval.typedesc = yyvsp[0].decl->getType(); ;
    break;}
case 199:
#line 1247 "c++21.gram.y"
{ yyval.typedesc = yyvsp[0].decl->getType(); ;
    break;}
case 201:
#line 1253 "c++21.gram.y"
{ yyval.typedesc = yyvsp[0].decl->getType(); ;
    break;}
case 202:
#line 1255 "c++21.gram.y"
{ yyval.typedesc = yyvsp[0].decl->getType(); ;
    break;}
case 204:
#line 1258 "c++21.gram.y"
{ yyval.typedesc = yyvsp[0].typedesc; ;
    break;}
case 205:
#line 1263 "c++21.gram.y"
{ 
		    yyval.typedesc = new QualifiedType(yyvsp[-1].typedesc, yyvsp[0].decl->getType());
                    exitTemporaryNest();
		;
    break;}
case 207:
#line 1272 "c++21.gram.y"
{ yyval.bclass = 0; ;
    break;}
case 208:
#line 1277 "c++21.gram.y"
{
		    Declaration *d = new Declaration(yyvsp[0].typedesc->simpleTypeName(NORMAL_DECL), yyvsp[0].typedesc);

		    d->makeTypedef();
		    GlobalInfo::putIn(d);
		    currentCEDecl = d;
		;
    break;}
case 209:
#line 1285 "c++21.gram.y"
{
		    Declaration *d = new Declaration(yyvsp[0].typedesc->simpleTypeName(NORMAL_DECL), yyvsp[0].typedesc);

		    d->makeTypedef();
		    GlobalInfo::putIn(d);
		    currentCEDecl = d;
		;
    break;}
case 210:
#line 1296 "c++21.gram.y"
{
		    if (declIsFriend)
		    {
			DeclContext *friendContext;

			friendContext = (currentClassType->getMemberContext())->getEnclosingContext();
			yyval.typedesc = declareClass(*yyvsp[0].text,yyvsp[-1].clSpec,0,friendContext, FALSE);
		    }
		    else
		    {
			yyval.typedesc = declareClass(*yyvsp[0].text,yyvsp[-1].clSpec,0,0, FALSE);
		    }
		    delete yyvsp[0].text;
		;
    break;}
case 211:
#line 1311 "c++21.gram.y"
{
		    if (yyvsp[0].decl->getContext() != GlobalInfo::getContext())
		    {
			yyval.typedesc = declareClass(yyvsp[0].decl->getName(),yyvsp[-1].clSpec,0,0,FALSE);
		    }
		    else
			yyval.typedesc = yyvsp[0].decl->getType();

		;
    break;}
case 212:
#line 1321 "c++21.gram.y"
{ yyval.typedesc = yyvsp[0].decl->getType(); ;
    break;}
case 213:
#line 1326 "c++21.gram.y"
{ yyval.clSpec = CLASS_TYPE; ;
    break;}
case 214:
#line 1328 "c++21.gram.y"
{ yyval.clSpec = STRUCT_TYPE; ;
    break;}
case 215:
#line 1330 "c++21.gram.y"
{ yyval.clSpec = UNION_TYPE; ;
    break;}
case 216:
#line 1335 "c++21.gram.y"
{ 
		    ClassType *clas = (ClassType*)(yyvsp[-1].decl->getType());
		    
                    GlobalInfo::pushContext(clas->getMemberContext());
                    pushClassLevel(clas);
		    temporaryNest++; 
#ifdef SG_DEBUG   
    debugStream << "class.scope() nest is " << temporaryNest <<"\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

		    yyval.typedesc = clas;
		;
    break;}
case 218:
#line 1353 "c++21.gram.y"
{   
		    yyval.typedesc = new QualifiedType(yyvsp[-1].typedesc, yyvsp[0].typedesc);
		;
    break;}
case 219:
#line 1360 "c++21.gram.y"
{ 
		    String eName;
		    
		    if (yyvsp[-3].text)
		    {
			eName = *yyvsp[-3].text;
			delete yyvsp[-3].text;
		    }
		    yyval.typedesc = new EnumType(eName, yyvsp[-1].decllist, TRUE);
		;
    break;}
case 221:
#line 1375 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 223:
#line 1382 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 224:
#line 1387 "c++21.gram.y"
{ 
		    yyvsp[0].decl->makeSpecial();
		    GlobalInfo::putIn(yyvsp[0].decl);
		    yyval.decllist = new DeclarationList(yyvsp[0].decl);
		;
    break;}
case 225:
#line 1393 "c++21.gram.y"
{
		    yyvsp[0].decl->makeSpecial();
		    GlobalInfo::putIn(yyvsp[0].decl);
		    yyvsp[-2].decllist->appendDecl(yyvsp[0].decl);
		;
    break;}
case 226:
#line 1402 "c++21.gram.y"
{ yyval.decl = new Declaration(*yyvsp[0].text); delete yyvsp[0].text;;
    break;}
case 227:
#line 1404 "c++21.gram.y"
{ yyval.decl = new Declaration(*yyvsp[-2].text, 0, new BinaryExpression(equal, 0, yyvsp[0].expr)); delete yyvsp[-2].text;;
    break;}
case 229:
#line 1413 "c++21.gram.y"
{
		    if (yyvsp[-3].linkspec)
			GlobalInfo::popLinkage();
		    yyval.decllist = yyvsp[-1].decllist;
		;
    break;}
case 230:
#line 1419 "c++21.gram.y"
{
		    if (yyvsp[-2].linkspec)
			GlobalInfo::popLinkage();
		    yyval.decllist = 0;
		;
    break;}
case 231:
#line 1425 "c++21.gram.y"
{ 
		    DeclarationListIterator next(yyvsp[0].decllist);
		    Declaration *d;

		    if (yyvsp[-1].linkspec)
		    {
			while ((d = next()))
			{
			    d->setLinkage(yyvsp[-1].linkspec);
			}
			GlobalInfo::popLinkage();
		    }
		    yyval.decllist = yyvsp[0].decllist;
		;
    break;}
case 232:
#line 1443 "c++21.gram.y"
{
		    if (*yyvsp[0].text == String("\"C\""))
		    {
			GlobalInfo::pushLinkage(cLinkage);
			yyval.linkspec = cLinkage;
		    }
		    else if (*yyvsp[0].text == String("\"C++\""))
		    {
			GlobalInfo::pushLinkage(cppLinkage);
			yyval.linkspec = cppLinkage;
		    }
		    else
		    {
			error("illegal language linkage specification");
			yyval.linkspec = 0;
		    }
		    delete yyvsp[0].text;
		;
    break;}
case 233:
#line 1465 "c++21.gram.y"
{ yyval.decllist = new DeclarationList(yyvsp[0].decl); ;
    break;}
case 234:
#line 1467 "c++21.gram.y"
{ yyvsp[-2].decllist->appendDecl(yyvsp[0].decl); ;
    break;}
case 235:
#line 1471 "c++21.gram.y"
{ yyval.decllist = new DeclarationList(yyvsp[0].decl); ;
    break;}
case 236:
#line 1473 "c++21.gram.y"
{ yyvsp[-2].decllist->appendDecl(yyvsp[0].decl); ;
    break;}
case 237:
#line 1478 "c++21.gram.y"
{ if (yyvsp[0].expr) yyvsp[-1].decl->setInit(yyvsp[0].expr); yyval.decl = yyvsp[-1].decl; ;
    break;}
case 241:
#line 1489 "c++21.gram.y"
{ if (yyvsp[0].expr) yyvsp[-1].decl->setInit(yyvsp[0].expr); yyval.decl = yyvsp[-1].decl; ;
    break;}
case 243:
#line 1495 "c++21.gram.y"
{ yyvsp[-1].decl->setInit(yyvsp[0].expr); ;
    break;}
case 244:
#line 1497 "c++21.gram.y"
{
		if (yyvsp[-1].decl)
		{
		    if (yyvsp[-2].typedesc)
			yyvsp[-1].decl->adjustType(yyvsp[-2].typedesc);
		    yyval.decl = yyvsp[-1].decl;
		} else
		    yyval.decl = new Declaration(NullString, yyvsp[-2].typedesc);
		yyval.decl->setInit(yyvsp[0].expr);
	    ;
    break;}
case 245:
#line 1512 "c++21.gram.y"
{ yyval.expr = new NestedExpression(yyvsp[-1].expr); ;
    break;}
case 247:
#line 1518 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 249:
#line 1524 "c++21.gram.y"
{ 
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 250:
#line 1528 "c++21.gram.y"
{
                    VecType *vp = new VecType(yyvsp[-1].expr, 0);
		    yyvsp[-3].decl->adjustType(vp);

                    yyval.decl = yyvsp[-3].decl;
                ;
    break;}
case 252:
#line 1540 "c++21.gram.y"
{ 
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 253:
#line 1544 "c++21.gram.y"
{
                    VecType *vp = new VecType(yyvsp[-1].expr, 0);

		    yyvsp[-3].decl->adjustType(vp);
                    yyval.decl = yyvsp[-3].decl;
                ;
    break;}
case 254:
#line 1554 "c++21.gram.y"
{
		if (yyvsp[0].decl)
		{
		    if (yyvsp[-1].typedesc)
			yyvsp[0].decl->adjustType(yyvsp[-1].typedesc);
		    yyval.decl = yyvsp[0].decl;
		} else
		    yyval.decl = new Declaration(NullString, yyvsp[-1].typedesc);
	    ;
    break;}
case 257:
#line 1569 "c++21.gram.y"
{
                if (yyvsp[-1].decl)
                {
                    yyvsp[-1].decl->adjustType(yyvsp[0].func);
                    yyval.decl = yyvsp[-1].decl;
                }
                else
                    yyval.decl = new Declaration(NullString,yyvsp[0].func);
            ;
    break;}
case 259:
#line 1584 "c++21.gram.y"
{
		if (yyvsp[0].decl)
		{
		    if (yyvsp[-1].typedesc)
		    {
			yyvsp[0].decl->adjustType(yyvsp[-1].typedesc);
		    } 
		    yyval.decl = yyvsp[0].decl;
		} else
		    yyval.decl = new Declaration(NullString, yyvsp[-1].typedesc);
	    ;
    break;}
case 260:
#line 1596 "c++21.gram.y"
{
		yyvsp[0].func->setReturnType(yyvsp[-2].typedesc);
		if (yyvsp[-1].decl)
		{
		    yyvsp[-1].decl->adjustType(yyvsp[0].func);
		    yyval.decl = yyvsp[-1].decl;
		}
		else
		    yyval.decl = new Declaration(NullString,yyvsp[0].func);
	    ;
    break;}
case 261:
#line 1607 "c++21.gram.y"
{
		if (yyvsp[-1].decl)
		{
		    yyvsp[-1].decl->adjustType(yyvsp[0].func);
		    yyval.decl = yyvsp[-1].decl;
		}
		else
		    yyval.decl = new Declaration(NullString,yyvsp[0].func);
	    ;
    break;}
case 262:
#line 1620 "c++21.gram.y"
{
		yyval.func = new FctType(currentClassType,yyvsp[-4].decllist,yyvsp[-2].typedesc,yyvsp[-1].decllist);
		if (yyvsp[0].text)
		{
		    yyval.func->makePure();
		    delete yyvsp[0].text;
		}
	    ;
    break;}
case 263:
#line 1632 "c++21.gram.y"
{ templateArgs->putIn(yyvsp[0].decl); yyvsp[0].decl->freezeDecl(); yyval.decllist = 0;;
    break;}
case 264:
#line 1634 "c++21.gram.y"
{ templateArgs->putIn(yyvsp[0].decl); yyvsp[0].decl->freezeDecl(); yyval.decllist = 0; ;
    break;}
case 265:
#line 1640 "c++21.gram.y"
{ 
		    if (yyvsp[-1].clSpec != CLASS_TYPE)
			error("template argument type must be 'class'");
		    
		    ClassType *ct = new ClassType(*yyvsp[0].text, yyvsp[-1].clSpec, 0, GlobalInfo::getContext());
		    ct->addStubModifier(STUB_NOREMOTE);
                    ct->addStubModifier(STUB_USERMARSHALL);
		    ct->addStubModifier(STUB_IGNORE);
		    ct->freezeType();
		    Declaration *d = new Declaration(*yyvsp[0].text, ct);
		    d->makeTypedef();
		    delete yyvsp[0].text;
		    yyval.decl = d; 
		;
    break;}
case 266:
#line 1655 "c++21.gram.y"
{ 
		    String name = yyvsp[0].decl->getName();
		    if (yyvsp[-1].clSpec != CLASS_TYPE)
			error("template argument type must be 'class'");
		    
		    ClassType *ct = new ClassType(name, yyvsp[-1].clSpec, 0, GlobalInfo::getContext());
		    ct->addStubModifier(STUB_NOREMOTE);
                    ct->addStubModifier(STUB_USERMARSHALL);
		    ct->addStubModifier(STUB_IGNORE); 
		    ct->freezeType();
		    Declaration *d = new Declaration(name, ct);
		    d->makeTypedef();
		    yyval.decl = d; 
		;
    break;}
case 267:
#line 1670 "c++21.gram.y"
{ 
		    yyvsp[-1].declspec->finalise();
		    yyvsp[0].decl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		    yyval.decl = yyvsp[0].decl;
		;
    break;}
case 269:
#line 1681 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 271:
#line 1687 "c++21.gram.y"
{ yyval.typedesc = 0; ;
    break;}
case 273:
#line 1693 "c++21.gram.y"
{
		    yyvsp[0].typedesc->combineTypes(yyvsp[-1].typedesc);
		    yyval.typedesc = yyvsp[0].typedesc;
		;
    break;}
case 274:
#line 1701 "c++21.gram.y"
{ yyval.typedesc = new PointerType(POINTER_TYPE, 0, yyvsp[0].typedesc); ;
    break;}
case 275:
#line 1703 "c++21.gram.y"
{ yyval.typedesc = new PointerType(REFERENCE_TYPE, 0, yyvsp[0].typedesc); ;
    break;}
case 276:
#line 1705 "c++21.gram.y"
{
		    PointerType *pt = new PointerType(POINTER_TYPE, 0, yyvsp[0].typedesc);
		    QualifiedType *qt = new QualifiedType(yyvsp[-3].typedesc, pt);
		    yyval.typedesc = qt;
		;
    break;}
case 277:
#line 1714 "c++21.gram.y"
{ yyval.typedesc = new CvType(yyvsp[0].cvq); ;
    break;}
case 278:
#line 1716 "c++21.gram.y"
{
		    yyvsp[-1].typedesc->addCvQualifier(yyvsp[0].cvq);
		;
    break;}
case 279:
#line 1723 "c++21.gram.y"
{ yyval.cvq = QUAL_CONST; ;
    break;}
case 280:
#line 1725 "c++21.gram.y"
{ yyval.cvq = QUAL_VOLATILE; ;
    break;}
case 281:
#line 1730 "c++21.gram.y"
{ yyval.decl = new Declaration(*yyvsp[0].text); delete yyvsp[0].text; ;
    break;}
case 282:
#line 1735 "c++21.gram.y"
{ yyval.decl = new Declaration(yyvsp[0].typedesc->simpleTypeName(NORMAL_DECL)); ;
    break;}
case 283:
#line 1737 "c++21.gram.y"
{ yyval.decl = new Declaration(yyvsp[0].typedesc->simpleTypeName(NORMAL_DECL)); ;
    break;}
case 284:
#line 1739 "c++21.gram.y"
{ yyval.decl = new Declaration(yyvsp[0].decl->getName()); ;
    break;}
case 285:
#line 1744 "c++21.gram.y"
{
		    yyvsp[-1].declspec->finalise();
		    if (yyvsp[0].decl)
		    {
			yyvsp[0].decl->attachBasicSpec(yyvsp[-1].declspec);
			yyval.decl = yyvsp[0].decl;
		    }
		    else
			yyval.decl = new Declaration(NullString, yyvsp[-1].declspec->getType());
		    delete yyvsp[-1].declspec;
		;
    break;}
case 286:
#line 1759 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].typedesc); ;
    break;}
case 287:
#line 1761 "c++21.gram.y"
{ yyval.declspec = yyvsp[-1].declspec->mergeSpecInto(new DeclSpecifier(yyvsp[0].typedesc)); delete yyvsp[-1].declspec; ;
    break;}
case 288:
#line 1763 "c++21.gram.y"
{ yyval.declspec = new DeclSpecifier(yyvsp[0].cvq); ;
    break;}
case 289:
#line 1765 "c++21.gram.y"
{ yyvsp[-1].declspec->addCvQualifier(yyvsp[0].cvq); yyval.declspec = yyvsp[-1].declspec; ;
    break;}
case 291:
#line 1771 "c++21.gram.y"
{ yyval.decl = 0; ;
    break;}
case 292:
#line 1776 "c++21.gram.y"
{
		if (yyvsp[0].decl)
		{
		    if (yyvsp[-1].typedesc)
			yyvsp[0].decl->adjustType(yyvsp[-1].typedesc);
		    yyval.decl = yyvsp[0].decl;
		} else
		    yyval.decl = new Declaration(NullString, yyvsp[-1].typedesc);
	    ;
    break;}
case 295:
#line 1791 "c++21.gram.y"
{ yyval.decl = 0; ;
    break;}
case 296:
#line 1796 "c++21.gram.y"
{
		if (yyvsp[-1].decl)
		{
		    yyvsp[-1].decl->adjustType(yyvsp[0].func);
		    yyval.decl = yyvsp[-1].decl;
		}
		else
		    yyval.decl = new Declaration(NullString, yyvsp[0].func);
	    ;
    break;}
case 297:
#line 1806 "c++21.gram.y"
{
		    VecType *vp = new VecType(yyvsp[-1].expr, 0);

		    if (yyvsp[-3].decl)
		    {
			yyvsp[-3].decl->adjustType(vp);
			yyval.decl = yyvsp[-3].decl;
		    }
		    else
			yyval.decl = new Declaration(NullString, vp);
                ;
    break;}
case 298:
#line 1818 "c++21.gram.y"
{ yyval.decl = yyvsp[-1].decl; ;
    break;}
case 300:
#line 1824 "c++21.gram.y"
{
		    Declaration *d = new Declaration("...", new DummyArgType());

		    if (yyvsp[-1].decllist)
		    {
			yyvsp[-1].decllist->appendDecl(d);
		    }
		    else
			yyval.decllist = new DeclarationList(d);
		;
    break;}
case 301:
#line 1835 "c++21.gram.y"
{ yyvsp[-2].decllist->appendDecl(new Declaration("...", new DummyArgType())); ;
    break;}
case 303:
#line 1841 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 304:
#line 1846 "c++21.gram.y"
{ yyval.decllist = new DeclarationList(yyvsp[0].decl); ;
    break;}
case 305:
#line 1848 "c++21.gram.y"
{ if (yyvsp[0].decl) yyvsp[-2].decllist->appendDecl(yyvsp[0].decl); ;
    break;}
case 306:
#line 1858 "c++21.gram.y"
{ 
		    yyvsp[-1].declspec->finalise();
		    yyvsp[0].decl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		    yyval.decl = yyvsp[0].decl;
		;
    break;}
case 307:
#line 1865 "c++21.gram.y"
{
		    yyvsp[-3].declspec->finalise();
		    yyvsp[-2].decl->attachBasicSpec(yyvsp[-3].declspec);
		    yyvsp[-2].decl->setInit(new BinaryExpression(equal, 0, yyvsp[0].expr));
		    delete yyvsp[-3].declspec;
		    yyval.decl = yyvsp[-2].decl;
		;
    break;}
case 308:
#line 1873 "c++21.gram.y"
{ 
		    yyvsp[-1].declspec->finalise();
		    if (yyvsp[0].decl)
			yyval.decl = yyvsp[0].decl;
		    else
			yyval.decl = new Declaration(NullString);
		    yyval.decl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		;
    break;}
case 309:
#line 1884 "c++21.gram.y"
{ 
		    yyvsp[-3].declspec->finalise();
		    if (yyvsp[-2].decl)
			yyval.decl = yyvsp[-2].decl;
		    else
			yyval.decl = new Declaration(NullString, yyvsp[-3].declspec->getType());

		    yyval.decl->attachBasicSpec(yyvsp[-3].declspec);
		    yyval.decl->setInit(new BinaryExpression(equal, 0, yyvsp[0].expr));
		    delete yyvsp[-3].declspec;
		;
    break;}
case 310:
#line 1896 "c++21.gram.y"
{
    		    yyvsp[-1].declspec->finalise();
		    yyvsp[0].decl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		    yyval.decl = yyvsp[0].decl;
		;
    break;}
case 311:
#line 1903 "c++21.gram.y"
{
		    yyvsp[-3].declspec->finalise();
		    yyvsp[-2].decl->attachBasicSpec(yyvsp[-3].declspec);
		    yyvsp[-2].decl->setInit(new BinaryExpression(equal, 0, yyvsp[0].expr));
		    delete yyvsp[-3].declspec;
		    yyval.decl = yyvsp[-2].decl;
		;
    break;}
case 312:
#line 1911 "c++21.gram.y"
{ 
		    yyvsp[-1].declspec->finalise();
		    if (yyvsp[0].decl)
			yyval.decl = yyvsp[0].decl;
		    else
			yyval.decl = new Declaration(NullString);

		    yyval.decl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		;
    break;}
case 313:
#line 1922 "c++21.gram.y"
{ 
		    yyvsp[-3].declspec->finalise();
 		    if (yyvsp[-2].decl)
 			yyval.decl = yyvsp[-2].decl;
 		    else
 			yyval.decl = new Declaration(NullString);

		    yyval.decl->attachBasicSpec(yyvsp[-3].declspec);
		    yyval.decl->setInit(new BinaryExpression(equal, 0, yyvsp[0].expr));
		    delete yyvsp[-3].declspec;
		;
    break;}
case 314:
#line 1934 "c++21.gram.y"
{ yyval.decl = 0; ;
    break;}
case 315:
#line 1939 "c++21.gram.y"
{
		    yyvsp[-1].decl->attachBasicSpec(0);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);

		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 316:
#line 1946 "c++21.gram.y"
{    
		    Declaration *decl = yyvsp[-2].decl;

		    exitTemporaryNest();

	            if (!isCtor && (yyvsp[-1].expr))
	            {
			delete yyvsp[-1].expr; yyvsp[-1].expr = 0;
	                error("constructor initialisers on non-constructor");
		    }

	            isCtor = FALSE;
		    
		    decl->makeQualified(yyvsp[-3].typedesc);
		    decl->attachBasicSpec(yyvsp[-4].declspec);
		    decl->setInit(yyvsp[-1].expr);
		    decl->setBody(yyvsp[0].expr);

		    yyval.decl = decl;

		    delete yyvsp[-4].declspec;
		;
    break;}
case 317:
#line 1969 "c++21.gram.y"
{ 
		    Declaration *decl = yyvsp[-2].decl;

	            if (!isCtor && (yyvsp[-1].expr))
	            {
			delete yyvsp[-1].expr; yyvsp[-1].expr = 0;
	                error("constructor initialisers on non-constructor");
		    }

	            isCtor = FALSE;
		    
		    exitTemporaryNest();
    		    decl->makeQualified(yyvsp[-3].typedesc);
		    decl->setInit(yyvsp[-1].expr);
		    decl->setBody(yyvsp[0].expr);
		    yyval.decl = decl;
		;
    break;}
case 318:
#line 1987 "c++21.gram.y"
{    
		    Declaration *decl = yyvsp[-1].decl;

		    exitTemporaryNest();
       		    decl->makeQualified(yyvsp[-2].typedesc);
		    decl->attachBasicSpec(yyvsp[-3].declspec);
		    decl->setBody(yyvsp[0].expr);

		    yyval.decl = decl;

		    delete yyvsp[-3].declspec;
		;
    break;}
case 319:
#line 2000 "c++21.gram.y"
{ 
		    Declaration *decl = yyvsp[-1].decl;

		    exitTemporaryNest();
		    decl->makeQualified(yyvsp[-2].typedesc);    
		    decl->setBody(yyvsp[0].expr);
		    yyval.decl = decl;
		;
    break;}
case 320:
#line 2009 "c++21.gram.y"
{ 
		    yyvsp[-1].decl->attachBasicSpec(yyvsp[-2].declspec);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
		    delete yyvsp[-2].declspec;
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 321:
#line 2016 "c++21.gram.y"
{ 
		    yyvsp[-1].decl->attachBasicSpec(yyvsp[-2].declspec);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
		    delete yyvsp[-2].declspec;
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 322:
#line 2023 "c++21.gram.y"
{ 
		    yyvsp[-1].decl->attachBasicSpec(yyvsp[-3].declspec);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
                    exitTemporaryNest();
		    yyvsp[-1].decl->makeQualified(yyvsp[-2].typedesc);
		    delete yyvsp[-3].declspec;
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 323:
#line 2032 "c++21.gram.y"
{ 
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
                    exitTemporaryNest();
		    yyvsp[-1].decl->makeQualified(yyvsp[-2].typedesc);
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 324:
#line 2042 "c++21.gram.y"
{
                    if (yyvsp[-2].declspec)
                    {
                        yyvsp[-2].declspec->finalise();
                        yyvsp[0].decl->attachBasicSpec(yyvsp[-2].declspec);

		        delete yyvsp[-2].declspec;
                    }
		    yyvsp[0].decl->addTemplateArgs(templateArgs);
		    templateNest--; templateArgs = 0;
		    GlobalInfo::popContext();
		    GlobalInfo::putIn(yyvsp[0].decl);
		    yyval.decl = yyvsp[0].decl;
		;
    break;}
case 325:
#line 2061 "c++21.gram.y"
{
	            if (isCtor)
	                currentClassType->addCtor(yyvsp[-2].decl);
	            else 
		        if (yyvsp[-1].expr)
	                {
			    delete yyvsp[-1].expr; yyvsp[-1].expr = 0;
	                    error("constructor initialisers on non-constructor");
			}

	            isCtor = FALSE;
	    
		    yyvsp[-2].decl->setInit(yyvsp[-1].expr);
		    yyvsp[-2].decl->setBody(yyvsp[0].expr);
		    GlobalInfo::putIn(yyvsp[-2].decl);

		    yyval.decl = yyvsp[-2].decl;
		;
    break;}
case 326:
#line 2080 "c++21.gram.y"
{
	            currentClassType->addDtor(yyvsp[-1].decl);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
		    GlobalInfo::putIn(yyvsp[-1].decl);
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 327:
#line 2087 "c++21.gram.y"
{
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
		    GlobalInfo::putIn(yyvsp[-1].decl);
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 328:
#line 2093 "c++21.gram.y"
{
	            if (isCtor)
	                currentClassType->addCtor(yyvsp[-2].decl);
	            else 
		        if (yyvsp[-1].expr)
	                {
			    delete yyvsp[-1].expr; yyvsp[-1].expr = 0;
	                    error("constructor initialisers on non-constructor");
			}

	            isCtor = FALSE;
		    yyvsp[-2].decl->setInit(yyvsp[-1].expr);
		    yyvsp[-2].decl->setBody(yyvsp[0].expr);
		    yyvsp[-2].decl->attachBasicSpec(yyvsp[-3].declspec);
		    GlobalInfo::putIn(yyvsp[-2].decl);
		    delete yyvsp[-3].declspec;
		    yyval.decl = yyvsp[-2].decl;
		;
    break;}
case 329:
#line 2112 "c++21.gram.y"
{
	            currentClassType->addDtor(yyvsp[-1].decl);
		    yyvsp[-1].decl->attachBasicSpec(yyvsp[-2].declspec);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
		    GlobalInfo::putIn(yyvsp[-1].decl);
		    delete yyvsp[-2].declspec;
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 330:
#line 2121 "c++21.gram.y"
{
		    yyvsp[-1].decl->attachBasicSpec(yyvsp[-2].declspec);
		    yyvsp[-1].decl->setBody(yyvsp[0].expr);
		    GlobalInfo::putIn(yyvsp[-1].decl);
		    delete yyvsp[-2].declspec;
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 331:
#line 2129 "c++21.gram.y"
{
		    yyvsp[-1].decl->attachBasicSpec(yyvsp[-2].declspec);
		    yyvsp[-1].decl->setInit(yyvsp[0].expr);
		    GlobalInfo::putIn(yyvsp[-1].decl);
		    delete yyvsp[-2].declspec;
		    yyval.decl = yyvsp[-1].decl;
		;
    break;}
case 333:
#line 2141 "c++21.gram.y"
{ yyval.expr = 0; ;
    break;}
case 334:
#line 2146 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[0].text); delete yyvsp[0].text;;
    break;}
case 335:
#line 2151 "c++21.gram.y"
{    
		    yyvsp[-3].typedesc->makeComplete();
		    GlobalInfo::popContext();
		    popClassLevel();
		    yyval.typedesc = yyvsp[-3].typedesc;
		    if (yyvsp[-1].decllist)
			delete yyvsp[-1].decllist;;
		;
    break;}
case 336:
#line 2163 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(equal, 0, yyvsp[0].expr); ;
    break;}
case 337:
#line 2165 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(equal, 0 ,
					    new NestedExpression(yyvsp[-2].expr, '{', '}')); ;
    break;}
case 338:
#line 2171 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 339:
#line 2173 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 341:
#line 2179 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(",", yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 342:
#line 2181 "c++21.gram.y"
{ yyval.expr = new NestedExpression(yyvsp[-2].expr, '{', '}'); ;
    break;}
case 345:
#line 2191 "c++21.gram.y"
{
		    yyvsp[-3].typedesc->makeComplete();
		    GlobalInfo::popContext();
		    popClassLevel();
		    yyval.typedesc = yyvsp[-3].typedesc;
		    if (yyvsp[-1].decllist)
			delete yyvsp[-1].decllist;
		;
    break;}
case 346:
#line 2203 "c++21.gram.y"
{ 
		    yyval.typedesc = declareClass(*yyvsp[-1].text, yyvsp[-2].clSpec, yyvsp[0].bclass);
		    delete yyvsp[-1].text;
		;
    break;}
case 347:
#line 2208 "c++21.gram.y"
{ 
		    yyval.typedesc = declareClass(NullString, yyvsp[-1].clSpec, 0);
		;
    break;}
case 348:
#line 2212 "c++21.gram.y"
{ 
		    String s1(yyvsp[-1].decl->getName());
		    yyval.typedesc = declareClass(s1, yyvsp[-2].clSpec, yyvsp[0].bclass);
		;
    break;}
case 349:
#line 2220 "c++21.gram.y"
{
		    DeclContext *dc = ((ClassType *)yyvsp[-1].decl->getType())->getMemberContext();
		    GlobalInfo::pushContext(dc);
		;
    break;}
case 350:
#line 2225 "c++21.gram.y"
{
		    TypeDescriptor *td = yyvsp[-4].decl->getType();
		    String tn = td->simpleTypeName();
		    
		    ClassType *ct = new InstantiatedClassType(tn,
							      (ClassType *)td, yyvsp[-1].decllist ,0);
		    
		    yyval.decl = new Declaration(ct->typeName(NORMAL_DECL), ct);
		    yyval.decl->makeSpecial();
		    GlobalInfo::popContext();
		    GlobalInfo::putIn(yyval.decl);
		    delete yyvsp[-1].decllist;
		;
    break;}
case 351:
#line 2242 "c++21.gram.y"
{
		    yyval.decllist = new DeclarationList(yyvsp[0].decl);
		;
    break;}
case 352:
#line 2246 "c++21.gram.y"
{
		    yyvsp[-2].decllist->appendDecl(yyvsp[0].decl);
		;
    break;}
case 353:
#line 2252 "c++21.gram.y"
{
		;
    break;}
case 354:
#line 2256 "c++21.gram.y"
{
		    yyval.decl = new Declaration(NullString, new DummyType(), yyvsp[0].expr);
		;
    break;}
case 356:
#line 2264 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 358:
#line 2270 "c++21.gram.y"
{ currentClassType->changeVisibility(yyvsp[-1].vspec); yyval.decllist = 0; ;
    break;}
case 359:
#line 2272 "c++21.gram.y"
{
			currentClassType->changeVisibility(yyvsp[-1].vspec);
			yyval.decllist = yyvsp[-2].decllist;
		;
    break;}
case 360:
#line 2277 "c++21.gram.y"
{
		    if (yyvsp[-1].decllist)
		    {
			if (yyvsp[0].decllist)
			{
			    yyvsp[-1].decllist->appendList(yyvsp[0].decllist);
			    delete yyvsp[0].decllist;
			}
		    } else
			yyval.decllist = yyvsp[0].decllist;
		;
    break;}
case 361:
#line 2292 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 362:
#line 2294 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 363:
#line 2296 "c++21.gram.y"
{ 
		    Declaration *d;
		    DeclarationListIterator next(yyvsp[-1].decllist);

		    if (yyvsp[-1].decllist)
		    {
			while ((d = next()) != 0)
			{
			    TypeDescriptor *t = d->getType();

			    d->attachBasicSpec(0);
			    if (t != 0 && t->kind() == FUNCTION_TYPE)
			    {
				d->addStubModifier(STUB_NOMARSHALL);
				d->setName(cppLinkage->mangle(t));
			    }

			    GlobalInfo::putIn(d);
		        }
		    }
		;
    break;}
case 364:
#line 2318 "c++21.gram.y"
{
		    /* Current decl is in currentCEDecl and is in symtab */

		    yyvsp[-1].declspec->finalise();
		    currentCEDecl->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;

		    yyval.decllist = 0;
		;
    break;}
case 365:
#line 2328 "c++21.gram.y"
{
		    TypeDescriptor *t = yyvsp[-1].declspec->getType();
		    Declaration *d = new Declaration(NullString, t);

		    yyvsp[-1].declspec->finalise();
		    d->attachBasicSpec(yyvsp[-1].declspec);
		    delete yyvsp[-1].declspec;
		    GlobalInfo::putIn(d);
		    yyval.decllist = 0;
		;
    break;}
case 366:
#line 2339 "c++21.gram.y"
{ 
		    yyvsp[-2].declspec->finalise();

		    if (yyvsp[-1].decllist)
		    {
			Declaration *d;
			DeclarationListIterator next(yyvsp[-1].decllist);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec(yyvsp[-2].declspec);
			    GlobalInfo::putIn(d);
		        }
		    }
		    else
		    {
			TypeDescriptor *td = yyvsp[-2].declspec->getType();

			if (td != 0)
			{
			    Type typ = td->kind();

			    if ((typ == AGGREGATE_TYPE) ||
				(typ == ENUMERATION_TYPE))
			    {
				yyvsp[-2].declspec->propagateAttributes(td);
				if (yyvsp[-2].declspec->isElaborated())
				{
				    Declaration *d = new Declaration(NullString, td);

				    d->attachBasicSpec(0);
				    GlobalInfo::putIn(d);
				}
			    }
			}
			else
			    error("null declarator in class");
		    }
		    declIsFriend = FALSE;
		    delete yyvsp[-2].declspec;
		    yyval.decllist = yyvsp[-1].decllist;
		;
    break;}
case 367:
#line 2382 "c++21.gram.y"
{ 
		    yyvsp[-2].declspec->finalise();

		    if (yyvsp[-1].decllist)
		    {
			Declaration *d;
			DeclarationListIterator next(yyvsp[-1].decllist);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec(yyvsp[-2].declspec);
			    GlobalInfo::putIn(d);
		        }
		    }
		    else
		    {
			error("null declarator in class");
		    }
		    declIsFriend = FALSE;
		    delete yyvsp[-2].declspec;
		    yyval.decllist = yyvsp[-1].decllist;
		;
    break;}
case 368:
#line 2405 "c++21.gram.y"
{ yyvsp[-1].decl->addStubModifier(STUB_NOMARSHALL);yyvsp[-1].decl->setVisibility(currentClassType->getVisibility()); yyval.decllist = 0; ;
    break;}
case 369:
#line 2407 "c++21.gram.y"
{
		  error("export of qualified name not supported");
		  yyval.decllist = 0;
		;
    break;}
case 370:
#line 2415 "c++21.gram.y"
{
		    String name = yyvsp[-6].decl->getName();

		    if (name == currentClassType->simpleTypeName(NORMAL_DECL))
		    {
			FctType *cFunc = new FctType(currentClassType, yyvsp[-4].decllist, yyvsp[-2].typedesc,
						     yyvsp[-1].decllist, CONSTRUCTOR);

			cFunc->addName(name);

			if (yyvsp[0].text)
			{
			    delete yyvsp[0].text;
			    error("Constructors cannot be pure");
			}

			isCtor = TRUE;
			
			yyval.decl = new Declaration(cppLinkage->mangle(cFunc), cFunc);
		      } else
		      {
			FctType *cFunc = new FctType(currentClassType, yyvsp[-4].decllist, yyvsp[-2].typedesc,
						     yyvsp[-1].decllist);

			cFunc->addName(name);
			isCtor = FALSE;

			yyval.decl = new Declaration(cppLinkage->mangle(cFunc), cFunc);
			if (yyvsp[0].text)
			{
			    delete yyvsp[0].text;
			    cFunc->makePure();
			}
		    }
		;
    break;}
case 371:
#line 2454 "c++21.gram.y"
{
		    String name = yyvsp[-2].decl->getName();

		    if (name == currentClassType->simpleTypeName(NORMAL_DECL))
		    {
			FctType *cFunc = new FctType(currentClassType, 0, 0,
						     yyvsp[0].decllist, DESTRUCTOR);

			cFunc->addName(name);
			yyval.decl = new Declaration(cppLinkage->mangle(cFunc),cFunc);
			
		    } else
		    {
			error("destructor not named same as current class");
			yyval.decl = 0;
		    }
		;
    break;}
case 372:
#line 2472 "c++21.gram.y"
{
		   String dname = *yyvsp[-2].text;

		    if (dname == currentClassType->simpleTypeName(NORMAL_DECL))
		    {
			FctType *cFunc = new FctType(currentClassType, 0, 0,
						     yyvsp[0].decllist, DESTRUCTOR);

			cFunc->addName(dname);
			yyval.decl = new Declaration(cppLinkage->mangle(cFunc),cFunc);
		    } else
		    {
			error("destructor not named same as current class");
			yyval.decl = 0;
		    }
		   delete yyvsp[-2].text;
		;
    break;}
case 373:
#line 2493 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 374:
#line 2495 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 375:
#line 2500 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 376:
#line 2502 "c++21.gram.y"
{ yyval.text = 0; ;
    break;}
case 377:
#line 2507 "c++21.gram.y"
{ 
		    yyval.decllist = new DeclarationList(yyvsp[0].decl);
		;
    break;}
case 378:
#line 2511 "c++21.gram.y"
{
		    yyvsp[-2].decllist->appendDecl(yyvsp[0].decl);
		;
    break;}
case 380:
#line 2519 "c++21.gram.y"
{ if (isCtor) currentClassType->addCtor(yyvsp[0].decl); ;
    break;}
case 381:
#line 2521 "c++21.gram.y"
{ currentClassType->addDtor(yyvsp[0].decl); ;
    break;}
case 383:
#line 2524 "c++21.gram.y"
{ yyval.decl = 0; ;
    break;}
case 384:
#line 2529 "c++21.gram.y"
{ 
		    yyval.decllist = new DeclarationList(yyvsp[0].decl);
		;
    break;}
case 385:
#line 2533 "c++21.gram.y"
{
		    yyvsp[-2].decllist->appendDecl(yyvsp[0].decl);
		;
    break;}
case 386:
#line 2540 "c++21.gram.y"
{ if (isCtor) currentClassType->addCtor(yyvsp[0].decl); ;
    break;}
case 387:
#line 2542 "c++21.gram.y"
{ currentClassType->addDtor(yyvsp[0].decl); ;
    break;}
case 389:
#line 2548 "c++21.gram.y"
{ 
		    if (*yyvsp[0].text != String("0"))
			error("Non-zero pure specifier");
		    yyval.text = yyvsp[0].text;
		;
    break;}
case 390:
#line 2554 "c++21.gram.y"
{
		    yyval.text = 0;
		;
    break;}
case 391:
#line 2561 "c++21.gram.y"
{
		    String name = yyvsp[-4].typedesc->typeName(NORMAL_DECL);

		    FctType *cFunc = new FctType(currentClassType, 0, yyvsp[-2].typedesc,
						 yyvsp[-1].decllist, CONVERSION);
		    cFunc->attachBaseType(yyvsp[-4].typedesc);
		    cFunc->addName(name);

		    if (yyvsp[0].text)
		    {
			cFunc->makePure();
			delete yyvsp[0].text;
		    }
		    yyval.decl = new Declaration(name, cFunc);
		;
    break;}
case 392:
#line 2580 "c++21.gram.y"
{ yyval.bclass = yyvsp[0].bclass; ;
    break;}
case 394:
#line 2586 "c++21.gram.y"
{ 
		    if (yyvsp[0].bclass && (!yyvsp[-2].bclass->append(yyvsp[0].bclass)))
		    {
			ostrstream errorStream;

			errorStream << "base class \"" << yyvsp[0].bclass->getClass()->simpleTypeName(NORMAL_DECL)
			    << "\" already mentioned in derivation list";
			error(errorStream);
			delete yyvsp[0].bclass;
		    }
		;
    break;}
case 395:
#line 2601 "c++21.gram.y"
{ yyval.bclass = new BaseClass((ClassType*)yyvsp[0].decl->getType()); ;
    break;}
case 396:
#line 2603 "c++21.gram.y"
{ yyval.bclass = new BaseClass((ClassType*)yyvsp[0].decl->getType(), TRUE, yyvsp[-1].vspec); ;
    break;}
case 397:
#line 2605 "c++21.gram.y"
{ yyval.bclass = new BaseClass((ClassType*)yyvsp[0].decl->getType(), TRUE); ;
    break;}
case 398:
#line 2607 "c++21.gram.y"
{ yyval.bclass = new BaseClass((ClassType*)yyvsp[0].decl->getType(), TRUE, yyvsp[-2].vspec); ;
    break;}
case 399:
#line 2609 "c++21.gram.y"
{ yyval.bclass = new BaseClass((ClassType*)yyvsp[0].decl->getType(), FALSE, yyvsp[-1].vspec); ;
    break;}
case 400:
#line 2611 "c++21.gram.y"
{
		    error("expecting base class specifier");
		    yyval.bclass = 0;
		;
    break;}
case 401:
#line 2619 "c++21.gram.y"
{ yyval.vspec = CLASS_PRIVATE; ;
    break;}
case 402:
#line 2621 "c++21.gram.y"
{ yyval.vspec = CLASS_PROTECTED; ;
    break;}
case 403:
#line 2623 "c++21.gram.y"
{ yyval.vspec = CLASS_PUBLIC; ;
    break;}
case 404:
#line 2628 "c++21.gram.y"
{
		    yyvsp[-1].declspec->finalise();
		    if (yyvsp[0].typedesc)
		    {
			yyvsp[0].typedesc->combineTypes(yyvsp[-1].declspec->getType());
			yyvsp[-1].declspec->setType(yyvsp[0].typedesc);
		    }
		    yyval.typedesc = yyvsp[-1].declspec->getType();
		    delete yyvsp[-1].declspec;
		;
    break;}
case 406:
#line 2643 "c++21.gram.y"
{ yyval.typedesc = 0; ;
    break;}
case 407:
#line 2648 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(": ", 0,yyvsp[0].expr); ;
    break;}
case 409:
#line 2654 "c++21.gram.y"
{ yyval.expr = new BinaryExpression(", ", yyvsp[-2].expr ,yyvsp[0].expr); ;
    break;}
case 410:
#line 2659 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("",
					    new Literal(yyvsp[-3].decl->getName()),
					    new NestedExpression(yyvsp[-1].expr)); ;
    break;}
case 411:
#line 2663 "c++21.gram.y"
{ yyval.expr = new Literal(yyvsp[-2].decl->getName() + "()"); ;
    break;}
case 412:
#line 2665 "c++21.gram.y"
{ yyval.expr = new BinaryExpression("",
					    new Literal(*yyvsp[-3].text),
					    new NestedExpression(yyvsp[-1].expr)); delete yyvsp[-3].text; ;
    break;}
case 413:
#line 2669 "c++21.gram.y"
{ yyval.expr = new Literal(*yyvsp[-2].text + "()"); delete yyvsp[-2].text; ;
    break;}
case 414:
#line 2674 "c++21.gram.y"
{ 
		    String opName = String("operator ") + *yyvsp[0].text;
		    delete yyvsp[0].text;
		    yyval.text = new String(opName);
		;
    break;}
case 415:
#line 2684 "c++21.gram.y"
{ yyval.text = new String("new"); ;
    break;}
case 416:
#line 2686 "c++21.gram.y"
{ yyval.text = new String("delete"); ;
    break;}
case 417:
#line 2688 "c++21.gram.y"
{ yyval.text = new String("+"); ;
    break;}
case 418:
#line 2690 "c++21.gram.y"
{ yyval.text = new String("-"); ;
    break;}
case 419:
#line 2692 "c++21.gram.y"
{ yyval.text = new String("*"); ;
    break;}
case 420:
#line 2694 "c++21.gram.y"
{ yyval.text = new String("/"); ;
    break;}
case 421:
#line 2696 "c++21.gram.y"
{ yyval.text = new String("%"); ;
    break;}
case 422:
#line 2698 "c++21.gram.y"
{ yyval.text = new String("^"); ;
    break;}
case 423:
#line 2700 "c++21.gram.y"
{ yyval.text = new String("&"); ;
    break;}
case 424:
#line 2702 "c++21.gram.y"
{ yyval.text = new String("|"); ;
    break;}
case 425:
#line 2704 "c++21.gram.y"
{ yyval.text = new String("~"); ;
    break;}
case 426:
#line 2706 "c++21.gram.y"
{ yyval.text = new String("!"); ;
    break;}
case 427:
#line 2708 "c++21.gram.y"
{ yyval.text = new String("="); ;
    break;}
case 428:
#line 2710 "c++21.gram.y"
{ yyval.text = new String("<"); ;
    break;}
case 429:
#line 2712 "c++21.gram.y"
{ yyval.text = new String(">"); ;
    break;}
case 430:
#line 2714 "c++21.gram.y"
{ yyval.text = new String("+="); ;
    break;}
case 431:
#line 2716 "c++21.gram.y"
{ yyval.text = new String("-="); ;
    break;}
case 432:
#line 2718 "c++21.gram.y"
{ yyval.text = new String("*="); ;
    break;}
case 433:
#line 2720 "c++21.gram.y"
{ yyval.text = new String("/="); ;
    break;}
case 434:
#line 2722 "c++21.gram.y"
{ yyval.text = new String("%="); ;
    break;}
case 435:
#line 2724 "c++21.gram.y"
{ yyval.text = new String("^="); ;
    break;}
case 436:
#line 2726 "c++21.gram.y"
{ yyval.text = new String("&="); ;
    break;}
case 437:
#line 2728 "c++21.gram.y"
{ yyval.text = new String("|="); ;
    break;}
case 438:
#line 2730 "c++21.gram.y"
{ yyval.text = new String("<<"); ;
    break;}
case 439:
#line 2732 "c++21.gram.y"
{ yyval.text = new String(">>"); ;
    break;}
case 440:
#line 2734 "c++21.gram.y"
{ yyval.text = new String("<<="); ;
    break;}
case 441:
#line 2736 "c++21.gram.y"
{ yyval.text = new String(">>="); ;
    break;}
case 442:
#line 2738 "c++21.gram.y"
{ yyval.text = new String("=="); ;
    break;}
case 443:
#line 2740 "c++21.gram.y"
{ yyval.text = new String("!="); ;
    break;}
case 444:
#line 2742 "c++21.gram.y"
{ yyval.text = new String("<="); ;
    break;}
case 445:
#line 2744 "c++21.gram.y"
{ yyval.text = new String(">="); ;
    break;}
case 446:
#line 2746 "c++21.gram.y"
{ yyval.text = new String("&&"); ;
    break;}
case 447:
#line 2748 "c++21.gram.y"
{ yyval.text = new String("||"); ;
    break;}
case 448:
#line 2750 "c++21.gram.y"
{ yyval.text = new String("++"); ;
    break;}
case 449:
#line 2752 "c++21.gram.y"
{ yyval.text = new String("--"); ;
    break;}
case 450:
#line 2754 "c++21.gram.y"
{ yyval.text = new String(","); ;
    break;}
case 451:
#line 2756 "c++21.gram.y"
{ yyval.text = new String("->*"); ;
    break;}
case 452:
#line 2758 "c++21.gram.y"
{ yyval.text = new String("->"); ;
    break;}
case 453:
#line 2760 "c++21.gram.y"
{ yyval.text = new String("()"); ;
    break;}
case 454:
#line 2762 "c++21.gram.y"
{ yyval.text = new String("[]"); ;
    break;}
case 455:
#line 2766 "c++21.gram.y"
{ yyval.text = collectInlineDef(); ;
    break;}
case 456:
#line 2767 "c++21.gram.y"
{ yyval.text = yyvsp[-1].text; ;
    break;}
case 457:
#line 2772 "c++21.gram.y"
{ yyval.decllist = yyvsp[-1].decllist; ;
    break;}
case 458:
#line 2774 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 460:
#line 2780 "c++21.gram.y"
{ yyval.decllist = 0; ;
    break;}
case 461:
#line 2785 "c++21.gram.y"
{ yyval.decllist = new DeclarationList(yyvsp[0].decl); ;
    break;}
case 462:
#line 2787 "c++21.gram.y"
{ yyvsp[-2].decllist->appendDecl(yyvsp[0].decl); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 2789 "c++21.gram.y"


/*
 * Bison gratuitously defines 'const' to be nothing if the compiler
 * does not define __STDC__ - which Cfront does not, compensate here
 * but be aware that if the user explictly defines const we may be in
 * trouble
 */

#undef const

static int anonCount = 0;

ClassType *declareClass ( const String& className, ClassSpecifier ctype,
			   BaseClass *baselist, 
			   DeclContext *context,
			   Boolean isDefn )
{
    ClassType *newClass = 0;
    Declaration *oldDecl = 0;
    Declaration *newDecl = 0;
    String cName(className);
    Boolean nameless = (className == NullString ? TRUE : FALSE);
    Boolean declare = isDefn;
    int token = getLookahead();		/* lookahead */
    Boolean maybeNew = ((token == ';') || (token == ',')  ? TRUE : FALSE);

    /* This is really the default case */
    if (context == 0)
    {
	context = GlobalInfo::getContext();

	/* Template handling: template context alive put this decl in
           surrounding context */
	if (context == templateArgs)
	  context = context->getEnclosingContext();
    }

    if (nameless)
    {
	/* class/struct/union with no name - make one up */
	cName = constructName("_NoName_", anonCount++);
    }

    /* See if class name is already known */

    oldDecl = GlobalInfo::lookFor(cName, AGGREGATE_TYPE);

    if (oldDecl)			/* known */
    {
	ClassType *oldType = (ClassType *)oldDecl->getType();

	if (oldDecl->getContext() == context)
	{
	    /* same context */
	    if (isDefn)
	    {
		if (templateNest)
		    newClass = new TemplateClassType(cName, ctype, baselist,
						     templateArgs,
						     context, nameless, isDefn);
		else
		    newClass = new ClassType(cName, ctype, baselist,
					     context, nameless, isDefn);
		oldType->combineTypes(newClass);
		declare = FALSE;
	    }
	    else
		newClass = oldType;
	}
	else
	{
	    /* different contexts */
	    if (maybeNew || isDefn)
	    {
		/* class X; or class X{...}; in nested scope */
		if (templateNest)
		    newClass = new TemplateClassType(cName, ctype, baselist,
						     templateArgs,
						     context, nameless, isDefn);
		else
		    newClass = new ClassType(cName, ctype, baselist,
					     context, nameless, isDefn);

		declare = TRUE;
	    }
	    else
		newClass = oldType;
	}
    } 
   else				/* not known */
    {
	if (templateNest)
	    newClass = new TemplateClassType(cName, ctype, baselist,
					      templateArgs,
					      context, nameless, isDefn);
	else
	    newClass = new ClassType(cName, ctype, baselist,
				      context, nameless, isDefn);
	declare = TRUE;
    }

    if (declare)
    {
	/* temporarily enter required context to declare in*/

	GlobalInfo::pushContext(context);
	newDecl = new Declaration(cName, newClass);
	newDecl->makeElaborated();

	/* resume previous context */

	GlobalInfo::popContext();
	
	newDecl->makeTypedef();
	newDecl->makeSpecial();
	context->putIn(newDecl);
    }
    
    if (isDefn)
    {
	GlobalInfo::pushContext(newClass->getMemberContext());
	pushClassLevel(newClass);
    }

    return newClass;
}

void popClassLevel ()
{
#ifdef SG_DEBUG
    debugStream << "popClassLevel() : leaving level " << classNestLevel;
    debugStream << ": " <<currentClassType->typeName(NORMAL_DECL);
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    currentClassType = classStack[--classNestLevel];
}

void pushClassLevel ( ClassType *ct )
{ 
#ifdef SG_DEBUG   
    debugStream << "pushClassLevel() : entering level " << classNestLevel+1;
    debugStream << ":" << ct->typeName(NORMAL_DECL);
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (classNestLevel < MAXCLASSNEST)
    {
	classStack[++classNestLevel] = ct;
	currentClassType = ct;
    }
    else
	fatal("(internal) class nesting level exceeded");
}

void exitTemporaryNest ()
{ 
#ifdef SG_DEBUG   
    debugStream << "exitTemporaryNest() nest is " << temporaryNest <<"\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    while (temporaryNest > 0)
    {
	temporaryNest--;
	
	popClassLevel();
	GlobalInfo::popContext();
    }  
  
#ifdef SG_DEBUG   
    debugStream << "exitTemporaryNest() nest is now " << temporaryNest <<"\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif
 
}

int yywrap ()
{
    return 1;
}

extern "C" {
#ifdef NONCONST_YYERROR
void yyerror ( char *s )
#else
void yyerror ( const char *s )
#endif
{
    char buffer[256];
    ostrstream temp(buffer, 256);

    memset(buffer, '\0', 256);
    
    temp << s;

    temp << " near : '" << tokenText<< "'" << ends;
    
    error(temp);
}
}
