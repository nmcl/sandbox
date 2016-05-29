/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: dummy_i.h,v 1.2 1998/07/17 09:00:16 nmcl Exp $
 */

#include <omniORB2/CORBA.h>

#include "dummy.h"

class dummy_i : public _sk_dummy
{
public:
    dummy_i ();
    virtual ~dummy_i ();
};
