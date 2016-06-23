/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclSpec.h,v 1.1 1997/09/25 15:29:52 nmcl Exp $
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

    /* Decl spec building */

    Boolean addCvQualifier (CvQualifier);
    Boolean addFuncModifier (FuncModifier);
    Boolean addStClass (StorageSpecifier);
    Boolean addStubModifier (StubModifier);
    Boolean addTypeModifier (TypeModifier);
    Boolean adjustBasicType (TypeDescriptor *);

    void finalise ();
    void makeElaborated ();

    DeclSpecifier *mergeSpecInto (DeclSpecifier *) const;
    void propagateAttributes (Declaration *) const;
    void propagateAttributes (TypeDescriptor *) const;

    void setType (TypeDescriptor *);
    void setStClass (StorageSpecifier);

    /* Decl spec interrogation */

    StorageSpecifier getStClass () const;
    TypeDescriptor *getType () const;

    Boolean isElaborated () const;

private:

    void mergeBasicAttributes (DeclSpecifier *) const;

    /* private state */

    Boolean declConst;			/* placemarker */
    Boolean declInline;			/* placemarker */
    Boolean declIsElaborated;		/* explicit class/enum mentioned */
    StubModifier declMarshall;		/* marshalling attributes */
    StubModifier declStubType;		/* local or remote */
    StubModifier declTransfer;		/* parameter semantics */
    StubModifier declOptions;		/* other stub specific things */
    StorageSpecifier declStClass;	/* AUTO, STATIC etc */
    TypeDescriptor *declBtype;		/* basic type of declaration */
    Boolean declVirtual;
    Boolean declVolatile;
    Boolean warningsSupressed;
};

#include "DeclSpec.n"

#endif
