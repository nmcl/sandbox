/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServ.h
 */

#ifndef NAMESERV_H_
#define NAMESERV_H_

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef LOCK_H_
#include <Arjuna/Lock.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

typedef char* string;

#ifndef REPLICADESC_H_
#include "RepDesc.h"
#endif

// @NoRemote, @NoMarshall
class GroupView;

// @NoRemote, @NoMarshall
class ServerView;

class NameServer : public LockManager
{
public:
    NameServer (unsigned short&);
    NameServer (const Uid&, unsigned short&);
    ~NameServer ();

// GroupView database operations

    Boolean Get_View (ReplicaDescriptor&, const Uid&, int&, unsigned long&, const LockMode = READ);
    Boolean Add_Replicas (const ReplicaDescriptor&, const Uid&, int&, unsigned long&);
    Boolean Delete_Replicas (const Uid&, const ReplicaDescriptor&, int&, unsigned long&, const Boolean = FALSE);
    Boolean Release_Group (const Uid&, int&, unsigned long&);
    Boolean Traverse_Database (ReplicaDescriptor&, Uid&, int&, int&, unsigned long&);
    Boolean ChangeReplicaStatus (const ReplicaDescriptor&, const Uid&, const Boolean, int&, unsigned long&);
    Boolean NeedsUpdating (const Uid&, ReplicaDescriptor&, int&, unsigned long&);
    Boolean GetNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, string, Boolean&, int&, unsigned long&);
    Boolean GetExclusiveGroupView (ReplicaDescriptor&, const Uid&, int&, unsigned long&);
    
    Boolean CheckAndSet (const Uid&, int&, unsigned long&);
    Boolean AlterLocation (const Uid&, const Uid&, string, int&, unsigned long&);

    Boolean GetGroupViewState (ObjectState&, int&, unsigned long&);
    Boolean ImposeNewGroupViewState (ObjectState&, int&);
    
// Conventional Arjuna naming service operations

    void StoreName (Boolean&, Buffer, Buffer, unsigned long&);
    void FetchName (Boolean&, Buffer, Buffer&, unsigned long&);
    void RemoveName (Boolean&, Buffer, unsigned long&);
    void ReplaceName (Boolean&, Buffer, Buffer, unsigned long&);
    void GetNextName (Boolean&, Buffer&, int&, unsigned long&);

    void Reset(int&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    Boolean gv_ok, sv_ok; // has groupview or nameserver been initialised yet?
    GroupView* GV; // groupview database
    ServerView* SV; // nameserver database
};

#endif
