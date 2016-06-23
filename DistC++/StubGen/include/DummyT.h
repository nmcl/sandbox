/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyT.h,v 1.1 1997/09/25 15:29:56 nmcl Exp $
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

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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
