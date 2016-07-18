/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntegralT.h,v 1.5 1993/03/22 09:27:27 ngdp Exp $
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

    virtual TypeDescriptor *combine_types (TypeDescriptor *);
};

#endif
