/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerT.cc,v 1.15 1993/04/14 13:46:19 ngdp Exp $
 */

/*
 * Stub generator pointer type handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
#endif

static const char RCSid[] = "$Id: PointerT.cc,v 1.15 1993/04/14 13:46:19 ngdp Exp $";

/*
 * PUBLIC operations
 */

PointerType::PointerType ( Type type, TypeDescriptor *base, TypeDescriptor *quals ) 
                         : TypeDescriptor(type),
			   pointer_quals(quals),
			   pointer_to(base)
{
#ifdef DEBUG
    debug_stream << "PointerType::PointerType(" << (void *)this << "," << type << ","
	         << (void *)base << "," << (void *)quals << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

PointerType::~PointerType ()
{
#ifdef DEBUG
    debug_stream << "PointerType::~PointerType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void PointerType::attach_base_type ( TypeDescriptor *base_type )
{
#ifdef DEBUG
    debug_stream << "PointerType::attach_base_type(" << (void *)this << ", " << (void *)base_type << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (pointer_to)
	pointer_to->attach_base_type(base_type);
    else
	pointer_to = base_type;
}

void PointerType::add_stub_modifier ( StubModifier sm )
{
#ifdef DEBUG
    debug_stream << "PointerType::add_stub_modifier("  << (void *)this
                 << "," << sm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	if (pointer_to != 0)
	    pointer_to->add_stub_modifier(sm);
	return;
    default:
	break;
    }

    TypeDescriptor::add_stub_modifier(sm);
}

/*
 * Adjust type of pointer. Sets pointer_to field, returns old value.
 * Used in building declarator type
 */

TypeDescriptor *PointerType::combine_types ( TypeDescriptor *nt )
{
#ifdef DEBUG
    debug_stream << "PointerType::combine_types(" << (void *)this << ", " << (void *)nt << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (pointer_to)
	pointer_to->combine_types(nt);
    else
	pointer_to = nt;

    return this;
}

StubModifier PointerType::default_par_transfer () const
{
    if (pointer_to != 0)
    {
	if (pointer_to->is_constant())
	    return STUB_ARG_IN;
    }
    
    return STUB_ARG_INOUT;
}

TypeDescriptor *PointerType::deref () const
{
    return pointer_to;
}

Boolean PointerType::is_fundamental () const
{
    return FALSE;
}

/*
 * Determine the (un)marshallability of the type being pointed at.
 * Note that we do not allow pointer to pointer (hence the fundamental check)
 */

Boolean PointerType::is_transmissible () const
{
    if (pointer_to != 0)
    {
	if (pointer_to->is_fundamental())
	    return pointer_to->is_transmissible();

	/* Not pointing at fundamental type - may still be ok if I'm a ref */

	if (kind() == POINTER_TYPE)
	    return FALSE;
	else
	    return pointer_to->is_transmissible();
    }
    else
	return FALSE;
}

/* Print default init for this type (ie = 0) */

void PointerType::printDefaultInit ( ostream& s ) const
{
    if (kind() == POINTER_TYPE)
	s << " = 0";
}

String PointerType::signature () const
{
    String newname;
    String ptrname;

    if (kind() == POINTER_TYPE)
	ptrname = "P";
    else
	ptrname = "R";
    
    if (pointer_to != 0)
	newname = ptrname + pointer_to->signature();
    else
	newname = ptrname;

    if (pointer_quals)
	newname = newname + pointer_quals->signature();

    return newname;

}

String PointerType::typename ( DeclStyle ds ) const
{
    String newname;
    String ptrname;

    if (kind() == POINTER_TYPE)
	ptrname = "*";
    else
	ptrname = "&";
    
    if (pointer_to != 0)
	newname = (pointer_to->typename(ds) + ptrname);
    else
	newname = ptrname;

    if (pointer_quals)
	newname = newname + pointer_quals->typename(ds);

    return newname;
}

void PointerType::postname ( ostream& s, DeclStyle ds )
{
    if (pointer_to != 0)
    {
	Type pointed_at = pointer_to->kind();

	if ((pointed_at == VECTOR_TYPE) || (pointed_at == FUNCTION_TYPE))
	    s << ")";
	pointer_to->postname(s,ds);
    }
}

void PointerType::print_bt ( ostream& s, DeclStyle ds, Boolean elaborated, Boolean noConst )
{
    if (pointer_to != 0)
    {
	pointer_to->print_bt(s, ds, elaborated, noConst);
    }
}

void PointerType::prename ( ostream& s, DeclStyle ds )
{

    if (pointer_to != 0)
    {	
	Type pointed_at = pointer_to->kind();

	pointer_to->prename(s, ds);

	if ((pointed_at == VECTOR_TYPE) || (pointed_at == FUNCTION_TYPE))
	    s << "(";
    }

    if (kind() == POINTER_TYPE)
	s << "* ";
    else

	s << "& ";

    if (pointer_quals)
	pointer_quals->prename(s, ds);

}


/*
 * (Un)Marshalling of pointers and references is special in that
 * we attempt to detect if a pointer has been (un)packed already to avoid
 * packing (and unpacking) extra copies.
 * References to fundamental types (int etc.) are sent as if they were pointers
 * to ensure aliasing is handled correctly.
 * If the reference is to another pointer we simply send the pointer directly
 * in the normal manner.
 * When a pointer is packed / unpacked the name of the variable
 * has to be adjusted to take account of the indirection. These
 * routine handles this.
 */

void PointerType::print_marshalling_code ( ostream& s,
					   const String& varname,
					   const String& buffname )
{
    String newname = varname;

    if (kind() == REFERENCE_TYPE)
    {
	if (pointer_to->is_fundamental())
	    newname = String("&") + varname;
    }
    pointer_to->print_marshalling_code(s, newname, buffname);
}

void PointerType::print_unmarshalling_code ( ostream& s,
					     const String& varname,
					     const String& buffname )
{
    String newname = varname;

    if ((kind() == REFERENCE_TYPE) && (pointer_to->is_fundamental()))
    {
	PointerType pt(POINTER_TYPE, pointer_to);
	newname += "Ptr";

	pt.print_instance(s, newname, NORMAL_DECL, FALSE, FALSE);
	s << " = &" << varname << ";\n\t";
    }
    pointer_to->print_unmarshalling_code(s, newname, buffname);
}
 
