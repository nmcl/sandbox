/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IncludeT.h,v 1.1 1997/06/09 19:52:14 nmcl Exp $
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

    virtual Boolean addStubModifier (StubModifier);

    String massageName ( const String&, DeclStyle ) const;
    
    virtual ostream& printInstanceOf (ostream&, const String&, 
				      DeclStyle = NORMAL_DECL,
				      TypeDescriptor * = 0,
				      Boolean = FALSE, 
				      Boolean = FALSE);

private:
    String includeDir;
    String includeFname;
    Boolean isSys;
    Boolean noMangle;
};

#endif
