/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BaseClass.h,v 1.1 1997/09/25 15:29:29 nmcl Exp $
 */

#ifndef BASECLASS_H_
#define BASECLASS_H_

#include <iostream>

using namespace std;

/*
 *
 * Class to describe attributes of a base class
 *     
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

class ClassType;

class BaseClass
{
public:
    BaseClass(ClassType *, Boolean = FALSE,
	      VisibilitySpec = CLASS_DEFAULT);

    Boolean append (BaseClass *);
    void freeze ();
    ClassType *getClass () const;
    BaseClass *getNext () const;
    VisibilitySpec getVisibility () const;

    Boolean isVirtual () const;

    void setLink (BaseClass *);

    ostream& printBaseClass (ostream&, DeclStyle);
    
    virtual StubModifier defaultLocation () const;
    virtual StubModifier defaultMarshall () const;

private:
    ClassType *baseClass;		/* ptr to actual class */
    BaseClass *nextBase;		/* next base class */
    Boolean virtualBase;		/* virtual base */
    VisibilitySpec visibility;		/* public, etc */
};

#include "BaseClass.n"

#endif
