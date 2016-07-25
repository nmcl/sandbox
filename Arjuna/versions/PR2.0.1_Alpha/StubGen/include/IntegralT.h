/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntegralT.h,v 1.1 1993/11/03 14:45:24 nmcl Exp $
 */

#ifndef INTEGRALT_H_
#define INTEGRALT_H_

/*
 *
 * Stub generator integral type class. 
 *
 */

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

class ostream;
class TypeDescriptor;

class IntegralType : public ModifiedType
{
public:
    IntegralType ();
    virtual ~IntegralType ();

    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
};

#endif
