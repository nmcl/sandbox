/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockDaemon.cc,v 1.3 1995/06/26 13:00:08 ngdp Exp $
 */

/*
 *
 * Lock concurrency controller daemon manager.
 * Handle the messy parts of interfacing to stub generated code
 *
 */ 

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef LOCKDAEMON_H_
#  include <ArjServers/LockDaemon.h>
#endif

#ifndef LOCKDAEMONIF_STUB_H_
#  include "LockDaemonIf_stub.h"
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

static const char RCSid[] = "$Id: LockDaemon.cc,v 1.3 1995/06/26 13:00:08 ngdp Exp $";

/* Prevents RPC failues terminating the client */

static Boolean rpcFailureHandler (RPC_Status, Int32)
{
    return FALSE;
}

/*
 * Public non-virtual functions 
 */

LockDaemon::LockDaemon( const char *onHost )
		      : clientHandle(0),
			clientManager(0),
			theDaemon(0),
			theHost(clone(onHost))
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LockDaemon::LockDaemon()" << endl;
#endif

    clientHandle = new ClientRpc("LockDaemon");

    if (clientHandle)
    {
	rpcAbortHandler oldHandler;
	
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
	UNIQUEID rUid;

	rUid.host = rUid.sec = rUid.msec = DAEMONUID;
	
	clientHandle->setObjectUid(rUid);
	
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_)
	clientHandle->disableAtomic();
#endif
#endif

	if (theHost)
	    clientHandle->setHost(theHost);
	
	/* establish global handler temporarily */
	
	oldHandler = setHandler(rpcFailureHandler);
	
	clientManager = new ClientRpcManager(clientHandle);

	if (clientManager)
	{
	    Boolean rVal = FALSE;

	    clientManager->setHandler(rpcFailureHandler);
	    
	    theDaemon = new LockDaemonIf(rVal, clientManager);
	    if (!rVal)			/* check for good start up */
	    {
		delete theDaemon;
		theDaemon = 0;
	    }
	}

	/* put back original global handler */
	setHandler(oldHandler);
	
    }
}

LockDaemon::~LockDaemon ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LockDaemon::~LockDaemon()" << endl;
#endif    

    if (theHost)
    {
#ifdef GNU_DELETE_BUG
	delete theHost;
#else
	delete [] theHost;
#endif
    }
	
    if (theDaemon)
	delete theDaemon;

    if (clientManager)
	delete clientManager;

    if (clientHandle)
	delete clientHandle;
}

/*
 * propagate: Change lock ownership as nested action commits. All
 * locks owned by the committing action have their owners changed to be
 * the parent of the committing action. AtomicAction ensures this is only
 * called at nested commit.
 * We pop things off one list onto another because insert might decide
 * that 2 locks are replicas of each other so we can discard one.
 */

Boolean LockDaemon::propagate ( const Uid& fromAction,
				const Uid& objUid,
				const TypeName type )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockDaemon::propagate(" << fromAction << ", " 
		 << objUid << ", " << type <<  ")" << endl;
#endif

    Boolean rVal = FALSE;
    
    if (theDaemon)
	theDaemon->propagate(fromAction, objUid, type, rVal);

    return rVal;
}

/*
 * releaseAll: Clear out all locks for a given action. Should be
 * triggered automatically at top-level commit but is also user
 * callable so is potentially dangerous.
 */

Boolean LockDaemon::releaseAll ( const Uid& actionUid,
				 const Uid& objUid,
				 const TypeName type )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockDaemon::releaseAll(" << actionUid << ")" << endl;
#endif

    Boolean rVal = FALSE;
 
    if (theDaemon)
	theDaemon->releaseAll(actionUid, objUid, type, rVal);

    return rVal;
}

/*
 * releaselock: Release a SINGLE LOCK lock that has the given uid. Can break
 * two-phase locking rules so watch out!
 */

Boolean LockDaemon::releaselock ( const Uid& lockUid,
				  const Uid& objUid,
				  const TypeName type )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockDaemon::releaselock(" << lockUid << ")" << endl;
#endif
    
    Boolean rVal = FALSE;

    if (theDaemon)
	theDaemon->releaselock(lockUid, objUid, type, rVal);

    return rVal;
}

/*
 * setlock: This is the main user visible operation. Attempts to set
 * the given lock on the current object.  lock cannot be set, then
 * the lock attempt is retried retry times before giving up and
 * returning an error. This gives a simple handle on deadlock.
 */

LockResult LockDaemon::setlock ( Lock *toSet,
				 const Uid& objUid,
				 const TypeName type )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockDaemon::setlock(" << toSet << ", "
		 << objUid << ", " << type << ")" << endl;
#endif

    LockResult rVal = REFUSED;
    
    if (theDaemon)
	theDaemon->setlock(toSet, objUid, type, rVal);

    return rVal;
}
