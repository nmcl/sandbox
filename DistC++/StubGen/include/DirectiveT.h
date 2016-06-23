/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DirectiveT.h,v 1.1 1997/09/25 15:29:55 nmcl Exp $
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

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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
