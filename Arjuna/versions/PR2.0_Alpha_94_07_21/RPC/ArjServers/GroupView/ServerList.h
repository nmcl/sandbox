/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerList.h,v
 */

#ifndef SERVERLIST_H_
#define SERVERLIST_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif


struct ServerData
{
    Buffer key, data;
};

class ServerList : public LockManager
{
public:
    static ServerList* create (int&);
    static ServerList* create (const Uid&, int&);
    static void remove (ServerList*&);

    Boolean storeName (Buffer, Buffer, unsigned long&);
    Boolean fetchName (Buffer&, unsigned long&);
    Boolean replaceName (Buffer, unsigned long&);
    Boolean compareBuffers (Buffer, unsigned long&);
    Boolean getKey (Buffer&, unsigned long&);


    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    ServerList (int&);
    ServerList (const Uid&, int&);
    ~ServerList ();

    ServerData* self;
    unsigned long v_number;

    unsigned long linkCount;
    ServerList* next;
    static ServerList* headOfList;
};


#endif
