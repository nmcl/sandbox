/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TestLock.h,v 1.4 1993/03/18 15:06:17 n048z Exp $
 */

#ifndef TESTLOCK_H_
#define TESTLOCK_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

enum LockCode { Set, NotSet, Unknown };

class TestLock : public LockManager
{
public:
    TestLock(int&);
    ~TestLock();

    Boolean acquire(int, Boolean);
    Boolean release(int, Boolean);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    Lock *held[10];
    Boolean Owner[10];
    int number;
};

#endif
