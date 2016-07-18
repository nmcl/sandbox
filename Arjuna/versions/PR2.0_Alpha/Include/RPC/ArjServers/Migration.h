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
#include <RPC/ArjServers/NSInterface.h>
#endif

class RemoteStateDaemon;

enum MigrateStatus { GroupLocked, MovedOk,
		     FromNodeFailed, ToNodeFailed,
		     RegisterFailed, MoveFailed };

class Migrate
{
public:
    Migrate (int&);
    ~Migrate ();

    void TypeIs (TypeName);
    MigrateStatus MoveFromTo (const char*, const char*,
			      const Uid&, const Uid&);

private:
    Boolean readFrom (const char*, const Uid&, Buffer&);
    Boolean writeTo (const char*, const Uid&, Buffer);

    TypeName tn;
    NSInterface* NS;
    RemoteStateDaemon *S1, *S2;
};

#endif
