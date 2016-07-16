/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BaseClass.h,v 1.6 1993/03/22 09:26:43 ngdp Exp $
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
    ClassType *get_class () const;
    BaseClass *get_next () const;
    VisibilitySpec get_visibility () const;

    Boolean is_virtual () const;

    void set_link (BaseClass *);

    ostream& print_baseclass (ostream&, DeclStyle);
private:
    ClassType *base_class;		/* ptr to actual class */
    BaseClass *next_base;		/* next base class */
    Boolean virtual_base;		/* virtual base */
    VisibilitySpec visibility;		/* public, etc */
};

#include "BaseClass.n"

#endif
