/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CharT.cc,v 1.9 1993/06/17 15:08:52 ngdp Exp $
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

#ifndef CHART_H_
#  include "CharT.h"
#endif

static const char RCSid[] = "$Id: CharT.cc,v 1.9 1993/06/17 15:08:52 ngdp Exp $";

/*
 * PUBLIC operations
 */

CharType::CharType ()
{
#ifdef DEBUG
    debugStream << "CharType::CharType("  << (void *)this << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

CharType::~CharType ()
{
#ifdef DEBUG
    debugStream << "CharType::~CharType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Adjust the modifier bits for the type. Fairly restricted for chars.
 */

Boolean CharType::addTypeModifier ( TypeModifier newmod )
{
#ifdef DEBUG
    debugStream << "CharType::addTypeModifier ("  << (void *)this << ","
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newmod == MOD_UNSIGNED || newmod == MOD_SIGNED)
	IntegralType::addTypeModifier(newmod);
    else
	error("illegal type modifier in 'char' declaration");

    return TRUE;
}

String CharType::signature () const
{
    return IntegralType::signature() + "c";
}

String CharType::preName ( DeclStyle ds, Boolean elab, 
			   Boolean noConst ) const
{    
    String root = IntegralType::preName(ds,elab, noConst);
    
    if (root != NullString)
	root += " ";
    
    root += "char";
    
    return root;
}


