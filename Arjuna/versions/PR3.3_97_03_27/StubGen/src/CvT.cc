/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvT.cc,v 1.10 1996/04/16 12:13:32 nmcl Exp $
 */

/*
 * Stub generator const/volatile type definition handler
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

#ifndef CVT_H_
#  include "CvT.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

static const char RCSid[] = "$Id: CvT.cc,v 1.10 1996/04/16 12:13:32 nmcl Exp $";

/*
 * PUBLIC operations
 */

CvType::CvType ( CvQualifier cvt )
               : TypeDescriptor(CV_TYPE),
		 isConst(FALSE),
		 isVolatile(FALSE),
		 cvType(0)
{
#ifdef SG_DEBUG
    debugStream << "CvType::CvType(" << (void *)this << ","
		<< cvt << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    addCvQualifier(cvt);
}

CvType::CvType ( TypeDescriptor *cvt )
               : TypeDescriptor(CV_TYPE),
		 isConst(FALSE),
		 isVolatile(FALSE),
		 cvType(cvt)
{
#ifdef SG_DEBUG
    debugStream << "CvType::CvType(" << (void *)this << ","
		<< (void*)cvt << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

CvType::CvType ()
               : TypeDescriptor(CV_TYPE),
		 isConst(FALSE),
		 isVolatile(FALSE),
		 cvType(0)
{
#ifdef SG_DEBUG
    debugStream << "CvType::CvType\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

CvType::~CvType ()
{
#ifdef SG_DEBUG
    debugStream << "CvType::~CvType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean CvType::addCvQualifier ( CvQualifier newTq )
{
#ifdef SG_DEBUG
    debugStream << "CvType::addCvQualifier (" << (void *)this << ","
		<< newTq << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    if (newTq == QUAL_CONST)
    {
        if (isConst)
            warning("superfluous 'const' ignored");
	else if (isVolatile)
	    error("both 'const' and 'volatile' specified in this declaration");
        isConst = TRUE;
    }
    else
    {
        if (isVolatile)
            warning("superfluous 'volatile' ignored");
	else if (isConst)
	    error("both 'const' and 'volatile' specified in this declaration");
        isVolatile = TRUE;
    }
    
    return TRUE;
}

Boolean CvType::addStubModifier ( StubModifier sm )
{
    if ((sm == STUB_ARG_OUT) || (sm == STUB_ARG_INOUT))
    {
	ostrstream errorStream;

	errorStream << "stub command '" << sm 
		    << "' illegal in this declaration";
	error(errorStream);
    }

    return TRUE;
}

Boolean CvType::addTypeModifier ( TypeModifier newmod )
{
#ifdef SG_DEBUG
    debugStream << "CvType::addTypeModifier (" << (void *)this << "," 
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cvType == 0)
    {
	cvType = new ModifiedType();
    }

    return cvType->addTypeModifier(newmod);
}

/*
 * Merging  const & volatile types is different. Rather than merge
 * the const etc attributes into the indicated type, we merely point
 * to it.
 */

TypeDescriptor *CvType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef SG_DEBUG
    debugStream << "CvType::combineTypes ("  << (void *)this << ","
		<< (void *)newtype << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cvType == 0)
    {
	cvType = newtype;
    }
    else
	cvType = cvType->combineTypes(newtype);

    return this;
}

TypeDescriptor *CvType::deref ()
{
    if (cvType)
	return cvType;
    else
	return this;
}

String CvType::defaultInit ( ) const
{
    if (cvType)
	return cvType->defaultInit();
    else
	return NullString;
}

Boolean CvType::deleteable ( ) const
{
    if (cvType != 0)
	return cvType->deleteable();
    else
	return TypeDescriptor::deleteable();
}

String CvType::elaborator ( DeclStyle ds ) const
{
    if (cvType)
	return cvType->elaborator(ds);
    else
	return NullString;
}

Boolean CvType::freezeType ()
{
    if (cvType == 0)
	cvType = new IntType();

    return cvType->freezeType();
}

Boolean CvType::isConstant () const
{
    return isConst;
}

Boolean CvType::isFundamental () const
{
    if (cvType)
	return cvType->isFundamental();
    else
	return TRUE;
}

String CvType::postName ( DeclStyle ds ) const
{
    if (cvType != 0)
	return cvType->postName(ds);
    else
	return NullString;
}

String CvType::preName ( DeclStyle ds, Boolean elab, Boolean noConst ) const
{    
    String s;

    if (isConst && !noConst)
	s += "const ";

    if (isVolatile)
	s += "volatile ";

    if (cvType)
	s += cvType->preName(ds,elab,noConst);

    return s;
}

String CvType::signature () const
{
    String sig;

    if (isConst)
	sig += "C";
    if (isVolatile)
	sig += "V";

    if (cvType)
	sig += cvType->signature();

    return sig;    
}

StubModifier CvType::defaultTransfer () const
{
    if (isConst)
	return STUB_ARG_IN;
    else
	if (cvType)
	    return cvType->defaultTransfer();
	else
	    return STUB_ARG_IN;
}

Boolean CvType::isTransferable () const
{
    if (cvType)
	return cvType->isTransferable();
    else
	return FALSE;
}

