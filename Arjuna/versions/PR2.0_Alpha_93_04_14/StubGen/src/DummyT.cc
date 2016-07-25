/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyT.cc,v 1.8 1993/03/22 09:30:59 ngdp Exp $
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

static const char RCSid[] = "$Id: DummyT.cc,v 1.8 1993/03/22 09:30:59 ngdp Exp $";

/*
 * PUBLIC operations:
 */

DummyType::DummyType ( ) 
                     : TypeDescriptor(NONE_TYPE)
{
#ifdef DEBUG
    debug_stream << "DummyType::DummyType()\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

DummyType::~DummyType ()
{
#ifdef DEBUG
    debug_stream << "DummyType::~DummyType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * This should never be called in practice. Force a fatal error if it
 * ever is.
 */

TypeDescriptor *DummyType::combine_types ( TypeDescriptor * )
{
    fatal("(Internal stub error) combine_types called for DummyType");
    return this;
}

Boolean DummyType::is_transmissible () const
{
    return FALSE;
}

/* Do declarations involving this type need trailing separator */

Boolean DummyType::needs_sep () const
{
    return FALSE;
}

void DummyType::postname ( ostream&, DeclStyle )
{
}

void DummyType::prename ( ostream&, DeclStyle )
{
}

void DummyType::print_bt ( ostream&, DeclStyle, Boolean, Boolean )
{
}

