/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupInformation.h,v 1.3 1994/05/31 10:15:07 nmcl Exp $
 */

#ifndef GROUPINFORMATION_H_
#define GROUPINFORMATION_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif


//@NoRemote, @UserMarshall
class ReplicaDescriptor;

//@NoRemote, @NoMarshall
class NSInterface;


//@NoRemote, @NoMarshall
class GroupInformation
{
public:
    GroupInformation ();
    GroupInformation (const StateManager&);
    GroupInformation (const Uid&);
    virtual ~GroupInformation ();

    Boolean groupSize (unsigned long&);
    Boolean groupSize (unsigned long&, const Uid&);
    Boolean groupSize (unsigned long&, const StateManager&);

    Boolean currentGroup (ReplicaDescriptor*&);
    Boolean currentGroup (ReplicaDescriptor*&, const Uid&);
    Boolean currentGroup (ReplicaDescriptor*&, const StateManager&);

    Boolean lockGroup (LockMode, const Uid& = NIL_UID);

private:
    Boolean getGroup (ReplicaDescriptor*&);

    Boolean activateDatabase ();
    Boolean releaseGroup     ();

    Uid*         group_uid;
    NSInterface* NS;
    Boolean      doRelease;
};

#include <Replication/GroupInformation.h>

#endif
