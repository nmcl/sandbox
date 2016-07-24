/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Migrator.h,v 1.12 1995/09/25 15:02:59 nmcl Exp $
 */

#ifndef MIGRATOR_H_
#define MIGRATOR_H_

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <Replication/RemoteOSTypes.h>
#endif


class RemoteRPObjStore;
class ClientRpcManager;
class ClientRpc;


//@NoRemote, @NoMarshall
enum MigratorStatus { GroupLocked, MovedOk,
		      FromNodeFailed, ToNodeFailed,
		      RegisterFailed, MoveFailed };


/*
 * This could be a remotely invocable object but at present there is
 * no call for this, so we prevent it for now.
 */


//@NoRemote, @NoMarshall
class Migrator
{
public:
    Migrator (const char* = 0, const char* = 0);
    Migrator (int&, const char* = 0, const char* = 0);
    ~Migrator ();

    void           typeIs        (TypeName tn);
    MigratorStatus moveFromTo    (const char* from, const char* to,
				  const Uid& group, const Uid& replica);
    MigratorStatus moveFromTo    (const char* from, const char* to,
				  const Uid& replica);
    Boolean        setupDatabase ();

private:
    void tidyup ();
    
    Boolean readFrom (const char*, const Uid&, ObjectState*&);
    Boolean writeTo  (const char*, const Uid&, ObjectState);

    TypeName          tn;
    NSInterface       *NS;
    RemoteRPObjStore  *S1, *S2;
    ClientRpcManager  *RPCC1, *RPCC2;
    ClientRpc         *CRPC1, *CRPC2;
    const char        *objectStoreRoot1;
    const char        *objectStoreRoot2;
};

#endif
