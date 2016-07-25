/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassT.h,v 1.12 1993/04/14 13:45:34 ngdp Exp $
 */

#ifndef CLASST_H_
#define CLASST_H_

/*
 *
 * Stub generator class definition handler. Holds all information
 * about any particular class, struct or union.
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

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef BASECLASS_H_
#  include "BaseClass.h"
#endif

#ifndef CDCLCNTXT_H_
#  include "CDclCntxt.h"
#endif

enum ClassSpecifier { CLASS_TYPE, STRUCT_TYPE, UNION_TYPE };

class ostream;
class DeclarationList;

class ClassType : public TypeDescriptor
{
public:
    ClassType (const String&, ClassSpecifier, BaseClass *,
	       DeclContext *, Boolean = FALSE, Boolean = FALSE);
    ~ClassType ();

    /* non-virtual functions */

    void add_ctor (Declaration *);
    void add_dtor (Declaration *);

    BaseClass *get_bases () const;
    long get_class_hashval () const;
    Declaration *get_class_member (int) const;
    String get_clientname () const;
    String get_implname () const;
    DeclContext *get_member_context () const;
    String get_servername () const;
    BaseClass *get_vbases () const;
    VisibilitySpec get_visibility () const;

    Declaration *lookfor (const String&, Type, Context) const;

    ClassType *name_is_cname (const String&);

    /* virtual functions */

    virtual void add_stub_modifier (StubModifier);
    virtual void change_visibility (VisibilitySpec);
    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual StubModifier default_location () const;
    virtual StubModifier default_marshall () const;
    virtual StubModifier default_par_transfer () const;

    virtual void freeze_type ();

    virtual Boolean is_transmissible () const;

    virtual void make_complete ();

    void print_marshallfunc_code (ostream&, DeclStyle, Direction);
    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual void print_header (ostream&, DeclStyle, Boolean = TRUE);
    virtual void print_typedef (ostream&, DeclStyle);

    virtual Boolean print_code (ostream&, DeclStyle);
    virtual Boolean print_marshall_defs (ostream&, DeclStyle);
    virtual void produce_marshall_defs (ostream&);
    virtual void produce_stubs (ostream&);

    virtual String signature () const;
    virtual Boolean stub_possible () const;
    virtual String typename (DeclStyle) const;

private:
    Boolean check_classprops (StubModifier);
    void find_vbases ();
    void print_client_specials (ostream&);
    void print_server_typedef (ostream&);
    void print_client_typedef (ostream&);
    Boolean print_client_code (ostream&);
    Boolean print_server_code (ostream&);
    void print_server_specials (ostream&);

    DeclarationList *class_ctors;
    Declaration* class_dtor;

    BaseClass *base_list;		/* non vbase class(es) of this class */
    BaseClass *derive_list;		/* full derivation list */
    BaseClass *vbase_list;		/* ALL virtual bases */

    String class_clientname;		/* name in client side*/
    Boolean class_complete;		/* ie full definition */
    ClassType *class_full_decl;		/* ptr to full decl */
    long class_hashval;
    String class_implname;		/* name of true implemenation class */
    Boolean class_is_remote;
    ClassSpecifier class_kind;		/* class/struct/union */
    StubModifier class_marshall_opts;
    ClassDeclContext *class_members;	/* member list */
    String class_name;			/* typename for parsing etc. */
    Boolean class_nameless;		/* class has no real name */
    String class_servername;		/* name of server class */
    StubModifier class_stubloc;
    VisibilitySpec cur_visibility;    
    VisibilitySpec def_visibility;	/* default public if struct */

    Boolean frozen;
    Boolean hasconstructor;
    Boolean hasdestructor;		/* true if destructor known */
};

#include "ClassT.n"

#endif
