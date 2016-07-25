/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntegralT.cc,v 1.7 1993/06/17 15:09:37 ngdp Exp $
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

static const char RCSid[] = "$Id: IntegralT.cc,v 1.7 1993/06/17 15:09:37 ngdp Exp $";

/*
 * PUBLIC operations
 */

IntegralType::IntegralType ()
{
#ifdef DEBUG
    debugStream << "IntegralType::IntegralType("  << (void *)this << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

IntegralType::~IntegralType ()
{
#ifdef DEBUG
    debugStream << "IntegralType::~IntegralType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *IntegralType::combineTypes ( TypeDescriptor *newT )
{
    if (kind() == newT->kind())
	return ModifiedType::combineTypes(newT);
    else
    {
	errorStream << "illegal type specification : " << typeName(NORMAL_DECL);
	errorStream << newT->typeName(NORMAL_DECL);
	error(errorStream);

	return this;
    }
}
