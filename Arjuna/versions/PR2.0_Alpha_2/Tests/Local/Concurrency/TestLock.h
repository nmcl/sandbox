/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TestLock.h,v 1.1 1993/11/03 15:00:35 nmcl Exp $
 */

#ifndef TestLock_
#define TestLock_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif


enum LockCode { Set, NotSet, Unknown };

class TestLock : public LockManager
{
public:
    TestLock ();
    ~TestLock ();

    LockCode acquire (int, Boolean);
    Boolean release (int, Boolean);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    Lock *held[10];
    Boolean Owner[10];
};

#endif
