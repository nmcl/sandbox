/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ModifiedT.cc,v 1.11 1993/04/14 13:46:18 ngdp Exp $
 */

/*
 * Stub generator modified type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

static const char RCSid[] = "$Id: ModifiedT.cc,v 1.11 1993/04/14 13:46:18 ngdp Exp $";

/*
 * PUBLIC operations
 */

ModifiedType::ModifiedType ( TypeModifier tm )
                           : TypeDescriptor(INTEGRAL_TYPE),
			     is_long(FALSE),
			     is_short(FALSE),
			     is_signed(FALSE),
			     is_unsigned(FALSE)
{
#ifdef DEBUG
    debug_stream << "ModifiedType::ModifiedType("  << (void *)this << ","
	         << tm << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    add_type_modifier(tm);
}

ModifiedType::ModifiedType ( Type t )
                           : TypeDescriptor(t),
			     is_long(FALSE),
			     is_short(FALSE),
			     is_signed(FALSE),
			     is_unsigned(FALSE)
{
#ifdef DEBUG
    debug_stream << "ModifiedType::ModifiedType(" << (void *)this << "," 
	         << t <<")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

ModifiedType::~ModifiedType ()
{
#ifdef DEBUG
    debug_stream << "ModifiedType::~ModifiedType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void ModifiedType::add_type_modifier ( TypeModifier newmod )
{
#ifdef DEBUG
    debug_stream << "ModifiedType::add_type_modifier("  << (void *)this << ","
	         << newmod << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (newmod)
    {
    case MOD_SHORT:
    case MOD_LONG:
	if ((is_long && (newmod == MOD_SHORT)) ||
	    (is_short && (newmod == MOD_LONG)))
	{
	    error("cannot have both 'long' and 'short' in a type declaration");
	} else
	    if (newmod == MOD_SHORT)
	    {
		if (is_short)
		    warning("superfluous 'short' ignored");
		else
		    is_short = TRUE;
	    }
	    else
	    {
		if (is_long)
		    warning("superfluous 'long' ignored");
		else
		    is_long = TRUE;
	    }
	break;
    case MOD_SIGNED:
    case MOD_UNSIGNED:
	if ((is_signed && (newmod == MOD_SIGNED)) ||
	    (is_unsigned && (newmod == MOD_UNSIGNED)))
	{
	    error("cannot have both 'signed' and 'unsigned' in a type declaration");
	} else
	    if (newmod == MOD_UNSIGNED)
	    {
		if (is_unsigned)
		    warning("superfluous 'unsigned' ignored");
		else
		    is_unsigned = TRUE;
	    }
	    else
	    {
		if (is_signed)
		    warning("superfluous 'signed' ignored");
		else
		    is_signed = TRUE;
	    }
	break;
    default:
	error("illegal type modifier in declaration");
    }
}

void ModifiedType::add_stub_modifier ( StubModifier sm )
{
#ifdef DEBUG
    debug_stream << "ModifiedType::add_stub_modifier("  << (void *)this
                 << "," << sm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (sm == STUB_ARG_IN)
	return;

    TypeDescriptor::add_stub_modifier(sm);

}

/*
 * Type adjustment. Merge type details described by 'this' into the
 * type described by the parameter. Return new merged type as result
 * if no errors, otherwise return 'this'.
 * 'This' not modified.
 * 
 */

TypeDescriptor *ModifiedType::combine_types ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debug_stream << "ModifiedType::combine_types ("  << (void *)this
	         << "," << (void *)newtype << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newtype == 0)
    {
	fatal("(internal stub error) nil pointer passed to ModifiedType::combine_types");
	return this;
    }
    
    if (is_short)
	newtype->add_type_modifier(MOD_SHORT);
    if (is_long)
	newtype->add_type_modifier(MOD_LONG);
    if (is_signed)
	newtype->add_type_modifier(MOD_SIGNED);
    if (is_unsigned)
	newtype->add_type_modifier(MOD_UNSIGNED);

    return newtype;
}

Boolean ModifiedType::is_transmissible () const
{
    return TRUE;
}

/* Print default init for this type (ie = 0) */

void ModifiedType::printDefaultInit ( ostream& s ) const
{
    s << " = ";
    
    if (is_unsigned)
	s << "0";
    else
	s << "-1";
}

void ModifiedType::print_marshalling_code ( ostream& s,
					 const String& varname, 
					 const String& buffname) 
{
    s << "\t" << buffname << " << " ;
    s << varname << ";\n";
}

void ModifiedType::print_unmarshalling_code ( ostream& s,
					   const String& varname, 
					   const String& buffname) 
{
    s << "\t" << buffname << " >> " ;
    s << varname << ";\n";
}

String ModifiedType::signature () const
{
    String sig;

    if (is_signed)
	sig += "S";
    if (is_unsigned)
	sig += "U";
    if (is_short)
	sig += "s";
    if (is_long)
	sig += "l";
    return sig;    
}

String ModifiedType::typename ( DeclStyle ) const
{
    String root;

    if (is_signed)
	root += "signed ";
    if (is_unsigned)
	root += "unsigned ";
    if (is_short)
	root += "short ";
    if (is_long)
	root += "long ";
    return root;
}

void ModifiedType::postname ( ostream&, DeclStyle ) 
{
}

void ModifiedType::prename ( ostream&, DeclStyle )
{
}

void ModifiedType::print_bt ( ostream& s, DeclStyle, Boolean, Boolean ) 
{
    if (is_signed)
	s << "signed ";
    if (is_unsigned)
	s << "unsigned ";
    if (is_short)
	s << "short ";
    if (is_long)
	s << "long ";
}


