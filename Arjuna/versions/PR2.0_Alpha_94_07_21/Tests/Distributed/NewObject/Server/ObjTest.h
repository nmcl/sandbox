/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjTest.h,v 1.2 1993/03/18 15:06:31 n048z Exp $
 */

#ifndef OBJTEST_H_
#define OBJTEST_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

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
