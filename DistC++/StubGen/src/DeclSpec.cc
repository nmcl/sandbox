/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclSpec.cc,v 1.1 1997/09/25 15:30:50 nmcl Exp $
 */

/*
 * Stub generator declaration specifier handler. Declaration specifiers
 * capture storage class, type qualifiers and type of a declaration during
 * initial stages of the parse. They are then combined with declarations
 * once the actual declarators have been seen.
 *
 */

#ifndef WIN32
#  include <strstream>
#else
#  include <strstrea.h>
#endif

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CVT_H_
#  include "CvT.h"
#endif

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLSPEC_H_
#  include "DeclSpec.h"
#endif

static const char RCSid[] = "$Id: DeclSpec.cc,v 1.1 1997/09/25 15:30:50 nmcl Exp $";

/*
 * Public constructors
 */

DeclSpecifier::DeclSpecifier ( CvQualifier cv )
                             : declConst(FALSE),
			       declInline(FALSE),
			       declIsElaborated(FALSE),
			       declMarshall(STUB_PENDING),
			       declStubType(STUB_PENDING),
			       declTransfer(STUB_PENDING),
			       declOptions(STUB_PENDING),
			       declStClass(SC_DEFAULT),
			       declBtype(0),
			       declVirtual(FALSE),
			       declVolatile(FALSE),
			       warningsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
		<< cv << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    addCvQualifier(cv);
}

DeclSpecifier::DeclSpecifier ( FuncModifier fm )
                             : declConst(FALSE),
			       declInline(FALSE),
			       declIsElaborated(FALSE),
			       declMarshall(STUB_PENDING),
			       declStubType(STUB_PENDING),
			       declTransfer(STUB_PENDING),
			       declOptions(STUB_PENDING),
			       declStClass(SC_DEFAULT),
			       declBtype(0),
			       declVirtual(FALSE),
			       declVolatile(FALSE),
			       warningsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
		<< fm << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    addFuncModifier(fm);
}

DeclSpecifier::DeclSpecifier ( StorageSpecifier st )
                             : declConst(FALSE),
			       declInline(FALSE),
			       declIsElaborated(FALSE),
			       declMarshall(STUB_PENDING),
			       declStubType(STUB_PENDING),
			       declTransfer(STUB_PENDING),
			       declOptions(STUB_PENDING),
			       declStClass(st),
			       declBtype(0),
			       declVirtual(FALSE),
			       declVolatile(FALSE),
			       warningsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
		<< st << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

}

DeclSpecifier::DeclSpecifier ( StubModifier sc )
                             : declConst(FALSE),
			       declInline(FALSE),
			       declIsElaborated(FALSE),
			       declMarshall(STUB_PENDING),
			       declStubType(STUB_PENDING),
			       declTransfer(STUB_PENDING),
			       declOptions(STUB_PENDING),
			       declStClass(SC_DEFAULT),
			       declBtype(0),
			       declVirtual(FALSE),
			       declVolatile(FALSE),
			       warningsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
		<< sc << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    addStubModifier(sc);    
}

DeclSpecifier::DeclSpecifier ( TypeDescriptor *td, Boolean elaborated )
                             : declConst(FALSE),
			       declInline(FALSE),
			       declIsElaborated(elaborated),
			       declMarshall(STUB_PENDING),
			       declStubType(STUB_PENDING),
			       declTransfer(STUB_PENDING),
			       declOptions(STUB_PENDING),
			       declStClass(SC_DEFAULT),
			       declBtype(td),
			       declVirtual(FALSE),
			       declVolatile(FALSE),
			       warningsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::DeclSpecifier("  << (void *)this << "," 
		<< (void *)td << ", " << elaborated << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

}

/*
 * Public non-virtual functions
 */

Boolean DeclSpecifier::addCvQualifier ( CvQualifier cv )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::addCvQualifier("  << (void *)this
	         << "," << cv << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cv == QUAL_CONST)
    {
	if (declConst)
	    warning("superfluous 'const' ignored"); 
    	declConst = TRUE;
    }
    else
    {
	if (declVolatile)
	    warning("superfluous 'volatile' ignored");
	declVolatile = TRUE;
    }
    return TRUE;
}

Boolean DeclSpecifier::addFuncModifier ( FuncModifier newFm )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::addFuncModifier("  << (void *)this
	         << "," << newFm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newFm == FUNC_INLINE)
    {
	if (declInline)
	    warning("superfluous 'inline' ignored"); 
	declInline = TRUE;
    }
    else
    {
	if (declVirtual)
	    warning("superfluous 'virtual' ignored"); 
	declVirtual = TRUE;
    }
    return TRUE;
}

Boolean DeclSpecifier::addStClass ( StorageSpecifier newSt )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::addStClass("  << (void *)this << "," 
		<< newSt << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (declStClass ==  SC_DEFAULT)
    {
	declStClass = newSt;
    } else
        if (declStClass == SC_TYPEDEF)
            error("cannot have storage class in typedef");
        else
            error("storage class already specified in this declaration");
    return TRUE;
}

Boolean DeclSpecifier::addStubModifier ( StubModifier newSm )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::addStubModifier("  << (void *)this << "," 
	         << newSm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (newSm)
    {
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_NOMARSHALL:
	if ((declMarshall == STUB_PENDING) || (declMarshall == newSm))
	{
	    declMarshall = newSm;
	    return TRUE;
	}
	break;
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
	if ((declStubType == STUB_PENDING) || (declStubType == newSm))
	{
	    declStubType = newSm;
	    return TRUE;
	}
	break;
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	if ((declTransfer == STUB_PENDING) ||
	    (declTransfer == newSm))
	{
	    declTransfer = newSm;
	    return TRUE;
	}
	break;
    case STUB_NOWARNINGS:
	warningsSupressed = TRUE;
	return TRUE;
    case STUB_DELETE:
    case STUB_NODELETE:
	if ((declOptions == STUB_PENDING) ||
	    (declOptions == newSm))
	{
	    declOptions = newSm;
	    return TRUE;
	}
	break;
    case STUB_PENDING:
	return TRUE;
    default:
	break;
    }
    
    error("conflicting or illegal stub-specific modifiers in this declaration");
    return FALSE;
}

Boolean DeclSpecifier::addTypeModifier ( TypeModifier tm )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::addTypeModifier("  << (void *)this << "," 
		<< tm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (declBtype)
    {
	declBtype->addTypeModifier(tm);
    }
    else
	declBtype = new ModifiedType(tm);

    return TRUE;
}
/*
 * Adjust type associated with a decl spec. If there is none already this
 * is easy. If one exists assume that the incoming one represents earlier
 * gathered info to be merged into whatever we have currently
 */

Boolean DeclSpecifier::adjustBasicType ( TypeDescriptor * origTd )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::adjustBasicType("  << (void *)this << "," 
	         << (void *)origTd << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (declBtype)
    {
	TypeDescriptor *oldBtype = declBtype;

	if ((declBtype = origTd->combineTypes(declBtype)) != oldBtype)
	    delete oldBtype;
	else
	    delete origTd;
    }
    else
	declBtype = origTd;

    return TRUE;
}

/*
 * Complete this declaration specifier. Used when completing a declaration
 * to form the basic decl specs that will be attached to each individual
 * declarator.
 * Creates CvTypes if needed.
 * May be called multiple times (without problems)
 */

void DeclSpecifier::finalise ( )
{
#ifdef SG_DEBUG
    debugStream << "DeclSpecifier::finalise("  << (void *)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    /* correct for const or volatile */
    
    if (declConst || declVolatile)
    {
	CvType *cvt = new CvType(declBtype);
	
	if (declConst)
	    cvt->addCvQualifier(QUAL_CONST);
	if (declVolatile)
	    cvt->addCvQualifier(QUAL_VOLATILE);
	
	declBtype = cvt;
    }
	
    declConst = declVolatile = FALSE;
}

DeclSpecifier *DeclSpecifier::mergeSpecInto ( DeclSpecifier *updatedSpec ) const
{
#ifdef SG_DEBUG
    /* 
     * The void * const cast in the next statement should really be to
     * a const void * const but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debugStream << "DeclSpecifier::mergeSpecInto("  << (void * const)this << "," 
		<< (void *)updatedSpec << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    /* start by merging in basic attributes */

    mergeBasicAttributes(updatedSpec);

    /* Then type info if it exists */

    if (declBtype)
	updatedSpec->adjustBasicType(declBtype);

    return updatedSpec;
}

void DeclSpecifier::propagateAttributes ( Declaration *decl ) const
{
    if (decl)
    {
	TypeDescriptor *td = decl->getType();

	if (td)
	    propagateAttributes(td);
	else
	    error("(stub internal) declaration has no type");
	    
	if (declIsElaborated)
	    decl->makeElaborated();

	if (declStClass != SC_DEFAULT)
	    decl->addStClass(declStClass);

	if (warningsSupressed)
	    decl->addStubModifier(STUB_NOWARNINGS);
	
	if (declMarshall != STUB_PENDING)
	    decl->addStubModifier(declMarshall);

	if (declStubType != STUB_PENDING)
	    decl->addStubModifier(declStubType);
	
	if (declTransfer != STUB_PENDING)
	    decl->addStubModifier(declTransfer);
    }
}

void DeclSpecifier::propagateAttributes ( TypeDescriptor *td ) const
{
    if (declInline)
	td->addFuncModifier(FUNC_INLINE);
    
    if (declVirtual)
	td->addFuncModifier(FUNC_VIRTUAL);
	
    if (declOptions != STUB_PENDING)
	td->addStubModifier(declOptions);

}

/*
 * Merge basic attributes of decl spec into argument. Only handles
 * storage class etc. Type information merged elsewhere.
 */

void DeclSpecifier::mergeBasicAttributes ( DeclSpecifier *updatedSpec ) const
{
#ifdef SG_DEBUG
    /* 
     * The void * const cast in the next statement should really be to
     * a const void * const but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debugStream << "DeclSpecifier::mergeBasicAttributes("  << (void * const)this << "," 
	         << (void *)updatedSpec << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (declConst)
	updatedSpec->addCvQualifier(QUAL_CONST);

    if (declVolatile)
	updatedSpec->addCvQualifier(QUAL_VOLATILE);
    
    if (declInline)
	updatedSpec->addFuncModifier(FUNC_INLINE);

    if (declIsElaborated)
	updatedSpec->makeElaborated();

    if (warningsSupressed)
	updatedSpec->addStubModifier(STUB_NOWARNINGS);

    if (declMarshall != STUB_PENDING)
	updatedSpec->addStubModifier(declMarshall);

    if (declStubType != STUB_PENDING)
	updatedSpec->addStubModifier(declStubType);

    if (declTransfer != STUB_PENDING)
	updatedSpec->addStubModifier(declTransfer);

    if (declOptions != STUB_PENDING)
	updatedSpec->addStubModifier(declOptions);

    if (declStClass != SC_DEFAULT)
	updatedSpec->addStClass(declStClass);

    if (declVirtual)
	updatedSpec->addFuncModifier(FUNC_VIRTUAL);
}

#ifdef NO_INLINES
#  define DECLSPEC_CC_
#  include "DeclSpec.n"
#  undef DECLSPEC_CC_
#endif
