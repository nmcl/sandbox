/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyArgT.cc,v 1.1 1993/11/03 14:47:33 nmcl Exp $
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

#ifndef DUMMYARGT_H_
#  include "DummyArgT.h"
#endif

static const char RCSid[] = "$Id: DummyArgT.cc,v 1.1 1993/11/03 14:47:33 nmcl Exp $";

/*
 * PUBLIC operations:
 */

DummyArgType::DummyArgType ( ) 
                     : DummyType()
{
#ifdef DEBUG
    debugStream << "DummyArgType::DummyArgType()\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

DummyArgType::~DummyArgType ()
{
#ifdef DEBUG
    debugStream << "DummyArgType::~DummyArgType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

String DummyArgType::signature () const
{
    return "e";
}

