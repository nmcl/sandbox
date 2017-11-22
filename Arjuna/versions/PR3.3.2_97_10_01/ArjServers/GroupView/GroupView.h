/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupView.h,v 1.10 1995/02/22 11:27:20 nmcl Exp $
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

#ifndef NAMESERV_H_
#  include "NameServ_stub.h"
#endif

#ifndef REPLICALIST_H_
#  include "ReplicaList.h"
#endif


class CompositeObject;


class ObjectGroups
{
public:
    ObjectGroups  ();
    ObjectGroups  (ReplicaList*);
    ~ObjectGroups ();

    ReplicaList*  me;
    Uid*          replicaListUid;
    Uid*          groupUid;
    ObjectGroups* next;
};


class ToPurge
{
public:
    ToPurge ();
    ~ToPurge ();

    Boolean      add      (ReplicaList*);
    ReplicaList* retrieve (Uid&);
    void         purge    ();

private:
    ObjectGroups* toRemove;
    int           number;
};



class GroupView : public LockManager
{
    friend CompositeObject;

public:
    GroupView (int&);
    GroupView (const Uid&, int&);
    ~GroupView ();

    // general GVD operations

    Boolean getView          (ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&);
    Boolean lockGroup        (const Uid&, const char*, pid_t, LockMode, unsigned long&);
    Boolean getUseCount      (const Uid&, long&, unsigned long&);
    Boolean traverseDatabase (ReplicaDescriptor&, Uid&, int&, const char*, pid_t, unsigned long&);
    OutCome addReplicas      (const ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome deleteReplicas   (const ReplicaDescriptor&, const Uid&, const Boolean&, int&, const char*,
			      pid_t, unsigned long&);
    OutCome alterLocation    (const Uid&, const Uid&, const char*, const char*, pid_t, unsigned long&);
    OutCome exclude          (const ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome include          (const ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome releaseGroup     (const Uid&, const char*, pid_t, unsigned long&);
    Boolean groupAssociation (const Uid&, Uid&, unsigned long&);

    // operations concerned with node recovery

    OutCome recover             (const char*, unsigned long&);
    OutCome getNodeUpdateList   (ReplicaDescriptor&, ReplicaDescriptor&, const char*, unsigned long&);
    Boolean testAndSet          (ReplicaDescriptor&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome needsUpdating       (const Uid&, Boolean&, ReplicaDescriptor&, const char*, pid_t, unsigned long&);

    // operations concerned with composite objects

    OutCome addTemplate     (const Uid&, Buffer&, const char*, pid_t, unsigned long&);
    Boolean getTemplate     (const Uid&, Buffer&, LockMode, unsigned long&);
    OutCome deleteTemplate  (const Uid&, const char*, pid_t, unsigned long&);
    OutCome replaceTemplate (const Uid&, Buffer&, const char*, pid_t, unsigned long&);

    OutCome assignCompositeObject (const Uid&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome assignCompositeObject (const Uid&, const ReplicaDescriptor&, const char*, pid_t, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const Uid&, const char*, pid_t, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const ReplicaDescriptor&, const char*, pid_t, unsigned long&);

    // operations concerned with GVD recovery

    Boolean getState    (ObjectState&, unsigned long&);
    Boolean imposeState (ObjectState&);

    Boolean getReplicaState    (const Uid&, ObjectState&, unsigned long&);
    Boolean imposeReplicaState (const Uid&, ObjectState&, unsigned long&);
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    // list manipulation routines

    ObjectGroups* extendList     (ObjectGroups*) const;
    Boolean       removeFromList (ObjectGroups*, int&, const char*, pid_t) const;
    
    static ObjectGroups* searchAllLists        (const Uid&, ObjectGroups*, Boolean = TRUE);
    static ObjectGroups* searchList            (const Uid&, ObjectGroups*);
    static ObjectGroups* searchCompositeObject (const Uid&, ObjectGroups*);
    static ObjectGroups* searchWildCard        (ObjectGroups*);

    // create new replica list

    static ReplicaList* createReplicaList (const Uid&);
    
    static ObjectGroups* head;
    static int           numberofgroups;
    static ToPurge       deleteList;
    static unsigned long v_number;
};

#endif
