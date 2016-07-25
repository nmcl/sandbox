/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerView.h
 */

#ifndef SERVERVIEW_H_
#define SERVERVIEW_H_

#ifndef LOCK_H_
#include <Arjuna/Lock.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#include "GroupView.h"
#include "ServerList.h"


class ServerGroups
{
public:
    ServerGroups ();
    ~ServerGroups ();
    
    ServerList* me;
    ServerGroups* next;
};


class toDestroy
{
public:
    toDestroy ();
    ~toDestroy ();

    Boolean Add (ServerList*);
    ServerList* Retrieve (Uid&);
    void Purge ();

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

    void StoreName (Boolean&, Buffer, Buffer, unsigned long&);
    void FetchName (Boolean&, Buffer, Buffer&, unsigned long&);
    void RemoveName (Boolean&, Buffer, int&, unsigned long&);
    void ReplaceName (Boolean&, Buffer, Buffer, unsigned long&);
    void GetNextName (Boolean&, Buffer&, int&, unsigned long&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    ServerGroups* head;
    int numberofservers;
    toDestroy deleteList;
};

#endif
