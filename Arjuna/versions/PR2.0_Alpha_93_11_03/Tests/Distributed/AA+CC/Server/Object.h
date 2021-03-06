/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.h,v 1.1 1993/11/03 14:51:11 nmcl Exp $
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

class Object : public LockManager
{
    int number;
public:
    Object(int&);
    Object(Uid&, int&);
    ~Object();

    Boolean change(int);
    int value();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif

