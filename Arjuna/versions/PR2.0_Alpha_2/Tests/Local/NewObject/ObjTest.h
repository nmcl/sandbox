/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjTest.h,v 1.1 1993/11/03 15:01:04 nmcl Exp $
 */

#ifndef OBJTEST_
#define OBJTEST_

#include <iostream.h>

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/AtomicA.h>

class ObjTest : public LockManager
{
private:
    int state_number;
public:
    ObjTest(int&, Boolean);
    ~ObjTest();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif
