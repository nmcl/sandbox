/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockMan.h,v 1.20 1993/09/06 10:44:13 ngdp Exp $
 */

#ifndef LOCKMAN_H_

/*
 * WARNING - ROAD WORKS AHEAD!
 *
 * Stub Generation Nasties...
 *
 * Various cpp tricks are used in this file to massage the name of
 * the class defined herein. Be warned!
 * Certain Arjuna classes need to see ONLY the local definition of
 * LockManager (this file) while user classes need to see the stub
 * generated version (LockMan_stub.h). Problems can arise if this
 * file is included since it prevents inclusion of LockMan_stub.h
 * Short-circuit this by incuding the stub file by default.
 */

#ifndef STUB
#  define LOCKMANAGER_SERVER
#  define STATEMANAGER_SERVER
#  include <Arjuna/LockMan_stub.h>
#else

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

// @NoRemote, @NoMarshall
class LockStore;

// @NoRemote, @NoMarshall
class Semaphore;

// @Remote, @NoMarshall
class LockManager : public StateManager
{
  public:
    // @NoClient
    void propagate (const Uid& from, const Uid& to);
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
    LockStore *lockStore;		/* locks held in shared memory */
    
    Semaphore *mutex;			/* for mutual exclusion purposes */
    Boolean stateLoaded;

};

#endif

#endif
