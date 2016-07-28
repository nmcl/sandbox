/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Container.h,v 1.1 1994/01/20 12:00:56 ngdp Exp $
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include <Common/Boolean.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

class Object;

class Container : public LockManager
{
public:
    Container();
    Container(const Uid&);
    ~Container();

    Boolean insert (Object *);
    Object *pop ();
    Boolean push (Object *);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
private:
    Object *head;
    int count;
};

#endif

