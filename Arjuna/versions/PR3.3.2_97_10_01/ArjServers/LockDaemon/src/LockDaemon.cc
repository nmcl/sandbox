/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockDaemon.cc,v 1.3.2.1 1996/03/06 11:39:54 ngdp Exp $
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

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

static const char RCSid[] = "$Id: LockDaemon.cc,v 1.3.2.1 1996/03/06 11:39:54 ngdp Exp $";

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

}

LockDaemon::~LockDaemon ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LockDaemon::~LockDaemon()" << endl;
#endif    

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
    
    return rVal;
}

/*
 * releaseAll: Clear out all locks for a given action. Should be
 * triggered automatically at top-level commit but is also user
 * callable so is potentially dangerous.
 */

Boolean LockDaemon::releaseAll ( const Uid& actionUid,
				 const Uid&,
				 const TypeName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockDaemon::releaseAll(" << actionUid << ")" << endl;
#endif

    Boolean rVal = FALSE;
 
    return rVal;
}

/*
 * releaselock: Release a SINGLE LOCK lock that has the given uid. Can break
 * two-phase locking rules so watch out!
 */

Boolean LockDaemon::releaselock ( const Uid& lockUid,
				  const Uid&,
				  const TypeName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockDaemon::releaselock(" << lockUid << ")" << endl;
#endif
    
    Boolean rVal = FALSE;

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

    delete toSet;
    
    return rVal;
}

