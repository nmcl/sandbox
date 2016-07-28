/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.h,v 1.3 1996/03/29 14:10:20 ngdp Exp $
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

// @NoMarshall
class Object : public LockManager
{
    int number;
public:
    Object(int&);
    Object(Uid&, int&);
    ~Object();

    Boolean change(int);
    int value();

    Boolean prematureCommit ();
    Boolean prematureAbort ();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif

