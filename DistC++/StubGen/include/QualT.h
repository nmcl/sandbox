/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QualT.h,v 1.1 1997/09/25 15:30:22 nmcl Exp $
 */

#ifndef QUALT_H_
#define QUALT_H_

/*
 *
 * Stub generator class for qualified types (eg x::y)
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

class QualifiedType : public TypeDescriptor
{
public:
    QualifiedType (TypeDescriptor *, TypeDescriptor *);
    virtual ~QualifiedType ();

    virtual Boolean addBody (Expression *);
    virtual Boolean addFuncModifier (FuncModifier);
    virtual Boolean addStubModifier (StubModifier);
    
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    virtual TypeDescriptor *deref ();

    virtual String massageName (const String&, DeclStyle) const;
    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;

    virtual String signature () const;

    /* Stub generation specifics */
 
    virtual Boolean isTransferable () const;
    virtual Boolean produceMarshallingCode (ostream&, DeclStyle, Direction,
					    const String&, const String&);


    /* Type printing */

    virtual ostream& printAsTypedef (ostream&, DeclStyle);
 
private:
    TypeDescriptor *qualifiedType;
    TypeDescriptor *qualifyingType;
};

#endif
 
