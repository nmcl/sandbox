/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyArgT.h,v 1.1 1997/09/25 15:29:55 nmcl Exp $
 */

#ifndef DUMMYARGT_H_
#define DUMMYARGT_H_

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef DUMMYT_H_
#  include "DummyT.h"
#endif

class DummyArgType : public DummyType
{
public:
    DummyArgType ();
    virtual ~DummyArgType ();

    virtual String signature () const;
};

#endif
