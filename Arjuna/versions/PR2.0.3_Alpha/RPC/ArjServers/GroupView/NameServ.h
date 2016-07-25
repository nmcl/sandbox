/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServ.h,v
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
    NotDone, Done,
    Locked, InUse
};


class NameServer : public LockManager
{
public:
    NameServer (unsigned short&);
    NameServer (const Uid&, unsigned short&);
    ~NameServer ();

    // general GVD operations

    Boolean getView (ReplicaDescriptor&, const Uid&, const char*, pid_t, int&,
		     unsigned long&, const LockMode = READ);
    Boolean getUseCount (const Uid&, long&, int&, unsigned long&);
    Boolean traverseDatabase (ReplicaDescriptor&, Uid&, int&, const char*,
			      pid_t, int&, unsigned long&);
    OutCome addReplicas (const ReplicaDescriptor&, const Uid&, int&,
			 unsigned long&);
    OutCome deleteReplicas (const Uid&, const ReplicaDescriptor&, int&,
			    unsigned long&, const Boolean& = FALSE);
    OutCome alterLocation (const Uid&, const Uid&, const char*, int&,
			   unsigned long&);
    OutCome exclude (const ReplicaDescriptor&, const Uid&, int&,
		     unsigned long&);
    OutCome include (const ReplicaDescriptor&, const Uid&, int&,
		     unsigned long&);

    OutCome releaseGroup (const Uid&, const char*, pid_t, int&, unsigned long&);
    Boolean groupAssociation (const Uid&, Uid&, int&, unsigned long&);

    // operations concerned with node recovery

    OutCome recover (const char*, int&, unsigned long&);
    Boolean getExclusiveAccess (int&);
    Boolean testAndSet (ReplicaDescriptor&, const Uid&, const char*, pid_t,
			int&, unsigned long&);
    OutCome needsUpdating (const Uid&, Boolean&, ReplicaDescriptor&,
			   const char*, pid_t, int&, unsigned long&);
    OutCome getNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&,
			       const char*, int&, unsigned long&);

    // operations concerned with GVD recovery

    Boolean getGroupViewState (ObjectState&, int&, unsigned long&);
    Boolean imposeNewGroupViewState (ObjectState&, int&);

    // operations concerned with composite objects

    OutCome addTemplate (const Uid&, Buffer&, int&, unsigned long&);
    Boolean getTemplate (const Uid&, Buffer&, int&, unsigned long&);
    OutCome deleteTemplate (const Uid&, int&, unsigned long&);
    OutCome replaceTemplate (const Uid&, Buffer&, int&, unsigned long&);
    OutCome assignCompositeObject (const Uid&, const Uid&, int&,
				   unsigned long&);
    OutCome assignCompositeObject (const Uid&, const ReplicaDescriptor&, int&,
				   unsigned long&);
    OutCome removeCompositeObject (const Uid&, const Uid&, int&,
				   unsigned long&);
    OutCome removeCompositeObject (const Uid&, const ReplicaDescriptor&, int&,
				   unsigned long&);

    // Conventional Arjuna naming service operations

    void storeName (Boolean&, Buffer, Buffer, unsigned long&);
    void fetchName (Boolean&, Buffer, Buffer&, unsigned long&);
    void removeName (Boolean&, Buffer, unsigned long&);
    void replaceName (Boolean&, Buffer, Buffer, unsigned long&);
    void getNextName (Boolean&, Buffer&, int&, unsigned long&);

    void reset(int&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    GroupView* GV; // groupview database
    ServerView* SV; // nameserver database
};

#endif
