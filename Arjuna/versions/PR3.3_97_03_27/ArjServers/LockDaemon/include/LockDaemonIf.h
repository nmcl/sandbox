/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockDaemonIf.h,v 1.3 1995/03/07 10:31:24 ngdp Exp $
 */

#ifndef LOCKDAEMONIF_H_
#define LOCKDAEMONIF_H_

/*
 * Local lock manager for lock daemon. Not accessed directly only
 * through LockDaemon interface.
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

static const Int32 DAEMONUID = 666;

class LockTable;

// @NoRemote, @UserMarshall
class Lock;

// @NoRemote, @AutoMarshall
class Uid;

// @NoRemote, @NoMarshall
class LockList;

// @Remote, @NoMarshall
class LockDaemonIf
{
public:
    LockDaemonIf (Boolean&);
    ~LockDaemonIf ();
    
    void propagate (const Uid&, const Uid&, const TypeName, Boolean&);
    void releaseAll (const Uid&, const Uid&, const TypeName, Boolean&);
    void releaselock (const Uid&, const Uid&, const TypeName, Boolean&);
    
    void setlock (/* @In, @NoDelete */ Lock*, const Uid&, const TypeName, LockResult&);
    
private:
    Boolean doRelease (const Uid&, const Uid&, const TypeName, Boolean);
    Boolean initialise ();
    Boolean isAncestorOf (const Lock&, const Lock&);
    ConflictType lockConflict (Lock*, const Uid&, const TypeName);
    
    LockTable *locksHeld;
    Semaphore *mutex;
};

#endif
