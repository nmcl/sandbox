/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerList.h
 */

#ifndef SERVERLIST_H_
#define SERVERLIST_H_

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

class ServerData
{
public:
    ServerData ();
    ~ServerData ();

    Buffer key, data;
};

class ServerList : public LockManager
{
public:
    ServerList (int&);
    ServerList (const Uid&, int&);
    ~ServerList ();

    Boolean StoreName (Buffer, Buffer, unsigned long&);
    Boolean FetchName (Buffer&, unsigned long&);
    Boolean ReplaceName (Buffer, unsigned long&);
    Boolean CompareBuffers (Buffer);
    Boolean GetKey (Buffer&);

    unsigned long GetVersionNumber ();

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    ServerData* self;
    Boolean state_set;
    unsigned long v_number;
};

#endif
