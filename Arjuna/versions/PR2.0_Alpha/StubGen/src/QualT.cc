/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QualT.cc,v 1.9 1993/04/14 13:46:21 ngdp Exp $
 */

/*
 * Stub generator qualified type definition handler
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

#ifndef QUALT_H_
#  include "QualT.h"
#endif

static const char RCSid[] = "$Id: QualT.cc,v 1.9 1993/04/14 13:46:21 ngdp Exp $";

/*
 * PUBLIC operations
 */

QualifiedType::QualifiedType ( TypeDescriptor *parent, TypeDescriptor *qt )
                             : TypeDescriptor(TYPE_TYPE),
			       qualified_type(qt),
			       qualifying_type(parent)
{
#ifdef DEBUG
    debug_stream << "QualifiedType::QualifiedType(" << (void *)this << ","
	         << (void *)parent << "," << (void *)qt << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

QualifiedType::~QualifiedType ()
{
#ifdef DEBUG
    debug_stream << "QualifiedType::~QualifiedType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void QualifiedType::add_body ( Expression* body )
{
    if (qualified_type != 0)
    {
	qualified_type->add_body(body);
    }
    else
	TypeDescriptor::add_body(body);
    
}

void QualifiedType::add_func_modifier ( FuncModifier new_fm )
{
#ifdef DEBUG
    debug_stream << "QualifiedType::add_func_modifier("  << (void *)this
                 << "," << new_fm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (qualified_type != 0)
    {
	qualified_type->add_func_modifier(new_fm);
    }
    else
	TypeDescriptor::add_func_modifier(new_fm);
}

TypeDescriptor *QualifiedType::combine_types ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debug_stream << "QualifiedType::combine_types ("  << (void *)this << ","
	         << (void *)newtype << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (qualified_type == 0)
    {
	qualified_type = newtype;
    }
    else
	qualified_type = combine_types(newtype);

    return this;
}

TypeDescriptor *QualifiedType::deref () const
{
    if (qualified_type == 0)
	return 0;

    if (qualified_type->kind() != TYPE_TYPE)
	return qualified_type;
    else
	return qualified_type->deref();
}

Boolean QualifiedType::is_transmissible () const
{
    if (qualified_type != 0)
	return qualified_type->is_transmissible ();
    else
	return FALSE;
}

String QualifiedType::massage_name ( const String&, DeclStyle ds ) const
{    
    String tn = NullString;

    if (qualifying_type != 0)
	tn  = qualifying_type->typename(ds) + "::";

    if (qualified_type != 0)
	tn += qualified_type->typename(ds);

    return tn;    
}

String QualifiedType::signature () const
{
    String sig = NullString;

    if (qualifying_type != 0)
	sig = qualifying_type->signature();

    if (qualified_type != 0)
	sig += qualified_type->signature();

    return sig;
}

String QualifiedType::typename ( DeclStyle ds ) const
{
    String tn = NullString;

    if (qualifying_type != 0)
	tn  = qualifying_type->typename(ds) + "::";

    if (qualified_type != 0)
	tn += qualified_type->typename(ds);

    return tn;
}

void QualifiedType::postname ( ostream& s , DeclStyle ds ) 
{
    if (qualified_type != 0)
	qualified_type->postname(s, ds);
}

void QualifiedType::prename ( ostream& s, DeclStyle ds ) 
{
    if (qualified_type != 0)
    {
	qualified_type->prename(s, ds);
	s << " "; 
    }
}

void QualifiedType::print_bt ( ostream& s, DeclStyle ds, Boolean b, Boolean ic ) 
{
    if (qualified_type != 0)
	qualified_type->print_bt(s, ds, b, ic);
}

void QualifiedType::print_typedef ( ostream& s, DeclStyle ds)
{
    if (qualified_type != 0)
	qualified_type->print_typedef(s, ds);
}

void QualifiedType::print_unmarshalling_code ( ostream& s,
					       const String& vn,
					       const String& bn )
{
    if (qualified_type)
	qualified_type->print_unmarshalling_code(s,vn,bn);
}
