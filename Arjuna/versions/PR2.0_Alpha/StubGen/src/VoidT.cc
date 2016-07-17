/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VoidT.cc,v 1.8 1993/04/14 13:46:25 ngdp Exp $
 */

/*
 * Stub generator char type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif


#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef VOIDT_H_
#  include "VoidT.h"
#endif

static const char RCSid[] = "$Id: VoidT.cc,v 1.8 1993/04/14 13:46:25 ngdp Exp $";

/*
 * PUBLIC operations
 */

VoidType::VoidType () : TypeDescriptor(VOID_TYPE)
{
#ifdef DEBUG
    debug_stream << "VoidType::VoidType(" << (void *)this << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

VoidType::~VoidType ()
{
#ifdef DEBUG
    debug_stream << "VoidType::~VoidType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *VoidType::combine_types ( TypeDescriptor *new_t )
{
#ifdef DEBUG
    debug_stream << "VoidType::combine_types ("  << (void *)this << ","
	         << (void *)new_t << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    error("illegal type adjustment in 'void' declaration");
    return this;
}

Boolean VoidType::is_transmissible () const
{
    return FALSE;
}

void VoidType::postname (ostream&, DeclStyle )
{
}

void VoidType::prename (ostream&, DeclStyle )
{
}

void VoidType::print_bt ( ostream& s, DeclStyle, Boolean, Boolean ) 
{
    s << "void ";
}

String VoidType::signature () const
{
    return "v";
}

String VoidType::typename ( DeclStyle ) const
{
    return "void ";
}


