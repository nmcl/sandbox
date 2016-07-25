/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TDesc.cc,v 1.13 1993/04/02 10:21:05 ngdp Exp $
 */

/*
 * Stub generator basic type attribute class. The operations here provide
 * default implementations for the operations on the type description 
 * hierarchy rooted on the class TypeDescriptor. Many are simple no-ops
 * or have trivial semantics. More complicated behaviour should be 
 * implemented in the appropriate derived class.
 * Some operations are defined pure virtual to force redefinition - mostly
 * those for which a reasonable default does not exist.
 * We do not make these inline since they are all virtual and cannot be
 * inlined as a general rule.
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif
     
static const char RCSid[] = "$Id: TDesc.cc,v 1.13 1993/04/02 10:21:05 ngdp Exp $";
     
/*
 * Public operations
 */

/*
 * Operation to deal with function bodies for declarations. Called when
 * inline body is added.
 */

void TypeDescriptor::add_body ( Expression * )
{
    error("Function body illegal in this context");
}

     
/*
 * Operation to deal with function type modifiers (INLINE etc) If we reach here
 * then the modifier is illegal in this context. Say so
 */

void TypeDescriptor::add_func_modifier ( FuncModifier fm )
{
    error_stream << "illegal function modifier '" << fm << "'in declaration";
    error(error_stream);
}

/*
 * Default to no-op as normally basic types do not have names. Not true
 * for classes and functions
 */

void TypeDescriptor::add_name ( const String& )
{
}

/*
 * Operation to deal with stub specific modifiers.  If we reach here
 * then the modifier is generally illegal in this context. We ignore
 * 'harmless' modifiers such as NoRemote since they get set implicitly
 * for class members
 */

void TypeDescriptor::add_stub_modifier ( StubModifier sm )
{
    if ((sm != STUB_DEFAULT) && (sm != STUB_NOREMOTE))
    {
	error_stream << "stub command '" << sm 
	             << "' illegal in this declaration";
	error(error_stream);
    }
}

/*
 * Operation to deal with type modifiers (SHORT etc) If we reach here
 * then the modifier is illegal in this context. Say so
 */

void TypeDescriptor::add_type_modifier ( TypeModifier tm )
{
    error_stream << "illegal type modifier '" << tm << "'in declaration";
    error(error_stream);
}

/*
 * Operation to deal with type qualifier (CONST etc) If we reach here
 * then the qualifier is illegal in this context. Say so
 */

void TypeDescriptor::add_cv_qualifier ( CvQualifier cvq )
{
    error_stream << "illegal type qualifier '" << cvq << "'in declaration";
    error(error_stream);
}

/*
 * This routine is used by the process that attaches the basic delarator
 * type (eg int) to a declaration.
 */

void TypeDescriptor::attach_base_type ( TypeDescriptor * )
{
}


/*
 * Operation to deal with visibility changes. Generally illegal
 */

void TypeDescriptor::change_visibility ( VisibilitySpec vs )
{
    error_stream << "illegal change of visibility '" << vs << "'in declaration";
    error(error_stream);
}

/*
 * Stub generation default attribute handling. 
 */

StubModifier TypeDescriptor::default_location () const
{
    return STUB_NOREMOTE;
}

StubModifier TypeDescriptor::default_marshall () const
{
    return STUB_AUTOMARSHALL;
}


StubModifier TypeDescriptor::default_par_transfer () const
{
    return STUB_ARG_IN;
}

/*
 * Typically only valid for pointers so return 'this' as a default
 */

TypeDescriptor *TypeDescriptor::deref () const
{
    return 0;
}

void TypeDescriptor::freeze_type ()
{
}

Boolean TypeDescriptor::is_constant () const
{
    return FALSE;
}

/*
 * Is this type a fundamental one (int, char, class etc)
 */

Boolean TypeDescriptor::is_fundamental () const
{
    return TRUE;
}

Boolean TypeDescriptor::is_pure () const
{
    return FALSE;
}

/*
 * This could be a protected data member but I try never to export data
 * members to anyone - even derived classes. Overhead not high in this
 * case anyway.
 */

Type TypeDescriptor::kind () const
{
    return is_kind_of;
}

void TypeDescriptor::make_complete ()
{
}

void TypeDescriptor::make_pure ()
{
    error("Pure (=0) specifier illegal in this declaration");
}

String TypeDescriptor::massage_name ( const String& orig_name,
				       DeclStyle ) const
{
    return orig_name;
}

/* Do declarations involving this type need trailing separator */

Boolean TypeDescriptor::needs_sep () const
{
    return TRUE;
}

/*
 * Null output operations
 */

Boolean TypeDescriptor::print_code ( ostream&, DeclStyle ) 
{
    return FALSE;
}

/* Print default init for this type (ie = 0) */

void TypeDescriptor::printDefaultInit ( ostream& ) const
{
}

/*
 * Print the declaration of an instance of any type. Var_name is the
 * name the instance should have (which may be ignored), elaborated
 * indicates that an elaborated type name is required. Uses prename
 * and postname.
*/
    
void TypeDescriptor::print_instance ( ostream& s,
				      const String& var_name,
				      DeclStyle ds,
				      Boolean elaborated,
				      Boolean supressConst
				  )
{
    /* First get a handle on the basic type of the decl */
    print_bt(s, ds, elaborated, supressConst);

    /* Then any pre-name stuff (eg * or & ) */
    prename(s,ds);

    s << massage_name(var_name,ds);

    /* Finally postname stuff (eg []) */
    postname(s, ds);
}
    
void TypeDescriptor::print_marshalling_code ( ostream& s,
					      const String& vn,
					      const String& bn ) 
{
    s << "\t" << bn << " << " ;
    s << vn << ";\n";
}

Boolean TypeDescriptor::print_marshall_defs ( ostream&, DeclStyle ) 
{
    return FALSE;
}

void TypeDescriptor::print_typedef ( ostream&, DeclStyle ) 
{
}

void TypeDescriptor::print_unmarshalling_code ( ostream& s,
					        const String& vn,
					        const String& bn) 
{
    s << "\t" << bn << " >> " ;
    s << vn << ";\n";
}

void TypeDescriptor::produce_marshall_defs ( ostream& ) 
{
}

Boolean TypeDescriptor::produce_stub ( ostream&, DeclStyle ) 
{
    return FALSE;
}

void TypeDescriptor::produce_stubs ( ostream& ) 
{
}

/*
 * Generally stub generation is not possible
 */

Boolean TypeDescriptor::stub_possible () const
{
    return FALSE;
}

/*
 * Protected constructor
 */

TypeDescriptor::TypeDescriptor ( Type kind )
{
#ifdef DEBUG
    debug_stream << "TypeDescriptor::TypeDescriptor ("  << (void *)this << ","
	         <<  kind << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    is_kind_of = kind;
}

TypeDescriptor::~TypeDescriptor ()
{
#ifdef DEBUG
    debug_stream << "TypeDescriptor::~TypeDescriptor ()\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

#ifdef NO_INLINES
#  define TDESC_CC_
#  include "TDesc.n"
#  undef TDESC_CC_
#endif
