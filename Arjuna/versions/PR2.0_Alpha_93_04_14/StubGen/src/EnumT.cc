/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EnumT.cc,v 1.13 1993/04/14 13:46:08 ngdp Exp $
 */

/*
 * Stub generator enumerated type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef ENUMT_H_
#  include "EnumT.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif

#ifndef DECLSPEC_H_
#  include "DeclSpec.h"
#endif

extern String constructname ( const String&, long );

static const char RCSid[] = "$Id: EnumT.cc,v 1.13 1993/04/14 13:46:08 ngdp Exp $";

long EnumType::enum_count = 0;

/*
 * Constructor
 */

EnumType::EnumType ( const String& name, DeclarationList *enumvals,
		     Boolean complete ) 
                   : TypeDescriptor(ENUMERATION_TYPE),
		     enum_is_complete(complete),
		     enum_name(name),
		     enumerators(enumvals)
{
#ifdef DEBUG
    debug_stream << "EnumType::EnumType(" << name << ","
	         << (void *)enumvals << "," << complete << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    DeclarationListIterator next(enumerators);
    Declaration *enum_decl;

    if (name == NullString)
    {
	enum_name = constructname("_Enum_", enum_count++);
	enum_is_anon = TRUE;
    }
    else
    {
	enum_is_anon = FALSE;
    }

    while (enum_decl = next())
    {
	enum_decl->set_dtype(this);
    }
}

EnumType::~EnumType ()
{
#ifdef DEBUG
    debug_stream << "EnumType::~EnumType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *EnumType::combine_types ( TypeDescriptor * )
{
    error("(stub internal) Attempt to adjust enum type");
    return this;
}

Boolean EnumType::is_transmissible () const
{
    return TRUE;
}

/*
 * Print type description for enumerated types. Prints keyword 'enum'
 * followed by the enum type name, then walks the list of enumerations
 * printing each. Note we don't call print_decl since the enumeration has
 * been given the type of the enum and we don't want that printed
 */
					
void EnumType::print_typedef ( ostream& s, DeclStyle ) 
{
    s << "\nenum ";
    if (enum_name != NullString)
	s << enum_name << " ";
    if (enum_is_complete)
    {
	Expression *init_val;

	s << "{";
	if (enumerators != 0)
	{
	    DeclarationListIterator next(enumerators);
	    Declaration *current;
	    
	    current = next();
	    while (current != 0)
	    {
		s << current->get_name();
		init_val = current->get_initialiser();
		/* Do not need = here its held as part of the initiliser */
		if (init_val)
		    s << *init_val;

		current = next();
		if (current != 0)
		    s << ", ";
	    }
	}
	s << "}";
    }
}

void EnumType::postname ( ostream&, DeclStyle )
{
}

void EnumType::prename ( ostream&, DeclStyle )
{
}

/*
 * Print only the type name, for use in declarations
 */

void EnumType::print_bt ( ostream& s, DeclStyle, Boolean elaborated, Boolean ) 
{
    if (elaborated)
	s << "enum ";
    
    if (enum_name != NullString)
	s << enum_name << " ";
}

void EnumType::produce_stubs ( ostream& hdr )
{
    print_typedef(hdr, CLIENT_DECL);

    hdr << ";\n";

    hdr << "\ninline RpcBuffer& operator<< ";
    hdr << "( RpcBuffer& rpcbuff, " << enum_name << "* enumPtr )\n";
    hdr << "{\n\treturn rpcbuff.operator<< ((int*)enumPtr);\n";
    hdr << "}\n\n";

    hdr << "\ninline RpcBuffer& operator>> ";
    hdr << "( RpcBuffer& rpcbuff, " << enum_name << "& enumRef )\n";
    hdr << "{\n\tint dummy;\n";
    hdr << "\trpcbuff >> dummy;\n";
    hdr << "\tenumRef = (" << enum_name << ")dummy;\n";
    hdr << "\treturn rpcbuff;\n";
    hdr << "}\n\n"; 
  
    hdr << "\ninline RpcBuffer& operator>> ";
    hdr << "( RpcBuffer& rpcbuff, " << enum_name << "*& enumPtr)\n";
    hdr << "{\n";
    hdr << "\treturn rpcbuff.operator>> ((int*&)enumPtr);\n";
    hdr << "}\n\n";   
}

Boolean EnumType::stub_possible () const
{
    return TRUE;
}

String EnumType::signature () const
{
    return enum_name;
}

String EnumType::typename ( DeclStyle ) const
{
    return enum_name;
}
