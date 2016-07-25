/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeDefT.cc,v 1.10 1993/04/14 13:46:22 ngdp Exp $
 */

/*
 * Stub generator typedef definition handler
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

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

static const char RCSid[] = "$Id: TypeDefT.cc,v 1.10 1993/04/14 13:46:22 ngdp Exp $";

/*
 * PUBLIC operations
 */

TypeDefType::TypeDefType ( const String& name, TypeDescriptor *td )
                             : TypeDescriptor(TYPE_TYPE),
			       real_type(td),
			       typedefname(name)
{
#ifdef DEBUG
    debug_stream << "TypeDefType::TypeDefType(" << (void *)this << ","
	         << name << "," << (void *)td << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDefType::~TypeDefType ()
{
#ifdef DEBUG
    debug_stream << "TypeDefType::~TypeDefType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *TypeDefType::combine_types ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debug_stream << "TypeDefType::combine_types ("  << (void *)this << ","
	         << (void *)newtype << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (real_type == 0)
    {
	real_type = newtype;
    }
    else
	real_type = combine_types(newtype);

    return this;
}

TypeDescriptor *TypeDefType::deref () const
{
    if (real_type == 0)
	return 0;

    if (real_type->kind() != TYPE_TYPE)
	return real_type;
    else
	return real_type->deref();
}

Boolean TypeDefType::is_fundamental () const
{
    if (real_type != 0)
	return real_type->is_fundamental();
    else
	return FALSE;
}

Boolean TypeDefType::is_transmissible () const
{
    if (real_type != 0)
	return real_type->is_transmissible();
    else
	return FALSE;
}

void TypeDefType::printDefaultInit ( ostream& s ) const
{
    if (real_type != 0)
	real_type->printDefaultInit(s);
}

String TypeDefType::signature () const
{
    return typedefname;    
}

String TypeDefType::typename ( DeclStyle ) const
{
    return typedefname;
}

void TypeDefType::postname ( ostream&, DeclStyle ) 
{
}

void TypeDefType::prename ( ostream&, DeclStyle ) 
{
}

void TypeDefType::print_bt ( ostream& s, DeclStyle, Boolean, Boolean  ) 
{
    s << typedefname << " ";
}

void TypeDefType::print_typedef ( ostream& s, DeclStyle ds )
{
    /* 'typedef' printed as storage class in DeclSpecifier */

    real_type->print_bt(s, ds, FALSE, FALSE);
    real_type->prename(s, ds);
    s << typedefname;
    real_type->postname(s,ds);
}

void TypeDefType::print_marshalling_code ( ostream& s,
					   const String& varname, 
					   const String& buffname) 
{  
    if (real_type)
	real_type->print_marshalling_code(s, varname,buffname);
}

void TypeDefType::print_unmarshalling_code ( ostream& s,
					     const String& varname, 
					     const String& buffname) 
{
    if (real_type)
	real_type->print_unmarshalling_code(s, varname, buffname);
}
