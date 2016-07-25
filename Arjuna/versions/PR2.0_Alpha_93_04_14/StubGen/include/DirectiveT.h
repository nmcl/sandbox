/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DirectiveT.h,v 1.6 1993/04/14 13:45:37 ngdp Exp $
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

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    
    virtual String typename (DeclStyle) const;
    virtual String signature () const;

private:
    String directive;
};

#endif
