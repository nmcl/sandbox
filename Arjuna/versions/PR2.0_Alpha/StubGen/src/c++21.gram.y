/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: c++21.gram.y,v 1.26 1993/04/23 14:59:01 ngdp Exp $
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

extern String *collect_inline_def ();
extern int get_lookahead ();
extern "C" int yylex ();
extern "C" yyless (int);

static ClassType *declare_class (const String&, ClassSpecifier, BaseClass *,
			         DeclContext * = 0, Boolean = TRUE);
static void pop_class_level ();
static void push_class_level (ClassType *);
static void exit_temporary_nest ();

static const char RCSid[] = "$Id: c++21.gram.y,v 1.26 1993/04/23 14:59:01 ngdp Exp $";

static Boolean decl_is_friend = FALSE;
static const int max_class_nest = 50;
static int class_nest_level = 0;
static int temporary_nest = 0;
static ClassType *current_class_type = 0;
static ClassType *class_stack[max_class_nest];

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
    ClassSpecifier cl_spec;
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

%token	SIZEOF THIS

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

%type   <cl_spec>
        class.key
	
%type	<decl>
        TYPEDEFTYPE CLASSTYPE ENUMTYPE
	declarator init.declarator argument.declaration constructor
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

%type   <func>
        param.type.list

%type	<text>
    	IDENTIFIER DIRECTIVE
	INTEGERCONSTANT FLOATINGCONSTANT
	CHARACTERCONSTANT STRING
	semi.opt pure.specifier
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
			    $1->append_list($2);
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

		    if ($2 && ((tp = $2->get_type()) != 0))
			sp = tp->typename(NORMAL_DECL);

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

		    if ($3 && ((tp = $3->get_type()) != 0))
			sp = tp->typename(NORMAL_DECL);


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

		    if ($5 && ((tp = $5->get_type()) != 0))
			sp = tp->typename(NORMAL_DECL);

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
			$2->attach_basic_spec($1);
		    else
			$2 = new Declaration(NullString, $1->get_type());
		    delete $1;
		    $$ = new String($2->get_type()->typename(NORMAL_DECL));
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
			$3->adjust_type(pt);
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
			$5->adjust_type(qt);
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
			$1->adjust_type(vp);
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
			tn = $1->typename(NORMAL_DECL);

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
		{ $$ = new String(String("~") + $2->get_name()); }
        | qualified.name
	;

qualified.name:
        nested.scope IDENTIFIER
		{ 
		    $$ = new String($1->typename(NORMAL_DECL) + "::" + *$2);
                    exit_temporary_nest();
		    delete $2;
		}
        | nested.scope operator.function.name
		{ 
		    $$ = new String($1->typename(NORMAL_DECL) + "::" + *$2);
                    exit_temporary_nest();
		    delete $2;
		}
        | nested.scope '~' CLASSTYPE
		{ 
		    $$ = new String($1->typename(NORMAL_DECL) + "::~" + $3->get_name());
                    exit_temporary_nest();
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

declaration:
	';'
                { $$ = 0; }
	| id.declarator.list ';'
                { 
		    DeclarationListIterator nd($1);
		    Declaration* d;
		    
		    while (d = nd())
		    {
			error_stream << "type expected for ";
			error_stream << d->get_name();
			error(error_stream);

			d->attach_basic_spec(0);
			GlobalInfo::putin(d);
		    }
		}
        | class.or.enum.specifiers ';'
                {
		    /* Current decl is in currentCEDecl and is in symtab */

		    $1->finalise();
		    currentCEDecl->attach_basic_spec($1);
		    delete $1;
		    $$ = new DeclarationList(currentCEDecl);
		}
        | elaborated.type.specifiers ';'
                {
		    TypeDescriptor *t = $1->get_type();
		    Declaration *d = new Declaration(NullString, t);

		    $1->finalise();
		    d->attach_basic_spec($1);
		    GlobalInfo::putin(d);
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
			    d->attach_basic_spec($1);
			    GlobalInfo::putin(d);
			}
		    }
		    else
		    {
			TypeDescriptor *td = $1->get_type();

			if (td == 0)
			    error("null declarator");
			else
			    if ((td->kind() != AGGREGATE_TYPE) &&
				(td->kind() != ENUMERATION_TYPE))
				error("null declarator");
			    else
			    {
				$1->propagate_attributes(td);
				if ($1->is_elaborated())
				{
				    Declaration *d = new Declaration(NullString, td);

				    d->make_elaborated();
				    d->attach_basic_spec(0);
				    GlobalInfo::putin(d);
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
			    d->attach_basic_spec($1);
			    GlobalInfo::putin(d);
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
	| untyped.decl.specifiers untyped.decl.specifier
		{ $$ = $1->merge_spec_into($2); delete $1; }
	;

untyped.decl.specifier:
	sc.specifier
		{ $$ = new DeclSpecifier($1); }
	| fct.specifier
		{ $$ = new DeclSpecifier($1); }
	| cv.qualifier
		{ $$ = new DeclSpecifier($1); }
	| stub.command.specifier
		{ $$ = new DeclSpecifier($1); }
	;

typedef.type.specifiers:
	typedef.type.specifier
	| untyped.decl.specifiers typedef.type.specifier
		{ $$ = $1->merge_spec_into($2); delete $1; }
	;

basic.type.specifiers:
	basic.type.specifier
	| untyped.decl.specifiers basic.type.specifier
		{ $$ = $1->merge_spec_into($2); delete $1; }
	;

typedef.type.specifier:
        simple.type
		{ $$ = new DeclSpecifier($1); }
	| typedef.type.specifier cv.qualifier
		{ $1->add_cv_qualifier($2); }
	;

elaborated.type.specifiers:
	elaborated.type.specifier
		{ $$ = new DeclSpecifier($1, TRUE); }
	| untyped.decl.specifiers elaborated.type.specifier
		{ $$ = $1->merge_spec_into(new DeclSpecifier($2, TRUE)); delete $1; }
	;

class.or.enum.specifiers:
	class.or.enum.specifier
		{ $$ = new DeclSpecifier($1, FALSE); }
	| untyped.decl.specifiers class.or.enum.specifier
		{ $$ = $1->merge_spec_into(new DeclSpecifier($2, FALSE)); delete $1; }
	;


basic.type.specifier:
	basic.type.list
		{ $$ = new DeclSpecifier($1); }
        ;

basic.type.list:
        basic.type
	| basic.type.list basic.type
		{ 
		    $$ = $1->combine_types($2);
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
		    $$ = new QualifiedType($1, $2->get_type());
                    exit_temporary_nest();
		}
	| nested.scope ENUMTYPE
		{
		    $$ = new QualifiedType($1, $2->get_type());
                    exit_temporary_nest();
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
		{ $$ = SC_FRIEND; decl_is_friend = TRUE;}
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
                { $$ = $1->get_type(); } 
	| TYPEDEFTYPE
                { $$ = $1->get_type(); } 
	| qualified.type
	;

complete.class.type:
        CLASSTYPE
                { $$ = $1->get_type(); } 
	| COLONCOLON CLASSTYPE
                { $$ = $2->get_type(); } 
	| qualified.class.type
	| COLONCOLON qualified.class.type
		{ $$ = $2; }
	;

qualified.class.type:
        nested.scope CLASSTYPE
                { 
		    $$ = new QualifiedType($1, $2->get_type());
                    exit_temporary_nest();
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
		    Declaration *d = new Declaration($1->typename(NORMAL_DECL), $1);

		    d->make_typedef();
		    GlobalInfo::putin(d);
		    currentCEDecl = d;
		}
	| enum.specifier
                {
		    Declaration *d = new Declaration($1->typename(NORMAL_DECL), $1);

		    d->make_typedef();
		    GlobalInfo::putin(d);
		    currentCEDecl = d;
		}
	;
	
elaborated.type.specifier:
	class.key IDENTIFIER
                {
		    if (decl_is_friend)
		    {
			DeclContext *friend_context;

			friend_context = (current_class_type->get_member_context())->get_enclosing_context();
			$$ = declare_class(*$2,$1,0,friend_context, FALSE);
		    }
		    else
		    {
			$$ = declare_class(*$2,$1,0,0, FALSE);
		    }
		    delete $2;
		}
	| class.key CLASSTYPE
		{
		    if ($2->get_context() != GlobalInfo::get_context())
		    {
			$$ = declare_class($2->get_name(),$1,0,0,FALSE);
		    }
		    else
			$$ = $2->get_type();

		}
	| ENUM ENUMTYPE
		{ $$ = $2->get_type(); }
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
	CLASSTYPE COLONCOLON
		{ 
		    ClassType *clas = (ClassType*)($1->get_type());
		    
                    GlobalInfo::push_context(clas->get_member_context());
                    push_class_level(clas);
		    temporary_nest++; 
#ifdef DEBUG   
    debug_stream << "class.scope() nest is " << temporary_nest <<"\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
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
		    String ename;
		    
		    if ($2)
		    {
			ename = *$2;
			delete $2;
		    }
		    $$ = new EnumType(ename, $4, TRUE);
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
		    $1->make_special();
		    GlobalInfo::putin($1);
		    $$ = new DeclarationList($1);
		}
	| enum.list ',' enumerator
                {
		    $3->make_special();
		    GlobalInfo::putin($3);
		    $1->append_decl($3);
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
			GlobalInfo::pop_linkage();
		    $$ = $3;
		}
	| linkage '{' '}'
		{
		    if ($1)
			GlobalInfo::pop_linkage();
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
			    d->set_linkage($1);
			}
			GlobalInfo::pop_linkage();
		    }
		    $$ = $2;
		}
	;

linkage:
	EXTERN STRING
                {
		    if (*$2 == String("\"C\""))
		    {
			GlobalInfo::push_linkage(c_linkage);
			$$ = c_linkage;
		    }
		    else if (*$2 == String("\"C++\""))
		    {
			GlobalInfo::push_linkage(cpp_linkage);
			$$ = cpp_linkage;
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
		{ $1->append_decl($3); }
	;
id.declarator.list:
	id.init.declarator
		{ $$ = new DeclarationList($1); }
	| id.declarator.list ',' init.declarator
		{ $1->append_decl($3); }
	;

init.declarator:
	declarator initializer.opt
		{ if ($2) $1->set_init($2); $$ = $1; }
	| constructed.id.declarator
	;

declarator:
        id.declarator
	| typedef.declarator
	;

id.init.declarator:
	id.declarator initializer.opt
		{ if ($2) $1->set_init($2); $$ = $1; }
	| constructed.id.declarator
	;

constructed.id.declarator:
	nonf.id.direct.declarator paren.exp.list
		{ $1->set_init($2); }
	| ptr.operator.list nonf.id.direct.declarator paren.exp.list
            {
		if ($2)
		{
		    if ($1)
			$2->adjust_type($1);
		    $$ = $2;
		} else
		    $$ = new Declaration(NullString, $1);
		$$->set_init($3);
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
		    $1->adjust_type(vp);

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

		    $1->adjust_type(vp);
                    $$ = $1;
                }
	;

typedef.declarator:
	ptr.operator.list direct.declarator
            {
		if ($2)
		{
		    if ($1)
			$2->adjust_type($1);
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
                    $1->adjust_type($2);
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
			$2->adjust_type($1);
		    } 
		    $$ = $2;
		} else
		    $$ = new Declaration(NullString, $1);
	    }
	| ptr.operator.list nonf.id.direct.declarator param.type.list
            {
		$3->set_return_type($1);
		if ($2)
		{
		    $2->adjust_type($3);
		    $$ = $2;
		}
		else
		    $$ = new Declaration(NullString,$3);
	    }
	| nonf.id.direct.declarator param.type.list
            {
		if ($1)
		{
		    $1->adjust_type($2);
		    $$ = $1;
		}
		else
		    $$ = new Declaration(NullString,$2);
	    }
	;

param.type.list:
        '(' argument.declaration.list ')' cv.qualifier.list.opt
            {
		$$ = new FctType(current_class_type,$2);
		$$->set_quals($4);
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
		    $2->combine_types($1);
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
		    $1->add_cv_qualifier($2);
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
                { $$ = new Declaration($1->typename(NORMAL_DECL)); }
        | qualified.type
                { $$ = new Declaration($1->typename(NORMAL_DECL)); }
        | TYPEDEFTYPE
                { $$ = new Declaration($1->get_name()); }
	;

type.name:
	type.specifier.list abstract.declarator.opt
		{
		    $1->finalise();
		    if ($2)
		    {
			$2->attach_basic_spec($1);
			$$ = $2;
		    }
		    else
			$$ = new Declaration(NullString, $1->get_type());
		    delete $1;
		}
	;

type.specifier.list:
	type.specifier 
		{ $$ = new DeclSpecifier($1); }
	| type.specifier.list type.specifier
		{ $$ = $1->merge_spec_into(new DeclSpecifier($2)); delete $1; }
	| cv.qualifier
		{ $$ = new DeclSpecifier($1); }
	| type.specifier.list cv.qualifier
		{ $1->add_cv_qualifier($2); $$ = $1; }
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
			$2->adjust_type($1);
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
		    $1->adjust_type($2);
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
			$1->adjust_type(vp);
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
			$1->append_decl(d);
		    }
		    else
			$$ = new DeclarationList(d);
		}
	| arg.declaration.list ',' ELLIPSIS
		{ $1->append_decl(new Declaration("...", new DummyArgType())); }
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
		{ $1->append_decl($3); }
	;

/*
 * Note we don't use = expression here otherwise the parser gets
 * confused between an initializer and a comma-expression
 */

argument.declaration:
	typed.decl.specifiers declarator
		{ 
		    $1->finalise();
		    $2->attach_basic_spec($1);
		    delete $1;
		    $$ = $2;
		}
	| typed.decl.specifiers declarator '=' assignment.expression
		{
		    $1->finalise();
		    $2->attach_basic_spec($1);
		    $2->set_init(new BinaryExpression(equal, 0, $4));
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
		    $$->attach_basic_spec($1);
		    delete $1;
		}
	| typed.decl.specifiers abstract.declarator.opt '=' assignment.expression

		{ 
		    $1->finalise();
		    if ($2)
			$$ = $2;
		    else
			$$ = new Declaration(NullString, $1->get_type());

		    $$->attach_basic_spec($1);
		    $$->set_init(new BinaryExpression(equal, 0, $4));
		    delete $1;
		}
	| untyped.decl.specifiers id.declarator
                {
    		    $1->finalise();
		    $2->attach_basic_spec($1);
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers id.declarator '=' assignment.expression
		{
		    $1->finalise();
		    $2->attach_basic_spec($1);
		    $2->set_init(new BinaryExpression(equal, 0, $4));
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

		    $$->attach_basic_spec($1);
		    delete $1;
		}
	| untyped.decl.specifiers abstract.declarator.opt '=' assignment.expression
		{ 
		    $1->finalise();
 		    if ($2)
 			$$ = $2;
 		    else
 			$$ = new Declaration(NullString);

		    $$->attach_basic_spec($1);
		    $$->set_init(new BinaryExpression(equal, 0, $4));
		    delete $1;
		}

	;

function.definition:
	id.declarator function.body
		{
		    $1->attach_basic_spec(0);
		    $1->set_body($2);
		    GlobalInfo::putin($1);

		    $$ = $1;
		}
	|  untyped.decl.specifiers nested.scope CLASSTYPE '(' arg.declaration.list.opt ')' cv.qualifier.list.opt ctor.initializer.opt function.body
		{    
		    Declaration *decl;
		    TypeDescriptor *td;
		    ClassType *cl = (ClassType *)($3->get_type());
		    FctType *c_func = new FctType(cl, $5, CONSTRUCTOR);

		    exit_temporary_nest();
		    c_func->add_name(cl->typename(NORMAL_DECL));
		    if ($7)
			c_func->set_quals($7);
		    
		    td = new QualifiedType($2, c_func);
		    
		    decl = new Declaration(cpp_linkage->mangle(c_func), td);
		    decl->attach_basic_spec($1);
		    decl->set_init($8);
		    decl->set_body($9);

		    GlobalInfo::putin(decl);
		    
		    $$ = decl;

		    delete $1;
		}
	| nested.scope CLASSTYPE '(' arg.declaration.list.opt ')' cv.qualifier.list.opt ctor.initializer.opt function.body
		{ 
		    Declaration *decl;
		    TypeDescriptor *td;
		    ClassType *cl = (ClassType *)($2->get_type());
		    FctType *c_func = new FctType(cl, $4, CONSTRUCTOR);

		    exit_temporary_nest();
		    c_func->add_name(cl->typename(NORMAL_DECL));
		    if ($6)
			c_func->set_quals($6);
		    
		    td = new QualifiedType($1, c_func);
		    
		    decl = new Declaration(cpp_linkage->mangle(c_func), td);
		    decl->set_init($7);
		    decl->set_body($8);

		    GlobalInfo::putin(decl);
		    
		    $$ = decl;
		}
	| typed.decl.specifiers declarator function.body
		{ 
		    $2->attach_basic_spec($1);
		    $2->set_body($3);
		    GlobalInfo::putin($2);
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers id.declarator function.body
		{ 
		    $2->attach_basic_spec($1);
		    $2->set_body($3);
		    GlobalInfo::putin($2);
		    delete $1;
		    $$ = $2;
		}
        | untyped.decl.specifiers nested.scope conversion.operator function.body
		{ 
		    $3->attach_basic_spec($1);
		    $3->set_body($4);
                    exit_temporary_nest();
		    GlobalInfo::putin($3);
		    delete $1;
		    $$ = $3;
		}
	;

member.function.definition:
	constructor ctor.initializer.opt function.body
                {
		    $1->set_init($2);
		    $1->set_body($3);
		    GlobalInfo::putin($1);

		    $$ = $1;
		}
        | destructor function.body
                {
		    $1->set_body($2);
		    GlobalInfo::putin($1);
		    $$ = $1;
		}
	| conversion.operator function.body
                {
		    $1->set_body($2);
		    GlobalInfo::putin($1);
		    $$ = $1;
		}
	| untyped.decl.specifiers constructor ctor.initializer.opt function.body
                {
		    $2->set_init($3);
		    $2->set_body($4);
		    $2->attach_basic_spec($1);
		    GlobalInfo::putin($2);
		    delete $1;
		    $$ = $2;
		}
        | untyped.decl.specifiers destructor function.body
                {
		    $2->attach_basic_spec($1);
		    $2->set_body($3);
		    GlobalInfo::putin($2);
		    delete $1;
		    $$ = $2;
		}
	| untyped.decl.specifiers conversion.operator function.body
                {
		    $2->attach_basic_spec($1);
		    $2->set_body($3);
		    GlobalInfo::putin($2);
		    delete $1;
		    $$ = $2;
		}
	| typed.decl.specifiers member.declarator function.body
                {
		    $2->attach_basic_spec($1);
		    $2->set_init($3);
		    GlobalInfo::putin($2);
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

class.specifier:
	class.head '{' member.list.opt '}'
		{
		    $1->make_complete();
		    GlobalInfo::pop_context();
		    pop_class_level();
		    $$ = $1;
		    if ($3)
			delete $3;
		}
	;

class.head:
	class.key IDENTIFIER base.spec.opt
		{ 
		    $$ = declare_class(*$2, $1, $3);
		    delete $2;
		}
	| class.key base.spec.opt
		{ 
		    $$ = declare_class(NullString, $1, 0);
		}
	| class.key CLASSTYPE base.spec.opt
		{ 
		    String s1($2->get_name());
		    $$ = declare_class(s1, $1, $3);
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
			    $1->append_list($2);
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
			    TypeDescriptor *t = d->get_type();
			    if (t != 0 && t->kind() == FUNCTION_TYPE)
				d->set_name(cpp_linkage->mangle(t));
			    GlobalInfo::putin(d);
		        }
		    }
		}
        | class.or.enum.specifiers ';'
                {
		    /* Current decl is in currentCEDecl and is in symtab */

		    $1->finalise();
		    currentCEDecl->attach_basic_spec($1);
		    delete $1;

		    $$ = 0;
		}
        | elaborated.type.specifiers ';'
                {
		    TypeDescriptor *t = $1->get_type();
		    Declaration *d = new Declaration(NullString, t);

		    $1->finalise();
		    d->attach_basic_spec($1);
		    delete $1;
		    GlobalInfo::putin(d);
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
			    d->attach_basic_spec($1);
			    GlobalInfo::putin(d);
		        }
		    }
		    else
		    {
			TypeDescriptor *td = $1->get_type();

			if (td != 0)
			{
			    Type typ = td->kind();

			    if ((typ == AGGREGATE_TYPE) ||
				(typ == ENUMERATION_TYPE))
			    {
				$1->propagate_attributes(td);
				if ($1->is_elaborated())
				{
				    Declaration *d = new Declaration(NullString, td);

				    d->attach_basic_spec(0);
				    GlobalInfo::putin(d);
				}
			    }
			}
			else
			    error("null declarator in class");
		    }
		    decl_is_friend = FALSE;
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
			    d->attach_basic_spec($1);
			    GlobalInfo::putin(d);
		        }
		    }
		    else
		    {
			error("null declarator in class");
		    }
		    decl_is_friend = FALSE;
		    delete $1;
		    $$ = $2;
		}
	| member.function.definition semi.opt
		{ $1->set_visibility(current_class_type->get_visibility()); $$ = 0; }
	| qualified.name ';'
		{ $$ = 0; }
	| access.specifier ':'
		{ current_class_type->change_visibility($1); $$ = 0; }
	;

constructor:
	CLASSTYPE '(' arg.declaration.list.opt ')' cv.qualifier.list.opt
		{
		    String name = $1->get_name();

		    if (name == current_class_type->typename(NORMAL_DECL))
		    {
			FctType *c_func = new FctType(current_class_type, $3,
						      CONSTRUCTOR);

			c_func->add_name(name);
			if ($5)
			    c_func->set_quals($5);
			$$ = new Declaration(cpp_linkage->mangle(c_func), c_func);
			current_class_type->add_ctor($$);
		    } else
		    {
			error_stream << "constructor not named same as current"
			    << "class " << name << ": " << current_class_type->typename(NORMAL_DECL);
			error(error_stream);
			$$ = 0;
		    }
		}
	;

destructor:
	 '~' CLASSTYPE void.parameter.list
		{
		    String name = $2->get_name();

		    if (name == current_class_type->typename(NORMAL_DECL))
		    {
			FctType *c_func = new FctType(current_class_type, 0,
						      DESTRUCTOR);

			c_func->add_name(name);
			$$ = new Declaration(cpp_linkage->mangle(c_func),c_func);
			current_class_type->add_dtor($$);
			
		    } else
		    {
			error("destructor not named same as current class");
			$$ = 0;
		    }
		}
	 | '~' IDENTIFIER void.parameter.list
               {
		   String dname = *$2;

		    if (dname == current_class_type->typename(NORMAL_DECL))
		    {
			FctType *c_func = new FctType(current_class_type, 0,
						      DESTRUCTOR);

			c_func->add_name(dname);
			$$ = new Declaration(cpp_linkage->mangle(c_func),c_func);
			current_class_type->add_dtor($$);
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
		    $1->append_decl($3);
		}
	;

member.declarator:
	declarator pure.specifier
		{ $1->make_pure(); }
	| declarator 
	| constructor
	| destructor
	| conversion.operator
        | conversion.operator pure.specifier
		{ $1->make_pure(); }
	| identifier.opt ':' constant.expression
		{ $$ = 0; }
	;

pure.specifier:
	'=' INTEGERCONSTANT
		{ 
		    if (*$2 != String("0"))
			error("Non-zero pure specifier");
		    delete $2;
		    $$ = 0;
		}
	;

conversion.operator:
        OPERATOR conversion.type.name void.parameter.list cv.qualifier.list.opt
		{
		    String name = $2->typename(NORMAL_DECL);

		    FctType *c_func = new FctType(current_class_type, 0,
						  CONVERSION);

		    c_func->add_name(name);
		    if ($4)
			c_func->set_quals($4);
		    $$ = new Declaration(name, c_func);
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
			error_stream << "base class \"" << $3->get_class()->typename(NORMAL_DECL)
			    << "\" already mentioned in derivation list";
			error(error_stream);
			delete $3;
		    }
		}
	;	

base.specifier:
	CLASSTYPE
		{ $$ = new BaseClass((ClassType*)$1->get_type()); }
	| VIRTUAL access.specifier CLASSTYPE
		{ $$ = new BaseClass((ClassType*)$3->get_type(), TRUE, $2); }
	| VIRTUAL CLASSTYPE
		{ $$ = new BaseClass((ClassType*)$2->get_type(), TRUE); }
	| access.specifier VIRTUAL CLASSTYPE
		{ $$ = new BaseClass((ClassType*)$3->get_type(), TRUE, $1); }
	| access.specifier CLASSTYPE
		{ $$ = new BaseClass((ClassType*)$2->get_type(), FALSE, $1); }
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
			$2->combine_types($1->get_type());
			$1->set_type($2);
		    }
		    $$ = $1->get_type();
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
	CLASSTYPE '(' expression.list ')'
		{ $$ = new BinaryExpression("",
					    new Literal($1->get_name()),
					    new NestedExpression($3)); }
	| CLASSTYPE '(' ')'
		{ $$ = new Literal($1->get_name() + "()"); }
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
		    String opname = String("operator ") + *$2;
		    delete $2;
		    $$ = new String(opname);
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
	'{' { $$ = collect_inline_def(); }  '}'
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

static int anon_count = 0;

ClassType *declare_class ( const String& classname, ClassSpecifier ctype,
			   BaseClass *baselist, 
			   DeclContext *context,
			   Boolean is_defn )
{
    ClassType *new_class = 0;
    Declaration *old_decl = 0;
    Declaration *new_decl = 0;
    String cname(classname);
    Boolean nameless = (classname == NullString ? TRUE : FALSE);
    Boolean declare = is_defn;
    int token = get_lookahead();		/* lookahead */
    Boolean maybe_new = (token == ';' ? TRUE : FALSE);

    if (context == 0)
	context = GlobalInfo::get_context();

    if (nameless)
    {
	/* class/struct/union with no name - make one up */
	cname = constructname("_NoName_", anon_count++);
    }

    /* See if class name is already known */

    old_decl = GlobalInfo::lookfor(cname, AGGREGATE_TYPE);

    if (old_decl)			/* known */
    {
	ClassType *old_type = (ClassType *)old_decl->get_type();

	if (old_decl->get_context() == context)
	{
	    /* same context */
	    if (is_defn)
	    {
		new_class = new ClassType(cname, ctype, baselist,
					  context, nameless, is_defn);
		old_type->combine_types(new_class);
		declare = FALSE;
	    }
	    else
		new_class = old_type;
	}
	else
	{
	    /* different contexts */
	    if (maybe_new || is_defn)
	    {
		/* class X; or class X{...}; in nested scope */
		new_class = new ClassType(cname, ctype, baselist,
					  context, nameless, is_defn);

		declare = TRUE;
	    }
	    else
		new_class = old_type;
	}
    }
    else				/* not known */
    {
	new_class = new ClassType(cname, ctype, baselist,
				  context, nameless, is_defn);

	declare = TRUE;
    }

    if (declare)
    {
	/* temporarily enter required context to declare in*/

	GlobalInfo::push_context(context);
	new_decl = new Declaration(cname, new_class);
	new_decl->make_elaborated();

	/* resume previous context */

	GlobalInfo::pop_context();
	
	new_decl->make_typedef();
	new_decl->make_special();
	context->putin(new_decl);
    }
    
    if (is_defn)
    {
	GlobalInfo::push_context(new_class->get_member_context());
	push_class_level(new_class);
    }

    return new_class;
}

void pop_class_level ()
{
#ifdef DEBUG
    debug_stream << "pop_class_level() : leaving level " << class_nest_level;
    debug_stream << ": " <<current_class_type->typename(NORMAL_DECL);
    debug_stream << "\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    current_class_type = class_stack[--class_nest_level];
}

void push_class_level ( ClassType *ct )
{ 
#ifdef DEBUG   
    debug_stream << "push_class_level() : entering level " << class_nest_level+1;
    debug_stream << ":" << ct->typename(NORMAL_DECL);
    debug_stream << "\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (class_nest_level < max_class_nest)
    {
	class_stack[++class_nest_level] = ct;
	current_class_type = ct;
    }
    else
	fatal("(internal) class nesting level exceeded");
}

void exit_temporary_nest ()
{ 
#ifdef DEBUG   
    debug_stream << "exit_temporary_nest() nest is " << temporary_nest <<"\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    while (temporary_nest > 0)
    {
	temporary_nest--;
	
	pop_class_level();
	GlobalInfo::pop_context();
    }  
  
#ifdef DEBUG   
    debug_stream << "exit_temporary_nest() nest is now " << temporary_nest <<"\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif
 
}

int yywrap ()
{
    return 1;
}
