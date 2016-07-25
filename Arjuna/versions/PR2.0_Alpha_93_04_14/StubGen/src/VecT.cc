/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VecT.cc,v 1.10 1993/04/14 13:46:24 ngdp Exp $
 */

/*
 * Stub generator vector (array) type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef VECT_H_
#  include "VecT.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

static const char RCSid[] = "$Id: VecT.cc,v 1.10 1993/04/14 13:46:24 ngdp Exp $";

/*
 * PUBLIC operations
 */

VecType::VecType ( Expression *vs, TypeDescriptor *is_of )
                 : TypeDescriptor(VECTOR_TYPE),
		   vector_size(vs),
		   vector_of(is_of)
{
#ifdef DEBUG
    debug_stream << "VecType::VecType("  << (void *)this << ","
	         << vs << ", " << (void *)is_of << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

VecType::~VecType ()
{
#ifdef DEBUG
    debug_stream << "VecType::~VecType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void VecType::attach_base_type ( TypeDescriptor *base_type )
{
#ifdef DEBUG
    debug_stream << "VecType::attach_base_type(" << (void *)this << ", " << (void *)base_type << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (vector_of)
	vector_of->attach_base_type(base_type);
    else
	vector_of = base_type;
}

/*
 * Adjust type of vector. Sets vector_of field, returns old value.
 * Used in building declarator type
 */

TypeDescriptor *VecType::combine_types ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debug_stream << "VecType::combine_types ("  << (void *)this << ","
	         << (void *)newtype << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (vector_of)
	vector_of->combine_types(newtype);
    else
	vector_of = newtype;

    return this;
}

StubModifier VecType::default_par_transfer () const
{
    return STUB_ARG_INOUT;
}

Boolean VecType::is_fundamental () const
{
    return FALSE;
}

Boolean VecType::is_transmissible () const
{
    if (vector_of)
	return vector_of->is_transmissible();
    else
	return TRUE;
}

void VecType::postname ( ostream& s, DeclStyle ds ) 
{
    s << "[";
    if (vector_size != 0)
        s << *vector_size;
    s << "]";
    if (vector_of != 0)
        vector_of->postname(s,ds);

}

void VecType::prename ( ostream& s, DeclStyle ds ) 
{
    if (vector_of != 0)
        vector_of->prename(s,ds);

}

void VecType::print_bt ( ostream& s, DeclStyle ds, Boolean b, Boolean ic ) 
{
    if (vector_of != 0)
        vector_of->print_bt(s,ds,b, ic);

}

void VecType::print_marshalling_code ( ostream& s,
					 const String& varname, 
					 const String& buffname) 
{  
    String indexvarname = indexname();
    String newstring = varname + "[" + indexvarname + "]";

    s << "\t";
    s << "for (int " << indexvarname << " = 0; ";
    s << indexvarname << " < " << *vector_size << "; ";
    s << indexvarname << "++ )\n";
    vector_of->print_marshalling_code(s, newstring, buffname);
}

void VecType::print_unmarshalling_code ( ostream& s,
					   const String& varname, 
					   const String& buffname) 
{
    String indexvarname = indexname();
    String newstring = varname + "[" + indexvarname + "]";

    s << "\t";
    s << "for (int " << indexvarname << " = 0; ";
    s << indexvarname << " < " << *vector_size << "; ";
    s << indexvarname << "++ )\n";
    vector_of->print_unmarshalling_code(s, newstring, buffname);
}

String VecType::signature () const
{
    String sig;

    sig += "A_";

    if (vector_of != 0)
        sig += vector_of->signature();

    return sig;    
}

String VecType::typename ( DeclStyle ds ) const
{
    String newname("[]");

    if (vector_of != 0)
        newname = (vector_of->typename(ds) + "[]");

    return newname;
}

/*
 * PRIVATE operations
 */

String VecType::indexname ()
{
    static long index_number;           /* ensure each index is unique */

    return constructname("i_", index_number++);
}
