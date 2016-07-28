/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyT.h,v 1.3 1994/09/02 10:15:20 ngdp Exp $
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

    virtual Boolean addStubModifier (StubModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);

    virtual String massageName (const String&, DeclStyle) const;
    virtual Boolean needsSep () const;

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;
    virtual String signature () const;

    virtual Boolean isTransferable () const;};

#endif
