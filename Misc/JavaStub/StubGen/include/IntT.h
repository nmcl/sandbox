/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntT.h,v 1.3 1995/02/22 15:06:16 ngdp Exp $
 */

#ifndef INTT_H_
#define INTT_H_

/*
 *
 * Stub generator integer type class. Redefines inherited operations 
 * as appropriate for ints.
 *
 */

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;
class TypeDescriptor;

class IntType : public ModifiedType
{
public:
    IntType ();
    virtual ~IntType ();

    virtual Boolean addTypeModifier (TypeModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    
    virtual String preName (DeclStyle  = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;
    virtual String signature () const;
private:
    String intTypeName;
};

#endif
