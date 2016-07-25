/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CharT.cc,v 1.8 1993/04/14 13:46:00 ngdp Exp $
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

static const char RCSid[] = "$Id: CharT.cc,v 1.8 1993/04/14 13:46:00 ngdp Exp $";

/*
 * PUBLIC operations
 */

CharType::CharType ()
{
#ifdef DEBUG
    debug_stream << "CharType::CharType("  << (void *)this << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

CharType::~CharType ()
{
#ifdef DEBUG
    debug_stream << "CharType::~CharType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Adjust the modifier bits for the type. Fairly restricted for chars.
 */

void CharType::add_type_modifier ( TypeModifier newmod )
{
#ifdef DEBUG
    debug_stream << "CharType::add_type_modifier ("  << (void *)this << ","
	         << newmod << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newmod == MOD_UNSIGNED || newmod == MOD_SIGNED)
	IntegralType::add_type_modifier(newmod);
    else
	error("illegal type modifier in 'char' declaration");
}

void CharType::print_bt ( ostream& s, DeclStyle, Boolean, Boolean ) 
{
    IntegralType::print_bt(s, NORMAL_DECL, FALSE, FALSE);
    s << "char ";
}

String CharType::signature () const
{
    return IntegralType::signature() + "c";
}

String CharType::typename ( DeclStyle ds ) const
{
    return (IntegralType::typename(ds) + "char ");
}


