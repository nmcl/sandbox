/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockMan.h,v 1.3 1993/11/16 09:34:20 ngdp Exp $
 */

#ifndef LOCKMAN_H_
#define LOCKMAN_H_

/*
 * Lock-base concurrency control manager class. The remainder of this
 * file only processed by the stub generator.
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

enum LockResult
{
    GRANTED, REFUSED, RELEASED
};

enum ReleaseType
{
    SINGLE_LOCK, ALL_LOCKS
};

enum ConflictType
{
    CONFLICT, COMPATIBLE, PRESENT
};

// @NoRemote, @NoMarshall
class LockList;

#if !defined(OS2) && !defined(MSDOS)

// @NoRemote, @NoMarshall
class LockStore;

// @NoRemote, @NoMarshall
class Semaphore;

#endif

// @Remote, @NoMarshall
class LockManager : public StateManager
{
public:
    // @NoClient
    Boolean propagate (const Uid& from, const Uid& to);
    // @NoClient
    LockResult releaseAll (const Uid& actionUid);	/* release all locks for action */
    // @NoClient
    LockResult releaselock (const Uid& lockUid); /* release one lock */
    
    // @NoClient
    LockResult setlock (Lock *toSet, int retry = 100,
			unsigned int sleepTime = 250000 /* .25 seconds */);
    
    /* virtual functions inherited from StateManager */
    
    // @NoClient
    virtual ostream& print (ostream& strm) const;
    // @NoClient
    virtual ostream& print (ostream& strm);
    
    virtual Boolean restore_state (ObjectState& os, ObjectType ot) = 0;
    virtual Boolean save_state (ObjectState& os, ObjectType ot) = 0;
    virtual const TypeName type () const;
    
protected:
    /* Constructors and destructor */
    
    LockManager (ObjectType ot = RECOVERABLE, Boolean isShareable = TRUE);
    LockManager (const Uid& storeUid, ObjectType ot = ANDPERSISTENT);
    ~LockManager ();
    
    virtual void terminate ();
    
private:
    /* non-virtual member functions */
    
    void doRelease (const Uid& u, ReleaseType oneOrAll);
    void freeState ();
    Boolean initialise ();
    Boolean isAncestorOf (const Lock& heldLock);
    Boolean loadState ();
    ConflictType lockConflict (const Lock& otherLock);
    Boolean unloadState ();
    
    /* private state variables - ORDER is CRUCIAL HERE DO NOT CHANGE */
    
    key_t systemKey;			/* used in accessing system resources */
    
    LockList *locksHeld;		/* the actual list of locks set */

    Boolean stateLoaded;
};

#endif
