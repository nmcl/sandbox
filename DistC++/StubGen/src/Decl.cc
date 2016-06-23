/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Decl.cc,v 1.1 1997/09/25 15:30:47 nmcl Exp $
 */


/*
 * Stub generator declaration handler. Declarations are the key data structure
 * used in the stub generator and consist of several parts:
 *
 * 	A filename/linenumber (possibly) used in error reporting
 * 	A name
 * 	A type plus type specifiers
 * 	A initial value.
 *
 */

#ifndef WIN32     
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif
     
#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

#ifndef DUMMYARGT_H_
#  include "DummyArgT.h"
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

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif
     
static const char RCSid[] = "$Id: Decl.cc,v 1.1 1997/09/25 15:30:47 nmcl Exp $";

long Declaration::countOfDecls = 0;

/*
 * Constructor. Create a declaration. The values set here may be overridden
 * later. The location information is determined by accessing information
 * maintained by the parser as it reads the input, as are the default stub 
 * generation options.
 */

Declaration::Declaration ( const String& name,
			   TypeDescriptor *dt,
			   Expression *value )
                         : declBody(0),
			   declContext(0),
			   declFrozen(FALSE),
                           declInit(value),
			   declIsElaborated(FALSE),
			   declQualName(0),
			   declIsSpecial(FALSE),
			   declIsTypedef(FALSE),
			   declLinkage(GlobalInfo::getLinkage()),
			   declLoc(GlobalInfo::getCurrentloc()),
			   declName(name),
			   declMarshall(STUB_PENDING),
			   declObjType(STUB_PENDING),
			   declTransfer(STUB_PENDING),
			   declStclass(SC_DEFAULT),
			   declaratorType(dt),
			   declTemplateArgs(0),
			   declVis(FILE_EXTERN),
			   defaultLinkage(TRUE)
{
#ifdef SG_DEBUG
    debugStream << "Declaration::Declaration (" << (void *)this << ", " 
	         << name << ", " << (void *)dt << ", " << value << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Declaration::~Declaration ()
{
    if (declaratorType && declaratorType->canDiscard())
	delete declaratorType;
}

Boolean Declaration::addStClass ( StorageSpecifier newSt )
{
#ifdef SG_DEBUG
    debugStream << "Declaration::addStClass("  << (void *)this << "," 
		<< newSt << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (declStclass ==  SC_DEFAULT)
    {
	declStclass = newSt;
	if (declStclass == SC_TYPEDEF)
	    declIsTypedef = TRUE;
    } else
        if (declStclass == SC_TYPEDEF)
            error("cannot have storage class in typedef");
        else
            error("storage class already specified in this declaration");

    return TRUE;
}

Boolean Declaration::addStubModifier ( StubModifier newSm )
{
#ifdef SG_DEBUG
    debugStream << "Declaration::addStubModifier("  << (void *)this << "," 
		<< newSm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    Boolean rValue = FALSE;
    Boolean conflicting = FALSE;
    StubModifier earlier = STUB_PENDING;    
    
    switch (newSm)
    {
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_NOMARSHALL:
	earlier = declMarshall;
	if (declMarshall == newSm)
	    break;
	else
	    if (declMarshall == STUB_PENDING)
	    {
		declMarshall = newSm;
		rValue = TRUE;
	    }
	    else
		conflicting = TRUE;

	break;
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
    case STUB_REMOTE:
	earlier = declObjType;
	if (declObjType == newSm)
	    break;
	else
	    if (declObjType == STUB_PENDING)
	    {
		declObjType = newSm;
		rValue = TRUE;
	    }
	    else
		conflicting = TRUE;
	break;
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	earlier = declTransfer;
	if (declTransfer == newSm)
	    break;
	else
	    if (declTransfer == STUB_PENDING)
	    {
		declTransfer = newSm;
		rValue = TRUE;
	    }
	    else
		conflicting = TRUE;
	break;
    default:
	break;
    }

    if (declaratorType)
	rValue = declaratorType->addStubModifier(newSm);
    else 
	if (!rValue)
	{
	    ostrstream errorStream;

	    errorStream << "Stub modifier '" << newSm << "' ";
	    if (conflicting)
		errorStream << "conflicts with earlier modifier '"
			    << earlier << "'";
	    else
		errorStream << " illegal in this declaration";
	    
	    error(errorStream);
	}
    
    return rValue;
}

Boolean Declaration::addTemplateArgs( DeclContext* context )
{
    declTemplateArgs = context;
    return TRUE;
}

/*
 * Adjust the type of a DECLARATOR. Used during parsing prior
 * to combination with basic type. These types will indicate 
 * pointer/reference, arrays, and functions
 */

Boolean Declaration::adjustType ( TypeDescriptor *declType )
{
    if (declaratorType)
	declaratorType = declaratorType->combineTypes(declType);
    else
	declaratorType = declType;

    return TRUE;
}

/*
 * Attach basic declaration specifiers after freezing them. If there is none 
 * currently we use the one given. Otherwise we merge the one given INTO
 * the one already held. This ensures that for declarations like:
 *    const char *p, *q
 * the decl spec "const char" is merged into the two declarations for "p"
 * and "q" which will already have decl specs (with type "ptr to unknown").
 */

Boolean Declaration::attachBasicSpec ( DeclSpecifier *basicSpec )
{
#ifdef SG_DEBUG
    debugStream << "Declaration::attachBasicSpec("  << (void *)this
		<< ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    TypeDescriptor *basic_dtype = 0;

    if (basicSpec)
    {
	basic_dtype = basicSpec->getType();

	/* Complete type information */

	if (declaratorType)
	    declaratorType->attachBaseType(basic_dtype);
	else
	    declaratorType = basic_dtype;
    }

    /* Default to Int if needed */

    if (declaratorType == 0)
    {
	/*
	 * Visual C++ 2.0 finds this ambiguous.
	 */
	
#ifndef WIN32
	if (declName == "...")
#else	    
	if (declName == (const char*) "...")
#endif	    
	    declaratorType = new DummyArgType();
	else
	    declaratorType = new IntType();
    }

    /* Propagate name down to type */

    if (declName != NullString)
	declaratorType->addName(declName);

    /* Propagate the remaining attributes down to the type */

    if (basicSpec)
	basicSpec->propagateAttributes(this);

    /* Set up typedef info if required */

    if (declStclass == SC_TYPEDEF)
    {
	TypeDefType *td = new TypeDefType(declName, declaratorType);

	declaratorType = td;
	declIsTypedef = TRUE;
    }
     
    return TRUE;
}


/*
 * Freeze decl. Check that any stub modifiers explicitly set are applicable.
 * Marshalling is odd in that the marshalling keywords are generally
 * inapplicable in all contexts UNLESS the declaration is a typdef
 * OR its a forward reference to a class (marked as elaborated with no name)
 */

void Declaration::freezeDecl ()
{
#ifdef SG_DEBUG
    debugStream << "Declaration::freezeDecl("  << (void *)this
		<< "," << declName << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    if (!declFrozen)
    {
	Location savedLoc = errorLocation;

	errorLocation = declLoc;

	declFrozen = TRUE;

	if ((declVis == FILE_EXTERN) && (declStclass == SC_STATIC))
	    declVis = FILE_STATIC;

	if (declaratorType == 0)
	    fatal("(Stub internal) Declaration has no type");
    
	if (!declIsSpecial)
	{
	    Declaration *d = 0;
	    String qualName = declContext->getContextName(NORMAL_DECL);
	    ostrstream errorStream;

	    /* Forward refs are marked elaborated with no name */
	    
	    if ((declName == NullString) && declIsElaborated)
		qualName += declaratorType->simpleTypeName(NORMAL_DECL);
	    else
		qualName += declaratorType->massageName(declName,NORMAL_DECL);
	    
	    d = GlobalInfo::getPragma(qualName.removeChars(" "));

	    if (d != 0)
	    {
		addStubModifier(d->declMarshall);
		addStubModifier(d->declTransfer);
		addStubModifier(d->declObjType);
	    }
	    
	    if (declMarshall == STUB_PENDING)
	    {
		TypeDescriptor *tempType = declaratorType->deref();

		if (tempType && (tempType->kind() == FUNCTION_TYPE))
		    addStubModifier(STUB_NOMARSHALL);
	    }
	    
		
	    if (declObjType != STUB_PENDING)
	    { 
		if (declContext->isApplicable(declObjType))
		{
		    declaratorType->overrideStubModifiers(declObjType);
		} else
		{
		    errorStream << "'" << declObjType 
				<< "' illegal in this declaration";
		    error(errorStream);
		}
	    }
	    else
	    {
		/* for class members default to NoRemote unless public */
		switch (declVis)
		{
		case CLASS_PROTECTED:
		case CLASS_PRIVATE:
		    declaratorType->overrideStubModifiers(STUB_NOREMOTE);
		    break;
		default:
		    break;  
		}
	    }
	
	    if (declTransfer != STUB_PENDING)
	    {
		if(declContext->isApplicable(declTransfer))
		{
		    declaratorType->overrideStubModifiers(declTransfer);
		} else
		{
		    errorStream << "'" << declTransfer
				<< "' illegal in this declaration";
		    error(errorStream);
		}
	    }
	    
	    if (declMarshall != STUB_PENDING)
	    {
		Boolean ok = TRUE;
		
		if (declContext->isApplicable(declMarshall) ||
		    declIsTypedef ||
		    ((declName == NullString) && declIsElaborated))
		{
		    ok = declaratorType->overrideStubModifiers(declMarshall);
		}

		if (!ok)
		{
		    errorStream << "'" << declMarshall
				<< "' illegal in this declaration";
		    error(errorStream);
		}
	    }

	    declaratorType->freezeType();
	}

	errorLocation = savedLoc;
    }
}

void Declaration::setBody ( Expression *body )
{
    declBody = body;
    if (declaratorType)
	declaratorType->addBody(body);
}

void Declaration::setContext ( DeclContext *context )
{
#ifdef SG_DEBUG
    debugStream << "Declaration::setContext("  << (void *)this
		<< ", " << (void *)context << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    declContext = context;

    if (declaratorType != 0 && (declIsTypedef))
	declaratorType->setContext(context);
    
}

void Declaration::setInit ( Expression *expr )
{
    if (declInit == 0)
    {
	declInit = expr;
    }
    else
    {
	error("declaration already has initial value\n");
    }

}

void Declaration::setLinkage ( Linkage *l )
{
    if (defaultLinkage)
    {
	defaultLinkage = FALSE;
	declLinkage = l;
    }
    else
	if (declLinkage != l)
	    error("conflicting linkage specification\n");

}

String Declaration::getName () const
{
    return declName;
}

Type Declaration::kind () const
{
    if (declaratorType)
	return declaratorType->kind();
    else
	return NONE_TYPE;
}
/*
 * Print a declaration. Declarations are printed in two parts. The type
 * before the name, the name, and then the type after the name. This is to
 * handle array and function declarations where the array dimensions or
 * function parameters appear after the variable name. Most of this is handled
 * by the virtual printInstance function found in all types.
 * Genuine declarations should ALWAYS have a declSpec describing the 
 * type of the declaration otherwise there is something funny going on.
 */

void Declaration::printDecl ( ostream& s, DeclStyle ds,
			      const char *declSep,
			      Boolean noname, Boolean novalue,
			      Boolean noConst,
			      Boolean printSpecial )
{

    Location savedLoc = errorLocation;
    
    /* Only print if not a 'special' declaration */

    if (declIsSpecial && !printSpecial)
	return;

    errorLocation = declLoc;
    
    if (declContext &&
	(ds != CLIENT_DECL && ds != SERVER_DECL) &&
	!(declIsTypedef))
    {
	if ((ds != IDL_DECL) &&
	    (declContext->isApplicable(declObjType) ||
	     declContext->isApplicable(declMarshall) ||
	     declContext->isApplicable(declTransfer)))
	    
	{
            /* Stub specifics first */

	    s << "/* ";

	    if (declContext->isApplicable(declObjType))
		s << declObjType << " ";

	    if (declContext->isApplicable(declMarshall))
		s << declMarshall << " ";

	    if (declContext->isApplicable(declTransfer))
		s << declTransfer << " ";

	    s << "*/ ";
	}
	if ((ds == IDL_DECL) &&
	    declContext->isApplicable(declaratorType->defaultTransfer()))
	{
	    printStubModifier(s, declaratorType->defaultTransfer(), ds);
	    s << " ";
	}
	
    }
    
    /* Linkage first - eg extern "C" */
    
    if (declLinkage && !defaultLinkage)
	s << declLinkage;

    if (declTemplateArgs && !declIsTypedef)
	s << *declTemplateArgs;

    if ((ds != IDL_DECL) || (declStclass == SC_TYPEDEF))
	s << declStclass;
    
    /* Typedefs always have a decl spec */
    
    if (declIsTypedef)
	declaratorType->printAsTypedef(s,ds);
    else
    {
	/* 'Normal' style declaration here - NOTE special handling of ... */

	/*
	 * Visual C++ 2.0 finds this ambiguous.
	 */
	
#ifndef WIN32
	if (noname && (declName != "..."))
#else	    
	if (noname && (declName != (const char*) "..."))
#endif	    
	    declaratorType->printInstanceOf(s, NullString, ds, 0, declIsElaborated,noConst);
	else
	    declaratorType->printInstanceOf(s, declName, ds, declQualName, declIsElaborated, noConst);
    }
    
    if (!novalue)
    {
	if (declInit != 0)
	    s << " " << *declInit;
	if (declBody != 0)
	    s << *declBody << "\n\n";
	else
	    if (declaratorType->needsSep())
		s << declSep;
    }
    else
	if (declaratorType->needsSep())
	    s << declSep;

    errorLocation = savedLoc;
}

StubModifier Declaration::getMarshall ( Boolean explicitWanted ) const
{
    if ((declStclass == SC_FRIEND) || (declIsSpecial))
	return STUB_NOMARSHALL;
    else
	if (!explicitWanted &&
	    ((declMarshall == STUB_PENDING) ||(declMarshall == STUB_DEFAULT)))
	    return declaratorType->defaultMarshall();
	else
	    return declMarshall;
}

StubModifier Declaration::getRemote () const
{
    if (declIsSpecial)
	return STUB_IGNORE;
    else
	if ((declObjType == STUB_PENDING) ||
	    (declObjType == STUB_DEFAULT))
	    return declaratorType->defaultLocation();
	else
	    return declObjType;
}

StubModifier Declaration::getTransfer () const
{
    if (declIsSpecial)
	return STUB_IGNORE;
    else
	if ((declTransfer == STUB_PENDING) ||
	    (declTransfer == STUB_DEFAULT))
	    return  declaratorType->defaultTransfer();
	else
	    return declTransfer;
}

void Declaration::marshall (ostream& s , Direction d,
			    DeclStyle ds , const String& bname)
{
    if (!declIsSpecial && !declIsTypedef && (declaratorType != 0))
	declaratorType->marshall(s,d,ds,bname,declName);
}

Boolean Declaration::produceAllStubs ( ostream& masterHdrStream,
				       OutputStyle inStyle )
{
    Location savedLoc = errorLocation;
    Boolean needBrace = FALSE;

    if (declIsSpecial)
	return TRUE;

    errorLocation = declLoc;
    
    if (declLinkage && !defaultLinkage)
    {
	masterHdrStream << declLinkage;
	masterHdrStream << "\n{\n";
	needBrace = TRUE;
    }

    if (((inStyle == SHADOWS_STYLE) || (inStyle == CPLUSPLUS_STYLE)) &&
	declIsTypedef && declaratorType->stubIsPossible())
	declaratorType->produceAllStubs(masterHdrStream);
    else
    {
	if (inStyle != IDL_STYLE)
	{
	    masterHdrStream << this;
	    if (declMarshall != STUB_NOMARSHALL)
	    {
		declaratorType->produceMarshallDefs(masterHdrStream,NORMAL_DECL, FALSE);
	    }
	}
	else
	    printDecl(masterHdrStream, IDL_DECL);
    }

    if (needBrace)
	masterHdrStream << "\n}\n";

    errorLocation = savedLoc;
    return TRUE;
}

Boolean Declaration::produceStubCode ( ostream& codeStream, DeclStyle ds )
{
    return declaratorType->produceStubCode(codeStream,ds);
}

Boolean Declaration::produceMarshallCode ( ostream& codeStream, DeclStyle ds,
					   Direction d, const String& buffName )
{
    return declaratorType->produceMarshallingCode(codeStream, ds, d, declName, buffName);
}

Boolean Declaration::produceMarshallDefs ( ostream& hdrStream, DeclStyle ds,
					   Boolean full )
{
    return declaratorType->produceMarshallDefs(hdrStream, ds, full);
}


#ifdef NO_INLINES
#  define DECL_CC_
#  include "Decl.n"
#  undef DECL_CC_
#endif
