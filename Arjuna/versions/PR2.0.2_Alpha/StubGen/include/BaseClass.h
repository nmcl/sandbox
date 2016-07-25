/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BaseClass.h,v 1.7 1993/06/17 15:06:55 ngdp Exp $
 */

#ifndef BASECLASS_H_
#define BASECLASS_H_

/*
 *
 * Class to describe attributes of a base class
 *     
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;
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
private:
    ClassType *baseClass;		/* ptr to actual class */
    BaseClass *nextBase;		/* next base class */
    Boolean virtualBase;		/* virtual base */
    VisibilitySpec visibility;		/* public, etc */
};

#include "BaseClass.n"

#endif
