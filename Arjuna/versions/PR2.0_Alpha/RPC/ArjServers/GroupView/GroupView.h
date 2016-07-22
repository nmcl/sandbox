/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupView.h
 */

#ifndef GROUPVIEW_H_
#define GROUPVIEW_H_

#ifndef LOCK_H_
#include <Arjuna/Lock.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#include "NameServ_stub.h"

const int groupnumbers = 100;

#include "ReplicaList.h"


class ObjectGroups
{
public:
    ObjectGroups ();
    ~ObjectGroups ();
    
    ReplicaList* me;
    ObjectGroups* next;
};


class toPurge
{
public:
    toPurge ();
    ~toPurge ();

    Boolean Add (ReplicaList*);
    ReplicaList* Retrieve (Uid&);
    void Purge ();

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

    Boolean Get_View (ReplicaDescriptor&, const Uid&,
		      unsigned long&, const LockMode = READ, char* = 0);

    Boolean Add_Replicas (const ReplicaDescriptor&, const Uid&, unsigned long&);
    Boolean Delete_Replicas (const ReplicaDescriptor&, const Uid&, const Boolean, int&, unsigned long&);

    Boolean GetExclusiveGroupView (ReplicaDescriptor&, const Uid&, unsigned long&);
    
    Boolean Recover (const char* = 0);
    Boolean Release_Group (const Uid&, unsigned long&, char* = 0);
    Boolean CheckAndSet (const Uid&, unsigned long&);

    Boolean AlterLocation (const Uid&, const Uid&, string, unsigned long&);

    Boolean Traverse_Database (ReplicaDescriptor&, Uid&, int&, unsigned long&);

    Boolean ChangeReplicaStatus (const ReplicaDescriptor&, const Uid&, const Boolean,unsigned long& );

    Boolean NeedsUpdating (const Uid&, ReplicaDescriptor&, unsigned long&);
    Boolean GetNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, string, Boolean&, unsigned long&);

    Boolean GetState (ObjectState&, unsigned long&);
    Boolean ImposeState (ObjectState&);
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    ObjectGroups* head;  // list of replica groups
    int numberofgroups;  // number of replica groups mentioned in database
    toPurge deleteList;  // contains groups to be deleted
};

#endif
