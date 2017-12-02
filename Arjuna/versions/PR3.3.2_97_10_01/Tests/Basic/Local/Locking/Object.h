/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.h,v 1.1 1994/10/14 11:09:08 nmcl Exp $
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

class Object : public LockManager
{
public:
    Object(int&);
    Object(Uid&, int&);
    ~Object();

    Boolean change(int);
    int value();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    int number;
};

#endif
