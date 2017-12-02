/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServ.h,v 1.11 1994/08/30 08:17:15 ngdp Exp $
 */

#ifndef NAMESERV_H_
#define NAMESERV_H_

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef REPLICADESC_H_
#  include "RepDesc.h"
#endif



// @NoRemote, @NoMarshall
class GroupView;

// @NoRemote, @NoMarshall
class ServerView;


enum OutCome
{
    NOTDONE, DONE, LOCKED, INUSE, NORESPONSE
};

// @Remote, @NoMarshall
class NameServer : public LockManager
{
public:
    NameServer  (int&);
    NameServer  (const Uid&, int&);
    ~NameServer ();

    // general GVD operations

    Boolean getView          (ReplicaDescriptor&, const Uid&, const char*, pid_t, int&, unsigned long&);
    Boolean lockGroup        (const Uid&, const char*, pid_t, LockMode, int&, unsigned long&);
    Boolean getUseCount      (const Uid&, long&, int&, unsigned long&);
    Boolean traverseDatabase (ReplicaDescriptor&, Uid&, int&, const char*, pid_t, int&, unsigned long&);
    OutCome addReplicas      (const ReplicaDescriptor&, const Uid&, const char*, pid_t, int&, unsigned long&);
    OutCome deleteReplicas   (const Uid&, const ReplicaDescriptor&, const char*, pid_t, int&,
			      unsigned long&, const Boolean& = FALSE);
    OutCome alterLocation    (const Uid&, const Uid&, const char*, const char*, pid_t, int&, unsigned long&);
    OutCome exclude          (const ReplicaDescriptor&, const Uid&, const char*, pid_t, int&, unsigned long&);
    OutCome include          (const ReplicaDescriptor&, const Uid&, const char*, pid_t, int&, unsigned long&);

    OutCome releaseGroup     (const Uid&, const char*, pid_t, int&, unsigned long&);
    Boolean groupAssociation (const Uid&, Uid&, int&, unsigned long&);

    // operations concerned with node recovery

    OutCome recover             (const char*, int&, unsigned long&);
    Boolean testAndSet          (ReplicaDescriptor&, const Uid&, const char*, pid_t, int&, unsigned long&);
    OutCome needsUpdating       (const Uid&, Boolean&, ReplicaDescriptor&, const char*, pid_t, int&, unsigned long&);
    OutCome getNodeUpdateList   (ReplicaDescriptor&, ReplicaDescriptor&, const char*, int&, unsigned long&);

    // operations concerned with GVD recovery

    Boolean getGroupViewState       (ObjectState&, int&, unsigned long&);
    Boolean imposeNewGroupViewState (ObjectState&, int&);

    Boolean getReplicaState         (const Uid&, ObjectState&, int&, unsigned long&);
    Boolean imposeReplicaState      (const Uid&, ObjectState&, int&, unsigned long&);

    // operations concerned with composite objects

    OutCome addTemplate           (const Uid&, Buffer&, const char*, pid_t, int&, unsigned long&);
    Boolean getTemplate           (const Uid&, Buffer&, LockMode, int&, unsigned long&);
    OutCome deleteTemplate        (const Uid&, const char*, pid_t, int&, unsigned long&);
    OutCome replaceTemplate       (const Uid&, Buffer&, const char*, pid_t, int&, unsigned long&);
    OutCome assignCompositeObject (const Uid&, const Uid&, const char*, pid_t, int&, unsigned long&);
    OutCome assignCompositeObject (const Uid&, const ReplicaDescriptor&, const char*, pid_t, int&, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const Uid&, const char*, pid_t, int&, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const ReplicaDescriptor&, const char*, pid_t, int&, unsigned long&);

    // Conventional Arjuna naming service operations

    void storeName   (Boolean&, Buffer, Buffer, int&, unsigned long&);
    void fetchName   (Boolean&, Buffer, Buffer&, int&, unsigned long&);
    void removeName  (Boolean&, Buffer, int&, unsigned long&);
    void replaceName (Boolean&, Buffer, Buffer, int&, unsigned long&);
    void getNextName (Boolean&, Buffer&, int&, int&, unsigned long&);

    void reset(int&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    GroupView* GV; // groupview database
    ServerView* SV; // nameserver database
};

#endif
