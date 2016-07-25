/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object1.h,v 1.2 1993/07/19 10:59:17 n048z Exp $
 */

#ifndef OBJECT1_H_
#define OBJECT1_H_

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

class Object1: public LockManager
{
public:
    Object1(ObjectType, Uid&, int&);
    Object1(ObjectType, int, Uid&, int&);
    Object1(Uid&, ObjectType, int&);
    ~Object1();

    Boolean change(int, int&);
    Boolean get(int&);
    
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
private:
    int i;
};

#endif
