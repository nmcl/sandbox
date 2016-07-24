/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BoolT.cc,v 1.4 1996/04/15 14:50:23 nmcl Exp $
 */

/*
 * Stub generator char type definition handler
 *
 */

#ifndef WIN32

#include <strstream.h>

#else

#include <strstrea.h>

#endif

#ifdef SG_DEBUG
#  include "Debug.h"
#endif


#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef BOOLT_H_
#  include "BoolT.h"
#endif

static const char RCSid[] = "$Id: BoolT.cc,v 1.4 1996/04/15 14:50:23 nmcl Exp $";

/*
 * PUBLIC operations
 */

BoolType::BoolType () : TypeDescriptor(INTEGRAL_TYPE)
{
#ifdef SG_DEBUG
    debugStream << "BoolType::BoolType(" << (void *)this << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

BoolType::~BoolType ()
{
#ifdef SG_DEBUG
    debugStream << "BoolType::~BoolType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean BoolType::addStubModifier ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "BoolType::addStubModifier("  << (void *)this
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

TypeDescriptor *BoolType::combineTypes ( TypeDescriptor *newT )
{
#ifdef SG_DEBUG
    debugStream << "BoolType::combineTypes ("  << (void *)this << ","
	         << (void *)newT << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    error("illegal type adjustment in 'bool' declaration");
    return this;
}

String BoolType::postName (DeclStyle ) const
{
    return NullString;
}

String BoolType::preName (DeclStyle, Boolean, Boolean ) const
{
    return "bool";
}

String BoolType::signature () const
{
    return "b";
}

StubModifier BoolType::defaultMarshall () const
{
    return STUB_AUTOMARSHALL;
}

StubModifier BoolType::defaultTransfer () const
{
    return STUB_ARG_IN;
}

Boolean BoolType::isTransferable () const
{
    return TRUE;
}
