/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntT.cc,v 1.8 1993/04/14 13:46:15 ngdp Exp $
 */

/*
 * Stub generator integer type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

static const char RCSid[] = "$Id: IntT.cc,v 1.8 1993/04/14 13:46:15 ngdp Exp $";

/*
 * PUBLIC operations
 */

IntType::IntType ()
{
#ifdef DEBUG
    debug_stream << "IntType::IntType("  << (void *)this << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

IntType::~IntType ()
{
#ifdef DEBUG
    debug_stream << "IntType::~IntType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

String IntType::signature () const
{
    return IntegralType::signature() + "i";
}

String IntType::typename ( DeclStyle ds ) const
{
    return (IntegralType::typename(ds) + "int ");
}

void IntType::print_bt ( ostream& s, DeclStyle, Boolean, Boolean )
{
    IntegralType::print_bt(s, NORMAL_DECL, FALSE, FALSE);
    s << "int ";
}
