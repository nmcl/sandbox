/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VecT.h,v 1.11 1993/06/24 14:25:15 ngdp Exp $
 */

#ifndef VECT_H_
#define VECT_H_

/*
 *
 * Stub generator vector type class definition.
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


class Expression;
class ostream;

class VecType : public TypeDescriptor
{
public:
    VecType (Expression *, TypeDescriptor *);
    virtual ~VecType ();

    virtual Boolean attachBaseType (TypeDescriptor *);

    virtual TypeDescriptor *combineTypes (TypeDescriptor *);


    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;

    virtual Boolean isFundamental () const;
    virtual String signature () const;

    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;
    virtual Boolean produceMarshallingCode (ostream&, Direction,
					    const String&, const String&);
private:
    String indexName ();		/* generate new index var name */

    Expression *vectorSize;		/* basic size of vector */
    TypeDescriptor *vectorOf;		/* is a vector of this type */
};

#endif
