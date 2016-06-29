/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ModifiedT.cc,v 1.1 1997/06/09 19:52:23 nmcl Exp $
 */

/*
 * Stub generator modified type definition handler
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

static const char RCSid[] = "$Id: ModifiedT.cc,v 1.1 1997/06/09 19:52:23 nmcl Exp $";

/*
 * PUBLIC operations
 */

ModifiedType::ModifiedType ( TypeModifier tm )
                           : TypeDescriptor(MODIFIED_TYPE),
			     isShort(FALSE),
			     isSigned(FALSE),
			     isUnsigned(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "ModifiedType::ModifiedType("  << (void *)this << ","
		<< tm << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    isLong[0] = isLong[1] = FALSE;
    
    addTypeModifier(tm);
}

ModifiedType::ModifiedType ( Type t )
                           : TypeDescriptor(t),
			     isShort(FALSE),
			     isSigned(FALSE),
			     isUnsigned(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "ModifiedType::ModifiedType(" << (void *)this << "," 
		<< t <<")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    isLong[0] = isLong[1] = FALSE;
}

ModifiedType::~ModifiedType ()
{
#ifdef SG_DEBUG
    debugStream << "ModifiedType::~ModifiedType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean ModifiedType::addStubModifier ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "ModifiedType::addStubModifier("  << (void *)this
		<< "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_ARG_IN:
    case STUB_NOREMOTE:
	return TRUE;
    default:
	break;
    }
    
    return TypeDescriptor::addStubModifier(sm);

}

Boolean ModifiedType::addTypeModifier ( TypeModifier newmod )
{
#ifdef SG_DEBUG
    debugStream << "ModifiedType::addTypeModifier("  << (void *)this << ","
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (newmod)
    {
    case MOD_SHORT:
    case MOD_LONG:
	if ((isLong[0] && (newmod == MOD_SHORT)) ||
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
		if (isLong[0])
		{
		    if (isLong[1])
			warning("superfluous 'long' ignored");
		    else
			isLong[1] = TRUE;
		}
		else
		    isLong[0] = TRUE;
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
#ifdef SG_DEBUG
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
    if (isLong[0])
	newtype->addTypeModifier(MOD_LONG);
    if (isLong[1])
	newtype->addTypeModifier(MOD_LONG);
    if (isSigned)
	newtype->addTypeModifier(MOD_SIGNED);
    if (isUnsigned)
	newtype->addTypeModifier(MOD_UNSIGNED);

    return newtype;
}

Boolean ModifiedType::canDiscard () const
{
    return TRUE;
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

/*
 * Allow several modifiers that are harmless
 */

Boolean ModifiedType::overrideStubModifiers ( StubModifier newSm )
{
#ifdef SG_DEBUG
    debugStream << "ModifiedType::overrideModifiers("  << (void *)this << ","
		<< newSm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    switch (newSm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOMARSHALL:
    case STUB_AUTOMARSHALL:
	return TRUE;
    default:
	return addStubModifier(newSm);
    }
    
}

String ModifiedType::postName ( DeclStyle ) const
{
    return NullString;
}

String ModifiedType::preName ( DeclStyle ds, Boolean, Boolean ) const
{
    if (ds == JAVA_DECL)
	return NullString;
    
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
	if (isLong[0])
	{
	    tName += "long";
	    if (isLong[1])
		tName += " long";
	}
 
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
    if (isLong[0])
    {
	if (isLong[1])
	    sig += "L";
	else
	    sig += "l";
    }
    
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
