/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupView.h,v
 */

#ifndef GROUPVIEW_H_
#define GROUPVIEW_H_

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#include "NameServ_stub.h"


#ifndef REPLICALIST_H_
#  include "ReplicaList.h"
#endif


class ObjectGroups
{
public:
    ObjectGroups ();
    ObjectGroups (ReplicaList*);
    ~ObjectGroups ();

    ReplicaList* me;
    Uid* replicaListUid;
    Uid* groupUid;
    ObjectGroups* next;
};


class ToPurge
{
public:
    ToPurge ();
    ~ToPurge ();

    Boolean add (ReplicaList*);
    ReplicaList* retrieve (Uid&);
    void purge ();

private:
    ObjectGroups* toRemove;
    int number;
};



class GroupView : public LockManager
{
public:
    GroupView (int&);
    GroupView (const Uid&, int&);
    ~GroupView ();

    // general GVD operations

    Boolean getView (ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&, const LockMode = READ);
    Boolean getUseCount (const Uid&, long&, unsigned long&);
    Boolean traverseDatabase (ReplicaDescriptor&, Uid&, int&, const char*, pid_t, unsigned long&);
    OutCome addReplicas (const ReplicaDescriptor&, const Uid&, unsigned long&);
    OutCome deleteReplicas (const ReplicaDescriptor&, const Uid&, const Boolean&, int&, unsigned long&);
    OutCome alterLocation (const Uid&, const Uid&, const char*, unsigned long&);
    OutCome exclude (const ReplicaDescriptor&, const Uid&, unsigned long&);
    OutCome include (const ReplicaDescriptor&, const Uid&, unsigned long&);
    OutCome releaseGroup (const Uid&, const char*, pid_t, unsigned long&);
    Boolean groupAssociation (const Uid&, Uid&, unsigned long&);

    // operations concerned with node recovery

    OutCome recover (const char*, unsigned long&);
    OutCome getNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, const char*, unsigned long&);
    Boolean getExclusiveAccess ();
    Boolean testAndSet (ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome needsUpdating (const Uid&, Boolean&, ReplicaDescriptor&, const char*, pid_t, unsigned long&);

    // operations concerned with composite objects

    OutCome addTemplate (const Uid&, Buffer&, unsigned long&);
    Boolean getTemplate (const Uid&, Buffer&, unsigned long&);
    OutCome deleteTemplate (const Uid&, unsigned long&);
    OutCome replaceTemplate (const Uid&, Buffer&, unsigned long&);

    OutCome assignCompositeObject (const Uid&, const Uid&, unsigned long&);
    OutCome assignCompositeObject (const Uid&, const ReplicaDescriptor&, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const Uid&, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const ReplicaDescriptor&, unsigned long&);

    // operations concerned with GVD recovery

    Boolean getState (ObjectState&, unsigned long&);
    Boolean imposeState (ObjectState&);
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    // create new replica list

    ReplicaList* createReplicaList (const Uid&) const;

    // list manipulation routines

    ObjectGroups* searchAllLists (const Uid&, ObjectGroups*, Boolean = TRUE) const;
    ObjectGroups* searchList (const Uid&, ObjectGroups*) const;
    ObjectGroups* searchCompositeObject (const Uid&, ObjectGroups*) const;
    ObjectGroups* searchWildCard (ObjectGroups*) const;

    Boolean extendList (ObjectGroups*);
    Boolean removeFromList (ObjectGroups*, int&);
    
    ObjectGroups* head;
    int numberofgroups;
    ToPurge deleteList;
};

#endif
