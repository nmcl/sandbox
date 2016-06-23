/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IncludeT.h,v 1.1 1997/09/25 15:30:06 nmcl Exp $
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

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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
