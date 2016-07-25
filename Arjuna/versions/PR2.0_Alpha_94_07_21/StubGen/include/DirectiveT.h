/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DirectiveT.h,v 1.7 1993/06/17 15:07:33 ngdp Exp $
 */

#ifndef DIRECTIVET_H_
#define DIRECTIVET_H_

/*
 *
 * Stub generator cpp directive handling. Treats directives as
 * instances of a special type for consistency with other
 * forms of type manipulation .
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef DUMMYT_H_
#  include "DummyT.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class Directive : public DummyType
{
public:
    Directive (const String&);
    virtual ~Directive ();

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;

private:
    String directive;
};

#endif
