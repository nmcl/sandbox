/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyT.cc,v 1.5.6.1 1996/10/10 12:26:57 ngdp Exp $
 */

/*
 * Stub generator dummy type handler
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

#ifndef DUMMYT_H_
#  include "DummyT.h"
#endif

static const char RCSid[] = "$Id: DummyT.cc,v 1.5.6.1 1996/10/10 12:26:57 ngdp Exp $";

/*
 * PUBLIC operations:
 */

DummyType::DummyType ( ) 
                     : TypeDescriptor(NONE_TYPE)
{
#ifdef SG_DEBUG
    debugStream << "DummyType::DummyType()\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

DummyType::~DummyType ()
{
#ifdef SG_DEBUG
    debugStream << "DummyType::~DummyType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Allow all stub mods regardless
 */

Boolean DummyType::addStubModifier ( StubModifier )
{
    return TRUE;
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

String DummyType::massageName ( const String&,
				DeclStyle ) const
{
    return NullString;
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

