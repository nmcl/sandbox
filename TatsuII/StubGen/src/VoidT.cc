/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VoidT.cc,v 1.1 1997/06/09 19:52:25 nmcl Exp $
 */

/*
 * Stub generator char type definition handler
 *
 */

#include <strstream>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif


#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef VOIDT_H_
#  include "VoidT.h"
#endif

static const char RCSid[] = "$Id: VoidT.cc,v 1.1 1997/06/09 19:52:25 nmcl Exp $";

/*
 * PUBLIC operations
 */

VoidType::VoidType () : TypeDescriptor(VOID_TYPE)
{
#ifdef SG_DEBUG
    debugStream << "VoidType::VoidType(" << (void *)this << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

VoidType::~VoidType ()
{
#ifdef SG_DEBUG
    debugStream << "VoidType::~VoidType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *VoidType::combineTypes ( TypeDescriptor *newT )
{
#ifdef SG_DEBUG
    debugStream << "VoidType::combineTypes ("  << (void *)this << ","
	         << (void *)newT << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    error("illegal type adjustment in 'void' declaration");
    return this;
}

Boolean VoidType::canDiscard () const
{
    return TRUE;
}

String VoidType::postName (DeclStyle ) const
{
    return NullString;
}

String VoidType::preName (DeclStyle, Boolean, Boolean ) const
{
    return "void";
}

String VoidType::signature () const
{
    return "v";
}

Boolean VoidType::isTransferable () const
{
    return FALSE;
}
