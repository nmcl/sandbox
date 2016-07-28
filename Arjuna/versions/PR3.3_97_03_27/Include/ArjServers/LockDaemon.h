/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockDaemon.h,v 1.2 1995/06/26 11:37:44 ngdp Exp $
 */

#ifndef LOCKDAEMON_H_
#define LOCKDAEMON_H_

/*
 * Lock-base concurrency control daemon interface
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

class LockTable;
class ClientRpc;
class ClientRpcManager;
class RemoteLockDaemonIf;


// @NoRemote, @NoMarshall
class LockList;

// @Remote, @NoMarshall
class LockDaemon
{
public:
    LockDaemon (const char * = 0);
    ~LockDaemon ();
    
    Boolean propagate (const Uid&, const Uid&, const TypeName);
    Boolean releaseAll (const Uid&, const Uid&, const TypeName);
    Boolean releaselock (const Uid&, const Uid&, const TypeName);
    
    LockResult setlock (Lock*, const Uid&, const TypeName);
private:
    ClientRpc *clientHandle;
    ClientRpcManager *clientManager;
    RemoteLockDaemonIf *theDaemon;
    char *theHost;
};

#endif
