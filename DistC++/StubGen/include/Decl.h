/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Decl.h,v 1.1 1997/09/25 15:29:48 nmcl Exp $
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

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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

#ifndef OUTPUTSTYLE_H_
#  include "OutputStyle.h"
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
    ~Declaration ();
    
    /* Public non-virtual operations */

    /* Declaration building */

    Boolean addStClass (StorageSpecifier);
    Boolean addStubModifier (StubModifier);
    Boolean addTemplateArgs (DeclContext *);
    Boolean adjustType (TypeDescriptor *);
    Boolean attachBasicSpec (DeclSpecifier *);

    void freezeDecl ();

    Boolean makeElaborated ();
    Boolean makeQualified (TypeDescriptor *);
    Boolean makeSpecial ();
    Boolean makeTypedef ();

    void setBody (Expression *);
    void setContext (DeclContext *);
    void setDtype (TypeDescriptor *);

    void setInit (Expression *);
    void setLinkage (Linkage *);
    void setName (const String&);
    void setQualifier (const String&);
    void setVisibility (VisibilitySpec);

    /* Genral declaration interrogation */

    DeclContext *getContext () const;
    Location getLoc () const;
    Expression *getInitialiser () const;
    Linkage *getLinkage () const;
    String getName () const;
    TypeDescriptor *getType () const;
    VisibilitySpec getVisibility () const;

    Boolean isTypedef () const;
    Type kind () const;

    /* General declaration printing */

    void printDecl (ostream&, DeclStyle = NORMAL_DECL, const char * = ";\n",
		    Boolean = FALSE, Boolean = FALSE,
		    Boolean = FALSE, Boolean = FALSE);

    /* Stub generation specifics */

    StubModifier getMarshall (Boolean) const;
    StubModifier getRemote () const;
    StubModifier getTransfer () const;

    void marshall (ostream&, Direction, DeclStyle, const String&);

    Boolean produceAllStubs (ostream&, OutputStyle);

    Boolean produceMarshallCode (ostream&, DeclStyle, Direction, const String&);
    Boolean produceMarshallDefs (ostream&, DeclStyle, Boolean = TRUE);
    Boolean produceOneStub (ostream&, DeclStyle);
    Boolean produceStubCode (ostream&, DeclStyle);

private:
    /* private state */
    static long countOfDecls;

    Expression *declBody;		/* only for functions */
    DeclContext *declContext;		/* ptr to context info */
    Boolean declFrozen;
    Expression *declInit;		/* initialiser */
    Boolean declIsElaborated;		/* requires type elaboration */
    TypeDescriptor *declQualName;	/* seen x:: in decl */
    Boolean declIsSpecial;		/* created to aid parsing */
    Boolean declIsTypedef;

    Linkage *declLinkage;		/* C or C++ */
    Location declLoc;			/* file and line where decl found */

    String declName;			/* name associated with this decl */

    StubModifier declMarshall;		/* marshalling attributes */
    StubModifier declObjType;		/* local or remote */
    StubModifier declTransfer;		/* parameter semantics */

    StorageSpecifier declStclass;	/* AUTO, STATIC etc */
    TypeDescriptor *declaratorType;	/* type of declaration */
    DeclContext *declTemplateArgs;

    VisibilitySpec declVis;

    Boolean defaultLinkage;		/* has default linkage */

};

#include "Decl.n"

#endif
