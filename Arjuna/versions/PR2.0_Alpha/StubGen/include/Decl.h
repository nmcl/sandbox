/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Decl.h,v 1.8 1993/03/22 09:26:58 ngdp Exp $
 */

#ifndef DECLARATION_H_
#define DECLARATION_H_

/*
 *
 * Stub generator declaration class. Holds all information
 * about any particular declaration.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef CVQUALS_H_
#  include "CvQuals.h"
#endif

#ifndef FUNCMODS_H_
#  include "FuncMods.h"
#endif

#ifndef STORESPEC_H_
#  include "StoreSpec.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

#ifndef DIRECTION_H_
#  include "Direction.h"
#endif

#ifndef LINKAGE_H_
#  include "Linkage.h"
#endif

#ifndef LOCATION_H_
#  include "Location.h"
#endif

/*
 * Avoid including other .h files unnecessarily
 */

class DeclarationList;
class DeclContext;
class DeclSpecifier;
class Expression;
class ClassType;
class TypeDescriptor;

class Declaration
{
public:
    /* Constructor */

    Declaration (const String&,  TypeDescriptor* = 0, Expression * = 0);

    /* Public non-virtual operations */

    void add_stclass (StorageSpecifier);
    void add_stub_modifier (StubModifier);
    void adjust_type (TypeDescriptor *);
    void attach_basic_spec (DeclSpecifier *);

    void clear_special ();

    void freeze_decl ();

    DeclContext *get_context () const;
    Location get_loc () const;

    Expression *get_initialiser () const;
    Linkage *get_linkage () const;

    String get_name () const;

    StubModifier get_marshall ();
    StubModifier get_remote ();
    StubModifier get_par_transfer ();

    String get_signature () const;
    TypeDescriptor *get_type () const;
    VisibilitySpec get_visibility () const;

    Boolean is_typedef () const;

    Type kind () const;

    void make_elaborated ();
    void make_pure ();
    void make_special ();
    void make_typedef ();

    void marshall (ostream&, Direction, DeclStyle, const String&);

    Boolean print_code (ostream&, DeclStyle);
    void print_decl (ostream&, DeclStyle = NORMAL_DECL, const char * = ";\n",
		     Boolean = FALSE, Boolean = FALSE);
    void print_marshall_code (ostream&, const String&);
    Boolean print_marshall_defs (ostream&, DeclStyle);
    void print_unmarshall_code (ostream&, const String&);
    Boolean produce_stub (ostream&, DeclStyle);
    void produce_stubs (ostream&);

    void set_body (Expression *);
    void set_context (DeclContext *);
    void set_dtype (TypeDescriptor *);

    void set_init (Expression *);
    void set_linkage (Linkage *);
    void set_name (const String&);
    void set_qualifier (const String&);
    void set_visibility (VisibilitySpec);

private:
    /* private state */
    static long count_of_decls;

    Expression *decl_body;		/* only for functions */
    DeclContext *decl_context;		/* ptr to context info */
    Boolean decl_frozen;
    Expression *decl_init;		/* initialiser */
    Boolean decl_is_elaborated;		/* requires type elaboration */
    Boolean decl_is_special;		/* created to aid parsing */
    Boolean decl_is_typedef;

    Linkage *decl_linkage;		/* C or C++ */
    Location decl_loc;			/* file and line where decl found */

    String decl_name;			/* name associated with this decl */

    StubModifier decl_marshall;		/* marshalling attributes */
    StubModifier decl_obj_type;		/* local or remote */
    StubModifier decl_par_transfer;	/* parameter semantics */

    StorageSpecifier decl_stclass;	/* AUTO, STATIC etc */
    TypeDescriptor *declarator_type;	/* type of declaration */

    VisibilitySpec decl_vis;

    Boolean default_linkage;		/* has default linkage */

};

#include "Decl.n"

#endif
