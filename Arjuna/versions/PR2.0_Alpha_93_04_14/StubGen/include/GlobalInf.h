/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GlobalInf.h,v 1.5 1993/03/22 09:27:22 ngdp Exp $
 */

#ifndef GLOBALINF_H_
#define GLOBALINF_H_

/*
 *
 * Global information structure for stub generator. Constructed by
 * yacc generated parser as the interface definition file is scanned.
 * All members of this class are defined as static.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

#ifndef FILEINFO_H_
#  include "FileInfo.h"
#endif

#ifndef LINKAGE_H_
#  include "Linkage.h"
#endif

#ifndef LOCATION_H_
#  include "Location.h"
#endif

class InterfaceDef;
class TypeDescriptor;
class String;

static const int max_linkage_nesting = 50;
static const int max_context_nesting = 50;
static const int max_idir_level = 50;

class GlobalInfo
{
public:
    /* public operations */

    static void add_directive (const String&);
    static void add_inc_dir (const String&);

    static void compatability_mode ();

    static void init ();		/* set up - call early in main */

    static InterfaceDef *get_current_if ();
    static DeclContext *get_context ();
    static Location get_currentloc ();
    static Declaration *get_decl (int);
    static Linkage *get_linkage ();
    static String get_servername ();
    static StubModifier get_stub_globalmods ();

    static Declaration *lookfor (const String&,
				 Type = ANY_TYPE,
				 Context = ANY_CONTEXT);
    
    static void newline ();

    static void pop_context ();
    static void pop_linkage ();
    static void push_context (DeclContext *);

    static void push_linkage (Linkage *);
    static void putin (Declaration *);

    static void set_current_if (InterfaceDef *);
    static void set_currentline (long);
    static void set_currentfile (const String&);
    static void set_currentmods (StubModifier);
    static void set_interfacename (const String&);
    static void set_servername (const String&);

    static void stamp (ostream&);
    
private:
    static int context_level;
    static DeclContext *context_stack[max_context_nesting];
    static DeclContext *current_context;

    static FileInfo *current_file;
    static InterfaceDef *current_if;
    static Linkage *current_linkage;
    static Location current_loc;       	/* current scanner location */

    static int dir_count;

    static FileInfo *files_read[max_idir_level];

    static DeclContext global_symbols;
    
    static DeclContext includes;	/* directly included filenames */
    static int include_level;
    static String *include_stack[max_idir_level];
    static Boolean ininclude;		/* if TRUE processing include */
    static String interfacename;
    
    static int linkage_level;
    static Linkage *linkage_stack[max_linkage_nesting];

    static String server_filename;

    static DeclContext *type_context;	/* for 2.0 compatability */
    static Boolean types_global;
    static String version;
};

#include "GlobalInf.n"

#endif
