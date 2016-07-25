%{
/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: c++21.lex.l,v 1.27 1993/09/20 14:09:36 ngdp Exp $
 */

/*
 * Lexer for stub generator.
 * Uses several states:
 *  INITIAL - Normal operation while processing
 * COM,MCOM - Active only during comment processing (looking for stub commands)
 *	      Only difference is what terminates the comment
 *      DIR - Used during processing of # directives
 *    INLIN - Used during inline function collection
 *
 */

#include <stdlib.h>
#include <strstream.h>
#include <memory.h>

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
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

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef FUNCT_H_
#  include "FuncT.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
#endif

#ifndef BASECLASS_H_
#  include "BaseClass.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLSPEC_H_
#  include "DeclSpec.h"
#endif

#include "c++21.gram.h"

/*
 * Helper routines included at end of lex file
 */

String *doDirective (const String&);
Boolean obeyDirective ();
int tokenType (const String&);
int tokenPeek ();
StubModifier stubCommand (const String&);

/*
 * Standard 'lex' routines
 */

#ifdef FLEX_SCANNER
#  define input yyinput
#else
extern "C" void yyunput (int);
extern int yywrap ();
#endif

#ifndef input
#  define input lex_input
#endif

#ifdef _AIX
extern "C" int yyless (int);
extern "C" void yymbunput(int c);
#else
extern "C" void yyless (int);
#endif
extern "C" int yylex();

extern int yylook ();
extern int yyback (int *, int);

static const char RCSid[] = "$Id: c++21.lex.l,v 1.27 1993/09/20 14:09:36 ngdp Exp $";

static Boolean collectingInlines = FALSE;

static Boolean lookahead = FALSE;
static int lookaheadToken;
%}

/* Helper definitions for certain regular expressions */

identifier 	[a-zA-Z_][0-9a-zA-Z_]*
escapeSeq	\\[ntvbrfa?'"\\]|\\[0-7]{1,3}
charConst	\'([^'\\\n]|{escapeSeq})\'
string		\"([^"\\\n]|\\\n|{escapeSeq})*\"
exponent	[eE][-+]?[0-9]+
fractionalConstant ([0-9]*"."[0-9]+)|([0-9]+".")
floatingConstant (({fractionalConstant}{exponent}?)|([0-9]+{exponent}))[FfLl]?
integerSuffix_opt	([uU]?[lL]?)|([lL][uU])
decimalConstant	[0-9][0-9]*{integerSuffix_opt}
octalConstant	"0"[0-7]*{integerSuffix_opt}
hexConstant	"0"[xX][0-9a-fA-F]+{integerSuffix_opt}

whitespace	[ \t\f]
stubcommand	@{identifier}/([ ,\t\f\n])

%start COM MCOM DIR INLIN
%%

<INITIAL>^#.*           { 
                            String *s;
			    yyless(1); /* reset lexer */
                            if ((s = doDirective(yytext)) != 0)
                            {
                                GlobalInfo::addDirective(*s);
				delete s;
			    }
			    BEGIN(INITIAL); 
			}
<INLIN>^#.*             {  
			    yyless(1); /* reset lexer */
                            if ((yylval.text = doDirective(yytext)) != 0)
                                return DIRECTIVE;
			    else
				delete yylval.text;
			}
<INITIAL,INLIN>"//"	{ BEGIN(COM); }
<INITIAL,INLIN>"/*"	{ BEGIN(MCOM); }
<DIR,INITIAL>{whitespace}+  { ; }
<INITIAL>\n		{ GlobalInfo::newLine(); }
<DIR,INLIN>\n		{ return (int)(yytext[0]); }

<INITIAL>auto		{ return (AUTO); }
<INITIAL>char		{ return (CHAR); }
<INITIAL>const		{ return (CONST); }
<INITIAL>delete		{ return (DELETE); }
<INITIAL>double		{ return (DOUBLE); }
<INITIAL>enum		{ return (ENUM); }
<INITIAL>extern		{ return (EXTERN); }
<INITIAL>float		{ return (FLOAT); }
<INITIAL>int		{ return (INT); }
<INITIAL>long		{ return (LONG); }
<INITIAL>register	{ return (REGISTER); }
<INITIAL>short		{ return (SHORT); }
<INITIAL>signed		{ return (SIGNED); }
<INITIAL>sizeof		{ return (SIZEOF); }
<INITIAL>static		{ return (STATIC); }
<INITIAL>struct		{ return (STRUCT); }
<INITIAL>template	{ return (TEMPLATE); }
<INITIAL>typedef	{ return (TYPEDEF); }
<INITIAL>union		{ return (UNION); }
<INITIAL>unsigned	{ return (UNSIGNED); }
<INITIAL>void		{ return (VOID); }
<INITIAL>volatile	{ return (VOLATILE); }

<INITIAL>class		{ return (CLASS); }
<INITIAL>friend		{ return (FRIEND); }
<INITIAL>inline		{ return (INLINE); }
<INITIAL>new		{ return (NEW); }
<INITIAL>operator	{ return (OPERATOR); }
<INITIAL>protected	{ return (PROTECTED); }
<INITIAL>private	{ return (PRIVATE); }
<INITIAL>public		{ return (PUBLIC); }
<INITIAL>this		{ return (THIS); }
<INITIAL>virtual	{ return (VIRTUAL); }

<INITIAL>".*"		{ return (DOTSTAR); }
<INITIAL>"::"		{ return (COLONCOLON); }
<INITIAL>"->"		{ return (ARROW); }
<INITIAL>"->*"		{ return (ARROWSTAR); }
<INITIAL>"++"		{ return (PLUSPLUS); }
<INITIAL>"--"		{ return (MINUSMINUS); }
<INITIAL>"<<"		{ return (LSHIFT); }
<INITIAL>">>"		{ return (RSHIFT); }
<INITIAL>"<="		{ return (LESSEQUAL); }
<INITIAL>">="		{ return (MOREEQUAL); }
<INITIAL>"=="		{ return (EQUALEQUAL); }
<INITIAL>"!="		{ return (NOTEQUAL); }
<INITIAL>"&&"		{ return (ANDAND); }
<INITIAL>"||"		{ return (OROR); }
<INITIAL>"*="		{ return (STAREQUAL); }
<INITIAL>"/="		{ return (SLASHEQUAL); }
<INITIAL>"%="		{ return (PERCENTEQUAL); }
<INITIAL>"+="		{ return (PLUSEQUAL); }
<INITIAL>"-="		{ return (MINUSEQUAL); }
<INITIAL>"<<="		{ return (LSHIFTEQUAL); }
<INITIAL>">>="		{ return (RSHIFTEQUAL); }
<INITIAL>"&="		{ return (ANDEQUAL); }
<INITIAL>"^="		{ return (XOREQUAL); }
<INITIAL>"|="		{ return (OREQUAL); }
<INITIAL>"..."		{ return (ELLIPSIS); }

<INITIAL>{identifier}	{ 
                            return tokenType(yytext);
			}
<DIR>{identifier}	{ yylval.text = new String(yytext); return IDENTIFIER; }
<INITIAL>{charConst}   { yylval.text = new String(yytext); return CHARACTERCONSTANT; }
<DIR,INITIAL>{string}	{ yylval.text = new String(yytext); return STRING; }

<DIR,INITIAL>{decimalConstant} { yylval.text = new String(yytext); return INTEGERCONSTANT; }

<INITIAL>{octalConstant}	{ yylval.text = new String(yytext); return INTEGERCONSTANT; }

<INITIAL>{hexConstant}	{ yylval.text = new String(yytext); return INTEGERCONSTANT; }
<INITIAL>{floatingConstant} { yylval.text = new String(yytext); return FLOATINGCONSTANT; } 

<INITIAL,INLIN>.        { return ((int) yytext[0]); }

<COM,MCOM,DIR>@{identifier}/([ ,\t\f\n]) {
                                yylval.stubcommand = stubCommand(yytext);
				if (yylval.stubcommand == STUB_DEFAULT)
                                {
				    errorStream << "Illegal stub generation option: ";
				    errorStream << yytext;
				    error(errorStream);
				} else
				return STUB_CMD;
			    }

<COM>"\n"		{ 
                           GlobalInfo::newLine();
			   if (collectingInlines)
                              BEGIN(INLIN);
			   else
			      BEGIN(INITIAL);
			}
<MCOM>"\n"		{ GlobalInfo::newLine(); }
<MCOM>"*/"		{ 
                          if (collectingInlines)
                              BEGIN(INLIN);
			   else
			      BEGIN(INITIAL);
			}
<COM,MCOM,DIR>.		{}
%%

String *doDirective ( const String& text )
{
    BEGIN(DIR);
    
    if (obeyDirective())
	return 0;
    else
	return new String(text);
}

/*
 * Obey a cpp generated directive. These start with a # and are typically
 * line directives or pragmas. Line directives are obeyed by changing
 * location information maintained in GlobalInfo. Pragamas are checked
 * against stub specific ones and obeyed if they are otherwise they are
 * ignored and a False result returned.
 * If a directive is obeyed a (or discarded as being illegal) a True
 * reuslt is returned 
 */

Boolean obeyDirective ()
{
    register int c;
    register int token;

    /* skip to first non-whitespace character */

    token = yylex();

    if (token == '\n')
    {
	/* Empty directive - ignore */
	warning("Empty # directive");
        unput(token);
        return TRUE;
    }

    /* check for pragmas/ident/line */
	
    if (token == IDENTIFIER)
    {
	if (strcmp("line", (const char *)yytext) == 0)
	{
	    /* discard saved text */

	    delete yylval.text;
	    token = yylex();
	} else
	    if (strcmp("ident", (const char *)yytext) == 0)
	    {
		delete yylval.text;

		/* skip forwards */

		c = token;
		while (( c > 0) && (c != '\n'))
		    c = input();

		unput(c);

		return FALSE;			/* directive not obeyed */
	    }
	    else 
		if (strcmp("pragma", (const char *)yytext) == 0)
		{
		    delete yylval.text;

		    /* check for stub specific pragma */
	
		    token = yylex();
	
		    if (token == STUB_CMD)
		    {
			do
			{
			    GlobalInfo::setCurrentMods(yylval.stubcommand);
			    token = yylex();
			} while (token == STUB_CMD);
		    
			if (token == '\n')
			{
			    unput(token);
			    return TRUE;
			}
			else
			    warning("Illegal #pragma directive");
		    }
		
		    /* skip forwards */
		
		    c = token;
		    while (( c > 0) && (c != '\n'))
			c = input();
		
		    unput(c);
		
		    return FALSE;	/* directive not obeyed */
		}
    }
    if (token == INTEGERCONSTANT)
    {
	/* set line number and free up space */

	GlobalInfo::setCurrentLine(atol(*yylval.text)-1);
	delete yylval.text;

	/* see if there is a trailing filename */

	token = yylex();
	if (token == '\n')
	{
	    unput(token);
	    return TRUE;;
	}

	/* more there, must be a filename */
	    
	if (token == STRING)
	{
	    char *cptr = *yylval.text;
	    cptr++;                         /* skip over double quote */
	    cptr[strlen(cptr)-1] = '\0';
		
	    GlobalInfo::setCurrentFile(String(cptr));
	    
	    delete yylval.text;
	}
	else
	    warning("Malformed #line directive");
    }
    else
	warning("Unrecognised # directive");
    
    /* now ignore rest of line */
    
    c = token;
    while ((c > 0) && (c != '\n'))
	c = input();
    
    unput(c);
    
    return TRUE;
}

/*
 * Collect the contents of inline functions 
 */

static const int MAX_INLINE_SIZE = 4096;

String *collectInlineDef ()
{
    int bracelevel = 1;
    int token;
    char def[MAX_INLINE_SIZE];
    int inlineInd = 0;

    collectingInlines = TRUE;
    BEGIN INLIN;

    memset(def, '\0', MAX_INLINE_SIZE);
    def[inlineInd++] = '{';

    while (bracelevel > 0)
    {
        token = yylex();
	if (token > 0)
	{
	    /* Assume single char */
	    if (inlineInd > MAX_INLINE_SIZE)
		fatal("Inline function buffer overflow");

	    switch (token)
	    {
	    case '{':
		bracelevel++;
		def[inlineInd++] = '{';
		break;
	    case '}':
		bracelevel--;
		def[inlineInd++] = '}';
		if (bracelevel == 0)
		{
		    unput(token);
		    break;
		}
		break;
	    case STUB_CMD:
		break;
	    case DIRECTIVE:
	    {
		char *sp = *yylval.text;
		int len = yylval.text->size();

		if (inlineInd+len > MAX_INLINE_SIZE)
		    fatal("Inline function buffer overflow");

		strncpy(sp, &def[inlineInd], len);
		inlineInd += len;
		break;
	    }
	    default:
                if (token == '\n')
		    GlobalInfo::newLine();
		def[inlineInd++] = token;
	    }
	}
	else
	    fatal("Unexpected EOF reading inline function");
    }
    collectingInlines = FALSE;
    BEGIN INITIAL;
    
    return new String(def);
}

/* 
 * Peek ahead at next token. This should reset the lexer via yyless
 * but flex cannot handle this (yyless is a cpp macro) so we do it
 * in the rule for identifiers instead.
 */

int tokenPeek ()
{
    int token;
    
    /* peek ahead at next token */
    lookahead = TRUE;

    token = yylex();

#ifdef FLEX_SCANNER

    /* flex does not allow yyless here (its a macro) simulate its behaviour! */

    for (int i = yyleng-1;  i >= 0; i--)
	unput(yytext[i]);
#else
    yyless(0);
#endif

    lookahead = FALSE;

    switch (token)
    {
    case STRING:
    case CHARACTERCONSTANT:
    case INTEGERCONSTANT:
    case FLOATINGCONSTANT:
    case IDENTIFIER:
	delete yylval.text;
    default:
	break;
    }
    
    return token;
}

/* 
 * This is a HACK so that the parser can determine if it has seen the
 * token string "class X;" . Only called by declareClass which must
 * determine whether it should be declaring a new class or returning 
 * a previous one depending upon context. The lookahead token is NOT
 * maintained consistently and is ONLY valid in this particular case.
 * UGH!
 */

int getLookahead ()
{
    return lookaheadToken;
}

int tokenType ( const String& name )
{
    String idname(name);
    int token;

    if (!lookahead)
    {
	/* Peek at next token */

	token = tokenPeek();
	lookaheadToken = token;

	if (token == COLONCOLON)
	{
	    /* name better be a class name regardless */
	    yylval.decl = GlobalInfo::lookFor(idname, AGGREGATE_TYPE);
	}
	else
	    yylval.decl = GlobalInfo::lookFor(idname, ANY_TYPE);

	if (yylval.decl != 0)
	{
	    TypeDescriptor *declType = yylval.decl->getType();
	    
	    if (declType != 0 )
	    {
		if (yylval.decl->isTypedef())
		{		
		    Type typ = declType->kind();
		    
		    if (typ == AGGREGATE_TYPE)
			return CLASSTYPE;
		    
		    if (typ == ENUMERATION_TYPE)
			return ENUMTYPE;
		    
		    if (typ == TYPE_TYPE)
			return TYPEDEFTYPE;
		}
	    }
	}
    }
    yylval.text = new String(idname);
    return IDENTIFIER;
}

StubModifier stubCommand ( const String& text )
{
    String command = text.touppercase();
    StubModifier sm = STUB_DEFAULT;

    /* 
     * We have to explicitly create the Strings here otherwise
     * cfront cannot decide whether to convert command to a char *
     * or the constant string to a String. Aghh!
     */

    if (command == "@NOREMOTE")
	sm = STUB_NOREMOTE;
    else
	if (command == "@REMOTE")
	    sm = STUB_REMOTE;
	else
	    if (command == "@NOMARSHALL")
		sm = STUB_NOMARSHALL;
	    else
		if (command == "@AUTOMARSHALL")
		    sm = STUB_AUTOMARSHALL;
		else
		    if (command == "@USERMARSHALL")
			sm = STUB_USERMARSHALL;
		    else
			if (command == "@IN")
			    sm = STUB_ARG_IN;
			else
			    if (command == "@OUT")
				sm = STUB_ARG_OUT;
			    else
				if (command == "@INOUT")
				    sm = STUB_ARG_INOUT;
				else
				    if (command == "@NORENAME")
					sm = STUB_NORENAME;
				    else
					if (command == "@NOCLIENT")
					    sm = STUB_NOCLIENT;
					else
					    if (command == "@NOWARNINGS")
						sm = STUB_NOWARNINGS;

    return sm;
}

#ifndef FLEX_SCANNER
void yyunput (int c)
{
    unput(c);
}

/* 
 * Try to avoid stupid Ansi C / K&R C problem with yyunput definition
 * that is in ncform
 */

#define yyunput yyunput_hack
#endif
