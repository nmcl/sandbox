/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntegralT.h,v 1.6 1993/06/17 15:08:03 ngdp Exp $
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
