%{
/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: c++21.lex.l,v 1.1 1997/09/25 15:31:36 nmcl Exp $
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

#ifdef MEMFNS_IN_STRING_H
#  include <string.h>
#else
#  include <memory.h>
#endif

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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
 * Linkage between lex & yacc
 */

String tokenText;

/*
 * Helper routines included at end of lex file
 *
 */

Boolean doDirective (const String&);
Boolean obeyDirective (const String&);
int tokenType (const String&);
int tokenPeek ();
StubModifier stubCommand (const String&);
void remember ();

/*
 * Standard 'lex' routines
 */

#ifdef FLEX_SCANNER
#  define input yyinput
#else
extern "C" void yyunput (int);
extern "C" int yywrap ();
#endif

#ifndef input
#  define input lex_input
#endif

#ifndef yyless
#ifdef INT_YYLESS
extern "C" int yyless (int);
#else
extern "C" void yyless (int);
#endif
#endif

#ifdef _AIX
extern "C" void yymbunput(int c);
extern "C" void yymbreturn(int c);
#endif

extern "C" int yylex();

extern int yylook ();
extern int yyback (int *, int);

static const char RCSid[] = "$Id: c++21.lex.l,v 1.1 1997/09/25 15:31:36 nmcl Exp $";

static Boolean collectingInlines = FALSE;

static Boolean lookahead = FALSE;
static int lookaheadToken;
%}

/* Helper definitions for certain regular expressions */

identifier 	[a-zA-Z_][0-9a-zA-Z_]*
escapeSeq	\\[ntvbrfa?'"\\]|\\[0-7]{1,3}
charConst	L?\'([^'\\\n]|{escapeSeq})\'
string		\"([^"\\\n]|\\\n|{escapeSeq})*\"
exponent	[eE][-+]?[0-9]+
fractionalConstant ([0-9]*"."[0-9]+)|([0-9]+".")
floatingConstant (({fractionalConstant}{exponent}?)|([0-9]+{exponent}))[FfLl]?
integerSuffix_opt	([uU]?[lL]?)|([lL][uU])
decimalConstant	[1-9][0-9]*{integerSuffix_opt}
octalConstant	"0"[0-7]*{integerSuffix_opt}
hexConstant	"0"[xX][0-9a-fA-F]+{integerSuffix_opt}

whitespace	[ \t\f]

%start COM MCOM DIR INLIN
%e 1500
%p 3500
%%

<COM,MCOM,DIR>@{whitespace}*{identifier}/([ ,\t\f\n]) {
                                yylval.stubcommand = stubCommand(yytext);
				if (yylval.stubcommand == STUB_DEFAULT)
                                {
                                    ostrstream errorStream;

				    errorStream << "Illegal stub generation option: ";
				    errorStream << yytext;
				    error(errorStream);
				} else
				return STUB_CMD;
			    }

<INITIAL>^#.*           { 
                            String s(yytext);
                            BEGIN(DIR);
                            yyless(1);
                            if (!doDirective(s))
                            {
                                GlobalInfo::addDirective(s);
			    }
			    BEGIN(INITIAL); 
			}
<INLIN>^#.*             {  
                            String s(yytext);
                            BEGIN(DIR);
                            yyless(1);
                            if (!doDirective(s))
                            {
                                yylval.text = new String(s);
                                return DIRECTIVE;
                            }
			}
<INITIAL,INLIN>"//"	{ BEGIN(COM); }
<INITIAL,INLIN>"/*"	{ BEGIN(MCOM); }

<DIR,INITIAL>{whitespace}+  { ; }

<INITIAL>\n		{ GlobalInfo::newLine(); }
<DIR,INLIN>\n		{ return (int)(yytext[0]); }


<INITIAL>"<%"		{ return '{'; }
<INITIAL>"%>"		{ return '}'; }
<INITIAL>"<:"		{ return '['; }
<INITIAL>":>"		{ return ']'; }
<INITIAL>"%:"		{ return '#'; }
<INITIAL>"bitand"	{ return '&'; }
<INITIAL>"and"		{ return (ANDAND); }
<INITIAL>"bitor"	{ return '|'; }
<INITIAL>"or"		{ return (OROR); }
<INITIAL>"xor"		{ return '^'; }
<INITIAL>"compl"	{ return '~'; }
<INITIAL>"and_eq"	{ return (ANDEQUAL); }
<INITIAL>"or_eq"	{ return (OREQUAL); }
<INITIAL>"xor_eq"	{ return (XOREQUAL); }
<INITIAL>"not"		{ return '!'; }
<INITIAL>"not_eq"	{ return (NOTEQUAL); }

<INITIAL>asm		{ return (ASM); }
<INITIAL>auto		{ return (AUTO); }

<INITIAL>bool           { remember (); return (BOOL_TYPE); }
<INITIAL>break		{ return (BREAK); }

<INITIAL>case		{ return (CASE); }
<INITIAL>catch          { return (CATCH); }
<INITIAL>char		{ remember (); return (CHAR); }
<INITIAL>class		{ remember (); return (CLASS); }
<INITIAL>const_cast	{ return (CONST_CAST); }
<INITIAL>const		{ remember (); return (CONST); }
<INITIAL>__const	{ remember ();
#ifdef __GNUG__
                            return (CONST);
#else
			    return tokenType(yytext);
#endif
                        }
<INITIAL>__const__	{ remember ();
#ifdef __GNUG__
                            return (CONST);
#else
			    return tokenType(yytext);
#endif
                        }
<INITIAL>continue	{ return (CONTINUE); }

<INITIAL>default	{ return (DEFAULT); }
<INITIAL>delete		{ return (DELETE); }
<INITIAL>double		{ remember (); return (DOUBLE); }
<INITIAL>do		{ return (DO); }
<INITIAL>dynamic_cast	{ return (DYNAMIC_CAST); }

<INITIAL>else		{ return (ELSE); }
<INITIAL>extern		{ remember (); return (EXTERN); }
<INITIAL>enum		{ remember (); return (ENUM); }

<INITIAL>false		{ remember (); return (BOOL_FALSE); }
<INITIAL>float		{ remember (); return (FLOAT); }
<INITIAL>friend		{ remember (); return (FRIEND); }
<INITIAL>for		{ return (FOR); }

<INITIAL>goto		{ return (GOTO); }

<INITIAL>if		{ return (IF); }
<INITIAL>inline		{ remember (); return (INLINE); }
<INITIAL>int		{ remember (); return (INT); }

<INITIAL>long		{ remember (); return (LONG); }

<INITIAL>mutable	{ return (MUTABLE); }

<INITIAL>new		{ return (NEW); }
<INITIAL>namespace	{ return (NAMESPACE); }

<INITIAL>operator	{ remember (); return (OPERATOR); }

<INITIAL>private	{ remember (); return (PRIVATE); }
<INITIAL>protected	{ remember (); return (PROTECTED); }
<INITIAL>public		{ remember (); return (PUBLIC); }

<INITIAL>register	{ remember (); return (REGISTER); }
<INITIAL>reinterpret_cast { return (REINTERPRET_CAST); }
<INITIAL>return		{ return (RETURN); }

<INITIAL>short		{ remember (); return (SHORT); }
<INITIAL>signed		{ remember (); return (SIGNED); }
<INITIAL>__signed	{ remember ();
#ifdef __GNUG__
                            return (SIGNED);
#else
			    return tokenType(yytext);
#endif
                        }
<INITIAL>__signed__	{ remember ();
#ifdef __GNUG__
                            return (SIGNED);
#else
			    return tokenType(yytext);
#endif
                        }
<INITIAL>sizeof		{ remember (); return (SIZEOF); }
<INITIAL>static_cast	{ return (STATIC_CAST); }
<INITIAL>static		{ remember (); return (STATIC); }
<INITIAL>struct		{ remember (); return (STRUCT); }
<INITIAL>switch		{ return (SWITCH); }

<INITIAL>template	{ remember (); return (TEMPLATE); }
<INITIAL>this		{ remember (); return (THIS); }
<INITIAL>throw          { return (THROW); }
<INITIAL>true		{ remember (); return (BOOL_TRUE); }
<INITIAL>try            { return (TRY); }
<INITIAL>typedef	{ remember (); return (TYPEDEF); }
<INITIAL>typeid		{ return (TYPEID); }

<INITIAL>union		{ remember (); return (UNION); }
<INITIAL>unsigned	{ remember (); return (UNSIGNED); }
<INITIAL>using		{ return (USING); }

<INITIAL>virtual	{ remember (); return (VIRTUAL); }
<INITIAL>volatile	{ remember (); return (VOLATILE); }
<INITIAL>void		{ remember (); return (VOID); }

<INITIAL>wchar_t	{ 
			  remember (); 
#if defined(BROKEN_WCHAR_T) && !defined(HAVE_WCHAR_T)
			  return tokenType(yytext);
#else
			  return (WCHAR);
#endif
			}
<INITIAL>while		{ return (WHILE); }


<INITIAL>".*"		{ return (DOTSTAR); }
<INITIAL>"::"		{ return (COLONCOLON); }
<INITIAL>"->*"		{ return (ARROWSTAR); }
<INITIAL>"->"		{ return (ARROW); }

<INITIAL>"++"		{ return (PLUSPLUS); }
<INITIAL>"--"		{ return (MINUSMINUS); }
<INITIAL>"<<="		{ return (LSHIFTEQUAL); }
<INITIAL>"<<"		{ return (LSHIFT); }
<INITIAL>">>"		{ return (RSHIFT); }
<INITIAL>">>="		{ return (RSHIFTEQUAL); }
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


<INITIAL>"&="		{ return (ANDEQUAL); }
<INITIAL>"^="		{ return (XOREQUAL); }
<INITIAL>"|="		{ return (OREQUAL); }
<INITIAL>"..."		{ return (ELLIPSIS); }

<INITIAL>{identifier}	{ 
                            remember (); return tokenType(yytext);
			}

<INITIAL>{charConst}   { remember (); yylval.text = new String(yytext); return CHARACTERCONSTANT; }
<DIR,INITIAL>{string}	{ remember (); yylval.text = new String(yytext); return STRING; }

<DIR,INITIAL>{decimalConstant} { remember (); yylval.text = new String(yytext); return INTEGERCONSTANT; }

<INITIAL>{octalConstant}	{ remember (); yylval.text = new String(yytext); return INTEGERCONSTANT; }

<INITIAL>{hexConstant}	{ remember (); yylval.text = new String(yytext); return INTEGERCONSTANT; }
<INITIAL>{floatingConstant} { remember (); yylval.text = new String(yytext); return FLOATINGCONSTANT; } 
<DIR>{identifier}	{ remember (); yylval.text = new String(yytext); return IDENTIFIER; }
<INITIAL,INLIN>.        { return ((int) yytext[0]); }

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

Boolean doDirective ( const String& text )
{
    return obeyDirective(text);
}

/*
 * Obey a cpp generated directive. These start with a # and are typically
 * line directives or pragmas. Line directives are obeyed by changing
 * location information maintained in GlobalInfo. Pragamas are checked
 * against stub specific ones and obeyed if they are otherwise they are
 * ignored and a False result returned.
 * If a directive is obeyed a (or discarded as being illegal) a True
 * result is returned 
 * Parameter is for error reporting purposes only
 */

Boolean obeyDirective ( const String& text )
{
    register int c;
    register int token;
    ostrstream errorStream;

    /* skip to first non-whitespace character */

    token = yylex();

    if (token == '\n')
    {
	/* Empty directive - ignore */
	warning("Empty # directive (ignored)");
        unput(token);
        return TRUE;
    }

    /* check for pragmas/ident/line */

    if (token == IDENTIFIER)
    {
	/* have to do this first as it is essentially noise */
	
	if (strcmp("line", (const char *)yytext) == 0)
	{
	    /* discard saved text */

	    delete yylval.text;
	    token = yylex();
	}
    }

    switch (token)
    {
    case INTEGERCONSTANT:
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
	
	/* Check for DEC cxx pragma format '$' already ignored by lexer */
	
	if (token == IDENTIFIER)
	{
	    /* beware of fall through to pragma code here */
	}
	else
	{
	    if (token == STRING)
	    {
		/* more there, must be a filename */
		
		char *cptr = *yylval.text;
		cptr++;                         /* skip over double quote */
		cptr[strlen(cptr)-1] = '\0';
		
		GlobalInfo::setCurrentFile(String(cptr));
		
		delete yylval.text;
	    }
	    else
	    {
		errorStream << "Malformed #line directive"
			    << text << endl;
		
		warning(errorStream);
	    }
	    break;
	}

    case IDENTIFIER:
	/* #line taken care of above */
	
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
		    
		    DeclSpecifier *ds = new DeclSpecifier(STUB_PENDING);
		    
		    do
		    {
			switch (yylval.stubcommand)
			{
			case STUB_NORENAME:
			    GlobalInfo::setCurrentMods(yylval.stubcommand);
			case STUB_DEFAULT:
			    break;
			default:
			    ds->addStubModifier(yylval.stubcommand);
			}
			
			token = yylex();
		    } while (token == STUB_CMD);
		    
		    while (token == STRING)
		    {
			String temp = (*yylval.text).removeChars(" \t\"");
			
			GlobalInfo::addPragma(temp, ds);
			delete yylval.text;
			
			token = yylex();
			
		    }
		    
		    delete ds;
		    
		    if (token == '\n')
		    {
			unput(token);
			return TRUE;
		    }
		    else
		    {
			errorStream << "Illegal #pragma directive:"
				    << text << endl;
			
			warning(errorStream);
		    }
		}
		
		/* skip forwards */
		
		c = token;
		while (( c > 0) && (c != '\n'))
		    c = input();
		
		unput(c);
		
		return FALSE;	/* directive not obeyed */
	    }
    default:
	errorStream << "Unrecognised # directive"
		    << text << endl;
	
	warning(errorStream);
    }
    
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
                    {
			TypeDescriptor *typOf = declType;
			
			do
			{
			    typOf = typOf->deref();
			    typ = typOf->kind();
			} while ((typ == TYPE_TYPE) || (typ == CV_TYPE));
			

			if (typ == AGGREGATE_TYPE)
			    return CLASSTYPE;
			else if (typ == ENUMERATION_TYPE)
			    return  ENUMTYPE;

			return TYPEDEFTYPE;
		    }
		}
	    }
	}
    }
    yylval.text = new String(idname);
    return IDENTIFIER;
}

/*
 * Check if this is a stub command.
 * We use strpbrk looking for the first letter of the available
 * options, this allows whitespace after the '@' (SPARCcompiler C++ 4.0
 * tokenises differently to other compilers)
 */

StubModifier stubCommand ( const String& text )
{
    /* 
     * We have to explicitly create the Strings here otherwise
     * cfront cannot decide whether to convert command to a char *
     * or the constant string to a String. Aghh!
     */

    String command = text.touppercase().strpbrk("NRAUIOFD");
    StubModifier sm = STUB_DEFAULT;

    if (command == "NOREMOTE")
	sm = STUB_NOREMOTE;
    else
	if (command == "REMOTE")
	    sm = STUB_REMOTE;
	else
	    if (command == "NOMARSHALL")
		sm = STUB_NOMARSHALL;
	    else
		if (command == "AUTOMARSHALL")
		    sm = STUB_AUTOMARSHALL;
		else
		    if (command == "USERMARSHALL")
			sm = STUB_USERMARSHALL;
		    else
			if (command == "IN")
			    sm = STUB_ARG_IN;
			else
			    if (command == "OUT")
				sm = STUB_ARG_OUT;
			    else
				if (command == "INOUT")
				    sm = STUB_ARG_INOUT;
				else
				    if (command == "NORENAME")
					sm = STUB_NORENAME;
				    else
					if (command == "NOCLIENT")
					{
					    caution("@NoCLient no longer valid - use @NoRemote");
                                            sm = STUB_NOREMOTE;
                                        }
					else
					    if (command == "NOWARNINGS")
						sm = STUB_NOWARNINGS;
					    else
						if (command == "FREEMEMORY")
						{
						    caution("@FreeMemory no longer valid - use @Delete");
						    sm = STUB_DELETE;
						}
						else
						    if (command == "NODELETE")
							sm = STUB_NODELETE;
						    else
							if (command == "DELETE")
							    sm = STUB_DELETE;
    

    return sm;
}

void remember ()
{
    tokenText = yytext;
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
