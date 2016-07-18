/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyT.h,v 1.9 1993/04/14 13:45:39 ngdp Exp $
 */

#ifndef DUMMYT_H_
#define DUMMYT_H_

/*
 *
 * Dummy type class. Used as a base class for things that are not
 * really types such as include files & compiler directives.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class DummyType : public TypeDescriptor
{
public:
    DummyType ();
    virtual ~DummyType ();

    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual Boolean is_transmissible () const;
    virtual Boolean needs_sep () const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);

    virtual String typename (DeclStyle) const = 0;
    virtual String signature () const = 0;
};

#endif
