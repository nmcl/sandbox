/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerView.h,v
 */

#ifndef SERVERVIEW_H_
#define SERVERVIEW_H_

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef SERVERLIST_H_
#  include "ServerList.h"
#endif


class ServerGroups
{
public:
    ServerGroups ();
    ~ServerGroups ();
    
    Uid* serverListUid;
    ServerList* me;
    ServerGroups* next;
};


class ToDestroy
{
public:
    ToDestroy ();
    ~ToDestroy ();

    Boolean add (ServerList*);
    ServerList* retrieve (Uid&);
    void purge ();

private:
    ServerGroups* toRemove;
    int number;
};


class ServerView : public LockManager
{
public:
    ServerView (int&);
    ServerView (const Uid&, int&);
    ~ServerView ();

    ServerList* createServerList (const Uid&);

    void storeName (Boolean&, Buffer, Buffer, unsigned long&);
    void fetchName (Boolean&, Buffer, Buffer&, unsigned long&);
    void removeName (Boolean&, Buffer, int&, unsigned long&);
    void replaceName (Boolean&, Buffer, Buffer, unsigned long&);
    void getNextName (Boolean&, Buffer&, int&, unsigned long&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    ServerGroups* head;
    int numberofservers;
    ToDestroy deleteList;
};

#endif
