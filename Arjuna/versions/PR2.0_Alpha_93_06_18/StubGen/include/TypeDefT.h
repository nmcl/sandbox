/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeDefT.h,v 1.12 1993/06/25 13:13:37 ngdp Exp $
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

#ifndef STRING_H_
#  include "String.h"
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
    virtual TypeDescriptor *deref () const;

    virtual String defaultInit () const;
    virtual Boolean isFundamental () const;

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;

    virtual String signature () const;
    virtual String simpleTypeName (DeclStyle = NORMAL_DECL) const;

    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;
    virtual Boolean produceMarshallingCode (ostream&, Direction,
					    const String&, const String&);

    virtual ostream& printAsTypedef (ostream&, DeclStyle);

private:
    TypeDescriptor *realType;
    String typedefName;
};

#endif
