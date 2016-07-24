/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyT.cc,v 1.1 1993/11/03 14:47:34 nmcl Exp $
 */

/*
 * Stub generator dummy type handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef DUMMYT_H_
#  include "DummyT.h"
#endif

static const char RCSid[] = "$Id: DummyT.cc,v 1.1 1993/11/03 14:47:34 nmcl Exp $";

/*
 * PUBLIC operations:
 */

DummyType::DummyType ( ) 
                     : TypeDescriptor(NONE_TYPE)
{
#ifdef DEBUG
    debugStream << "DummyType::DummyType()\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

DummyType::~DummyType ()
{
#ifdef DEBUG
    debugStream << "DummyType::~DummyType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * This should never be called in practice. Force a fatal error if it
 * ever is.
 */

TypeDescriptor *DummyType::combineTypes ( TypeDescriptor * )
{
    fatal("(Internal stub error) combineTypes called for DummyType");
    return this;
}

/* Do declarations involving this type need trailing separator */

Boolean DummyType::needsSep () const
{
    return FALSE;
}

String DummyType::postName ( DeclStyle ) const
{
    return NullString;
}

String DummyType::preName ( DeclStyle, Boolean, Boolean ) const
{
    return NullString;
}

String DummyType::signature () const
{
    return NullString;
}

Boolean DummyType::isTransferable () const
{
    return FALSE;
}

