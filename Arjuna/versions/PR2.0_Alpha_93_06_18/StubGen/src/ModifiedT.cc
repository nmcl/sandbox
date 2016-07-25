/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ModifiedT.cc,v 1.12 1993/06/17 15:09:46 ngdp Exp $
 */

/*
 * Stub generator modified type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

static const char RCSid[] = "$Id: ModifiedT.cc,v 1.12 1993/06/17 15:09:46 ngdp Exp $";

/*
 * PUBLIC operations
 */

ModifiedType::ModifiedType ( TypeModifier tm )
                           : TypeDescriptor(INTEGRAL_TYPE),
			     isLong(FALSE),
			     isShort(FALSE),
			     isSigned(FALSE),
			     isUnsigned(FALSE)
{
#ifdef DEBUG
    debugStream << "ModifiedType::ModifiedType("  << (void *)this << ","
		<< tm << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    addTypeModifier(tm);
}

ModifiedType::ModifiedType ( Type t )
                           : TypeDescriptor(t),
			     isLong(FALSE),
			     isShort(FALSE),
			     isSigned(FALSE),
			     isUnsigned(FALSE)
{
#ifdef DEBUG
    debugStream << "ModifiedType::ModifiedType(" << (void *)this << "," 
		<< t <<")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

ModifiedType::~ModifiedType ()
{
#ifdef DEBUG
    debugStream << "ModifiedType::~ModifiedType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean ModifiedType::addStubModifier ( StubModifier sm )
{
#ifdef DEBUG
    debugStream << "ModifiedType::addStubModifier("  << (void *)this
		<< "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (sm == STUB_ARG_IN)
	return TRUE;

    return TypeDescriptor::addStubModifier(sm);

}

Boolean ModifiedType::addTypeModifier ( TypeModifier newmod )
{
#ifdef DEBUG
    debugStream << "ModifiedType::addTypeModifier("  << (void *)this << ","
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (newmod)
    {
    case MOD_SHORT:
    case MOD_LONG:
	if ((isLong && (newmod == MOD_SHORT)) ||
	    (isShort && (newmod == MOD_LONG)))
	{
	    error("cannot have both 'long' and 'short' in a type declaration");
	} else
	    if (newmod == MOD_SHORT)
	    {
		if (isShort)
		    warning("superfluous 'short' ignored");
		else
		    isShort = TRUE;
	    }
	    else
	    {
		if (isLong)
		    warning("superfluous 'long' ignored");
		else
		    isLong = TRUE;
	    }
	break;
    case MOD_SIGNED:
    case MOD_UNSIGNED:
	if ((isSigned && (newmod == MOD_SIGNED)) ||
	    (isUnsigned && (newmod == MOD_UNSIGNED)))
	{
	    error("cannot have both 'signed' and 'unsigned' in a type declaration");
	} else
	    if (newmod == MOD_UNSIGNED)
	    {
		if (isUnsigned)
		    warning("superfluous 'unsigned' ignored");
		else
		    isUnsigned = TRUE;
	    }
	    else
	    {
		if (isSigned)
		    warning("superfluous 'signed' ignored");
		else
		    isSigned = TRUE;
	    }
	break;
    default:
	error("illegal type modifier in declaration");
    }

    return TRUE;
}

/*
 * Type adjustment. Merge type details described by 'this' into the
 * type described by the parameter. Return new merged type as result
 * if no errors, otherwise return 'this'.
 * 'This' not modified.
 * 
 */

TypeDescriptor *ModifiedType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debugStream << "ModifiedType::combineTypes ("  << (void *)this
		<< "," << (void *)newtype << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newtype == 0)
    {
	fatal("(internal stub error) nil pointer passed to ModifiedType::combineTypes");
	return this;
    }
    
    if (isShort)
	newtype->addTypeModifier(MOD_SHORT);
    if (isLong)
	newtype->addTypeModifier(MOD_LONG);
    if (isSigned)
	newtype->addTypeModifier(MOD_SIGNED);
    if (isUnsigned)
	newtype->addTypeModifier(MOD_UNSIGNED);

    return newtype;
}

/* Print default init for this type (ie = 0) */

String ModifiedType::defaultInit ( ) const
{
    String s = " = ";
    
    if (isUnsigned)
	s += "0";
    else
	s += "-1";

    return s;
}

String ModifiedType::postName ( DeclStyle ) const
{
    return NullString;
}

String ModifiedType::preName ( DeclStyle, Boolean, Boolean ) const
{   
    String tName;

    if (isSigned)
	tName += "signed";
    else
	if (isUnsigned)
	    tName += "unsigned";

    if (tName != NullString)
	tName += " ";
    
    if (isShort)
	tName += "short";
    else
	if (isLong)
	    tName += "long";

 
    return tName;
}

String ModifiedType::signature () const
{
    String sig;

    if (isSigned)
	sig += "S";
    if (isUnsigned)
	sig += "U";
    if (isShort)
	sig += "s";
    if (isLong)
	sig += "l";
    return sig;    
}

StubModifier ModifiedType::defaultMarshall () const
{
    return STUB_AUTOMARSHALL;
}

StubModifier ModifiedType::defaultTransfer () const
{
    return STUB_ARG_IN;
}

Boolean ModifiedType::isTransferable () const
{
    return TRUE;
}
