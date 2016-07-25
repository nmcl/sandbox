/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IncludeT.h,v 1.8 1993/04/14 13:45:44 ngdp Exp $
 */

#ifndef INCLUDET_H_
#define INCLUDET_H_

/*
 *
 * Stub generator include file mechanism. Treats included files as
 * instances of a special 'includetype' for consistency with other
 * forms of type manipulation .
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

#ifndef DUMMYT_H_
#  include "DummyT.h"
#endif

class IncludeType : public DummyType
{
public:
    IncludeType (const String&, const String&, StubModifier = STUB_DEFAULT);
    virtual ~IncludeType ();

    virtual void add_stub_modifier (StubModifier);
    
    virtual String massage_name (const String&, DeclStyle) const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    
    virtual String typename (DeclStyle) const;
    virtual String signature () const;
    
private:
    String include_dir;
    String include_fname;
    Boolean is_sys;
    Boolean no_mangle;
};

#endif
