/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncT.cc,v 1.35 1993/04/14 13:46:11 ngdp Exp $
 */

/*
 * Stub generator function definition handler
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

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef FUNCT_H_
#  include "FuncT.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
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

static const char RCSid[] = "$Id: FuncT.cc,v 1.35 1993/04/14 13:46:11 ngdp Exp $";

/*
 * PUBLIC operations
 */

/*
 * Construct a function type. Note that at the time we construct this
 * in the parse we typically do not know the name of the function - 
 * it gets added later
 */

FctType::FctType ( ClassType *inclass,
		   DeclarationList *args, FunctionType ftyp ) 
                 : TypeDescriptor(FUNCTION_TYPE),
		   args_pending(args),
		   ftype(ftyp),
		   func_body(0),
		   func_clientname(NullString),
		   func_clientvisibility(STUB_PENDING),
		   func_frozen(FALSE),
		   func_hashval(0),
		   func_implname(NullString),
		   func_inline(FALSE),
		   func_ismemberof(inclass),
		   func_name(NullString),
		   func_pure(FALSE),
		   func_quals(0),
		   func_return_type(0),
		   func_fake_return(0),
		   func_servername(NullString),
		   func_signature(NullString),
		   func_virtual(FALSE),
		   has_varargs(FALSE),
		   has_result_args(FALSE)
{
#ifdef DEBUG
    debug_stream << "FctType::FctType (" << (void *)inclass
	<< "," << (void *)args << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

FctType::~FctType ()
{
}

void FctType::make_pure ()
{
    func_pure = TRUE;
}

/*
 * Warning. Ordering of functions calls here is tricky due to when
 * things are set up by complete/freeze_decl.
 */

void FctType::set_arglist ( DeclarationList *d )
{
#ifdef DEBUG
    debug_stream << "FctType::set_arglist (" << (void *)d
	<< ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    DeclarationListIterator next(d);
    register Declaration *decl;
    long parnum = 0;

    if (!func_frozen)
    {
	if ((ftype == DESTRUCTOR) && (d != 0))
	    error("destructors have no arguments");
	else
	    while (decl = next())
	    {
		String name = decl->get_name();
		TypeDescriptor *typ = decl->get_type();

		if ((name == NullString))
		    decl->set_name(constructname("_Par_", parnum++));

		/* Add to arg list NOW */

		arglist.putin(decl);

		if (typ != 0)
		    func_signature += typ->signature();
	    }
    }
    else
	error("(internal) function already has an argument list");
}

void FctType::set_quals ( TypeDescriptor *qt )
{
#ifdef DEBUG
    debug_stream << "FctType::set_quals(" << (void *)qt << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (func_quals == 0)
	func_quals = qt;
    else
	error("function already has qualifiers");
}

void FctType::add_func_modifier ( FuncModifier new_fm )
{
#ifdef DEBUG
    debug_stream << "FctType::add_func_modifier("  << (void *)this
                 << "," << new_fm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (new_fm == FUNC_INLINE)
        func_inline = TRUE;
    else
        func_virtual = TRUE;
}

/*
 * Function initialisers are treated as if they added the inline attribute
 */

void FctType::add_body ( Expression *body )
{
    func_inline = TRUE;
    if (func_body == 0)
    {
	func_body = body;
    }
    else
	error("function already has a definition");
}

void FctType::add_name ( const String& fname )
{
#ifdef DEBUG
    debug_stream << "FctType::add_name(" << (void *)this <<
	", " << fname << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (func_name == NullString)
    {
	func_name = fname;
	func_clientname = fname;
	func_servername = fname;
	func_implname = fname;

	if (ftype == CONSTRUCTOR)
	    func_signature = "__ct";
	else
	    if (ftype == DESTRUCTOR)
	    {
		func_signature = "__dt";
	    }
	    else
		func_signature = fname;

	if (fname(0, 8) == "operator")
	{
	    ftype = OPER;
	    func_servername = "op__";
	}

	func_signature += "__";

	if (func_ismemberof != 0)
	{
	    func_signature += func_ismemberof->typename(NORMAL_DECL);
	    if ((ftype == CONSTRUCTOR) || (ftype == DESTRUCTOR))
	    {
		func_name = func_ismemberof->typename(NORMAL_DECL);
		func_implname = NullString;
		func_clientname = NullString;
	    }
	}

	func_signature += "F";
    
	if (args_pending != 0)
	{
	    set_arglist(args_pending);
	    delete args_pending;
	    args_pending = 0;
	}
	else
	    func_signature += "v";

    }
}

void  FctType::add_stub_modifier ( StubModifier sm )
{
#ifdef DEBUG
    debug_stream << "FctType::add_stub_modifier("  << sm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
	if (func_clientvisibility == sm)
	    return;
	else
	    if (func_clientvisibility == STUB_PENDING)
	    {
		if ((sm == STUB_REMOTE) && (ftype == OPER))
		{
		    error("operator functions cannot be remotely called");
		    sm = STUB_NOCLIENT;
		}
		func_clientvisibility = sm;
		return;
	    }
    default:
	break;
    }
    
    TypeDescriptor::add_stub_modifier(sm);
}

/*
 * Note we ignore the incoming first parameter
 */

String FctType::massage_name ( const String&, DeclStyle ds ) const
{
    return typename(ds);
}

TypeDescriptor *FctType::combine_types ( TypeDescriptor *nt )
{
#ifdef DEBUG
    debug_stream << "FctType::combine_types(" << (void *)nt << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (!func_frozen)
    {
	if (func_return_type == 0)
	    func_return_type = nt;
	else
	    func_return_type = func_return_type->combine_types(nt);
    }
    else
	error("(internal) function return type cannot be adjusted further");

    return this;

}

StubModifier FctType::default_location () const
{
#ifdef DEBUG
    /* 
     * The void * const cast in the next statement should really be to
     * a const void * const but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << "FctType::default_location(" << (void * const)this << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return func_clientvisibility;
}

StubModifier FctType::default_marshall () const
{
#ifdef DEBUG
    debug_stream << "FctType::default_marshall(" << (void * const)this << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return TypeDescriptor::default_marshall();
}

StubModifier FctType::default_par_transfer () const
{
#ifdef DEBUG
    debug_stream << "FctType::default_par_transfer(" << (void * const)this << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return TypeDescriptor::default_par_transfer();
}

void FctType::freeze_type ()
{
#ifdef DEBUG
    debug_stream << "FctType::freeze_type(" << (void *)this << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    register Declaration *decl;
    int parnum = 0;
    ostrstream conv_strm;
    
    if (func_frozen)
	return;
    /* Set up stub specific options */

    if (func_clientvisibility == STUB_PENDING)
    {
	if (ftype != OPER)
	{
	    if (func_ismemberof != 0)
		func_clientvisibility = func_ismemberof->default_location();
	    else
		func_clientvisibility = STUB_NOREMOTE;
	}
	else
	    if (func_ismemberof != 0)
		func_clientvisibility = STUB_NOCLIENT;
	    else
		func_clientvisibility = STUB_NOREMOTE;
    }
    /* Check return type and freeze it */

    if (ftype == NORMAL && func_return_type == 0)
    {
	error_stream << "return type defaulting to 'int'";
	warning(error_stream);
	func_return_type = new IntType();
    }
    
    if (func_return_type)
    {
	func_return_type->freeze_type();
	if (func_clientvisibility == STUB_REMOTE)
	{
	    String tName = func_return_type->typename(NORMAL_DECL);

	    if ((tName != "void ") && (tName != "const void ") &&
		!func_return_type->is_transmissible())
	    {
		error_stream << "return type of ";
		if (func_ismemberof)
		    error_stream << func_ismemberof->typename(NORMAL_DECL) << "::";

		error_stream << typename(NORMAL_DECL) << " cannot be marshalled";
		error(error_stream);
	    }
	}
    }

    /* Handle arglist freezing */

    while ((decl = arglist.get_member(parnum++)) != 0)
    {
	TypeDescriptor *td = decl->get_type();	
	String name = decl->get_name();
	Boolean canMarshall = TRUE;

	decl->freeze_decl();
		
	switch(decl->get_par_transfer())
	{
	case STUB_ARG_INOUT:
	case STUB_ARG_OUT:
	    has_result_args = TRUE;	/* BEWARE deliberate fall through */
	case STUB_ARG_IN:
	    canMarshall = td->is_transmissible();
	    break;
	default:
	    canMarshall = FALSE;
	    break;
	}

	if ((func_clientvisibility == STUB_REMOTE) && !canMarshall)
	{
	    error_stream << "argument " << parnum << " of ";
	    if (func_ismemberof)
		error_stream << func_ismemberof->typename(NORMAL_DECL) << "::";
	    error_stream << func_name << " cannot be marshalled";
	    error(error_stream);
	}
	
	if (name == "...")
	    has_varargs = TRUE;
    }

    if (func_pure && !func_virtual)
    {
	error_stream << "only virtual functions can be pure";
	error(error_stream);
	func_pure = FALSE;
    }
    func_hashval = hashit(func_signature);
    
    conv_strm << func_servername << (func_hashval) << '\0';

    /* The call on str() prevents the buffer being deleted */

    func_servername = conv_strm.str();

    /* And this allows it to be deleted after all ! */

    conv_strm.rdbuf()->freeze(0);

    if ((func_ismemberof != 0) &&
	((ftype == CONSTRUCTOR) || (ftype == DESTRUCTOR)))
    {
	func_clientname = func_ismemberof->get_clientname();
	func_implname = func_ismemberof->get_implname();
    }

    func_frozen = TRUE;
}

void FctType::attach_base_type ( TypeDescriptor *base_type )
{
#ifdef DEBUG
    debug_stream << "FctType::attach_base_type(" << (void *)this << ", " << (void *)base_type << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (func_return_type)
	func_return_type->attach_base_type(base_type);
    else
	func_return_type = base_type;
}

Boolean FctType::is_fundamental () const
{
    return FALSE;
}

Boolean FctType::is_pure () const
{
    return func_pure;
}

Boolean FctType::is_transmissible () const
{
    return FALSE;
}

String FctType::signature () const
{
    return func_signature;
}

String FctType::typename ( DeclStyle ds ) const
{ 
    switch (ds)
    {
    case SERVER_DECL:
	return func_servername;
    case NORMAL_DECL:
	return func_name;
    case SERVERIMPLEMENTATION_DECL:
	return func_implname;
    case CLIENT_DECL:
	return func_clientname;
    }
}

Boolean FctType::print_client_code ( ostream& s )
{
    Boolean func_stub_ok = TRUE;
    String classname = func_ismemberof->get_clientname();
    TypeDescriptor *type = 0;
    Boolean wasvirtual = func_virtual;
    Boolean real_return = FALSE;

    /* If function is pure don't want a definition */

    if (func_pure)
	return TRUE;

    /* Check that we can produce stub code for this operation */

    if ((ftype == OPER) || 
	(ftype == CONVERSION) ||
        has_varargs)
    {
        error_stream << "stub code production for ";
	if (ftype == CONVERSION)
            error_stream << "conversion operation ";

	if (func_ismemberof)
	    error_stream << func_ismemberof->typename(NORMAL_DECL) << "::";
	if (ftype == CONVERSION)
            error_stream << "operator ";

	error_stream << typename(NORMAL_DECL);
	error_stream << " not supported";
	
        if(has_varargs)
            error_stream << " due to variable length argument list";

        error(error_stream);
        return FALSE;
    }

    /* Determine if we have a genuine return type */

    if (func_return_type)
    {
	if (!func_return_type->is_fundamental())
	{
	    error_stream << "reference/pointer return value may leak memory";
	    warning(error_stream);
	}

	if (func_return_type->typename(NORMAL_DECL) != "void ")
	{
	    real_return = TRUE;
	    if (!func_return_type->is_transmissible())
	    {
		return FALSE;
	    }
	}
    }

    /* OK - First produce the function header */

    s << "\n";

    func_virtual = FALSE;		/* temporarily remove status */

    print_bt(s, CLIENT_DECL, FALSE, FALSE);	/* print return type  */

    func_virtual = wasvirtual;		/* restore virtual status */
    s << classname << "::";             /* class :: */

    switch (ftype)
    {
    case DESTRUCTOR:
        s << "~";			/* NOTE fall through */
    case CONSTRUCTOR:
	s << func_ismemberof->get_clientname();
	break;
    default:
	s << func_name;
    }

    print_arglist(s, CLIENT_DECL, FALSE, TRUE, TRUE);	/* augmented argument list */

    if (func_quals)
    {
	s << " ";
	func_quals->print_bt(s, CLIENT_DECL, FALSE, FALSE);
    }

    if (ftype == CONSTRUCTOR)
    {
	/* Constructor - generate member init list */

	BaseClass *blist = func_ismemberof->get_vbases();

	s << ": ";

	/* Do virtual bases first */

	while (blist)
	{
	    ClassType *cl = blist->get_class();
	    
	    s << cl->get_clientname() << "(crpc, RpcBuffer(), 0)";
	    blist = blist->get_next();
	    s << ", ";
	}

	/* Now non-virtual bases */

	blist = func_ismemberof->get_bases();

	while (blist)
	{
	    ClassType *cl = blist->get_class();
	    
	    s << cl->get_clientname() << "(crpc, RpcBuffer(), 0)";
	    blist = blist->get_next();
	    s << ", ";
	}

	/* finally class members */

	s << "_client_handle (crpc ? crpc : "; 
	{
	    String sname = GlobalInfo::get_servername();

	    if (sname == NullString)
		sname = func_servername;

	    s << "new RpcControl(\"";
	    
	    s << sname << "\")";
	}
	s << "), _do_delete(1), ";
	s << "_myhash_val(-1)";
    }

    /* Now produce the function body */

    s << "\n{\n";

    /* First the declarations */

    declare_client_locals(s);
    
    if (arglist.get_no_of_entries() != 0)     /* pack up any args */
    {
        func_stub_ok = pack_call_arglist(s);
    }

    s << "\n";

    print_callto_server(s);

    /* General function processing. Unpack any return args and the */
    /* return type if it exists */

    s << "\n\tif (rpc_status == OPER_DONE && server_status == 0)\n";
    s << "\t{\n";

    if (has_result_args || real_return)
    {
        if (real_return)
        {
            s << "\t";
	    func_fake_return->print_unmarshalling_code(s, "returned_value", "rbuffer");
        }
        if (has_result_args)
            unpack_arglist(s);
    }

    s << "\t}\n\telse\n\t\trpc_abort();\n\n";

    /* If op has return value, return it now */

    if (real_return)
    {
        Type returnkind;
        int bracelevel = 1;

        type = func_return_type;
        s << "\treturn (";
        returnkind = func_return_type->kind();
#if 0
        while ((returnkind == POINTER_TYPE) ||
               (returnkind == REFERENCE_TYPE)) /* ensure correct return type */
        {
            if (returnkind == POINTER_TYPE)
            {
                s << "&(";               /* ie address of local var */
                bracelevel++;
            }
            type = type->deref();
            returnkind = type->kind();
        }
#endif
        s << "returned_value";
        for (int i = 0; i < bracelevel; i++)
            s << ")";
        s << ";\n";
    }

    /* Destructor - ensure server termination if needed */

    if (ftype == DESTRUCTOR)
    {
	s << "\tif(_do_delete)\n\t\tdelete _client_handle;\n";
    }
    /* Now the operation end brace, and we're done */

    s << "}\n";

    return func_stub_ok;

}

void FctType::print_client_instance ( ostream& s, const String&, Boolean b ) 
{
    print_bt(s, CLIENT_DECL, b, FALSE);
    prename(s, CLIENT_DECL);
    s << func_clientname;
    postname(s, CLIENT_DECL);
}

void FctType::print_typedef ( ostream& s, DeclStyle ds ) 
{
    print_bt(s, ds, FALSE, FALSE);
    prename(s, ds);
    postname(s, ds);
}

/*
 * prevar for functions has to print out the function modifiers (virtual, etc) 
 * and the return type only.
 */

void FctType::print_bt ( ostream& s, DeclStyle ds, Boolean elaborated, Boolean ) 
{
    if (func_inline && ds != CLIENT_DECL)
	s << "inline ";
    if (func_virtual)
	s << "virtual ";
    
    if (func_return_type != 0 && ftype != CONVERSION)
    {
	func_return_type->print_bt(s, ds, elaborated, FALSE);
	func_return_type->prename(s,ds);
    }
}

void FctType::prename ( ostream& s, DeclStyle )
{
    if (ftype == DESTRUCTOR)
	s << "~";
    else
	if (ftype == CONVERSION)
	    s << "operator ";
}

/*
 * postname handles the printing of the actual argument list.
 */

void FctType::postname ( ostream& s, DeclStyle ds ) 
{
    switch (ds)
    {
    case CLIENT_DECL:
	print_arglist(s, ds, TRUE, FALSE, TRUE);
	break;
    case SERVER_DECL:
	break;
    case SERVERIMPLEMENTATION_DECL:
	print_arglist(s, ds, func_inline?FALSE:TRUE, FALSE, FALSE);
	break;
    case NORMAL_DECL:
	print_arglist(s, ds, func_inline?FALSE:TRUE, FALSE, FALSE);

    }

    if (func_quals)
    {
	s << " ";
	func_quals->print_bt(s, NORMAL_DECL, FALSE, FALSE);
    }

    if (func_pure)
	s << "= 0";

    if (func_return_type)
	func_return_type->postname(s,ds);

}

void FctType::print_arglist ( ostream& s, DeclStyle ds, Boolean noname, 
			      Boolean nodefault, Boolean special )
{
    s << " (";
    if (arglist.get_no_of_entries() != 0)
    {
	Declaration *decl;
	String name;
	int limit = arglist.get_no_of_entries();
	int i = 0;

	while (decl = arglist.get_member(i++))
	{
	    name = decl->get_name();
	    if (name == "...")
		decl->print_decl(s, NORMAL_DECL, " ", FALSE, nodefault); /* print regardless */
	    else
	    {
		const char *sep = ( i < limit ? ", " : "");

		decl->print_decl(s, ds, sep, noname, nodefault);
	    }
	}
    }
    if (ftype == CONSTRUCTOR && special)
    {
	if (arglist.get_no_of_entries() != 0)
	    s << ", ";
	s << "RpcControl *crpc ";
	if (!nodefault)
	{
	    s << " = 0";
	}
    }
    s << ")";
}

void FctType::print_callto_server ( ostream& s )
{
    s << "\trpc_status = _get_handle()->Call(" << func_hashval;
    s << ", cbuffer, server_status, rbuffer);\n";

}


/*
 * Pack the argument list (if necessary). Return a Boolean value which
 * indicates whether the par list was packed ok. Arguments are packed via
 * the operator << applied to the rpc buffer. If the type is a pointer
 * or reference special things happen to avoid packing the same object more
 * than once and to preserve referential integrity.
 */

Boolean FctType::pack_call_arglist ( ostream& s )
{
    Declaration *decl;
    int parnum = 0;
    Boolean all_ok = TRUE;
    StubModifier decl_sm;

    while ((decl = arglist.get_member(parnum)) != 0) /* iterate over arglist */
    {
	decl_sm = decl->get_par_transfer();

        if ((decl_sm == STUB_ARG_IN) || (decl_sm == STUB_ARG_INOUT))
        {
            decl->print_marshall_code(s, "cbuffer");
        } 
	else
        {
            if (all_ok)
                all_ok = FALSE;
            error_stream << "argument " << parnum+1 << " of ";
	    if (func_ismemberof)
		error_stream << func_ismemberof->typename(NORMAL_DECL) << "::";
	    error_stream << func_name << " cannot be marshalled";
	    error(error_stream);
        }
        parnum++;
    }
    return all_ok;
}

void  FctType::pack_result_arglist ( ostream& s )
{
    Declaration *decl;
    int parnum = 0;
    StubModifier decl_sm;

    while ((decl = arglist.get_member(parnum)) != 0) /* iterate over arglist */
    {
	decl_sm = decl->get_par_transfer();
        if ((decl_sm == STUB_ARG_OUT) || (decl_sm == STUB_ARG_INOUT))
	{
	    Boolean deleteDummy = FALSE;
	    TypeDescriptor *type, *dummytype;
	    Type argkind;

	    type = decl->get_type();
	    argkind = type->kind();

	    if  ((argkind == REFERENCE_TYPE))
	    {
		dummytype = type->deref();
		
		if (dummytype->is_fundamental())
		{
		    deleteDummy = TRUE;
		    type = new PointerType(POINTER_TYPE, dummytype);
		}
	    }

            type->print_marshalling_code(s, decl->get_name(), "result");
	    if (deleteDummy)
		delete type;
	}
        parnum++;
    }
}

/*
 * Unpack the argument list (if necessary). Inverse of packarglist that
 * uses the operator >> of rpc buffers to retreive the value.
 */

void FctType::unpack_arglist ( ostream& s )
{
    Declaration *decl;
    StubModifier decl_sm;
    int parnum = 0;

    while ((decl = arglist.get_member(parnum)) != 0) /* iterate over arglist */
    {
	decl_sm = decl->get_par_transfer();
        if ((decl_sm == STUB_ARG_OUT) ||
            (decl_sm == STUB_ARG_INOUT))
        {
	    s << "\t";
            decl->print_unmarshall_code(s, "rbuffer");
        }
        parnum++;
    }
}

void FctType::declare_client_locals ( ostream& s )
{
    TypeDescriptor *type = 0;

    s << "\tRpcBuffer cbuffer(_myhash_val);\n";
    s << "\tRpcBuffer rbuffer;\n";
    s << "\tRPC_Status rpc_status = OPER_UNKNOWN;\n";
    s << "\tlong server_status = 0;\n\n";

    /* return type (if any). Take care - if the return type is a pointer */
    /* then it has to be dereferenced to get the actual type to declare and */
    /* then a pointer to that type returned. Such return values are */
    /* created via 'new' to ensure that the value persists after the end of */
    /* the  function */

    /* Cfront 2.1 can't cope with && here... */
    if (func_return_type != 0)
	if (func_return_type->typename(NORMAL_DECL) != "void ")
	{
	    Boolean use_new = FALSE;
	    Type rv_kind;
	    
	    type = func_return_type;
	    rv_kind = type->kind();

	    /* Discard 'const' flag if present (otherwise cannot marshall) */

	    s << "\t";

	    if ((rv_kind == CV_TYPE) || (rv_kind == POINTER_TYPE) || (rv_kind == REFERENCE_TYPE))
	    {
		if ((rv_kind == REFERENCE_TYPE))
		    use_new = TRUE;
		
		while ((rv_kind == REFERENCE_TYPE) ||
		       (rv_kind == CV_TYPE))
		{
		    /* do the deref */
		    type = type->deref();
		    rv_kind = type->kind();
		}
	    }

	    /* Set up the fake return type */
	    func_fake_return = type;

	    type->print_instance(s, NullString, CLIENT_DECL, TRUE, TRUE);
	    if (use_new)
	    {
		s << "& returned_value = * new ";
		s << type->typename(CLIENT_DECL);
	    }
	    else
		s << "returned_value";

	    type->printDefaultInit(s);

	    s << ";\n";
	}
}

void FctType::print_server_instance ( ostream&,
				      const String&,
				      Boolean )
{
}

void FctType::print_instance ( ostream& s,
			       const String& n,
			       DeclStyle ds,
			       Boolean b,
			       Boolean ic )
{
    switch (ds)
    {
    case SERVER_DECL:
	s << "long " << func_servername;
	s << "(" << func_ismemberof->get_implname() << " *, RpcBuffer&, RpcBuffer&)";
	break;
    case NORMAL_DECL:
    case SERVERIMPLEMENTATION_DECL:
	TypeDescriptor::print_instance(s,n,ds,b,ic);
	break;
    case CLIENT_DECL:
	print_client_instance(s,n,b);
    }
}

/*
 * Produce stub code for server side operations. Each function first creates
 * local variables for the arguments to the real function and unpacks them
 * from the supplied RpcBuffer. The 'real' call is then made and the result
 * RpcBuffer constructed and returned.
 * If the parameters were references then special things happen. In particular
 * references to fundamental types get sent as pointers - so have to be declared
 * as such here. References to pointers do not need such special treatment and so get
 * left alone
 */

Boolean FctType::print_server_code ( ostream& s )
{
    Boolean realreturn = (func_return_type != 0 ? TRUE : FALSE);
    Boolean cleanup = FALSE;
    int parnum = 0;
    Declaration *decl, *argdecl;
    Type argkind;
    TypeDescriptor *type = 0;
    
    /* No code possible for pure functions */
    
    if (func_pure)
	return TRUE;
    
    if (realreturn)
	if (func_return_type->typename(NORMAL_DECL) == "void ")
	    realreturn = FALSE;
    
    if ((ftype == OPER) || (ftype == CONVERSION) || has_varargs)
        return FALSE;
    
    /* First produce the function header */
    
    s << "long ";                       /* return type */
    s << func_ismemberof->get_servername() << "::"; /* class :: */
    s << func_servername << " (";
    s << func_ismemberof->typename(NORMAL_DECL);
    s << " *theobject, RpcBuffer& work, RpcBuffer& result)";
    
    /* Now produce the function body */
    
    s << "\n{\n";
    
    /* Produce the local variables for the parameters */
    
    if (arglist.get_no_of_entries() != 0)
    {
	Boolean deleteDummy;
	TypeDescriptor *dummytype = 0;
        Type argkind;
        String name;

	parnum = 0;

        while ((argdecl = arglist.get_member(parnum++)) != 0)
        {
            s << "\t";
	    deleteDummy = FALSE;
            dummytype = type = argdecl->get_type();
            name = argdecl->get_name();
            argkind = type->kind();

	    if ((argkind == POINTER_TYPE) || (argkind == REFERENCE_TYPE))
		cleanup = TRUE;

	    /* Treat references to fundamental types as pointers in server code! */

	    if (argkind == REFERENCE_TYPE)
	    {
		TypeDescriptor *t = type->deref();

		if (t->is_fundamental())
		{
		    deleteDummy = TRUE;
		    dummytype = new PointerType(POINTER_TYPE, t);
		}
		else
		    dummytype = t;
	    }
		
            dummytype->print_instance(s, name, SERVERIMPLEMENTATION_DECL, FALSE, TRUE);
	    dummytype->printDefaultInit(s);
	    
            s << ";\n";
	    
            dummytype->print_unmarshalling_code(s, name, "work");
	    if (deleteDummy)
		delete dummytype;
        }
    }
    s << "\n";
    
    if (ftype == DESTRUCTOR)            /* real object destruction */
    {
        s << "\tdelete theobject;\n";
        s << "\ttherealobject = 0;\n";
    } else
    {
        if (ftype == CONSTRUCTOR)       /* requires realobject construction */
        {
            s << "\ttherealobject = new " << func_ismemberof->typename(NORMAL_DECL);
        } else
        {                               /* normal operation */
            s << "\t";
            if (realreturn)
	    {
                func_return_type->print_instance(s, "returned_value", NORMAL_DECL, FALSE, FALSE);
		s << " = ";
	    }
	    
            s << "theobject->" << func_name;
        }
	
        s << "(";                       /* open parlist */
	
	parnum = 0;
        while (decl = arglist.get_member(parnum++))
        {
            type = decl->get_type();
            argkind = type->kind();

	    /* REMEMBER. References to fundamental types passed as pointers => need deref */

            if ((argkind == REFERENCE_TYPE) && (type->deref()->is_fundamental()))
            {
		s << "*";           /* ie dereference address of local var */
            }
            s << decl->get_name();
            if (parnum < arglist.get_no_of_entries())
                s << ", ";
        }
        s << ");";
    }
    s << "\n";
    
    /* Call has been made, see if any arguments and/or a return value is */
    /* indicated  */
    
    if (has_result_args || realreturn)
    {
        if (realreturn)
        {
	    func_return_type->print_marshalling_code(s, "returned_value", "result");
        }
        if (has_result_args)
            pack_result_arglist(s);
    }
    
    /* Cleanup up any allocated variables */

    if (cleanup)
    {
        Type argkind;
        String name;

	parnum = 0;
	s << "\n\tlong dummy = 0;\n";
	s << "\n\twork.clearMap();\n\n";	
        while ((argdecl = arglist.get_member(parnum++)) != 0)
        {
            type = argdecl->get_type();
            name = argdecl->get_name();
            argkind = type->kind();

            if ((argkind == REFERENCE_TYPE) || (argkind == POINTER_TYPE))
            {

		s << "\tif(work.addPtr(" << name << ", dummy))\n";
		s << "\t\tdelete " << name << ";\n";
	    }
        }
    }
    s << "\n";
    
    
    s << "\treturn OPER_DONE;\n";
    s << "}\n\n";
    return TRUE;
}

Boolean FctType::print_code ( ostream& s, DeclStyle ds )
{
    switch (ds)
    {
    case CLIENT_DECL:
	print_client_code(s);
	break;
    case SERVER_DECL:
	print_server_code(s);
	break;
    case SERVERIMPLEMENTATION_DECL:
    case NORMAL_DECL:
	break;
    }
    return TRUE;
}

void FctType::print_marshalling_code ( ostream&, const String&,
				       const String&)
{
}
    

void FctType::print_unmarshalling_code ( ostream&, const String&,
				         const String&)
{
}

#ifdef NO_INLINES
#  define FUNCT_CC_
#  include "FuncT.n"
#  undef FUNCT_CC_
#endif
