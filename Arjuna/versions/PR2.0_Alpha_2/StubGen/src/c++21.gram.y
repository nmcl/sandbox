/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: c++21.gram.y,v 1.1 1993/11/03 14:48:32 nmcl Exp $
 */

%{
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

#ifdef DEBUG
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

extern String *collectInlineDef ();
extern int getLookahead ();
extern "C" int yylex ();
extern "C" yyless (int);

static ClassType *declareClass (const String&, ClassSpecifier, BaseClass *,
				DeclContext * = 0, Boolean = TRUE);
static void popClassLevel ();
static void pushClassLevel (ClassType *);
static void exitTemporaryNest ();

static const char RCSid[] = "$Id: c++21.gram.y,v 1.1 1993/11/03 14:48:32 nmcl Exp $";

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
%}

%union
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
}

/* Identifiers and constants */

%token 	IDENTIFIER CHARACTERCONSTANT INTEGERCONSTANT FLOATINGCONSTANT STRING

/* Multicharacter operators */

%token 	STAREQUAL SLASHEQUAL PERCENTEQUAL PLUSEQUAL MINUSEQUAL RSHIFTEQUAL
	LSHIFTEQUAL ANDEQUAL XOREQUAL OREQUAL COLONCOLON ARROW DOTSTAR 
	ARROWSTAR PLUSPLUS MINUSMINUS OROR ANDAND EQUALEQUAL NOTEQUAL
	LESSEQUAL MOREEQUAL LSHIFT RSHIFT ELLIPSIS

/* Operator stuff */

%token	NEW DELETE OPERATOR

/* Keywords */

%token	SIZEOF THIS TEMPLATE

/* Function qualifiers */

%token	INLINE VIRTUAL

/* Basic Types */

%token	CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
	CLASS STRUCT UNION ENUM

/* Storage classes */

%token	AUTO REGISTER STATIC EXTERN FRIEND TYPEDEF

/* Access qualifiers */

%token	PRIVATE PROTECTED PUBLIC

/* Type qualifiers */

%token	CONST VOLATILE

/*
 * Lexical hooks. NOTE: other parsers only usually require TYPEDEFTYPE
 * this parser requires more than that. Maybe I'll fix this one day.
 */

%token	CLASSTYPE ENUMTYPE TYPEDEFTYPE

/*
 * Stub generation specials - hidden inside comments
 */

%token	STUB_CMD

%token  DIRECTIVE

%type	<bclass>
	base.specifier base.list base.spec base.spec.opt

%type   <clSpec>
        class.key
	
%type	<decl>
        TYPEDEFTYPE CLASSTYPE ENUMTYPE
	class.type
	declarator init.declarator argument.declaration constructor
	template.decl.arg
	template.inst.arg
	conversion.operator direct.declarator
	id.declarator id.init.declarator typedef.declarator
	nonf.direct.declarator nonf.id.direct.declarator
	destructor enumerator direct.abstract.declarator.opt
	abstract.declarator.opt abstract.declarator direct.abstract.declarator
        member.declarator new.declarator.opt new.declarator
	type.name
	dname id.dname
	member.function.definition constructed.id.declarator
	function.definition
    	template.class.id

%type	<decllist>
        program external.defs
	declaration 
	declarator.list id.declarator.list id.declarator.list.opt
	enum.list enum.list.opt top.level.declaration 
	linkage.specification
	argument.declaration.list arg.declaration.list.opt
	arg.declaration.list
	member.list.opt member.list member.declaration 
	member.declarator.list.opt member.declarator.list
	template.decl.arglist template.inst.arglist
	template.head
	template.declaration

%type   <func>
        param.type.list

%type	<text>
    	IDENTIFIER DIRECTIVE
	INTEGERCONSTANT FLOATINGCONSTANT
	CHARACTERCONSTANT STRING
	semi.opt pure.specifier.opt
	identifier.opt
	name  operator.function.name 
	qualified.name
	unary.operator
        assignment.operator
	new.type.name 
	compound.statement comma.opt
	void.parameter.list
	operator

%type	<expr>
        literal
	paren.exp.list
	expression initializer initializer.opt
	constant.expression 
	assignment.expression conditional.expression logical.or.expression
	logical.and.expression inclusive.or.expression exclusive.or.expression
	and.expression equality.expression relational.expression 
	shift.expression additive.expression multiplicative.expression
	pm.expression cast.expression unary.expression allocation.expression
	deallocation.expression postfix.expression expression.list.opt 
	expression.list primary.expression
	new.initializer.opt new.initializer 
	initializer.list.opt initializer.list
	constant.expression.opt 
	ctor.initializer.opt ctor.initializer
	mem.initializer.list mem.initializer
	clcl.opt placement.opt placement
        function.body

%type   <linkspec>
        linkage 

%type	<declspec>
	type.specifier.list
	typed.decl.specifiers 
	untyped.decl.specifiers untyped.decl.specifier
        stub.decl.specifiers.opt stub.decl.specifiers stub.decl.specifier
	basic.type.specifiers basic.type.specifier
	elaborated.type.specifiers
	class.or.enum.specifiers
	typedef.type.specifiers
	typedef.type.specifier

%type	<funcspec>
	fct.specifier

%type	<storagespec>
	sc.specifier

%type	<stubcommand>
	stub.command.specifier STUB_CMD

%type   <cvq>
        cv.qualifier

%type	<typedesc>
	type.specifier enum.specifier class.specifier
	elaborated.type.specifier class.head 
	class.or.enum.specifier
	basic.type basic.type.list
	simple.type qualified.type complete.class.type
	qualified.class.type
	conversion.type.name
	cv.qualifier.list.opt cv.qualifier.list
	ptr.operator ptr.operator.opt ptr.operator.list
	nested.scope class.scope
	class.template.body 

%type	<vspec>
	access.specifier

%start program
%%
program:
                { $$ = 0; }
	| external.defs
		{ 
		    if ($1)
		    {
			delete $1;
		    }
		}
	;

external.defs:
	top.level.declaration
	| external.defs top.level.declaration
		{ 
		    if ($1)
		    {
			if ($2)
			{
			    $1->appendList($2);
			    delete $2;
			}
		    }
		    else
			$$ = $2;
		}
	;

top.level.declaration:
	declaration
	| function.definition
		{ $$ = new DeclarationList($1); }
	| linkage.specification
   	| template.declaration
	;

expression:
	assignment.expression
	| expression ',' assignment.expression
                { $$ = new BinaryExpression(",", $1, $3); }
	;

assignment.expression:
	conditional.expression
	| unary.expression assignment.operator assignment.expression
                { $$ = new BinaryExpression(*$2, $1, $3); delete $2; }
	;

assignment.operator:
	'='
               { $$ = new String("="); }
	| STAREQUAL
               { $$ = new String("*="); }
	| SLASHEQUAL
               { $$ = new String("/="); }
        | PERCENTEQUAL
               { $$ = new String("%="); }
	| PLUSEQUAL
               { $$ = new String("+="); }
	| MINUSEQUAL
               { $$ = new String("-="); }
	| RSHIFTEQUAL
               { $$ = new String(">>="); }
	| LSHIFTEQUAL
               { $$ = new String("<<="); }
	| ANDEQUAL
               { $$ = new String("&="); }
	| XOREQUAL
               { $$ = new String("^="); }
	| OREQUAL
               { $$ = new String("|="); }
	;

conditional.expression:
	logical.or.expression
	| logical.or.expression '?' expression ':' conditional.expression
                { $$ = new BinaryExpression("?", $1,
					    new BinaryExpression(":", $3, $5));
	        }
	;

logical.or.expression:
	logical.and.expression
	| logical.or.expression OROR logical.and.expression
                { $$ = new BinaryExpression("||", $1, $3); }
	;

logical.and.expression:
	inclusive.or.expression
	| logical.and.expression ANDAND inclusive.or.expression
                { $$ = new BinaryExpression("&&", $1, $3); }
	;

inclusive.or.expression:
	exclusive.or.expression
	| inclusive.or.expression '|' exclusive.or.expression
                { $$ = new BinaryExpression("|", $1, $3); }
	;

exclusive.or.expression:
	and.expression
	| exclusive.or.expression '^' and.expression
                { $$ = new BinaryExpression("^", $1, $3); }
	;

and.expression:
	equality.expression
	| and.expression '&' equality.expression
                { $$ = new BinaryExpression("&", $1, $3); }
	;

equality.expression:
	relational.expression
	| equality.expression EQUALEQUAL relational.expression
                { $$ = new BinaryExpression("==", $1, $3); }
	| equality.expression NOTEQUAL relational.expression
                { $$ = new BinaryExpression("!=", $1, $3); }
	;

relational.expression:
	shift.expression
	| relational.expression '<' shift.expression
                { $$ = new BinaryExpression("<", $1, $3); }
	| relational.expression '>' shift.expression
                { $$ = new BinaryExpression(">", $1, $3); }
	| relational.expression LESSEQUAL shift.expression
                { $$ = new BinaryExpression("<=", $1, $3); }
	| relational.expression MOREEQUAL shift.expression
                { $$ = new BinaryExpression(">=", $1, $3); }
	;

shift.expression:
	additive.expression
	| shift.expression LSHIFT additive.expression
                { $$ = new BinaryExpression("<<", $1, $3); }
	| shift.expression RSHIFT additive.expression
                { $$ = new BinaryExpression(">>", $1, $3); }
	;

additive.expression:
	multiplicative.expression
	| additive.expression '+' multiplicative.expression
                { $$ = new BinaryExpression("+", $1, $3); }
	| additive.expression '-' multiplicative.expression
                { $$ = new BinaryExpression("-", $1, $3); }
	;

multiplicative.expression:
	pm.expression
	| multiplicative.expression '*' pm.expression
                { $$ = new BinaryExpression("*", $1, $3); }
	| multiplicative.expression '/' pm.expression
                { $$ = new BinaryExpression("/", $1, $3); }
	| multiplicative.expression '%' pm.expression
                { $$ = new BinaryExpression("%", $1, $3); }
	;

pm.expression:
	cast.expression
	| pm.expression DOTSTAR cast.expression
                { $$ = new BinaryExpression(".*", $1, $3); }
	| pm.expression ARROWSTAR cast.expression
                { $$ = new BinaryExpression("->*", $1, $3); }
	;

cast.expression:
	unary.expression
	| '(' type.name ')' cast.expression
		{ 
		    TypeDescriptor *tp = 0;
		    String sp;

		    if ($2 && ((tp = $2->getType()) != 0))
			sp = tp->typeName(NORMAL_DECL);

		    $$ = new BinaryExpression("",
					      new NestedExpression(new Literal(sp)),
					      $4);
		}
	;

unary.expression:
	postfix.expression
	| PLUSPLUS unary.expression
		{ $$ = new BinaryExpression("++", 0, $2); }
	| MINUSMINUS unary.expression
		{ $$ = new BinaryExpression("--", 0, $2); }
	| unary.operator cast.expression
		{ $$ = new BinaryExpression(*$1, 0, $2); }
	| SIZEOF unary.expression
                { $$ = new BinaryExpression("sizeof", 0, $2); }
	| SIZEOF '(' type.name ')'		
		{ 
		    TypeDescriptor *tp = 0;
		    String sp;

		    if ($3 && ((tp = $3->getType()) != 0))
			sp = tp->typeName(NORMAL_DECL);


		    $$ = new BinaryExpression("sizeof", 0,
					      new NestedExpression(new Literal(sp)));
		    if ($3)
			delete $3;
		}
	| allocation.expression
	| deallocation.expression
	;

unary.operator:
	'*'
                { $$ = new String("*"); }
	| '&'
                { $$ = new String("&"); }
	| '+'
                { $$ = new String("+"); }
	| '-'
                { $$ = new String("-"); }
	| '!'
                { $$ = new String("!"); }
	| '~'
                { $$ = new String("~"); }
	;

allocation.expression:
	clcl.opt NEW placement.opt new.type.name new.initializer.opt
		{ 
		    Boolean useGlobal = $1 ? TRUE : FALSE;
		
		    $$ = new AllocExpression(useGlobal, $3, new Literal(*$4), $5);
		}
	| clcl.opt NEW placement.opt '(' type.name ')' new.initializer.opt
		{ 
		    Boolean useGlobal = $1 ? TRUE : FALSE;
		    TypeDescriptor *tp = 0;
		    String sp;

		    if ($5 && ((tp = $5->getType()) != 0))
			sp = tp->typeName(NORMAL_DECL);

		    $$ = new AllocExpression(useGlobal, $3, new Literal(sp), $7);
		}
	;

clcl.opt:
	COLONCOLON
                { $$ = new Literal("::"); }
	|
		{ $$ = 0; }
	;

placement.opt:
	placement
	|
		{ $$ = 0; }
	;

placement:
	'(' expression.list ')'
		{ $$ = new NestedExpression($2); }
	;

new.type.name:
	type.specifier.list new.declarator.opt
		{
		    $1->finalise();
		    if ($2)
			$2->attachBasicSpec($1);
		    else
			$2 = new Declaration(NullString, $1->getType());
		    delete $1;
		    $$ = new String($2->getType()->typeName(NORMAL_DECL));
		}
	;

new.declarator.opt:
	new.declarator
	|
		{ $$ = 0; }
	;

new.declarator:
        '*' cv.qualifier.list.opt new.declarator.opt
		{
		    PointerType *pt = new PointerType(POINTER_TYPE, 0, $2);

		    if ($3)
		    {
			$3->adjustType(pt);
			$$ = $3;
		    }
		    else
			$$ = new Declaration(NullString, pt);
		}
        | complete.class.type COLONCOLON '*' cv.qualifier.list.opt new.declarator.opt
		{
		    PointerType *pt = new PointerType(POINTER_TYPE, 0, $4);
		    QualifiedType *qt = new QualifiedType($1, pt);
		    if ($5)
		    {
			$5->adjustType(qt);
			$$ = $5;
		    }
		    else
			$$ = new Declaration(NullString, qt);
		}
	| new.declarator.opt '[' expression ']'
                {
                    VecType *vp = new VecType($3, 0);

		    if ($1)
		    {
			$1->adjustType(vp);
			$$ = $1;
		    } else
			$$ = new Declaration(NullString, vp);
                }
	;

new.initializer.opt:
	new.initializer
	|
		{ $$ = 0; }
	;

new.initializer:
	'(' initializer.list.opt ')'
		{ $$ = new NestedExpression($2); }
	;

initializer.list.opt:
	initializer.list
	|
		{ $$ = 0; }
	;

deallocation.expression:
	clcl.opt DELETE cast.expression
		{
		    String op($1 ? "::" : "");

		    op += "delete ";
		    $$ = new BinaryExpression(op, 0, $3);
                }
	| clcl.opt DELETE '[' ']' cast.expression
		{
		    String op($1 ? "::" : "");

		    op += "delete []";
		    $$ = new BinaryExpression(op, 0, $5);
                };

postfix.expression:
	primary.expression
	| postfix.expression '[' expression ']'
                {  $$ = new ArrayExpression($1, $3); }
	| postfix.expression '(' expression.list.opt ')'
                {  $$ = new BinaryExpression("", $1, new NestedExpression($3)); }
	| simple.type '(' expression.list.opt ')'
                {
		    String tn;

		    if ($1)
			tn = $1->typeName(NORMAL_DECL);

		    $$ = new BinaryExpression("", new Literal(tn),
					      new NestedExpression($3));
		}
	| postfix.expression '.' name
                {  $$ = new BinaryExpression(".", $1, new Literal(*$3)); }
	| postfix.expression ARROW name
                {  $$ = new BinaryExpression("->", $1, new Literal(*$3)); }
	| postfix.expression PLUSPLUS
                {  $$ = new BinaryExpression("++", $1, 0); }
	| postfix.expression MINUSMINUS
                {  $$ = new BinaryExpression("--", $1, 0); }
	;

expression.list.opt:
	expression.list
	|
		{ $$ = 0; }
	;

expression.list:
	assignment.expression
	| expression.list ',' assignment.expression
                {  $$ = new BinaryExpression(",", $1, $3); }
	;

primary.expression:
        literal
	| THIS
                { $$ = new Literal("this"); }
	| COLONCOLON IDENTIFIER
                {  $$ = new BinaryExpression("::", 0, new Literal(*$2)); delete $2;}
	| COLONCOLON operator.function.name
                {  $$ = new BinaryExpression("::", 0, new Literal(*$2)); }
	| COLONCOLON qualified.name
                {  $$ = new BinaryExpression("::", 0, new Literal(*$2)); }
	| '(' expression ')'
		{ $$ = new NestedExpression($2); }
	| name
                { $$ = new Literal(*$1); delete $1; }
	;

name:
	IDENTIFIER
	| operator.function.name
	| '~' CLASSTYPE
		{ $$ = new String(String("~") + $2->getName()); }
        | qualified.name
	;

qualified.name:
        nested.scope IDENTIFIER
		{ 
		    $$ = new String($1->typeName(NORMAL_DECL) + "::" + *$2);
                    exitTemporaryNest();
		    delete $2;
		}
        | nested.scope operator.function.name
		{ 
		    $$ = new String($1->typeName(NORMAL_DECL) + "::" + *$2);
                    exitTemporaryNest();
		    delete $2;
		}
        | nested.scope '~' CLASSTYPE
		{ 
		    $$ = new String($1->typeName(NORMAL_DECL) + "::~" + $3->getName());
                    exitTemporaryNest();
		}
	;

literal:
	INTEGERCONSTANT
                { $$ = new Literal(*$1); delete $1; }
	| CHARACTERCONSTANT
                { $$ = new Literal(*$1); delete $1; }
	| FLOATINGCONSTANT
                { $$ = new Literal(*$1); delete $1; }
	| STRING
                { $$ = new Literal(*$1); delete $1; }
	;

template.declaration:
	stub.decl.specifiers.opt template.head class.template.body ';'
		{ 
		    Declaration *d = new Declaration($3->simpleTypeName(NORMAL_DECL), $3);

                    if ($1)
                    {
                        $1->finalise();
                        d->attachBasicSpec($1);
		        delete $1;
                    }

		    d->makeTypedef();
		    GlobalInfo::putIn(d);
		    $$ = 0; templateNest--;
		    currentCEDecl = d;
		}
	| stub.decl.specifiers.opt template.head elaborated.type.specifier ';'
		{ 
		    Declaration *d = new Declaration(NullString, $3);
 
                    if ($1)
                    {
                        $1->finalise();
                        d->attachBasicSpec($1);
		        delete $1;
                    }

	            d->makeElaborated();
	            d->makeTypedef();
		    GlobalInfo::putIn(d);

		    $$ = 0; templateNest--;
		    currentCEDecl = d;
		}
	| stub.decl.specifiers.opt template.head typed.decl.specifiers init.declarator ';'
		{
		    if ($1)
                    {
                        $1->mergeSpecInto($3);
                        delete $1;
                    }

		    $3->finalise();

		    if ($4)
		    {
			    $4->attachBasicSpec($3);
			    $4->addTemplateArgs(templateArgs);
			    GlobalInfo::putIn($4);
		    }

		    delete $3;
		    $$ = 0;
		}
	;

stub.decl.specifiers.opt:
	stub.decl.specifiers
	|
                { $$ = 0; }

	;

stub.decl.specifiers:
	stub.decl.specifier
	| stub.decl.specifiers stub.decl.specifier
		{ $$ = $1->mergeSpecInto($2); delete $1; }
	;

stub.decl.specifier:
	stub.command.specifier
		{ $$ = new DeclSpecifier($1); }
	;

template.head:
	TEMPLATE '<'
		{ templateArgs = new TemplateContext(GlobalInfo::getContext()); $$ = 0; GlobalInfo::pushContext(templateArgs);}
	template.decl.arglist '>'
		{ $$ = 0; templateNest++; templateArgs->freeze();GlobalInfo::popContext();}
	;

declaration:
	';'
                { $$ = 0; }
	| id.declarator.list ';'
                { 
		    DeclarationListIterator nd($1);
		    Declaration* d;
		    
		    while (d = nd())
		    {
			errorStream << "type expected for ";
			errorStream << d->getName();
			error(errorStream);

			d->attachBasicSpec(0);
			GlobalInfo::putIn(d);
		    }
		}
        | class.or.enum.specifiers ';'
                {
		    /* Current decl is in currentCEDecl and is in symtab */

		    $1->finalise();
		    currentCEDecl->attachBasicSpec($1);
		    delete $1;
		    $$ = new DeclarationList(currentCEDecl);
		}
        | elaborated.type.specifiers ';'
                {
		    TypeDescriptor *t = $1->getType();
		    Declaration *d = new Declaration(NullString, t);

		    $1->finalise();
		    d->attachBasicSpec($1);
		    GlobalInfo::putIn(d);
		    delete $1;
		    $$ = new DeclarationList(d);
		}
	| typed.decl.specifiers declarator.list ';'
		{
		    $1->finalise();

		    if ($2)
		    {
			Declaration *d;
			DeclarationListIterator next($2);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec($1);
			    GlobalInfo::putIn(d);
			}
		    }
		    else
		    {
			TypeDescriptor *td = $1->getType();

			if (td == 0)
			    error("null declarator");
			else
			    if ((td->kind() != AGGREGATE_TYPE) &&
				(td->kind() != ENUMERATION_TYPE))
				error("null declarator");
			    else
			    {
				$1->propagateAttributes(td);
				if ($1->isElaborated())
				{
				    Declaration *d = new Declaration(NullString, td);

				    d->makeElaborated();
				    d->attachBasicSpec(0);
				    GlobalInfo::putIn(d);
				}
			    }
		    }
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers id.declarator.list.opt ';'
		{ 

		    $1->finalise();

		    if ($2)
		    {
			Declaration *d;
			DeclarationListIterator next($2);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec($1);
			    GlobalInfo::putIn(d);
		        }
		    }
		    else
		    {
			    error("null declarator");
		    }
		    $$ = $2;
		    delete $1; 
		}
	;

id.declarator.list.opt:
	id.declarator.list
	|
		{ $$ = 0; }
	;

typed.decl.specifiers:
    	basic.type.specifiers
	| typedef.type.specifiers
	| elaborated.type.specifiers
	| class.or.enum.specifiers
	;
	
untyped.decl.specifiers:
	untyped.decl.specifier
	| stub.decl.specifiers
	| untyped.decl.specifiers untyped.decl.specifier
		{ $$ = $1->mergeSpecInto($2); delete $1; }
	;

untyped.decl.specifier:
	sc.specifier
		{ $$ = new DeclSpecifier($1); }
	| fct.specifier
		{ $$ = new DeclSpecifier($1); }
	| cv.qualifier
		{ $$ = new DeclSpecifier($1); }
	;

typedef.type.specifiers:
	typedef.type.specifier
	| untyped.decl.specifiers typedef.type.specifier
		{ $$ = $1->mergeSpecInto($2); delete $1; }
	;

basic.type.specifiers:
	basic.type.specifier
	| untyped.decl.specifiers basic.type.specifier
		{ $$ = $1->mergeSpecInto($2); delete $1; }
	;

typedef.type.specifier:
        simple.type
		{ $$ = new DeclSpecifier($1); }
	| typedef.type.specifier cv.qualifier
		{ $1->addCvQualifier($2); }
	;

elaborated.type.specifiers:
	elaborated.type.specifier
		{ $$ = new DeclSpecifier($1, TRUE); }
	| untyped.decl.specifiers elaborated.type.specifier
		{ $$ = $1->mergeSpecInto(new DeclSpecifier($2, TRUE)); delete $1; }
	;

class.or.enum.specifiers:
	class.or.enum.specifier
		{ $$ = new DeclSpecifier($1, FALSE); }
	| untyped.decl.specifiers class.or.enum.specifier
		{ $$ = $1->mergeSpecInto(new DeclSpecifier($2, FALSE)); delete $1; }
	;


basic.type.specifier:
	basic.type.list
		{ $$ = new DeclSpecifier($1); }
        ;

basic.type.list:
        basic.type
	| basic.type.list basic.type
		{ 
		    $$ = $1->combineTypes($2);
		    delete ($$ == $1 ? $2 : $1);
		}
	;

basic.type:
	CHAR
		{ $$ = new CharType(); }
	| SHORT
		{ $$ = new ModifiedType(MOD_SHORT); }
	| INT
		{ $$ = new IntType(); }
	| LONG
		{ $$ = new ModifiedType(MOD_LONG); }
	| SIGNED
		{ $$ = new ModifiedType(MOD_SIGNED); }
	| UNSIGNED
		{ $$ = new ModifiedType(MOD_UNSIGNED); }
	| FLOAT
		{ $$ = new FloatType(FALSE); }
	| DOUBLE
		{ $$ = new FloatType(TRUE); }
	| VOID
		{ $$ = new VoidType(); }
	;

qualified.type:
        nested.scope TYPEDEFTYPE
		{
		    $$ = new QualifiedType($1, $2->getType());
                    exitTemporaryNest();
		}
	| nested.scope ENUMTYPE
		{
		    $$ = new QualifiedType($1, $2->getType());
                    exitTemporaryNest();
		}
	;

sc.specifier:
	AUTO
		{ $$ = SC_AUTO; }
	| REGISTER
		{ $$ = SC_REGISTER; }
	| STATIC
		{ $$ = SC_STATIC; }
	| EXTERN
		{ $$ = SC_EXTERN; }
	| FRIEND
		{ $$ = SC_FRIEND; declIsFriend = TRUE;}
	| TYPEDEF
		{ $$ = SC_TYPEDEF; }
	;

fct.specifier:
	INLINE
		{ $$ = FUNC_INLINE; }
	| VIRTUAL
		{ $$ = FUNC_VIRTUAL; }
	;

stub.command.specifier:
        STUB_CMD
		{ $$ = $1; }
	;

type.specifier:
	simple.type
	| basic.type
	| elaborated.type.specifier
	| class.or.enum.specifier
	;

simple.type:
        complete.class.type
	| ENUMTYPE
                { $$ = $1->getType(); } 
	| TYPEDEFTYPE
                { $$ = $1->getType(); } 
	| qualified.type
	;

complete.class.type:
        class.type
                { $$ = $1->getType(); } 
	| COLONCOLON class.type
                { $$ = $2->getType(); } 
	| qualified.class.type
	| COLONCOLON qualified.class.type
		{ $$ = $2; }
	;

qualified.class.type:
        nested.scope class.type
                { 
		    $$ = new QualifiedType($1, $2->getType());
                    exitTemporaryNest();
		} 
	;

base.spec.opt:
	base.spec
	|
		{ $$ = 0; }
	;

class.or.enum.specifier:
	class.specifier
                {
		    Declaration *d = new Declaration($1->simpleTypeName(NORMAL_DECL), $1);

		    d->makeTypedef();
		    GlobalInfo::putIn(d);
		    currentCEDecl = d;
		}
	| enum.specifier
                {
		    Declaration *d = new Declaration($1->simpleTypeName(NORMAL_DECL), $1);

		    d->makeTypedef();
		    GlobalInfo::putIn(d);
		    currentCEDecl = d;
		}
	;
	
elaborated.type.specifier:
	class.key IDENTIFIER
                {
		    if (declIsFriend)
		    {
			DeclContext *friendContext;

			friendContext = (currentClassType->getMemberContext())->getEnclosingContext();
			$$ = declareClass(*$2,$1,0,friendContext, FALSE);
		    }
		    else
		    {
			$$ = declareClass(*$2,$1,0,0, FALSE);
		    }
		    delete $2;
		}
	| class.key class.type
		{
		    if ($2->getContext() != GlobalInfo::getContext())
		    {
			$$ = declareClass($2->getName(),$1,0,0,FALSE);
		    }
		    else
			$$ = $2->getType();

		}
	| ENUM ENUMTYPE
		{ $$ = $2->getType(); }
	;

class.key:
	CLASS
                { $$ = CLASS_TYPE; }
	| STRUCT
                { $$ = STRUCT_TYPE; }
	| UNION
                { $$ = UNION_TYPE; }
	;

class.scope:
	class.type COLONCOLON
		{ 
		    ClassType *clas = (ClassType*)($1->getType());
		    
                    GlobalInfo::pushContext(clas->getMemberContext());
                    pushClassLevel(clas);
		    temporaryNest++; 
#ifdef DEBUG   
    debugStream << "class.scope() nest is " << temporaryNest <<"\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

		    $$ = clas;
		}
        ;

nested.scope:
        class.scope
	| nested.scope class.scope
		{   
		    $$ = new QualifiedType($1, $2);
		}
	;

enum.specifier:
	ENUM identifier.opt '{' enum.list.opt '}'
		{ 
		    String eName;
		    
		    if ($2)
		    {
			eName = *$2;
			delete $2;
		    }
		    $$ = new EnumType(eName, $4, TRUE);
		}
	;

identifier.opt:
	IDENTIFIER
	|
		{ $$ = 0; }
	;


enum.list.opt:
	enum.list
	|
		{ $$ = 0; }
	;

enum.list:
	enumerator
		{ 
		    $1->makeSpecial();
		    GlobalInfo::putIn($1);
		    $$ = new DeclarationList($1);
		}
	| enum.list ',' enumerator
                {
		    $3->makeSpecial();
		    GlobalInfo::putIn($3);
		    $1->appendDecl($3);
		}
	;

enumerator:
	IDENTIFIER
                { $$ = new Declaration(*$1); delete $1;}
	| IDENTIFIER '=' constant.expression
		{ $$ = new Declaration(*$1, 0, new BinaryExpression(equal, 0, $3)); delete $1;}
	;

constant.expression:
	conditional.expression
	;

linkage.specification:
	linkage '{' external.defs '}'
		{
		    if ($1)
			GlobalInfo::popLinkage();
		    $$ = $3;
		}
	| linkage '{' '}'
		{
		    if ($1)
			GlobalInfo::popLinkage();
		    $$ = 0;
		}
	| linkage top.level.declaration
		{ 
		    DeclarationListIterator next($2);
		    Declaration *d;

		    if ($1)
		    {
			while (d = next())
			{
			    d->setLinkage($1);
			}
			GlobalInfo::popLinkage();
		    }
		    $$ = $2;
		}
	;

linkage:
	EXTERN STRING
                {
		    if (*$2 == String("\"C\""))
		    {
			GlobalInfo::pushLinkage(cLinkage);
			$$ = cLinkage;
		    }
		    else if (*$2 == String("\"C++\""))
		    {
			GlobalInfo::pushLinkage(cppLinkage);
			$$ = cppLinkage;
		    }
		    else
		    {
			error("illegal language linkage specification");
			$$ = 0;
		    }
		    delete $2;
		}
	;

declarator.list:
	init.declarator
		{ $$ = new DeclarationList($1); }
	| declarator.list ',' init.declarator
		{ $1->appendDecl($3); }
	;
id.declarator.list:
	id.init.declarator
		{ $$ = new DeclarationList($1); }
	| id.declarator.list ',' init.declarator
		{ $1->appendDecl($3); }
	;

init.declarator:
	declarator initializer.opt
		{ if ($2) $1->setInit($2); $$ = $1; }
	| constructed.id.declarator
	;

declarator:
        id.declarator
	| typedef.declarator
	;

id.init.declarator:
	id.declarator initializer.opt
		{ if ($2) $1->setInit($2); $$ = $1; }
	| constructed.id.declarator
	;

constructed.id.declarator:
	nonf.id.direct.declarator paren.exp.list
		{ $1->setInit($2); }
	| ptr.operator.list nonf.id.direct.declarator paren.exp.list
            {
		if ($2)
		{
		    if ($1)
			$2->adjustType($1);
		    $$ = $2;
		} else
		    $$ = new Declaration(NullString, $1);
		$$->setInit($3);
	    }

	;

paren.exp.list:
        '(' expression.list ')'
		{ $$ = new NestedExpression($2); }
	;

initializer.opt:
	initializer
	|
		{ $$ = 0; }
	;

nonf.direct.declarator:
	dname
	| '(' declarator ')'
                { 
		    $$ = $2;
		}
	| nonf.direct.declarator '[' constant.expression.opt ']'
                {
                    VecType *vp = new VecType($3, 0);
		    $1->adjustType(vp);

                    $$ = $1;
                }

	;

nonf.id.direct.declarator:
	id.dname
	| '(' id.declarator ')'
                { 
		    $$ = $2;
		}
	| nonf.id.direct.declarator '[' constant.expression.opt ']'
                {
                    VecType *vp = new VecType($3, 0);

		    $1->adjustType(vp);
                    $$ = $1;
                }
	;

typedef.declarator:
	ptr.operator.list direct.declarator
            {
		if ($2)
		{
		    if ($1)
			$2->adjustType($1);
		    $$ = $2;
		} else
		    $$ = new Declaration(NullString, $1);
	    }
	| direct.declarator
	;

direct.declarator:
	nonf.direct.declarator
	| nonf.direct.declarator param.type.list
           {
                if ($1)
                {
                    $1->adjustType($2);
                    $$ = $1;
                }
                else
                    $$ = new Declaration(NullString,$2);
            }

	;

id.declarator:
	nonf.id.direct.declarator
	| ptr.operator.list nonf.id.direct.declarator
            {
		if ($2)
		{
		    if ($1)
		    {
			$2->adjustType($1);
		    } 
		    $$ = $2;
		} else
		    $$ = new Declaration(NullString, $1);
	    }
	| ptr.operator.list nonf.id.direct.declarator param.type.list
            {
		$3->setReturnType($1);
		if ($2)
		{
		    $2->adjustType($3);
		    $$ = $2;
		}
		else
		    $$ = new Declaration(NullString,$3);
	    }
	| nonf.id.direct.declarator param.type.list
            {
		if ($1)
		{
		    $1->adjustType($2);
		    $$ = $1;
		}
		else
		    $$ = new Declaration(NullString,$2);
	    }
	;

param.type.list:
        '(' argument.declaration.list ')' cv.qualifier.list.opt pure.specifier.opt
            {
		$$ = new FctType(currentClassType,$2,$4);
		if ($5)
		{
		    $$->makePure();
		    delete $5;
		}
	    }
	;

template.decl.arglist:
	template.decl.arglist ',' template.decl.arg
		{ $3->freezeDecl(); templateArgs->putIn($3); $$ = 0;}
	| template.decl.arg
		{ $1->freezeDecl(); templateArgs->putIn($1); $$ = 0; }
	;


template.decl.arg:
	class.key IDENTIFIER
		{ 
		    if ($1 != CLASS_TYPE)
			error("template argument type must be 'class'");
		    
		    ClassType *ct = new ClassType(*$2, $1, 0, GlobalInfo::getContext());
		    ct->addStubModifier(STUB_NOREMOTE);
                    ct->addStubModifier(STUB_USERMARSHALL);
		    ct->freezeType();
		    Declaration *d = new Declaration(*$2, ct);
//		    d->makeSpecial();
		    d->makeTypedef();
		    delete $2;
		    $$ = d; 
		}
	| class.key CLASSTYPE
		{ 
		    String name = $2->getName();
		    if ($1 != CLASS_TYPE)
			error("template argument type must be 'class'");
		    
		    ClassType *ct = new ClassType(name, $1, 0, GlobalInfo::getContext());
		    ct->addStubModifier(STUB_NOREMOTE);
                    ct->addStubModifier(STUB_USERMARSHALL);
		    ct->freezeType();
		    Declaration *d = new Declaration(name, ct);
//		    d->makeSpecial();
		    d->makeTypedef();
		    $$ = d; 
		}
	| typed.decl.specifiers declarator
		{ 
		    $1->finalise();
		    $2->attachBasicSpec($1);
		    delete $1;
		    $$ = $2;
		}
	;

constant.expression.opt:
	constant.expression
	|
		{ $$ = 0; }
	;

cv.qualifier.list.opt:
	cv.qualifier.list
	|
		{ $$ = 0; } 
	;

ptr.operator.list:
	ptr.operator
	| ptr.operator.list ptr.operator
		{
		    $2->combineTypes($1);
		    $$ = $2;
		}
	;

ptr.operator:
	'*' cv.qualifier.list.opt
		{ $$ = new PointerType(POINTER_TYPE, 0, $2); }
	| '&' cv.qualifier.list.opt
		{ $$ = new PointerType(REFERENCE_TYPE, 0, $2); }
	| complete.class.type COLONCOLON '*' cv.qualifier.list.opt
		{
		    PointerType *pt = new PointerType(POINTER_TYPE, 0, $4);
		    QualifiedType *qt = new QualifiedType($1, pt);
		    $$ = qt;
		}
	;

cv.qualifier.list:
        cv.qualifier
                { $$ = new CvType($1); }
	| cv.qualifier.list cv.qualifier
		{
		    $1->addCvQualifier($2);
		}
	;

cv.qualifier:
	CONST
		{ $$ = QUAL_CONST; }
	| VOLATILE
		{ $$ = QUAL_VOLATILE; }
	;

id.dname:
	name
                { $$ = new Declaration(*$1); delete $1; }
	;

dname:
	complete.class.type
                { $$ = new Declaration($1->simpleTypeName(NORMAL_DECL)); }
        | qualified.type
                { $$ = new Declaration($1->simpleTypeName(NORMAL_DECL)); }
        | TYPEDEFTYPE
                { $$ = new Declaration($1->getName()); }
	;

type.name:
	type.specifier.list abstract.declarator.opt
		{
		    $1->finalise();
		    if ($2)
		    {
			$2->attachBasicSpec($1);
			$$ = $2;
		    }
		    else
			$$ = new Declaration(NullString, $1->getType());
		    delete $1;
		}
	;

type.specifier.list:
	type.specifier 
		{ $$ = new DeclSpecifier($1); }
	| type.specifier.list type.specifier
		{ $$ = $1->mergeSpecInto(new DeclSpecifier($2)); delete $1; }
	| cv.qualifier
		{ $$ = new DeclSpecifier($1); }
	| type.specifier.list cv.qualifier
		{ $1->addCvQualifier($2); $$ = $1; }
	;

abstract.declarator.opt:
	abstract.declarator
	|
		{ $$ = 0; }
	;

abstract.declarator:
        ptr.operator.list direct.abstract.declarator.opt
            {
		if ($2)
		{
		    if ($1)
			$2->adjustType($1);
		    $$ = $2;
		} else
		    $$ = new Declaration(NullString, $1);
	    }
        | direct.abstract.declarator
	;

direct.abstract.declarator.opt:
	direct.abstract.declarator
	|
                { $$ = 0; }
	;

direct.abstract.declarator:
	direct.abstract.declarator.opt param.type.list
            {
		if ($1)
		{
		    $1->adjustType($2);
		    $$ = $1;
		}
		else
		    $$ = new Declaration(NullString, $2);
	    }
	| direct.abstract.declarator.opt '[' constant.expression.opt ']'
	        {
		    VecType *vp = new VecType($3, 0);

		    if ($1)
		    {
			$1->adjustType(vp);
			$$ = $1;
		    }
		    else
			$$ = new Declaration(NullString, vp);
                }
	| '(' abstract.declarator ')'
		{ $$ = $2; }
	;

argument.declaration.list:
	arg.declaration.list.opt 
	| arg.declaration.list.opt ELLIPSIS
		{
		    Declaration *d = new Declaration("...", new DummyArgType());

		    if ($1)
		    {
			$1->appendDecl(d);
		    }
		    else
			$$ = new DeclarationList(d);
		}
	| arg.declaration.list ',' ELLIPSIS
		{ $1->appendDecl(new Declaration("...", new DummyArgType())); }
	;

arg.declaration.list.opt:
	arg.declaration.list
	|
		{ $$ = 0; }
	;

arg.declaration.list:
	argument.declaration
		{ $$ = new DeclarationList($1); }
	| arg.declaration.list ',' argument.declaration
		{ $1->appendDecl($3); }
	;

/*
 * Note we don't use = expression here otherwise the parser gets
 * confused between an initializer and a comma-expression
 */

argument.declaration:
	typed.decl.specifiers declarator
		{ 
		    $1->finalise();
		    $2->attachBasicSpec($1);
		    delete $1;
		    $$ = $2;
		}
	| typed.decl.specifiers declarator '=' assignment.expression
		{
		    $1->finalise();
		    $2->attachBasicSpec($1);
		    $2->setInit(new BinaryExpression(equal, 0, $4));
		    delete $1;
		    $$ = $2;
		}
	| typed.decl.specifiers abstract.declarator.opt
		{ 
		    $1->finalise();
		    if ($2)
			$$ = $2;
		    else
			$$ = new Declaration(NullString);
		    $$->attachBasicSpec($1);
		    delete $1;
		}
	| typed.decl.specifiers abstract.declarator.opt '=' assignment.expression

		{ 
		    $1->finalise();
		    if ($2)
			$$ = $2;
		    else
			$$ = new Declaration(NullString, $1->getType());

		    $$->attachBasicSpec($1);
		    $$->setInit(new BinaryExpression(equal, 0, $4));
		    delete $1;
		}
	| untyped.decl.specifiers id.declarator
                {
    		    $1->finalise();
		    $2->attachBasicSpec($1);
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers id.declarator '=' assignment.expression
		{
		    $1->finalise();
		    $2->attachBasicSpec($1);
		    $2->setInit(new BinaryExpression(equal, 0, $4));
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers abstract.declarator.opt
		{ 
		    $1->finalise();
		    if ($2)
			$$ = $2;
		    else
			$$ = new Declaration(NullString);

		    $$->attachBasicSpec($1);
		    delete $1;
		}
	| untyped.decl.specifiers abstract.declarator.opt '=' assignment.expression
		{ 
		    $1->finalise();
 		    if ($2)
 			$$ = $2;
 		    else
 			$$ = new Declaration(NullString);

		    $$->attachBasicSpec($1);
		    $$->setInit(new BinaryExpression(equal, 0, $4));
		    delete $1;
		}

	;

function.definition:
	id.declarator function.body
		{
		    $1->attachBasicSpec(0);
		    $1->setBody($2);
		    GlobalInfo::putIn($1);

		    $$ = $1;
		}
	|  untyped.decl.specifiers nested.scope CLASSTYPE '(' arg.declaration.list.opt ')' cv.qualifier.list.opt ctor.initializer.opt function.body
		{    
		    Declaration *decl;
		    TypeDescriptor *td;
		    ClassType *cl = (ClassType *)($3->getType());
		    FctType *cFunc = new FctType(cl, $5, $7, CONSTRUCTOR);

		    exitTemporaryNest();
		    cFunc->addName(cl->simpleTypeName(NORMAL_DECL));
		    
		    td = new QualifiedType($2, cFunc);
		    
		    decl = new Declaration(cppLinkage->mangle(cFunc), td);
		    decl->attachBasicSpec($1);
		    decl->setInit($8);
		    decl->setBody($9);

		    GlobalInfo::putIn(decl);
		    
		    $$ = decl;

		    delete $1;
		}
	| nested.scope CLASSTYPE '(' arg.declaration.list.opt ')' cv.qualifier.list.opt ctor.initializer.opt function.body
		{ 
		    Declaration *decl;
		    TypeDescriptor *td;
		    ClassType *cl = (ClassType *)($2->getType());
		    FctType *cFunc = new FctType(cl, $4, $6, CONSTRUCTOR);

		    exitTemporaryNest();
		    cFunc->addName(cl->simpleTypeName(NORMAL_DECL));
		    
		    td = new QualifiedType($1, cFunc);
		    
		    decl = new Declaration(cppLinkage->mangle(cFunc), td);
		    decl->setInit($7);
		    decl->setBody($8);

		    GlobalInfo::putIn(decl);
		    
		    $$ = decl;
		}
	| typed.decl.specifiers declarator function.body
		{ 
		    $2->attachBasicSpec($1);
		    $2->setBody($3);
		    GlobalInfo::putIn($2);
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers id.declarator function.body
		{ 
		    $2->attachBasicSpec($1);
		    $2->setBody($3);
		    GlobalInfo::putIn($2);
		    delete $1;
		    $$ = $2;
		}
        | untyped.decl.specifiers nested.scope conversion.operator function.body
		{ 
		    $3->attachBasicSpec($1);
		    $3->setBody($4);
                    exitTemporaryNest();
		    GlobalInfo::putIn($3);
		    delete $1;
		    $$ = $3;
		}
	;

member.function.definition:
	constructor ctor.initializer.opt function.body
                {
		    $1->setInit($2);
		    $1->setBody($3);
		    GlobalInfo::putIn($1);

		    $$ = $1;
		}
        | destructor function.body
                {
		    $1->setBody($2);
		    GlobalInfo::putIn($1);
		    $$ = $1;
		}
	| conversion.operator function.body
                {
		    $1->setBody($2);
		    GlobalInfo::putIn($1);
		    $$ = $1;
		}
	| untyped.decl.specifiers constructor ctor.initializer.opt function.body
                {
		    $2->setInit($3);
		    $2->setBody($4);
		    $2->attachBasicSpec($1);
		    GlobalInfo::putIn($2);
		    delete $1;
		    $$ = $2;
		}
        | untyped.decl.specifiers destructor function.body
                {
		    $2->attachBasicSpec($1);
		    $2->setBody($3);
		    GlobalInfo::putIn($2);
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers conversion.operator function.body
                {
		    $2->attachBasicSpec($1);
		    $2->setBody($3);
		    GlobalInfo::putIn($2);
		    delete $1;
		    $$ = $2;
		}
	| typed.decl.specifiers member.declarator function.body
                {
		    $2->attachBasicSpec($1);
		    $2->setInit($3);
		    GlobalInfo::putIn($2);
		    delete $1;
		    $$ = $2;
		}
	;

ctor.initializer.opt:
	ctor.initializer
	|
		{ $$ = 0; }
	;

function.body:
	compound.statement
                { $$ = new Literal(*$1); delete $1;}
	;

class.template.body:
	class.head '{' member.list.opt '}'
		{    
		    $1->makeComplete();
		    GlobalInfo::popContext();
		    popClassLevel();
		    $$ = $1;
		    if ($3)
			delete $3;;
		}
	;

initializer:
	'=' assignment.expression
		{ $$ = new BinaryExpression(equal, 0, $2); }
	| '=' '{' initializer.list comma.opt '}'
		{ $$ = new BinaryExpression(equal, 0 ,
					    new NestedExpression($3, '{', '}')); }
	;

comma.opt:
	','
		{ $$ = 0; }
	|
		{ $$ = 0; }
	;

initializer.list:
	expression
	| initializer.list ',' expression
                { $$ = new BinaryExpression(",", $1, $3); }
	| '{' initializer.list comma.opt '}'
		{ $$ = new NestedExpression($2, '{', '}'); }
	;

class.type:
	CLASSTYPE
	| template.class.id
	;

class.specifier:
	class.head '{' member.list.opt '}'
		{
		    $1->makeComplete();
		    GlobalInfo::popContext();
		    popClassLevel();
		    $$ = $1;
		    if ($3)
			delete $3;
		}
	;

class.head:
	class.key IDENTIFIER base.spec.opt
		{ 
		    $$ = declareClass(*$2, $1, $3);
		    delete $2;
		}
	| class.key base.spec.opt
		{ 
		    $$ = declareClass(NullString, $1, 0);
		}
	| class.key CLASSTYPE base.spec.opt
		{ 
		    String s1($2->getName());
		    $$ = declareClass(s1, $1, $3);
		}
	;

template.class.id:
	CLASSTYPE '<' 
	        {
		    DeclContext *dc = ((ClassType *)$1->getType())->getMemberContext();
		    GlobalInfo::pushContext(dc);
		}
	template.inst.arglist '>'
		{
		    TypeDescriptor *td = $1->getType();
		    String tn = td->simpleTypeName();
		    
		    ClassType *ct = new InstantiatedClassType(tn,
							      (ClassType *)td, $4 ,0);
		    
		    $$ = new Declaration(ct->typeName(NORMAL_DECL), ct);
		    $$->makeSpecial();
		    GlobalInfo::popContext();
		    GlobalInfo::putIn($$);
		    delete $4;
		}
	;

template.inst.arglist:
	template.inst.arg
		{
		    $$ = new DeclarationList($1);
		}
	| template.inst.arglist ',' template.inst.arg
		{
		    $1->appendDecl($3);
		}
	;
template.inst.arg:
	type.name
                {
		}
		    
	| assignment.expression
		{
		    $$ = new Declaration(NullString, new DummyType(), $1);
		}
	;

member.list.opt:
	member.list
	|
		{ $$ = 0; }
	;
member.list:
	member.declaration
        | member.list member.declaration
                {
		    if ($1)
		    {
			if ($2)
			{
			    $1->appendList($2);
			    delete $2;
			}
		    } else
			$$ = $2;
		}
	;

member.declaration:
	';'
                { $$ = 0; }
	| member.declarator.list ';'
		{ 
		    Declaration *d;
		    DeclarationListIterator next($1);

		    if ($1)
		    {
			while ((d = next()) != 0)
			{
			    TypeDescriptor *t = d->getType();
			    if (t != 0 && t->kind() == FUNCTION_TYPE)
				d->setName(cppLinkage->mangle(t));
			    GlobalInfo::putIn(d);
		        }
		    }
		}
        | class.or.enum.specifiers ';'
                {
		    /* Current decl is in currentCEDecl and is in symtab */

		    $1->finalise();
		    currentCEDecl->attachBasicSpec($1);
		    delete $1;

		    $$ = 0;
		}
        | elaborated.type.specifiers ';'
                {
		    TypeDescriptor *t = $1->getType();
		    Declaration *d = new Declaration(NullString, t);

		    $1->finalise();
		    d->attachBasicSpec($1);
		    delete $1;
		    GlobalInfo::putIn(d);
		    $$ = 0;
		}
	| typed.decl.specifiers member.declarator.list ';'
		{ 
		    $1->finalise();

		    if ($2)
		    {
			Declaration *d;
			DeclarationListIterator next($2);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec($1);
			    GlobalInfo::putIn(d);
		        }
		    }
		    else
		    {
			TypeDescriptor *td = $1->getType();

			if (td != 0)
			{
			    Type typ = td->kind();

			    if ((typ == AGGREGATE_TYPE) ||
				(typ == ENUMERATION_TYPE))
			    {
				$1->propagateAttributes(td);
				if ($1->isElaborated())
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
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers member.declarator.list.opt ';'
		{ 
		    $1->finalise();

		    if ($2)
		    {
			Declaration *d;
			DeclarationListIterator next($2);

			while ((d = next()) != 0)
			{
			    d->attachBasicSpec($1);
			    GlobalInfo::putIn(d);
		        }
		    }
		    else
		    {
			error("null declarator in class");
		    }
		    declIsFriend = FALSE;
		    delete $1;
		    $$ = $2;
		}
	| member.function.definition semi.opt
		{ $1->setVisibility(currentClassType->getVisibility()); $$ = 0; }
	| qualified.name ';'
		{ $$ = 0; }
	| access.specifier ':'
		{ currentClassType->changeVisibility($1); $$ = 0; }
	;

constructor:
	CLASSTYPE '(' arg.declaration.list.opt ')' cv.qualifier.list.opt
		{
		    String name = $1->getName();

		    if (name == currentClassType->simpleTypeName(NORMAL_DECL))
		    {
			FctType *cFunc = new FctType(currentClassType, $3, $5,
						      CONSTRUCTOR);

			cFunc->addName(name);

			$$ = new Declaration(cppLinkage->mangle(cFunc), cFunc);
			currentClassType->addCtor($$);
		      } else
		      {
			  errorStream << "constructor not named same as current "
		    	    << "class " << name << ": " 
		    	    << currentClassType->simpleTypeName(NORMAL_DECL);
			  error(errorStream);
			  $$ = 0;
		    }
		}
	;

destructor:
	 '~' CLASSTYPE void.parameter.list
		{
		    String name = $2->getName();

		    if (name == currentClassType->simpleTypeName(NORMAL_DECL))
		    {
			FctType *cFunc = new FctType(currentClassType, 0, 0,
						     DESTRUCTOR);

			cFunc->addName(name);
			$$ = new Declaration(cppLinkage->mangle(cFunc),cFunc);
			currentClassType->addDtor($$);
			
		    } else
		    {
			error("destructor not named same as current class");
			$$ = 0;
		    }
		}
	 | '~' IDENTIFIER void.parameter.list
               {
		   String dname = *$2;

		    if (dname == currentClassType->simpleTypeName(NORMAL_DECL))
		    {
			FctType *cFunc = new FctType(currentClassType, 0, 0,
						     DESTRUCTOR);

			cFunc->addName(dname);
			$$ = new Declaration(cppLinkage->mangle(cFunc),cFunc);
			currentClassType->addDtor($$);
		    } else
		    {
			error("destructor not named same as current class");
			$$ = 0;
		    }
		   delete $2;
		}
	;

void.parameter.list:
	'(' ')'
		{ $$ = 0; }
	| '(' VOID ')'
		{ $$ = 0; }
	;

member.declarator.list.opt:
	member.declarator.list
	|
		{ $$ = 0; }
	;

semi.opt:
	';'
		{ $$ = 0; }
	|
		{ $$ = 0; }
	;

member.declarator.list:
	member.declarator
                { 
		    $$ = new DeclarationList($1);
		}
	| member.declarator.list ',' member.declarator
		{
		    $1->appendDecl($3);
		}
	;

member.declarator:
	declarator 
	| constructor
	| destructor
	| conversion.operator
	| identifier.opt ':' constant.expression
		{ $$ = 0; }
	;

pure.specifier.opt:
	'=' INTEGERCONSTANT
		{ 
		    if (*$2 != String("0"))
			error("Non-zero pure specifier");
		    $$ = $2;
		}
	|
		{
		    $$ = 0;
		}
	;

conversion.operator:
        OPERATOR conversion.type.name void.parameter.list cv.qualifier.list.opt pure.specifier.opt
		{
		    String name = $2->simpleTypeName(NORMAL_DECL);

		    FctType *cFunc = new FctType(currentClassType, 0, $4,
						 CONVERSION);

		    cFunc->addName(name);

		    if ($5)
		    {
			cFunc->makePure();
			delete $5;
		    }
		    $$ = new Declaration(name, cFunc);
		}
	;

base.spec: 
	':' base.list
		{ $$ = $2; }
	;

base.list:
	base.specifier
	| base.list ',' base.specifier
		{ 
		    if (!$1->append($3))
		    {
			errorStream << "base class \"" << $3->getClass()->simpleTypeName(NORMAL_DECL)
			    << "\" already mentioned in derivation list";
			error(errorStream);
			delete $3;
		    }
		}
	;	

base.specifier:
	class.type
		{ $$ = new BaseClass((ClassType*)$1->getType()); }
	| VIRTUAL access.specifier class.type
		{ $$ = new BaseClass((ClassType*)$3->getType(), TRUE, $2); }
	| VIRTUAL class.type
		{ $$ = new BaseClass((ClassType*)$2->getType(), TRUE); }
	| access.specifier VIRTUAL class.type
		{ $$ = new BaseClass((ClassType*)$3->getType(), TRUE, $1); }
	| access.specifier class.type
		{ $$ = new BaseClass((ClassType*)$2->getType(), FALSE, $1); }
	;

access.specifier:
	PRIVATE
		{ $$ = CLASS_PRIVATE; }
	| PROTECTED
		{ $$ = CLASS_PROTECTED; }
	| PUBLIC
		{ $$ = CLASS_PUBLIC; }
	;

conversion.type.name:
	type.specifier.list ptr.operator.opt
		{
		    $1->finalise();
		    if ($2)
		    {
			$2->combineTypes($1->getType());
			$1->setType($2);
		    }
		    $$ = $1->getType();
		    delete $1;
		}
	;

ptr.operator.opt:
	ptr.operator
	|
		{ $$ = 0; }
	;

ctor.initializer:
	':' mem.initializer.list
		{ $$ = new BinaryExpression(": ", 0,$2); }
	;

mem.initializer.list:
	 mem.initializer
	| mem.initializer.list ','  mem.initializer
		{ $$ = new BinaryExpression(", ", $1 ,$3); }
	;

mem.initializer:
	class.type '(' expression.list ')'
		{ $$ = new BinaryExpression("",
					    new Literal($1->getName()),
					    new NestedExpression($3)); }
	| class.type '(' ')'
		{ $$ = new Literal($1->getName() + "()"); }
	| IDENTIFIER  '(' expression.list ')'
		{ $$ = new BinaryExpression("",
					    new Literal(*$1),
					    $3); delete $1; }
	| IDENTIFIER  '(' ')'
		{ $$ = new Literal(*$1 + "()"); delete $1; }
	;

operator.function.name:
	OPERATOR operator
		{ 
		    String opName = String("operator ") + *$2;
		    delete $2;
		    $$ = new String(opName);
		}

	;

operator:
	NEW
		{ $$ = new String("new"); }
	| DELETE
		{ $$ = new String("delete"); }
	| '+'
		{ $$ = new String("+"); }
	| '-'
		{ $$ = new String("-"); }
	| '*'
		{ $$ = new String("*"); }
	| '/'
		{ $$ = new String("/"); }
	| '%'
		{ $$ = new String("%"); }
	| '^'
		{ $$ = new String("^"); }
	| '&'
		{ $$ = new String("&"); }
	| '|'
		{ $$ = new String("|"); }
	| '~'
		{ $$ = new String("~"); }
	| '!'
		{ $$ = new String("!"); }
	| '='
		{ $$ = new String("="); }
	| '<'
		{ $$ = new String("<"); }
	| '>'
		{ $$ = new String(">"); }
	| PLUSEQUAL
		{ $$ = new String("+="); }
	| MINUSEQUAL
		{ $$ = new String("-="); }
	| STAREQUAL
		{ $$ = new String("*="); }
	| SLASHEQUAL
		{ $$ = new String("/="); }
	| PERCENTEQUAL
		{ $$ = new String("%="); }
	| XOREQUAL
		{ $$ = new String("^="); }
	| ANDEQUAL
		{ $$ = new String("&="); }
	| OREQUAL
		{ $$ = new String("|="); }
	| LSHIFT
		{ $$ = new String("<<"); }
	| RSHIFT
		{ $$ = new String(">>"); }
	| LSHIFTEQUAL
		{ $$ = new String("<<="); }
	| RSHIFTEQUAL
		{ $$ = new String(">>="); }
	| EQUALEQUAL
		{ $$ = new String("=="); }
	| NOTEQUAL
		{ $$ = new String("!="); }
	| LESSEQUAL
		{ $$ = new String("<="); }
	| MOREEQUAL
		{ $$ = new String(">="); }
	| ANDAND
		{ $$ = new String("&&"); }
	| OROR
		{ $$ = new String("||"); }
	| PLUSPLUS
		{ $$ = new String("++"); }
	| MINUSMINUS
		{ $$ = new String("--"); }
	| ','
		{ $$ = new String(","); }
	| ARROWSTAR
		{ $$ = new String("->*"); }
	| ARROW
		{ $$ = new String("->"); }
	| '(' ')'
		{ $$ = new String("()"); }
	| '[' ']'
		{ $$ = new String("[]"); }
	;

compound.statement:
	'{' { $$ = collectInlineDef(); }  '}'
		{ $$ = $<text>2; }
	;

%%

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

    if (context == 0)
	context = GlobalInfo::getContext();

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
#ifdef DEBUG
    debugStream << "popClassLevel() : leaving level " << classNestLevel;
    debugStream << ": " <<currentClassType->simpleTypeName(NORMAL_DECL);
    debugStream << "\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    currentClassType = classStack[--classNestLevel];
}

void pushClassLevel ( ClassType *ct )
{ 
#ifdef DEBUG   
    debugStream << "pushClassLevel() : entering level " << classNestLevel+1;
    debugStream << ":" << ct->simpleTypeName(NORMAL_DECL);
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
#ifdef DEBUG   
    debugStream << "exitTemporaryNest() nest is " << temporaryNest <<"\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    while (temporaryNest > 0)
    {
	temporaryNest--;
	
	popClassLevel();
	GlobalInfo::popContext();
    }  
  
#ifdef DEBUG   
    debugStream << "exitTemporaryNest() nest is now " << temporaryNest <<"\n";
    _debug.dbgFlush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif
 
}

int yywrap ()
{
    return 1;
}
