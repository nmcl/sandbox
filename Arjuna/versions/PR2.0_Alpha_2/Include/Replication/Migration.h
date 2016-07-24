/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Migration.h,
 */

#ifndef MIGRATION_H_
#define MIGRATION_H_

#ifndef NSINTERFACE_H_
#include <ArjServers/NSInterface.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif


enum MigrateStatus { GroupLocked, MovedOk,
		     FromNodeFailed, ToNodeFailed,
		     RegisterFailed, MoveFailed };

class Migrate
{
public:
    Migrate (int&);
    ~Migrate ();

    void typeIs (TypeName);
    MigrateStatus moveFromTo (const char*, const char*, const Uid&, const Uid&);

private:
    Boolean readFrom (const char*, const Uid&, Buffer&);
    Boolean writeTo (const char*, const Uid&, Buffer);

    TypeName tn;
    NSInterface* NS;
    class RemoteStateDaemon *S1, *S2;
    class RpcControl *RPCC1, *RPCC2;
};

#endif
