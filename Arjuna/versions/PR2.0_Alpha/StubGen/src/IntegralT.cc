/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntegralT.cc,v 1.6 1993/04/14 13:46:17 ngdp Exp $
 */

/*
 * Stub generator integral type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef INTEGRALT_H_
#  include "IntegralT.h"
#endif

static const char RCSid[] = "$Id: IntegralT.cc,v 1.6 1993/04/14 13:46:17 ngdp Exp $";

/*
 * PUBLIC operations
 */

IntegralType::IntegralType ()
{
#ifdef DEBUG
    debug_stream << "IntegralType::IntegralType("  << (void *)this << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

IntegralType::~IntegralType ()
{
#ifdef DEBUG
    debug_stream << "IntegralType::~IntegralType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *IntegralType::combine_types ( TypeDescriptor *new_t )
{
    if (kind() == new_t->kind())
	return ModifiedType::combine_types(new_t);
    else
    {
	error_stream << "illegal type specification : " << typename(NORMAL_DECL);
	error_stream << new_t->typename(NORMAL_DECL);
	error(error_stream);

	return this;
    }
}
