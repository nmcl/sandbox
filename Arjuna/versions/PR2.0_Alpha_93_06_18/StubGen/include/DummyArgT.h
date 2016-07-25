/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyArgT.h,v 1.5 1993/06/17 15:07:35 ngdp Exp $
 */

#ifndef DUMMYARGT_H_
#define DUMMYARGT_H_

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
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
