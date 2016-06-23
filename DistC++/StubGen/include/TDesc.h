/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TDesc.h,v 1.1 1997/09/25 15:30:27 nmcl Exp $
 */

#ifndef TDESC_H_
#define TDESC_H_

/*
 *
 * Stub generator basic type attribute class. Used ONLY as a base class for
 * the 'real' type defining classes. Provides the basic definitions of all
 * the functions required in stub generation.
 * Most operations are virtual and should be (in general) redifined in all the
 * derived types appropriately.
 * Instances of this class cannot be created explicitly - its only
 * constructor is protected to enforce this. In addition some operations
 * are declared pure virtual where the default behaviour is likely to
 * be problematical to define.
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

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
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

class DeclContext;
class Expression;
class ostream;

class TypeDescriptor
{   
public:
    virtual ~TypeDescriptor ();

    /* Non-virtual functions */

    ostream& marshall(ostream&, Direction, DeclStyle, 
		      const String&, const String&);

    TypeDescriptor *simplify (int);
    
    /* public virtual interface */

    /* Type manipulation routines */

    virtual Boolean addBody (Expression *);
    virtual Boolean addCvQualifier (CvQualifier);
    virtual Boolean addFuncModifier (FuncModifier);
    virtual Boolean addName(const String&);
    virtual Boolean addStubModifier (StubModifier);
    virtual Boolean addTypeModifier (TypeModifier);

    virtual Boolean attachBaseType (TypeDescriptor *);

    virtual TypeDescriptor *combineTypes (TypeDescriptor *) = 0;
    virtual TypeDescriptor *deref ();
    virtual String elaborator (DeclStyle) const;
    virtual Boolean freezeType ();

    virtual Boolean makeComplete ();

    virtual Boolean overrideStubModifiers (StubModifier);
    virtual void setContext (DeclContext *);
    
    /* Type interrogation */

    virtual Boolean canDiscard () const;
    virtual String defaultInit () const;
    virtual Boolean deleteable () const;
    virtual DeclContext *getContext () const;
    virtual Boolean isConstant () const;
    virtual Boolean isFundamental () const;
    virtual Type kind () const;

    virtual String massageName (const String&, DeclStyle) const;
    virtual Boolean needsSep () const;

    virtual String postName (DeclStyle) const = 0;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const = 0;

    virtual String signature () const = 0;
    virtual String simpleTypeName (DeclStyle = NORMAL_DECL) const;
    virtual String typeName (DeclStyle, Boolean = FALSE, Boolean = FALSE) const;

    /* Stub generation specifics */

    virtual StubModifier defaultLocation () const;
    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;

    virtual Boolean isTransferable () const = 0;

    virtual Boolean produceAllStubs (ostream&);
    virtual Boolean produceMarshallingCode (ostream&, DeclStyle, Direction,
					    const String&, const String&);
    
    virtual Boolean produceMarshallDefs (ostream&, DeclStyle, Boolean = TRUE);
    virtual Boolean produceStubCode (ostream&, DeclStyle);

    virtual Boolean stubIsPossible () const;

    /* Type printing */

    virtual ostream& printAsTypedef (ostream&, DeclStyle);
    virtual ostream& printInstanceOf (ostream&, const String&, 
				      DeclStyle = NORMAL_DECL,
				      TypeDescriptor * = 0,
				      Boolean = FALSE, 
				      Boolean = FALSE);
protected:
    TypeDescriptor (Type);

private:
    DeclContext *declaredIn;
    Type isKindOf;
};

#include "TDesc.n"

#endif
