/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassT.h,v 1.8 1995/03/07 10:31:30 ngdp Exp $
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

    /* class information building */

    void addCtor (Declaration *);
    void addDtor (Declaration *);
    void addVirtual ();
    void exceptionsNeeded ();
    
    /* class info interrogation */

    Boolean membersUseExceptions () const;
    
    long getClassHashval () const;
    Declaration *getClassMember (int) const;
    BaseClass *getNVBaseList () const;
    BaseClass *getVbaseList () const;
    VisibilitySpec getVisibility () const;

    Declaration *lookFor (const String&, Type, Context) const;

    Boolean hasVirtualFuncs () const;
    
    ClassType *nameIsClassName (const String&);

    /* new virtual functions added by this class */

    virtual void changeVisibility (VisibilitySpec);
    virtual Boolean checkClassProps (StubModifier);
    virtual DeclContext *getMemberContext () const;

    /* virtual functions inherited from base class*/

    virtual Boolean addStubModifier (StubModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    virtual String elaborator (DeclStyle) const;
    virtual Boolean freezeType ();
    virtual Boolean makeComplete ();

    /* Type interrogation */

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;
    virtual String signature () const;
    virtual String simpleTypeName (DeclStyle) const;
    virtual String typeName (DeclStyle, Boolean = FALSE, Boolean = FALSE) const;
    
    /* Stub generation specifics */

    virtual StubModifier defaultLocation () const;
    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;

    virtual Boolean isTransferable () const;
    virtual Boolean produceAllStubs (ostream&);

    virtual Boolean produceMarshallDefs (ostream&, DeclStyle, Boolean = TRUE);

    virtual Boolean produceStubCode (ostream&, DeclStyle);
    virtual Boolean stubIsPossible () const;

    /* Type printing */

    virtual ostream& printAsTypedef (ostream&, DeclStyle);

protected:

    virtual ostream& printClassHeader (ostream&, DeclStyle, Boolean = TRUE);
    virtual Boolean produceClassMarshallingCode (ostream&, DeclStyle,
						 Direction );
						 
    virtual ostream& printTemplatePrefix (ostream&);
private:

    void findVbases ();
    void printClientSpecials (ostream&);
    void printClientTypedef (ostream&);
    void printIdlTypedef (ostream&);
    void printServerSpecials (ostream&);
    void printServerTypedef (ostream&);
    void printShadowsTypedef (ostream&);

    Boolean produceClientCode (ostream&);
    Boolean produceServerCode (ostream&);
    Boolean produceShadowsCode (ostream&);
    Boolean produceJavaCode (ostream&);
    
    DeclarationList *classCtors;	/* list of known constructors */
    Declaration* classDtor;		/* single destructor */

    BaseClass *baseList;		/* non vbase class(es) of this class */
    BaseClass *deriveList;		/* full derivation list */
    BaseClass *vBaseList;		/* ALL virtual bases */

    String classClientName;		/* name in client side*/
    Boolean classComplete;		/* ie full definition */
    ClassType *classFullDecl;		/* ptr to full decl */
    long classHashval;
    String classImplName;		/* name of true implemenation class */
    Boolean classIsRemote;
    ClassSpecifier classKind;		/* class/struct/union */
    StubModifier classMarshallOpts;
    ClassDeclContext *classMembers;	/* member list */
    String className;			/* typename for parsing etc. */
    Boolean classIsNameless;		/* class has no real name */
    String classServerName;		/* name of server class */
    StubModifier classStubLoc;
    VisibilitySpec curVisibility;    
    VisibilitySpec defVisibility;	/* default public if struct */

    int doneMarshallDefs;
    int doneTypeDefs;

    Boolean exceptionsThrown;		/* if throw seen in any func header */
    Boolean frozen;
    Boolean hasConstructor;
    Boolean hasDestructor;		/* true if destructor known */
    Boolean hasVirtuals;		/* virtual fns known to exist */
    Boolean stubOptionsSupressed;
    Boolean warningsSupressed;
};

#include "ClassT.n"

#endif
