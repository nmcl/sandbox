/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Decl.cc,v 1.20 1993/03/22 09:30:50 ngdp Exp $
 */


/*
 * Stub generator declaration handler. Declarations are the key data structure
 * used in the stub generator and consist of several parts:
 *
 * 	A filename/linenumber (possibly) used in error reporting
 * 	A name
 * 	A type plus type specifiers
 * 	A initial value.
 *
 */
     
#include <strstream.h>
     
#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

#ifndef DUMMYARGT_H_
#  include "DummyArgT.h"
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

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif
     
static const char RCSid[] = "$Id: Decl.cc,v 1.20 1993/03/22 09:30:50 ngdp Exp $";

long Declaration::count_of_decls = 0;

/*
 * Constructor. Create a declaration. The values set here may be overridden
 * later. The location information is determined by accessing information
 * maintained by the parser as it reads the input, as are the default stub 
 * generation options.
 */

Declaration::Declaration ( const String& name,
			   TypeDescriptor *dt,
			   Expression *value )
                         : decl_body(0),
			   decl_context(0),
			   decl_frozen(FALSE),
                           decl_init(value),
			   decl_is_elaborated(FALSE),
			   decl_is_special(FALSE),
			   decl_is_typedef(FALSE),
			   decl_linkage(GlobalInfo::get_linkage()),
			   decl_loc(GlobalInfo::get_currentloc()),
			   decl_name(name),
			   decl_marshall(STUB_PENDING),
			   decl_obj_type(STUB_PENDING),
			   decl_par_transfer(STUB_PENDING),
			   decl_stclass(SC_DEFAULT),
			   declarator_type(dt),
			   decl_vis(FILE_EXTERN),
			   default_linkage(TRUE)
{
#ifdef DEBUG
    debug_stream << "Declaration::Declaration (" << (void *)this << ", " 
	         << name << ", " << (void *)dt << ", " << value << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void Declaration::add_stclass ( StorageSpecifier new_st )
{
#ifdef DEBUG
    debug_stream << "Declaration::add_stclass("  << (void *)this << "," 
	         << new_st << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (decl_stclass ==  SC_DEFAULT)
    {
	decl_stclass = new_st;
	if (decl_stclass == SC_TYPEDEF)
	    decl_is_typedef = TRUE;
    } else
        if (decl_stclass == SC_TYPEDEF)
            error("cannot have storage class in typedef");
        else
            error("storage class already specified in this declaration");
}

void Declaration::add_stub_modifier ( StubModifier new_sm )
{
#ifdef DEBUG
    debug_stream << "Declaration::add_stub_modifier("  << (void *)this << "," 
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
	}
	break;
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
	if ((decl_obj_type == STUB_PENDING) || (decl_obj_type == new_sm))
	{
	    decl_obj_type = new_sm;
	}
	break;
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	if ((decl_par_transfer == STUB_PENDING) || (decl_par_transfer == new_sm))
	{
	    decl_par_transfer = new_sm;
	}
	break;
    default:
	break;
    }

    if (declarator_type)
	declarator_type->add_stub_modifier(new_sm);
}

/*
 * Adjust the type of a DECLARATOR. Used during parsing prior
 * to combination with basic type. These types will indicate 
 * pointer/reference, arrays, and functions
 */

void Declaration::adjust_type ( TypeDescriptor *decl_type )
{
    if (declarator_type)
	declarator_type = declarator_type->combine_types(decl_type);
    else
	declarator_type = decl_type;
}

/*
 * Attach basic declaration specifiers after freezing them. If there is none 
 * currently we use the one given. Otherwise we merge the one given INTO
 * the one already held. This ensures that for declarations like:
 *    const char *p, *q
 * the decl spec "const char" is merged into the two declarations for "p"
 * and "q" which will already have decl specs (with type "ptr to unknown").
 */

void Declaration::attach_basic_spec ( DeclSpecifier *basic_spec )
{
#ifdef DEBUG
    debug_stream << "Declaration::attach_basic_spec("  << (void *)this
	         << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    TypeDescriptor *basic_dtype = 0;

    if (basic_spec)
    {
	basic_dtype = basic_spec->get_type();

	/* Complete type information */

	if (declarator_type)
	    declarator_type->attach_base_type(basic_dtype);
	else
	    declarator_type = basic_dtype;
    }

    /* Default to Int if needed */

    if (declarator_type == 0)
    {
	if (decl_name == "...")
	    declarator_type = new DummyArgType();
	else
	    declarator_type = new IntType();
    }

    /* Propagate name down to type */

    if ((declarator_type !=0) && (decl_name != NullString))
	declarator_type->add_name(decl_name);

    /* Propagate the remaining attributes down to the type */

    if (basic_spec)
	basic_spec->propagate_attributes(this);

    /* Set up typedef info if required */

    if (decl_stclass == SC_TYPEDEF)
    {
	TypeDefType *td = new TypeDefType(decl_name, declarator_type);

	declarator_type = td;
	decl_is_typedef = TRUE;
    }
          
}

void Declaration::clear_special ()
{
    decl_is_special = FALSE;
}


void Declaration::freeze_decl ()
{
    if (!decl_frozen)
    {
	Location saved_loc = error_location;

	error_location = decl_loc;

	decl_frozen = TRUE;

	if ((decl_vis == FILE_EXTERN) && (decl_stclass == SC_STATIC))
	    decl_vis = FILE_STATIC;

	if (declarator_type == 0)
	    fatal("(Stub internal) Declaration has no type");
    
	if (!decl_is_special)
	{
	    if (decl_obj_type != STUB_PENDING)
		declarator_type->add_stub_modifier(decl_obj_type);
	    else
	    {
		/* for class members default to NoRemote unless public */
		switch (decl_vis)
		{
		case CLASS_PROTECTED:
		case CLASS_PRIVATE:
		    decl_obj_type = STUB_NOREMOTE;
		    declarator_type->add_stub_modifier(decl_obj_type);
		    break;
		case CLASS_PUBLIC:
		default:
		    decl_obj_type = declarator_type->default_location ();
		    break;  
		}
	    }
	    
	    if (decl_par_transfer != STUB_PENDING)
		declarator_type->add_stub_modifier(decl_par_transfer);
	    else
		decl_par_transfer = declarator_type->default_par_transfer ();
	    
	    if (decl_marshall != STUB_PENDING)
		declarator_type->add_stub_modifier(decl_marshall);
	    else
		decl_marshall = declarator_type->default_marshall ();

	    declarator_type->freeze_type();
	}

	error_location = saved_loc;
    }
}

String Declaration::get_name () const
{
    return decl_name;
}

StubModifier Declaration::get_marshall ()
{
    if ((decl_marshall == STUB_PENDING) ||
	(decl_marshall == STUB_DEFAULT))
	decl_marshall =  declarator_type->default_marshall();

    return decl_marshall;
}

StubModifier Declaration::get_remote ()
{
    if ((decl_obj_type == STUB_PENDING) ||
	(decl_obj_type == STUB_DEFAULT))
	decl_obj_type =  declarator_type->default_location();

    return decl_obj_type;
}

StubModifier Declaration::get_par_transfer ()
{
    if ((decl_par_transfer == STUB_PENDING) ||
	(decl_par_transfer == STUB_DEFAULT))
	decl_par_transfer =  declarator_type->default_par_transfer();

	return decl_par_transfer;
}
/*
 * Print a declaration. Declarations are printed in two parts. The type
 * before the name, the name, and then the type after the name. This is to
 * handle array and function declarations where the array dimensions or
 * function parameters appear after the variable name. Most of this is handled
 * by the virtual print_instance function found in all types.
 * Genuine declarations should ALWAYS have a decl_spec describing the 
 * type of the declaration otherwise there is something funny going on.
 */

void Declaration::print_decl ( ostream& s, DeclStyle ds,
			       const char *decl_sep,
			       Boolean noname, Boolean novalue )
{
    /* Only print if not a 'special' declaration */

    if (decl_is_special)
	return;

    /* Linkage first - eg extern "C" */
    
    if (decl_linkage && !default_linkage)
	s << decl_linkage;
    
    s << decl_stclass;
    
    /* Typedefs always have a decl spec */
    
    if (decl_is_typedef)
	declarator_type->print_typedef(s,ds);
    else
    {
	/* 'Normal' style declaration here */
	
	if (noname)
	    declarator_type->print_instance(s, NullString, ds, decl_is_elaborated, FALSE);
	else
	    declarator_type->print_instance(s, decl_name, ds, decl_is_elaborated, FALSE);
    }
    
    if (!novalue)
    {
	if (decl_init != 0)
	    s << *decl_init;
	if (decl_body != 0)
	    s << *decl_body << "\n\n";
	else
	    if (declarator_type->needs_sep())
		s << decl_sep;
    }
    else
	if (declarator_type->needs_sep())
	    s << decl_sep;
}
    
void Declaration::produce_stubs ( ostream& master_hdr_stream )
{
    Boolean need_brace = FALSE;

    if (decl_is_special)
	return;

    if (decl_linkage && !default_linkage)
    {
	master_hdr_stream << decl_linkage;
	master_hdr_stream << "\n{\n";
	need_brace = TRUE;
    }

    if (decl_is_typedef && declarator_type->stub_possible())
	declarator_type->produce_stubs(master_hdr_stream);
    else
    {
	master_hdr_stream << this;
	if (decl_marshall == STUB_AUTOMARSHALL)
	{
	    declarator_type->produce_marshall_defs(master_hdr_stream);
	}
    }

    if (need_brace)
	master_hdr_stream << "\n}\n";
}

void Declaration::set_body ( Expression *body )
{
    decl_body = body;
    if (declarator_type)
	declarator_type->add_body(body);
}

void Declaration::set_init ( Expression *expr )
{
    if (decl_init == 0)
    {
	decl_init = expr;
    }
    else
    {
	error("declaration already has initial value\n");
    }
}

void Declaration::set_linkage ( Linkage *l )
{
    if (default_linkage)
    {
	default_linkage = FALSE;
	decl_linkage = l;
    }
    else
	if (decl_linkage != l)
	    error("conflicting linkage specification\n");
}

Boolean Declaration::print_code ( ostream& code_stream, DeclStyle ds )
{
    return declarator_type->print_code(code_stream,ds);
}

Boolean Declaration::print_marshall_defs ( ostream& code_stream, DeclStyle ds )
{
    return declarator_type->print_marshall_defs(code_stream,ds);
}

void Declaration::print_marshall_code ( ostream& code_stream, 
				        const String& buff_name )
{
    declarator_type->print_marshalling_code(code_stream,decl_name, buff_name);
}

void Declaration::print_unmarshall_code ( ostream& code_stream, 
					  const String& buff_name )
{
    declarator_type->print_unmarshalling_code(code_stream,decl_name, buff_name);
}

String Declaration::get_signature () const
{
    return declarator_type->signature();
}

Type Declaration::kind () const
{
    if (declarator_type)
	return declarator_type->kind();
    else
	return NONE_TYPE;
}

void Declaration::make_pure ()
{
    if (declarator_type)
	declarator_type->make_pure();
    else
	error("declaration has no type to make pure!");
}

void Declaration::marshall (ostream& s , Direction d,
			    DeclStyle ds , const String& bname)
{
    if (!decl_is_special && !decl_is_typedef && (declarator_type != 0))
	declarator_type->marshall(s,d,ds,bname,decl_name);
}

/*
 * Decl has been inserted into some context. Check that any stub
 * modifiers explicitly set are applicable here.
 * Note that these may clash with modifiers set by the default mechanism
 * but this is deliberate to enforce consistency
 * Marshalling is odd in that the marshalling keywords are generally
 * inapplicable in all contexts UNLESS the declaration is a typdef
 * OR its a forward reference to a class (marked as elaborated with no name)
 */

void Declaration::set_context ( DeclContext *context )
{
#ifdef DEBUG
    debug_stream << "Declaration::set_context("  << (void *)this
	         << ", " << (void *)context << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    decl_context = context;

    if (!decl_is_special)
    {
	if (decl_obj_type != STUB_PENDING)
	{ 
	    if (decl_context->is_applicable(decl_obj_type))
	    {
		declarator_type->add_stub_modifier(decl_obj_type);
	    } else
	    {
		error_stream << "'" << decl_obj_type 
		             << "' illegal in this declaration";
		error(error_stream);
		decl_obj_type = STUB_PENDING;
	    }
	}
	
	if (decl_par_transfer != STUB_PENDING)
	{
	    if(decl_context->is_applicable(decl_par_transfer))
	    {
		declarator_type->add_stub_modifier(decl_par_transfer);
	    } else
	    {
		error_stream << "'" << decl_par_transfer
		             << "' illegal in this declaration";
		error(error_stream);
		decl_par_transfer = STUB_PENDING;
	    }
	}
	
	if (decl_marshall != STUB_PENDING)
	{
	    if (decl_context->is_applicable(decl_marshall) ||
		decl_is_typedef ||
		((decl_name == NullString) && decl_is_elaborated))
	    {
		declarator_type->add_stub_modifier(decl_marshall);
	    } else
	    {
		error_stream << "'" << decl_marshall
		             << "' illegal in this declaration";
		error(error_stream);
		decl_marshall = STUB_PENDING;
	    }
	}
    }
}

#ifdef NO_INLINES
#  define DECL_CC_
#  include "Decl.n"
#  undef DECL_CC_
#endif
