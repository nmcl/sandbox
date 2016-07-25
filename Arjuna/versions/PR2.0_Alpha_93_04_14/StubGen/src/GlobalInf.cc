/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GlobalInf.cc,v 1.18 1993/04/14 13:46:13 ngdp Exp $
 */


/*
 * Stub generator global information
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef FILEINFO_H_
#  include "FileInfo.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef DIRECTIVET_H_
#  include "DirectiveT.h"
#endif

#ifndef INCLUDET_H_
#  include "IncludeT.h"
#endif

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

static const char RCSid[] = "$Id: GlobalInf.cc,v 1.18 1993/04/14 13:46:13 ngdp Exp $";

/*
 * Declare static members of the class here
 */

int GlobalInfo::context_level = 1;
DeclContext *GlobalInfo::context_stack[max_context_nesting] = {0};
DeclContext *GlobalInfo::current_context = 0;

FileInfo *GlobalInfo::current_file = 0;
InterfaceDef *GlobalInfo::current_if = 0;
Linkage *GlobalInfo::current_linkage = 0;
Location GlobalInfo::current_loc("<stdin>", 0);

int GlobalInfo::dir_count = 0;
FileInfo *GlobalInfo::files_read[max_idir_level] = {0};
DeclContext GlobalInfo::global_symbols;
DeclContext GlobalInfo::includes;
int GlobalInfo::include_level = 0;
String *GlobalInfo::include_stack[max_idir_level] = {0};
Boolean GlobalInfo::ininclude = FALSE;
String GlobalInfo::interfacename("<stdin>");

int GlobalInfo::linkage_level = 0;
Linkage *GlobalInfo::linkage_stack[max_linkage_nesting] = {0};
String GlobalInfo::server_filename("");

/*
 * Table to hold typenames if in 2.0 compatability mode
 */

DeclContext *GlobalInfo::type_context = 0;
Boolean GlobalInfo::types_global = FALSE;

String GlobalInfo::version("Arjuna Stub Generator Version 2.5 alpha - $Date: 1993/04/14 13:46:13 $");

void GlobalInfo::add_directive ( const String& dir )
{
    String dname(constructname("_cppD_", dir_count++));
    
    putin(new Declaration(dname,new Directive(dir)));
}

/*
 * Add a directory name specified as an option to -I to the stack of 
 * known names. Used to determine which file was really included instead
 * of just using cpp generated name (which is already expanded)
 */

void GlobalInfo::add_inc_dir ( const String& dirname )
{
    if (include_level < max_idir_level)
    {
	if (dirname.size() > 0)
	    include_stack[include_level++] = new String(dirname);
    }
    else
	fatal("internal included stack overflow");
}

void GlobalInfo::init ()
{
    context_stack[0] = current_context = &GlobalInfo::global_symbols;
    linkage_stack[0] = current_linkage = cpp_linkage;
}

/*
 * A file switch has occurred. Remember the new current file name and
 * determine if this file has been visited before. If not add it to the
 * list of included files providing that the include depth
 * is not > 1.
 */

void GlobalInfo::set_currentfile ( const String& filename )
{
    /* Partition name into directory name & filename */

    int level;
    FileInfo *curr_file = 0;
    String curr_dir("./");
    String dirname;
    String fname(filename);
    
    /* Check prefix of filename against list of include dirs */

    for (level = 0; level < include_level; level++)
    {
	String idir(*include_stack[level]);
	int idir_len = idir.size();
	
	if (filename(0,idir_len) == idir)
	{
	    /* found matching prefix, split filename up */
	    
	    dirname = idir + "/";
	    fname = filename(idir_len+1); /* strip leading '/' */

	    break;
	}
    }

    /* strip leading './' if it exists (Sun cpp) */

    while (fname.size() > 2)
	if (fname(0,2) == curr_dir)
	    fname = fname(2);
	else
	    break;

    /* filename split - see if it has been visited before */

    for (level = 0; level < max_idir_level; level++)
    {
	curr_file = files_read[level];
	
	if (curr_file)
	{
	    /* would like && here but cfront 2.1 cannot cope */
	    if (curr_file->get_dname() == dirname)
		if (curr_file->get_fname() == fname)
		{
		    /* seen before */
		    current_file = curr_file;
		    break;
		}
	} else
	    break;
    }
	
    if (curr_file == 0)
    {
	current_file = new FileInfo(dirname, fname);
	
	if (level < max_idir_level)
	    files_read[level] = current_file;
	else
	    fatal("(Stub Internal) file table overflow");
    }

    if (interfacename == filename)	/* reverting to base ? */
    {
	ininclude = FALSE;
    }
    else if (curr_file == 0)
    {
	IncludeType *inctype = new IncludeType(dirname,fname,current_file->get_generic());
	Declaration *incdecl = new Declaration(filename, inctype);	    

	ininclude = TRUE;	    
	putin(incdecl);
	    
    }
    
    current_loc.set_filename(filename);
    error_location.set_filename(filename);
}

void GlobalInfo::set_currentmods ( StubModifier sm )
{
    Declaration *inc_decl = lookfor(current_loc.get_filename());

    current_file->add_stub_modifier(sm);

    if (inc_decl)
    {
	TypeDescriptor *inc_type = inc_decl->get_type();

	if (inc_type)
	    inc_type->add_stub_modifier(sm);
    }
}

Declaration *GlobalInfo::lookfor ( const String& name,
				   Type type,
				   Context lookin )
{
    Declaration *d = 0;
    
    if (types_global)
    {
	d = type_context->lookfor(name, type, lookin);
	if (d == 0)
	    d = current_context->lookfor(name, type, lookin);
    }
    else
	d = current_context->lookfor(name, type, lookin);

    return d;
}

void GlobalInfo::pop_context ()
{
#ifdef DEBUG
    debug_stream << "pop_context() : leaving : " << (void *)current_context;
    debug_stream << ", resuming :" << (void*)context_stack[context_level-2];
    debug_stream << "\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (context_level == 1)
	fatal("(internal) Declaration context stack underflow");
    else
    {
	context_level--;
	current_context = context_stack[context_level-1];
    }
}

void GlobalInfo::pop_linkage ()
{
#ifdef DEBUG
    debug_stream << "pop_linkage() : leaving : " << (void *)current_linkage;
    debug_stream << ", resuming :" << (void*)linkage_stack[linkage_level-1];
    debug_stream << "\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (linkage_level == 0)
	fatal("(internal) Language linkage stack underflow");
    else
	current_linkage = linkage_stack[--linkage_level];
}

void GlobalInfo::push_context ( DeclContext *dc )
{
#ifdef DEBUG
    debug_stream << "push_context() : leaving : " << (void *)current_context;
    debug_stream << ", entering :" << (void*)dc;
    debug_stream << "\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (context_level < max_context_nesting)
    {
	context_stack[context_level++] = dc;
	current_context = dc;
    }
    else
	fatal("(internal) Declaration contexts nested too deep");
}

void GlobalInfo::push_linkage ( Linkage *l )
{
#ifdef DEBUG
    debug_stream << "push_linkage() : leaving : " << (void *)current_linkage;
    debug_stream << ", entering :" << (void*)l;
    debug_stream << "\n";
    _debug.dbg_flush(GLOBAL, FAC_SCOPE, VIS_PUBLIC);
#endif

    if (linkage_level < max_linkage_nesting)
    {
	linkage_stack[++linkage_level] = l;
	current_linkage = l;
    }
    else
	error("(internal) Language linkage nested too deep");
}

void GlobalInfo::putin ( Declaration *decl )
{
    if (types_global && decl && context_level > 1 && decl->is_typedef())
	type_context->putin(decl);

    current_context->putin(decl);
}

void GlobalInfo::stamp ( ostream& s )
{
    s << "/*\n * Automatically Generated by:\n * " << version << "\n";
    s << " *\n * Do Not Edit By Hand\n *\n */\n\n";
}

#ifdef NO_INLINES
#  define GLOBALINF_CC_
#  include "GlobalInf.n"
#  undef GLOBALINF_CC_
#endif
