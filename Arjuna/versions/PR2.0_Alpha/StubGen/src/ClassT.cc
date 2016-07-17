/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassT.cc,v 1.29 1993/04/14 13:46:02 ngdp Exp $
 */

/*
 * Stub generator class definition handler
 *
 */

#include <strstream.h>
#include <fstream.h>

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

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
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

#ifndef INTERFACE_H_
#  include "Interface.h"
#endif

extern ClassType *currentclass;

static const char RCSid[] = "$Id: ClassT.cc,v 1.29 1993/04/14 13:46:02 ngdp Exp $";

#ifndef MAINNAME
#  define MAINNAME "main"
#endif

#ifdef SHORTNAMES
static const String client_hdrsuff("_S.h");
static const String class_hdrsuff("_SC.h");
static const String client_codesuff("_C.cc");
static const String server_codesuff("_S.cc");
static const String serverm_codesuff("_SM.cc");
#else
static const String client_hdrsuff("_stub.h");
static const String class_hdrsuff("_stubclass.h");
static const String client_codesuff("_client.cc");
static const String server_codesuff("_server.cc");
static const String serverm_codesuff("_servermain.cc");
#endif

static const String implpref("Local");
static const String remotepref("Remote");
static const String serverpref("Server");

/*
 * PUBLIC operations.
 */

/*
 * Constructor
 */

ClassType::ClassType ( const String& name, ClassSpecifier ckind,
		       BaseClass *bases, DeclContext *parent,
		       Boolean nameless, Boolean will_be_defn )
                     : TypeDescriptor(AGGREGATE_TYPE),
		       base_list(0),
		       derive_list(bases),
		       vbase_list(0),
		       class_ctors(0),
		       class_dtor(0),
		       class_clientname(remotepref + name),
		       class_complete(FALSE),
		       class_full_decl(0),
		       class_hashval(hashit(name)),
		       class_implname(implpref + name),
		       class_is_remote(TRUE),
		       class_kind(ckind),
		       class_members(new ClassDeclContext(this,parent,bases)),
		       class_marshall_opts(STUB_PENDING),
		       class_name(name),
		       class_nameless(nameless),
		       class_servername(serverpref + name),
		       class_stubloc(STUB_PENDING),
		       frozen(FALSE),
		       hasconstructor(FALSE),
		       hasdestructor(FALSE)
{
#ifdef DEBUG
    debug_stream << "ClassType::ClassType(" << (void *)this << "," << name
	         << "," << (void *)bases << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (ckind ==  CLASS_TYPE)
    {
	def_visibility = CLASS_PRIVATE;
	cur_visibility = CLASS_PRIVATE;
    }
    else
    {
	def_visibility = CLASS_PUBLIC;
	cur_visibility = CLASS_PUBLIC;
    }

    /* Calculate ALL virtual bases of this class */

    if (will_be_defn)
	find_vbases ();
}

ClassType::~ClassType ()
{
}

void ClassType::add_ctor ( Declaration *ct_decl )
{
    if (class_ctors != 0)
	class_ctors->append_decl(ct_decl);
    else
	class_ctors = new DeclarationList(ct_decl);
}

void ClassType::add_dtor ( Declaration *dt_decl )
{
    if (class_dtor != 0)
    {
	error_stream << "class " << class_name <<
	    " already has a destructor\n";
	error(error_stream);
    } else
	class_dtor = dt_decl;
}

/*
 * Check if name is name of current class or ANY from which it is derived
 * Return pointer to appropriate class if so
 */

ClassType *ClassType::name_is_cname ( const String& name ) 
{
    BaseClass *bases = derive_list;
    ClassType *rval = 0;
    ClassType *nclass = 0;

    /* Is it me? */

    if (name == class_name)
	return this;

    /* check my bases */

    while ((rval == 0) && (bases != 0))
    {
	nclass = bases->get_class();
	bases = bases->get_next();
	rval = nclass->name_is_cname(name);
    }

    return rval;
}

/*
 * Add stub modifier to class definition. These are only those that 
 * were explicitly stated. Rely on freeze_type to set up defaults
 * later if need be.
 */

void ClassType::add_stub_modifier ( StubModifier sm )

{
#ifdef DEBUG
    debug_stream << "ClassType::add_stub_modifier(" << (void *)this
	         << ", " << sm <<  ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    Boolean conflicting = FALSE;
    StubModifier earlier = STUB_PENDING;
    
    switch (sm)
    {
    case STUB_REMOTE:
	earlier = class_stubloc;
	if (class_stubloc == sm)
	    return;
	else
	    if (class_stubloc == STUB_PENDING)
	    {
		if (!class_nameless)
		{
		    class_stubloc = STUB_REMOTE;
		    check_classprops(class_stubloc);
		    return;
		}
		else
		    break;
	    }
	    else
		conflicting = TRUE;
	break;
    case STUB_NOREMOTE:
	earlier = class_stubloc;
	if (class_stubloc == sm)
	    return;
	else
	    if (class_stubloc == STUB_PENDING)
	    {
		class_stubloc = STUB_NOREMOTE;
		check_classprops(class_stubloc);
		return;
	    }
	    else
		conflicting = TRUE;
	break;
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_NOMARSHALL:
	earlier = class_marshall_opts;
	if (class_marshall_opts == sm)
	    return;
	else
	    if (class_marshall_opts == STUB_PENDING)
	    {
		class_marshall_opts = sm;
		return;
	    }
	    else
		conflicting = TRUE;
	break;
    default:
	break;
    }
    
    error_stream << "Stub modifier '" << sm << "' ";
    if (conflicting)
	error_stream << "conflicts with earlier modifier '" << earlier << "'";
    else
	error_stream << " illegal in this declaration";

    error(error_stream);
}

TypeDescriptor *ClassType::combine_types ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debug_stream << "ClassType::combine_types(" << (void *)this
	         << ", " << (void *)newtype << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (class_full_decl)
	return class_full_decl->combine_types(newtype);
    else
	if (class_complete)
	{
	    /*
	     * Attempt to handle typedef struct x {} x;
	     */

	    if (newtype->kind() == kind())
		if (newtype->typename(NORMAL_DECL) == typename(NORMAL_DECL))
		    return this;

	    error_stream << "attempt to redefine " << newtype->typename(NORMAL_DECL) << " as ";
	    print_instance(error_stream, NullString, NORMAL_DECL, TRUE, FALSE);
	    error(error_stream);
	}
	else
	{
	    class_full_decl = (ClassType *)newtype;
	    if (class_stubloc != STUB_PENDING)
		newtype->add_stub_modifier(class_stubloc);
	    if (class_marshall_opts != STUB_PENDING)
		newtype->add_stub_modifier(class_marshall_opts);
	}

    return this;
}

void ClassType::change_visibility ( VisibilitySpec newvis )
{
    if (class_full_decl)
    {
	class_full_decl->change_visibility(newvis);
    }
    else
    {
	if (class_kind == CLASS_TYPE || class_kind == STRUCT_TYPE)
	    cur_visibility = newvis;
	else
	    error("cannot change visibility of union members");
    }
}

StubModifier ClassType::default_location () const
{
    if (class_full_decl)
	return class_full_decl->default_location();
    else
	return class_stubloc;
    
}

StubModifier ClassType::default_marshall () const
{
    if (class_full_decl)
	return class_full_decl->default_marshall();
    else
	return class_marshall_opts;
}

StubModifier ClassType::default_par_transfer () const
{
    return TypeDescriptor::default_par_transfer();
}

/*
 * Freeze declaration of a class type. This includes setting stub
 * generation defaults (which may be ignored if they have already been
 * set explicitly). Note that depending upon whether this object represents
 * a forward reference or a full class the defaults are different.
 */

void ClassType::freeze_type ()
{
#ifdef DEBUG
    debug_stream << "ClassType::freeze_type(" << (void *)this << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (frozen)
	return;

    frozen = TRUE;

    if (class_full_decl)
    {
	if (class_stubloc != STUB_PENDING)
	    class_full_decl->add_stub_modifier(class_stubloc);
	
	if (class_marshall_opts != STUB_PENDING)
	    class_full_decl->add_stub_modifier(class_marshall_opts);
	
	class_full_decl->freeze_type();

	class_stubloc = default_location();
	class_marshall_opts = default_marshall();
    }
    else
    {
	/* freeze base classes (if any) */

	if (class_complete && (derive_list != 0))
	{
	    derive_list->freeze();
	}
	
	/* Fill in default stub attributes if required */

	if (class_nameless)
	{
	    class_stubloc = STUB_NOREMOTE;
	    class_marshall_opts = STUB_NOMARSHALL;
	}

	if (class_stubloc == STUB_PENDING)
	{
	    switch (class_marshall_opts)
	    {
	    case STUB_AUTOMARSHALL:
		add_stub_modifier(STUB_NOREMOTE);
		break;
	    default:
		if (class_complete)
		    add_stub_modifier(STUB_REMOTE);
		else
		    add_stub_modifier(STUB_NOREMOTE);
		}
	}

	if (class_marshall_opts == STUB_PENDING)
	{
	    switch (class_stubloc)
	    {
	    case STUB_REMOTE:
		add_stub_modifier(STUB_NOMARSHALL);
		break;
	    default:
		add_stub_modifier(STUB_AUTOMARSHALL);
	    }
	}

	if (class_stubloc == STUB_REMOTE)
	{
	    class_clientname = remotepref + class_name;
	    class_implname = implpref + class_name;
	    class_servername = serverpref + class_name;
	} else
	    {
		class_clientname = class_name;
		class_implname = class_name;
		class_servername = class_name;
	    }

	/* Freeze member list too */

	Declaration *decl;
	int i = 0;

	while (decl = class_members->get_member(i++))
	    decl->freeze_decl();
    }    
}

Boolean ClassType::is_transmissible () const
{
    if (class_full_decl)
	return class_full_decl->is_transmissible();
    else
	return (((class_marshall_opts == STUB_AUTOMARSHALL) ||
		 (class_marshall_opts == STUB_USERMARSHALL)) ? TRUE : FALSE);
}

void ClassType::make_complete ()
{
    if (class_full_decl)
	class_full_decl->make_complete();
    else
	class_complete = TRUE;
}

/*
 * Class output routines.
 */

void ClassType::print_bt ( ostream& s, DeclStyle ds, Boolean elaborated,
			   Boolean ignoreConst )
{
    if (class_full_decl)
	class_full_decl->print_bt(s,ds,elaborated, ignoreConst);
    else
    {
	if (elaborated)
	    switch (class_kind)
	    {
	    case CLASS_TYPE:
		s << "class ";
		break;
	    case STRUCT_TYPE:
		s << "struct ";
		break;
	    case UNION_TYPE:
		s << "union ";
		break;
	    }
    
	s << typename(ds);
    }
    s << " ";
}

/*
 * Only gets called for full class declarations so no need to check
 * full_decl pointer
 */

void ClassType::print_client_typedef ( ostream& s )
{
    int i = 0;
    Declaration *decl;
    
    if (class_stubloc == STUB_REMOTE)
    {        
	s << "\n#ifndef " << class_name.touppercase() << "_SERVER\n";
        s << "#undef " << class_name << "\n";
        s << "#define " << class_name << " ";
	s << class_clientname << "\n";
        s << "#endif\n";

        s << "\n";
	print_header(s, CLIENT_DECL);

	s << "\n{\n";
	
	/* Do public member functions only */
	
	s << "public:\n";

	if (class_marshall_opts == STUB_AUTOMARSHALL)
	{
	    s << "\tvirtual void marshall (RpcBuffer&);\n";
	    s << "\tvirtual void unmarshall (RpcBuffer&);\n";
	    s << "\n";
	}

	while (decl = class_members->get_member(i++))
	{
	    if (decl->is_typedef() ||
		((decl->get_visibility() == CLASS_PUBLIC) &&
		 (decl->get_remote() == STUB_REMOTE)))
	    {
		Type dkind = decl->get_type()->kind();

		if ((dkind == FUNCTION_TYPE) || decl->is_typedef())
		{
		    s << "\t";
		    decl->print_decl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
		}
		else
		{
		    error_stream << "ignoring non-function public member ";
		    error_stream << class_name << "::";
		    error_stream << decl->get_name();
		    error(error_stream);
		}
	    }
	}

	s << "protected:\n";
	s << "\t" << class_clientname << "(RpcControl *, const RpcBuffer&, char);\n";
	if (class_dtor == 0)
	{
	    /* generate default */
	    s << "\tvirtual ~" << class_clientname << "();\n";
	} else
	    if (class_dtor->get_visibility() == CLASS_PROTECTED)
	    {
		s << "\t";
		class_dtor->print_decl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
	    }

	s << "private:\n";

	if (class_dtor != 0 &&
	    (class_dtor->get_visibility() == CLASS_PRIVATE))
	{
	    s << "\t";
	    class_dtor->print_decl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
	}

	/* virtual function to (lazily) get Rpc control pointer */

	s << "\tvirtual RpcControl *_get_handle () const;\n\n";

	/* State variables */

	s << "\tRpcControl *_client_handle;\n";
	s << "\tchar _do_delete;\n";
	s << "\tRpcBuffer _myhash_val;\n";

	s << "}";
    }
    else
    {
	print_typedef(s, NORMAL_DECL);
    }
}

void ClassType::print_server_typedef ( ostream& s )
{
    int i = 0;
    Declaration *decl;

    if (class_stubloc != STUB_NOREMOTE)
    {
        s << "\n#ifdef " << class_name.touppercase() << "_SERVER\n";
        s << "#undef " << class_name << "\n";
        s << "#define " << class_name << " ";
	s << class_implname << "\n";
        s << "#endif\n";


        s << "\n";

	print_header(s, SERVERIMPLEMENTATION_DECL, FALSE);
	s << ";\n\n";

        print_header(s, SERVER_DECL);

        s << "\n{\n";
        s << "\npublic:\n";

        s << "\t" << class_servername << " ();\n";
        s << "\t~" << class_servername << " ();\n\n";
        s << "\tvoid Server (int, char **);\n";
        s << "\tlong DispatchToClass (" << class_implname;
	s << "*,long, RpcBuffer&, RpcBuffer&);\n\n";

	s << "private:\n";
	s << "\tlong DispatchToOper (" << class_implname;
	s << " *,long, RpcBuffer&, RpcBuffer&);\n\n";

        while (decl = class_members->get_member(i++))
        {
            if ((decl->get_visibility() == CLASS_PUBLIC) &&
		(decl->get_remote() == STUB_REMOTE))
            {
                if (decl->get_type()->kind() == FUNCTION_TYPE)
                {
                    s << "\t";
                    decl->print_decl(s, SERVER_DECL);
                }
            }
        }

        s << "\n\t" << class_implname << " *therealobject;\n\n";

        s << "}";
    }
}

void ClassType::prename ( ostream&, DeclStyle )
{
}

void ClassType::print_header ( ostream& s, DeclStyle ds, Boolean full )
{
    if (class_full_decl)
    {
	class_full_decl->print_header(s, ds, full);
    }
    else
    {
	BaseClass *bc = derive_list;
	
	print_bt(s, ds, TRUE, FALSE);
	
	if (full)
	{
	    /* print out derivation info */
	    if (bc != 0)
		s << ": ";
	    
	    while (bc != 0)
	    {
		bc->print_baseclass(s, ds);
		bc = bc->get_next();
		if (bc)
		    s << ", ";
	    }
	}
    }
}

void ClassType::print_typedef ( ostream& s, DeclStyle ds )
{
    if (ds == NORMAL_DECL)
	ds = SERVERIMPLEMENTATION_DECL;

    if (class_full_decl)
	class_full_decl->print_typedef(s,ds);
    else
    {
	int i = 0;
	Declaration *decl;
	VisibilitySpec current_vis, last_vis;
	
	last_vis = def_visibility;
    
	if (ds == CLIENT_DECL)
	    print_client_typedef(s);
	else if (ds == SERVER_DECL)
	    print_server_typedef(s);
	else
	{
	    print_header(s, ds);
	    
	    if (class_complete)
	    {
		s << "\n{\n";
		
		while ((decl = class_members->get_member(i++)) != 0)
		{
		    if ((current_vis = decl->get_visibility()) != last_vis)
		    {
			last_vis = current_vis;
			switch (current_vis)
			{
			case CLASS_PRIVATE:
			    s << "private:\n";
			    break;
			case CLASS_PROTECTED:
			    s << "protected:\n";
			    break;
			case CLASS_PUBLIC:
			    s << "public:\n";
			    break;
			default:
			    break;
			}
		    }
		    s << "\t";
		    decl->print_decl(s,ds);
		}
		
		if (class_marshall_opts == STUB_AUTOMARSHALL)
		{
		    if (current_vis != CLASS_PUBLIC)
			s << "public:\n";
		    
		    s << "\tvirtual void marshall (RpcBuffer&);\n";
		    s << "\tvirtual void unmarshall (RpcBuffer&);\n";
		    s << "\n";
		}
		
		s << "}";
	    }
	}
    }
}
 
Boolean ClassType::print_client_code ( ostream& code_stream )
{    
    if (class_full_decl)
	return class_full_decl->print_client_code(code_stream);
    else
    {
	Boolean stub_ok = TRUE;
	Declaration *decl;
	int i = 0;
	
	if (class_stubloc != STUB_NOREMOTE)
	{
	    code_stream << "\n#include \""
		        << GlobalInfo::get_current_if()->getHeaderFname()
			<< "\"\n";
	    
	    print_client_specials (code_stream);

	    while (decl = class_members->get_member(i++))
	    {
		error_location = decl->get_loc();
		
		if ((decl->get_visibility() == CLASS_PUBLIC) &&
		    (decl->get_remote() == STUB_REMOTE))
		{
		    if (decl->get_type()->kind() == FUNCTION_TYPE)
		    {
			if (decl->get_remote() != STUB_NOREMOTE)
			{
			    stub_ok = decl->print_code(code_stream, CLIENT_DECL);
			    code_stream << "\n";
			}
			else
			{
			    stub_ok = FALSE;
			    error_stream << "client stub generation impossible for "
				         << decl->get_type()->typename(NORMAL_DECL);
			    error(error_stream);
			}
		    }
		}
	    }
	} else
	    stub_ok = FALSE;

	return stub_ok;
    }
}

Boolean ClassType::print_server_code ( ostream& code_stream )
{
    if (class_full_decl)
	return class_full_decl->print_server_code(code_stream);
    else
    {
	Boolean stub_ok = TRUE;
	Declaration *decl;
	int i = 0;
	
	if (class_stubloc != STUB_NOREMOTE)
	{
	    code_stream << "#define " << class_name.touppercase() << "_SERVER";
	    code_stream << "\n#include \""
		        << GlobalInfo::get_current_if()->getHeaderFname()
			<< "\"\n";
	    
	    print_server_specials (code_stream);

	    while (decl = class_members->get_member(i++))
	    {
		if ((decl->get_visibility() == CLASS_PUBLIC) &&
		    (decl->get_remote() == STUB_REMOTE))
		{
		    if (decl->get_type()->kind() == FUNCTION_TYPE)
		    {
			error_location = decl->get_loc();
			if (decl->get_remote() != STUB_NOREMOTE)
			{
			    stub_ok = decl->print_code(code_stream, SERVER_DECL);
			    code_stream << "\n";
			}
			else
			{
			    stub_ok = FALSE;
			    error_stream << "server stub generation impossible for "
				         << decl->get_type()->typename(NORMAL_DECL);
			    error(error_stream);
			}
		    }
		}
	    }
	} else
	    stub_ok = FALSE;

	return stub_ok;
    }
}

void ClassType::produce_marshall_defs ( ostream& master_hdr )
{
    if (class_full_decl)
	class_full_decl->produce_marshall_defs(master_hdr);
    else
    {
	if (class_marshall_opts != STUB_NOMARSHALL)
	{
	    ofstream marshall_stream;

	    marshall_stream.open(class_name + "_marshall.cc");
	    
	    master_hdr << "\nextern RpcBuffer& operator<< ";
	    master_hdr << "( RpcBuffer&, " << class_implname << " *);\n";
	    
	    master_hdr << "\extern RpcBuffer& operator>> ";
	    master_hdr << "( RpcBuffer&, " << class_implname << " *&);\n";

	    master_hdr << "\ninline RpcBuffer& operator<< ";
	    master_hdr << "( RpcBuffer& rpcbuff, " << class_implname << "& topack)\n";
	    master_hdr << "{\n";
	    master_hdr << "\treturn rpcbuff << &topack;\n";
	    master_hdr << "}\n";
	    
	    master_hdr << "\ninline RpcBuffer& operator>> ";
	    master_hdr << "( RpcBuffer& rpcbuff, " << class_implname << "& tounpack)\n";
	    master_hdr << "{\n";
	    master_hdr << "\t" << class_implname << "*temp = &tounpack;\n";
	    master_hdr << "\treturn rpcbuff >> temp;\n";
	    master_hdr << "}\n\n";

	    marshall_stream << "#include \""
		<< GlobalInfo::get_current_if()->getHeaderFname() << "\"\n";

	    print_marshall_defs(marshall_stream, NORMAL_DECL);
	    marshall_stream << flush;
	}
	
	master_hdr << flush;
    }
}

void ClassType::produce_stubs ( ostream& master_hdr )
{
    if (class_full_decl)
	class_full_decl->produce_stubs(master_hdr);
    else
    {
	ofstream class_header;

	class_header.open(class_name + class_hdrsuff);
	
	master_hdr << "#include \"" << class_name + class_hdrsuff;
	master_hdr << "\"\n";

	if (class_stubloc == STUB_REMOTE)
	{
	    ofstream client_code;
	    ofstream server_code;
	    ofstream server_main;

	    client_code.open(class_name + client_codesuff);
	    server_code.open(class_name + server_codesuff);
	    server_main.open(class_name + serverm_codesuff);
	    
	    /* Class descriptions */
	
	    GlobalInfo::stamp(class_header);
	    print_typedef(class_header, CLIENT_DECL);
	    class_header << ";\n\n";
	    print_typedef(class_header, SERVER_DECL);
	    class_header << ";\n\n";
	    print_typedef(class_header, SERVERIMPLEMENTATION_DECL);
	    class_header << ";\n\n";

	    /* Code Generation */
	    
	    GlobalInfo::stamp(client_code);
	    print_code(client_code, CLIENT_DECL);
	    GlobalInfo::stamp(server_code);
	    print_code(server_code, SERVER_DECL);
	    
	    /* Simple main program for server - may not be needed */
	    
	    GlobalInfo::stamp(server_main);
	    server_main << "#include \""
		<< GlobalInfo::get_current_if()->getHeaderFname() << "\"\n";
	    
	    server_main << "int " << MAINNAME << " ( int argc, char **argv )\n{\n";

	    server_main << "\t" << class_servername << " ";
	    server_main << class_name << "server;\n\n";
	    server_main << "\t" << class_name << "server.Server(argc, argv);\n";
	    server_main << "\treturn 0;\n}\n";
	    
	    client_code << flush;
	    server_code << flush;
	    server_main << flush;
	}
	else
	{
	    GlobalInfo::stamp(class_header);
	    print_typedef(class_header, NORMAL_DECL);
	}
	
	if (class_marshall_opts == STUB_AUTOMARSHALL)
	{
	    produce_marshall_defs(master_hdr);
	}
	
	class_header << flush;
    }
}

String ClassType::signature () const
{
    return class_name;
}

Boolean ClassType::stub_possible () const
{
    if (class_complete)
    {
	if (class_stubloc == STUB_REMOTE)
	    return TRUE;
    }
    return FALSE;
}

String ClassType::typename ( DeclStyle ds ) const
{    
	switch (ds)
	{
	case CLIENT_DECL:
	    return class_clientname;
	case SERVER_DECL:
	    return class_servername;
	case SERVERIMPLEMENTATION_DECL:
	    return class_implname;
	case NORMAL_DECL:
	    return class_name;
	}
}

Boolean ClassType::check_classprops ( StubModifier tocheck )
{
    if (class_full_decl)
	return class_full_decl->check_classprops(tocheck);
    else
    {
	BaseClass *bc = derive_list;
	Boolean props_ok = TRUE;

	/* Compare stub properties of parent to me */

	if (tocheck == STUB_REMOTE)
	{
	    if (class_stubloc != STUB_REMOTE)
		return FALSE;
	}

	/* ok so far - check my bases */
	while (bc != 0 && props_ok)
	{
	    if (props_ok = bc->get_class()->check_classprops(tocheck))
		bc = bc->get_next();
	    else
	    {
		error_stream << "stub properties of derived class '" << class_name;
		error_stream << "' and base class '";
		error_stream << bc->get_class()->typename(NORMAL_DECL);
		error_stream << "' are not compatible";
		error(error_stream);
		
		break;
	    }
	}

	return props_ok;
    }
}

void ClassType::print_client_specials ( ostream& s )
{
    BaseClass *blist = vbase_list;

    /* Special protected constructor */

    s << "\n" << class_clientname << "::";
    s << class_clientname;
    s << " ( RpcControl *crpc, const RpcBuffer& phv, char idodelete )";
    s << ": ";

    /* initialise virtual bases */

    while (blist)
    {
	ClassType *cl = blist->get_class();
	
	s << cl->get_clientname() << "(crpc, RpcBuffer(phv,";
	s << class_hashval << "), idodelete)";
	blist = blist->get_next();
	s << ", ";
    }

    /* initialise non-virtual bases */

    blist = base_list;

    while (blist)
    {
	ClassType *cl = blist->get_class();
	
	s << cl->get_clientname() << "(crpc, RpcBuffer(phv,";
	s << class_hashval << "), idodelete)";
	blist = blist->get_next();
	s << ", ";
    }
    
    s << "_client_handle(crpc), ";
    s << "_do_delete(idodelete), ";
    s << "_myhash_val(phv)";

    s << "\n{\n\t_myhash_val<< ";
    s << class_hashval;
    s << " << -1;\n}\n";

    if ((class_dtor == 0) || (class_dtor->get_visibility() != CLASS_PUBLIC))
    {
	s << "\n" << class_clientname << "::~" << class_clientname;
	s << " ( )\n{\n}\n";
    }

    /* Lazy rpc control routine */
    
    s << "\nRpcControl *" << class_clientname << "::";
    s << "_get_handle ( ) const\n{\n\treturn _client_handle;\n}\n";
   
}

void ClassType::print_server_specials ( ostream& s )
{
    Declaration *decl = 0;
    TypeDescriptor *type = 0;
    int declnum = 0;
    FctType destructor = FctType(this, 0, DESTRUCTOR);
    
    s << "\n" << class_servername << "::";
    s << class_servername << " ()\n{\n";
    s << "\ttherealobject = 0;\n";
    s << "}\n";

    s << "\n" << class_servername << "::~";
    s << class_servername << " ()\n{\n";

    /* only delete if destructor visible */
    decl = class_members->lookfor(cpp_linkage->mangle(&destructor), FUNCTION_TYPE, INHERITED_CONTEXT);
    if ((decl != 0) && (decl->get_visibility() == CLASS_PUBLIC))
    {
	s << "\tif (therealobject != 0)\n";
	s << "\t\tdelete therealobject;\n";
    }
    s << "}\n";

    /* now produce the operation 'Server' */

    s << "\nvoid " << class_servername << "::Server ( int argc, char **argv )\n{\n";
    s << "\tServerRpc ServerRPC;\n";
    s << "\tlong opcode, errcode;\n\n";
    s << "\tServerRPC.Initialise(argc, argv);\n\n";
    s << "\tfor (;;)\n\t{\n";
    s << "\t\tRpcBuffer work;\n";
    s << "\t\tRpcBuffer result;\n\n";
    s << "\t\tServerRPC.GetWork(opcode, work);\n";
    s << "\t\terrcode = DispatchToClass(therealobject, opcode, work, result);\n";
    s << "\t\tServerRPC.SendResult(errcode, result);\n";
    s << "\t}\n";
    s << "}\n";

    /* Now the code for 'DispatchToClass' */

    s << "\nlong " << class_servername << "::DispatchToClass ( ";
    s << class_implname << " *theobject, ";
    s << "long funccode, RpcBuffer& work, RpcBuffer& result)\n{\n";
    s << "\tlong classcode;\n\n\twork >> classcode;\n\n";
    s << "\tswitch (classcode)\n\t{\n";

    s << "\tcase -1:\n";
    s << "\t\treturn DispatchToOper(theobject, funccode, work, result);\n";

    /* This is the main loop producing the actual calls */
    {
	BaseClass *blist = vbase_list;

	/* Do virtual bases first */

	while (blist)
	{
	    ClassType *cl = blist->get_class();
	    
	    s << "\tcase " << cl->get_class_hashval() << ":\n";
	    s << "\t\treturn ";
	    s << cl->get_servername() << "::DispatchToClass(theobject, funccode, work, result);\n";
	    blist = blist->get_next();
	}

	/* Now non-virtual bases */

	blist = base_list;

	while (blist)
	{
	    ClassType *cl = blist->get_class();
	    
	    s << "\tcase " << cl->get_class_hashval() << ":\n";
	    s << "\t\treturn ";
	    s << cl->get_servername() << "::DispatchToClass(theobject, funccode, work, result);\n";
	    blist = blist->get_next();
	}

    }
    
    /* Default action if class opcode not understood. */

    s << "\tdefault:\n";
    s << "\t\treturn OPER_NOTDONE;\n";

    /* Now wrap up finally */

    s << "\t}\n";
    s << "}\n";

    /* Now the code for 'DispatchToOper' */

    s << "\nlong " << class_servername << "::DispatchToOper ( ";
    s << class_implname;
    s << " *theobject, long funccode, RpcBuffer& work, RpcBuffer& result)\n{\n";
    s << "\tswitch (funccode)\n\t{\n";

    /* This is the main loop producing the actual calls */

    while (decl = class_members->get_member(declnum++))
    {
	error_location = decl->get_loc();

        if ((decl->get_visibility() == CLASS_PUBLIC) &&
	    (decl->get_remote() == STUB_REMOTE))
        {
            type = decl->get_type();

            if (type->kind() == FUNCTION_TYPE)
            {
                FctType *funcptr;

                funcptr = (FctType *)type;      /* legit cast here */

		if (!type->is_pure())
		{
		    s << "\tcase " << funcptr->get_func_hashval() << ":\n";
		    s << "\t\treturn ";
		    s << funcptr->get_servername() << "(theobject, work, result);\n";
		}
		else
		{
		    error_stream << "no stub code produced for pure function " << type->typename(NORMAL_DECL);
		    warning(error_stream);
		}
            }
        }
    }

    s << "\tdefault:\n";
    s << "\t\treturn OPER_NOTDONE;\n";

    /* Now wrap up finally */

    s << "\t}\n";
    s << "}\n";


}

/*
 * Set up base class lists
 * derive_list is the exact list of classes we are derived from
 * base_list is only those that are not virtual
 * vbase_list is ALL virtual bases both direct AND indirect
 */

void ClassType::find_vbases ()
{
    BaseClass *blist = derive_list;
    BaseClass vbases(0);		/* DUMMY - discarded later */
    BaseClass bases(0);			/* DUMMY - discarded later */

    if (blist)
    {
	/* Walk down the specified base class list for this class */

	while (blist)
	{
	    ClassType *bclass = blist->get_class();
	    BaseClass *dblist = bclass->get_vbases();

	    /* If its virtual add it into the virtual list */

	    if (blist->is_virtual())
	    {
		vbases.append(new BaseClass(bclass, TRUE,
					    blist->get_visibility()));
	    }
	    else
		/* add to normal list */
		bases.append(new BaseClass(bclass, FALSE,
					   blist->get_visibility()));

	    /* Now add in its virtual bases (if any) also */

	    while (dblist)
	    {
		vbases.append(new BaseClass(dblist->get_class(), TRUE,
					    dblist->get_visibility()));

		dblist = dblist->get_next();
	    }

	    /* And on to the next base class */

	    blist = blist->get_next();
	}
    }

    vbase_list = vbases.get_next();		/* throw away dummy vbase */
    base_list = bases.get_next();		/* throw away dummy base */
}

Boolean ClassType::print_code ( ostream& s, DeclStyle ds )
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

Boolean ClassType::print_marshall_defs ( ostream& s, DeclStyle ds )
{
    s << "\nRpcBuffer& operator<< ";
    s << "( RpcBuffer& rpcbuff, " << class_implname << " *classptr )\n";
    s << "{\n";
    s << "\tif (rpcbuff.packPtr(classptr))\n";
    s << "\t\tclassptr->marshall(rpcbuff);\n\t\n";
    s << "\treturn rpcbuff;\n";
    s << "}\n";
    s << "\nRpcBuffer& operator>> ";
    s << "( RpcBuffer& rpcbuff, " << class_implname << " *& classptr )\n";
    s << "{\n\tvoid *p = 0;\n\tlong index;\n";
    s << "\tif (rpcbuff.unpackPtr(p, index))\n\t{\n";
    s << "\t\tif (classptr == 0)\n";
    s << "\t\t\tclassptr = new " << class_implname << "();\n";
    s << "\t\tclassptr->unmarshall(rpcbuff);\n\t}\n";
    s << "\telse\n\t\tclassptr = (" << class_implname << " *)rpcbuff.pointerAt(index);\n";
    s << "\treturn rpcbuff;\n";
    s << "}\n";
	    
    switch (ds)
    {
    case CLIENT_DECL:
    case SERVER_DECL:
    case SERVERIMPLEMENTATION_DECL:
    case NORMAL_DECL:
	print_marshallfunc_code(s, ds, IN);
	print_marshallfunc_code(s, ds, OUT);
    }
    return TRUE;
}

void ClassType::postname ( ostream&, DeclStyle )
{
}

void ClassType::print_marshallfunc_code ( ostream& s, DeclStyle ds,
					  Direction d )
{
    int i = 0;
    Declaration *decl;
    String fname = (d == IN ? "unmarshall" : "marshall");

    s << "\nvoid " << class_implname << "::";
    s << fname;

    s << " ( RpcBuffer& rpc_buff )";
    s << "\n{\n";

    /* Generate calls on parent marshall routines */
    {
	/* virtual bases first */

	BaseClass *blist = vbase_list;

	while (blist)
	{
	    ClassType *cl = blist->get_class();

	    s << "\t" << cl->get_implname() << "::";
	    s << fname << "(rpc_buff);\n";

	    blist = blist->get_next();
	}

	/* now non-virtual */

	blist = base_list;
	
	while (blist)
	{
	    ClassType *cl = blist->get_class();

	    s << "\t" << cl->get_implname() << "::";
	    s << fname << "(rpc_buff);\n";

	    blist = blist->get_next();
	}
    }

    while (decl = class_members->get_member(i++))
    {
	if (decl->get_marshall() != STUB_NOMARSHALL)
	    decl->marshall(s, d, ds, "rpc_buff");
	else
	{
	    error_stream << "member " << decl->get_name() 
		<< " cannot be auto-(un)marshalled\n";
	    error(error_stream);
	}
    }
    s << "\n}\n";
}

#ifdef NO_INLINES
#  define CLASST_CC_
#  include "ClassT.n"
#  undef CLASST_CC_
#endif
