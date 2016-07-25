/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclSpec.h,v 1.6 1993/03/22 09:27:04 ngdp Exp $
 */

#ifndef DECLSPEC_H_
#define DECLSPEC_H_ 

/*
 *
 * Stub generator declaration specifier class. Holds all information
 * about any specifiers (basic type, storage class etc) of any particular
 * declaration whilst the declaration is being built. Once complete these
 * attributes are passed on and then discarded.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
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

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class Declaration;
class TypeDescriptor;

class DeclSpecifier
{
public:
    /* Constructors */

    DeclSpecifier (CvQualifier);
    DeclSpecifier (FuncModifier);
    DeclSpecifier (StorageSpecifier);
    DeclSpecifier (StubModifier);
    DeclSpecifier (TypeDescriptor *, Boolean = FALSE);

    /* Public non-virtual operations */

    void add_cv_qualifier (CvQualifier);
    void add_func_modifier (FuncModifier);
    void add_stclass (StorageSpecifier);
    void add_stub_modifier (StubModifier);
    void add_type_modifier (TypeModifier);
    void adjust_basic_type (TypeDescriptor *);

    void finalise ();

    StorageSpecifier get_stclass () const;
    TypeDescriptor *get_type () const;

    Boolean is_elaborated () const;

    void make_elaborated ();

    DeclSpecifier *merge_spec_into (DeclSpecifier *) const;
    void propagate_attributes (Declaration *) const;
    void propagate_attributes (TypeDescriptor *) const;

    void set_type (TypeDescriptor *);
    void set_stclass (StorageSpecifier);

private:

    void merge_basic_attributes (DeclSpecifier *) const;

    /* private state */

    Boolean decl_const;			/* placemarker */
    Boolean decl_inline;		/* placemarker */
    Boolean decl_is_elaborated;		/* explicit class/enum mentioned */
    StubModifier decl_marshall;		/* marshalling attributes */
    StubModifier decl_obj_type;		/* local or remote */
    StubModifier decl_par_transfer;	/* parameter semantics */
    StorageSpecifier decl_stclass;	/* AUTO, STATIC etc */
    TypeDescriptor *decl_btype;		/* basic type of declaration */
    Boolean decl_virtual;
    Boolean decl_volatile;
};

#include "DeclSpec.n"

#endif
