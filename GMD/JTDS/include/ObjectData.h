/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectData.h,v 1.1 1998/04/06 14:17:22 nmcl Exp $
 */

#ifndef OBJECTDATA_H_
#define OBJECTDATA_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

class ObjectData : public LockManager
{
public:
    ObjectData (int& res);
    ObjectData (const Uid& u, int& res);
    virtual ~ObjectData ();

    void* get (int& size);
    Boolean set (void* data, int size);
    
    Boolean save_state (ObjectState& os, ObjectType);
    Boolean restore_state (ObjectState& os, ObjectType);
    const TypeName type () const;

private:
    void* _data;
    int _size;
};

#endif
