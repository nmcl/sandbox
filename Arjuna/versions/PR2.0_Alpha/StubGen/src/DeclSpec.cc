/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclSpec.cc,v 1.11 1993/03/22 09:30:56 ngdp Exp $
 */

/*
 * Stub generator declaration specifier handler. Declaration specifiers
 * capture storage class, type qualifiers and type of a declaration.
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CVT_H_
#  include "CvT.h"
#endif

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLSPEC_H_
#  include "DeclSpec.h"
#endif

static const char RCSid[] = "$Id: DeclSpec.cc,v 1.11 1993/03/22 09:30:56 ngdp Exp $";

/*
 * Public constructors
 */

DeclSpecifier::DeclSpecifier ( CvQualifier cv )
                             : decl_inline(FALSE),
			       decl_is_elaborated(FALSE),
			       decl_marshall(STUB_PENDING),
			       decl_obj_type(STUB_PENDING),
			       decl_par_transfer(STUB_PENDING),
			       decl_stclass(SC_DEFAULT),
			       decl_btype(0),
			       decl_virtual(FALSE)
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
	         << cv << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cv == QUAL_CONST)
    {
	decl_const = TRUE;
	decl_volatile = FALSE;
    }
    else
    {
	decl_const = FALSE;
	decl_volatile = TRUE;
    }
}

DeclSpecifier::DeclSpecifier ( FuncModifier fm )
                             : decl_const(FALSE),
			       decl_is_elaborated(FALSE),
			       decl_marshall(STUB_PENDING),
			       decl_obj_type(STUB_PENDING),
			       decl_par_transfer(STUB_PENDING),
			       decl_stclass(SC_DEFAULT),
			       decl_btype(0),
			       decl_virtual(FALSE),
			       decl_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
	         << fm << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (fm == FUNC_INLINE)
    {
	decl_virtual = FALSE;
	decl_inline = TRUE;
    }
    else
    {
	decl_inline = FALSE;
	decl_virtual = TRUE;
    }
}

DeclSpecifier::DeclSpecifier ( StorageSpecifier st )
                             : decl_const(FALSE),
			       decl_inline(FALSE),
			       decl_is_elaborated(FALSE),
			       decl_marshall(STUB_PENDING),
			       decl_obj_type(STUB_PENDING),
			       decl_par_transfer(STUB_PENDING),
			       decl_stclass(st),
			       decl_btype(0),
			       decl_virtual(FALSE),
			       decl_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
	         << st << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

}

DeclSpecifier::DeclSpecifier ( StubModifier sc )
                             : decl_const(FALSE),
			       decl_inline(FALSE),
			       decl_is_elaborated(FALSE),
			       decl_marshall(STUB_PENDING),
			       decl_obj_type(STUB_PENDING),
			       decl_par_transfer(STUB_PENDING),
			       decl_stclass(SC_DEFAULT),
			       decl_btype(0),
			       decl_virtual(FALSE),
			       decl_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::DeclSpecifier("  << (void *)this << ","
	         << sc << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sc)
    {
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_NOMARSHALL:
	decl_marshall = sc;
	break;
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
	decl_obj_type = sc;
	break;
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	decl_par_transfer = sc;
	break;
    default:
	break;
    }
}


DeclSpecifier::DeclSpecifier ( TypeDescriptor *td, Boolean elaborated )
                             : decl_const(FALSE),
			       decl_inline(FALSE),
			       decl_is_elaborated(elaborated),
			       decl_marshall(STUB_PENDING),
			       decl_obj_type(STUB_PENDING),
			       decl_par_transfer(STUB_PENDING),
			       decl_stclass(SC_DEFAULT),
			       decl_btype(td),
			       decl_virtual(FALSE),
			       decl_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::DeclSpecifier("  << (void *)this << "," 
	         << (void *)td << ", " << elaborated << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

}

/*
 * Public non-virtual functions
 */

void DeclSpecifier::add_cv_qualifier ( CvQualifier cv )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::add_cv_qualifier("  << (void *)this
	         << "," << cv << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cv == QUAL_CONST)
    {
	if (decl_const)
	    warning("superfluous 'const' ignored"); 
    	decl_const = TRUE;
    }
    else
    {
	if (decl_volatile)
	    warning("superfluous 'volatile' ignored");
	decl_volatile = TRUE;
    }
}

void DeclSpecifier::add_func_modifier ( FuncModifier new_fm )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::add_func_modifier("  << (void *)this
	         << "," << new_fm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (new_fm == FUNC_INLINE)
    {
	if (decl_inline)
	    warning("superfluous 'inline' ignored"); 
	decl_inline = TRUE;
    }
    else
    {
	if (decl_virtual)
	    warning("superfluous 'virtual' ignored"); 
	decl_virtual = TRUE;
    }
}

void DeclSpecifier::add_stclass ( StorageSpecifier new_st )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::add_stclass("  << (void *)this << "," 
	         << new_st << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (decl_stclass ==  SC_DEFAULT)
    {
	decl_stclass = new_st;
    } else
        if (decl_stclass == SC_TYPEDEF)
            error("cannot have storage class in typedef");
        else
            error("storage class already specified in this declaration");
}

void DeclSpecifier::add_stub_modifier ( StubModifier new_sm )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::add_stub_modifier("  << (void *)this << "," 
	         << new_sm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (new_sm)
    {
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_NOMARSHALL:
	if ((decl_marshall == STUB_PENDING) || (decl_marshall == new_sm))
	{
	    decl_marshall = new_sm;
	    return;
	}
	break;
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
	if ((decl_obj_type == STUB_PENDING) || (decl_obj_type == new_sm))
	{
	    decl_obj_type = new_sm;
	    return;
	}
	break;
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	if ((decl_par_transfer == STUB_PENDING) ||
	    (decl_par_transfer == new_sm))
	{
	    decl_par_transfer = new_sm;
	    return;
	}
	break;
    default:
	break;
    }
    
    error("conflicting or illegal stub-specific modifiers in this declaration");
}

void DeclSpecifier::add_type_modifier ( TypeModifier tm )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::add_type_modifier("  << (void *)this << "," 
	         << tm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (decl_btype)
    {
	decl_btype->add_type_modifier(tm);
    }
    else
	decl_btype = new ModifiedType(tm);
}
/*
 * Adjust type associated with a decl spec. If there is none already this
 * is easy. If one exists assume that the incoming one represents earlier
 * gathered info to be merged into whatever we have currently
 */

void DeclSpecifier::adjust_basic_type ( TypeDescriptor * orig_td )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::adjust_basic_type("  << (void *)this << "," 
	         << (void *)orig_td << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (decl_btype)
    {
	TypeDescriptor *old_btype = decl_btype;

	if ((decl_btype = orig_td->combine_types(decl_btype)) != old_btype)
	    delete old_btype;
    }
    else
	decl_btype = orig_td;
}

/*
 * Complete this declaration specifier. Used when completing a declaration
 * to form the basic decl specs that will be attached to each individual
 * declarator.
 * Creates CvTypes if needed.
 * May be called multiple times (without problems)
 */

void DeclSpecifier::finalise ( )
{
#ifdef DEBUG
    debug_stream << "DeclSpecifier::finalise("  << (void *)this << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    /* correct for const or volatile */
    
    if (decl_const || decl_volatile)
    {
	CvType *cvt = new CvType(decl_btype);
	
	if (decl_const)
	    cvt->add_cv_qualifier(QUAL_CONST);
	if (decl_volatile)
	    cvt->add_cv_qualifier(QUAL_VOLATILE);
	
	decl_btype = cvt;
    }
	
    decl_const = decl_volatile = FALSE;
}

DeclSpecifier *DeclSpecifier::merge_spec_into ( DeclSpecifier *updated_spec ) const
{
#ifdef DEBUG
    /* 
     * The void * const cast in the next statement should really be to
     * a const void * const but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << "DeclSpecifier::merge_spec_into("  << (void * const)this << "," 
	         << (void *)updated_spec << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    /* start by merging in basic attributes */

    merge_basic_attributes(updated_spec);

    /* Then type info if it exists */

    if (decl_btype)
	updated_spec->adjust_basic_type(decl_btype);

    return updated_spec;
}

void DeclSpecifier::propagate_attributes ( Declaration *decl ) const
{
    if (decl)
    {
	TypeDescriptor *td = decl->get_type();

	if (td)
	    propagate_attributes(td);
	else
	    error("(stub internal) declaration has no type");
	    
	if (decl_is_elaborated)
	    decl->make_elaborated();

	if (decl_stclass != SC_DEFAULT)
	    decl->add_stclass(decl_stclass);

	if (decl_marshall != STUB_PENDING)
	    decl->add_stub_modifier(decl_marshall);

	if (decl_obj_type != STUB_PENDING)
	    decl->add_stub_modifier(decl_obj_type);
	
	if (decl_par_transfer != STUB_PENDING)
	    decl->add_stub_modifier(decl_par_transfer);

    }
}

void DeclSpecifier::propagate_attributes ( TypeDescriptor *td ) const
{
    if (decl_inline)
	td->add_func_modifier(FUNC_INLINE);
    
    if (decl_virtual)
	td->add_func_modifier(FUNC_VIRTUAL);
}

/*
 * Merge basic attributes of decl spec into argument. Only handles
 * storage class etc. Type information merged elsewhere.
 */

void DeclSpecifier::merge_basic_attributes ( DeclSpecifier *updated_spec ) const
{
#ifdef DEBUG
    /* 
     * The void * const cast in the next statement should really be to
     * a const void * const but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << "DeclSpecifier::merge_basic_attributes("  << (void * const)this << "," 
	         << (void *)updated_spec << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (decl_const)
	updated_spec->add_cv_qualifier(QUAL_CONST);

    if (decl_volatile)
	updated_spec->add_cv_qualifier(QUAL_VOLATILE);
    
    if (decl_inline)
	updated_spec->add_func_modifier(FUNC_INLINE);

    if (decl_is_elaborated)
	updated_spec->make_elaborated();

    if (decl_marshall != STUB_PENDING)
	updated_spec->add_stub_modifier(decl_marshall);

    if (decl_obj_type != STUB_PENDING)
	updated_spec->add_stub_modifier(decl_obj_type);

    if (decl_par_transfer != STUB_PENDING)
	updated_spec->add_stub_modifier(decl_par_transfer);

    if (decl_stclass != SC_DEFAULT)
	updated_spec->add_stclass(decl_stclass);

    if (decl_virtual)
	updated_spec->add_func_modifier(FUNC_VIRTUAL);
}

#ifdef NO_INLINES
#  define DECLSPEC_CC_
#  include "DeclSpec.n"
#  undef DECLSPEC_CC_
#endif
