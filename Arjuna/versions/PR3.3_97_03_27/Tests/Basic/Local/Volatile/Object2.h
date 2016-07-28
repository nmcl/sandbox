/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object2.h,v 1.1 1993/11/11 15:32:22 nmcl Exp $
 */

#ifndef OBJECT2_H_
#define OBJECT2_H_

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

class Object2: public LockManager
{
public:
    Object2(ObjectType, Uid&, int&);
    Object2(ObjectType, int, int, Uid&, int&);
    Object2(Uid&, ObjectType, int&);
    ~Object2();

    Boolean change(int, int, int&, int&);
    Boolean get(int&, int&);

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
private:
    int i, j;
};

#endif
