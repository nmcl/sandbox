/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeDefT.h,v 1.1 1997/09/25 15:30:30 nmcl Exp $
 */

#ifndef TYPEDEFT_H_
#define TYPEDEFT_H_

/*
 *
 * Stub generator class for forward references and typedefs.
 * Handles declarations of the form:
 *    class fred;
 *    class X { fred af; };
 *    ...
 *    class fred {...};
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;

class TypeDefType : public TypeDescriptor
{
public:
    TypeDefType (const String&, TypeDescriptor *);
    virtual ~TypeDefType ();

    
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    virtual TypeDescriptor *deref ();

    virtual Boolean addStubModifier (StubModifier);
    virtual String defaultInit () const;
    virtual Boolean deleteable () const;
    virtual Boolean isFundamental () const;

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;

    virtual String signature () const;
    virtual String simpleTypeName (DeclStyle = NORMAL_DECL) const;

    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;
    virtual Boolean produceAllStubs (ostream&);
    virtual Boolean produceMarshallDefs (ostream&, DeclStyle, Boolean = TRUE);
    virtual Boolean produceMarshallingCode (ostream&, DeclStyle, Direction,
					    const String&, const String&);
    virtual Boolean stubIsPossible () const;
 
    virtual ostream& printAsTypedef (ostream&, DeclStyle);

private:
    TypeDescriptor *realType;
    String typedefName;
};

#endif
