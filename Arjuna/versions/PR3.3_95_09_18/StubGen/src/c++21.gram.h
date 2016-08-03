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


extern YYSTYPE yylval;
