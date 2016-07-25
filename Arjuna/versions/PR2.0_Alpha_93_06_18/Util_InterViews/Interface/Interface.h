/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef OBJSTAT_H_
#include <Arjuna/ObjState.h>
#endif

#ifndef INTERFACESTATE_H_
#include "InterfaceState.h"
#endif

class Interface : public LockManager
{
public:
    Interface (int &res);
    Interface (const Uid& u, int &res);
    ~Interface ();

    Boolean Add (Buffer, int&);
    Boolean Delete (const int, Buffer, int&);
    Boolean Inspect (Buffer&, int&, int&);
    Boolean Get (Buffer, Buffer&, int&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    State *head;
    State *ptr;
    int number;
};

#endif
