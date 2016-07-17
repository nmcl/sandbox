/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FloatT.cc,v 1.8 1993/04/14 13:46:09 ngdp Exp $
 */

/*
 * Stub generator float type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef FLOATT_H_
#  include "FloatT.h"
#endif

static const char RCSid[] = "$Id: FloatT.cc,v 1.8 1993/04/14 13:46:09 ngdp Exp $";
/*
 * PUBLIC operations
 */

FloatType::FloatType ( Boolean double_type ) 
                     : ModifiedType(FLOATING_TYPE),
		       is_double(double_type)
{
#ifdef DEBUG
    debug_stream << "FloatType::FloatType(" << (void *)this
	         << "," << double_type << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

FloatType::~FloatType ()
{
#ifdef DEBUG
    debug_stream << "FloatType::~FloatType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Adjust the modifier bits for the type. Eg long or short etc. Mostly
 * meaningless for floats.
 */

void FloatType::add_type_modifier ( TypeModifier newmod )
{
#ifdef DEBUG
    debug_stream << "FloatType::add_type_modifier ("  << (void *)this << ","
	         << newmod << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);

#endif

    if (newmod == MOD_LONG)
	ModifiedType::add_type_modifier(newmod);
    else
	error("illegal type modifier in 'float' declaration");
}

void FloatType::print_bt ( ostream& s, DeclStyle, Boolean, Boolean )
{
    ModifiedType::print_bt(s, NORMAL_DECL, FALSE, FALSE);

    if (is_double)
	s << "double ";
    else
	s << "float ";
}

String FloatType::signature () const
{
    if (is_double)
	return ModifiedType::signature() + "d";
    else
	return ModifiedType::signature() + "f";
}

String FloatType::typename ( DeclStyle ds ) const
{
    if (is_double)
	return ModifiedType::typename(ds) + "double ";
    else
	return ModifiedType::typename(ds) + "float ";
}


