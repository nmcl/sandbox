/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntT.cc,v 1.1 1993/11/03 14:47:56 nmcl Exp $
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

static const char RCSid[] = "$Id: IntT.cc,v 1.1 1993/11/03 14:47:56 nmcl Exp $";

/*
 * PUBLIC operations
 */

IntType::IntType ()
{
#ifdef DEBUG
    debugStream << "IntType::IntType("  << (void *)this << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

IntType::~IntType ()
{
#ifdef DEBUG
    debugStream << "IntType::~IntType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

String IntType::signature () const
{
    return IntegralType::signature() + "i";
}

String IntType::preName ( DeclStyle ds, Boolean elab,
			  Boolean noConst ) const
{
    String root = IntegralType::preName(ds,elab, noConst);
    
    if (root != NullString)
	root += " ";
    
    root += "int";
    
    return root;
}
