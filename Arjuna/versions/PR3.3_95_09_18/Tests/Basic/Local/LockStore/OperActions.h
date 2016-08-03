/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActions.h,v 1.1 1994/10/12 14:55:20 ngdp Exp $
 */

#ifndef OPERACTIONS_H_
#define OPERACTIONS_H_

#include <System/sys/types.h>

#include <Common/Boolean.h>

#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

// @Remote, @NoMarshall
class OperActions : public LockManager
{
    int value;
public:
    OperActions();
    OperActions(const Uid&);
    ~OperActions();

    void anop();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif
