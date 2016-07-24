/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: WorkStack.h,v 1.1 1993/11/03 12:25:46 nmcl Exp $
 */

#ifndef WORKSTACK_H_
#define WORKSTACK_H_

// @NoRemote, @AutoMarshall
struct Work
{
    int start;
    int pinTo, pinFrom, pinTmp;
    int number;
};

#include <Common/Uid.h>
#include <Common/Boolean.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

class WorkStack : public LockManager
{
    Boolean remoteState;
    int  size;
    Work *works;
    int  workNumber;
public:
    WorkStack(int sz, int& res);
    WorkStack(int& res, Uid& U, Boolean = FALSE);
    ~WorkStack();

    void PushWork(Work, int& res);
    void PopWork(Work&, int& res);

    Boolean HasRemoteState () const;

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};
	
#endif
