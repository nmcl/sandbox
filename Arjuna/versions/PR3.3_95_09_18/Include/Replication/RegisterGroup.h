/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RegisterGroup.h,v 1.6 1994/05/11 11:36:46 nmcl Exp $
 */

#ifndef REGISTERGROUP_H_
#define REGISTERGROUP_H_

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
class ReplicaCreate;


//@NoRemote, @NoMarshall
class RegisterGroup
{
public:
    RegisterGroup (const Uid&);
    RegisterGroup (const Uid&, ReplicaCreate*);
    RegisterGroup (const Uid&, ReplicaDescriptor*);
    virtual ~RegisterGroup ();

    Boolean recordGroup ();
    Boolean removeGroup ();

    Boolean modifyGroup (ReplicaCreate*);
    Boolean modifyGroup (const ReplicaDescriptor&);

    Boolean lockGroup (LockMode);

protected:
    Boolean releaseGroup ();
    Boolean needToRelease () const;

private:
    Boolean activateDatabase ();

    const Uid               *group_uid;
    ReplicaDescriptor       *rd1, *rd2;
    NSInterface             *NS;
    Boolean                 doRelease;
};

#include <Replication/RegisterGroup.n>

#endif
