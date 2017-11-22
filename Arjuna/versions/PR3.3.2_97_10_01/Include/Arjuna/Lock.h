/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Lock.h,v 1.6 1995/06/26 11:39:05 ngdp Exp $
 */

#ifndef LOCK_H_
#define LOCK_H_

#ifndef STUB
#  include <Arjuna/Lock_stub.h>
#else

/*
 * The remainder is only processed by the stub generator
 */

/*
 *
 * The Basic Lock object
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @NoMarshall
class RpcBuffer;

//  @NoRemote, @NoMarshall	
class ActionHierarchy;

typedef unsigned int LockMode;

extern const LockMode READ;
extern const LockMode WRITE;

enum LockStatus
{
    LOCKFREE, LOCKHELD, LOCKRETAINED
};

/*
 * We deliberately derive lock types from the stub mangled
 * class name to ensure that we can never accidentally
 * create a lock that has a remote StateManager component.
 * If we are running the stub generator over this file however use cpp
 * to temporarily put the 'real' name back.
 */

#ifdef STUB
#  define LocalStateManager StateManager
#endif

//  @NoWarnings, @NoRemote, @UserMarshall
class Lock : public LocalStateManager
{
    friend class LockList;
    friend class LockListI;

public:
    /* Constructors and destructor */

    Lock ();
    Lock (LockMode lm);			/* Lock constructor */
    Lock (const Uid& storeUid);		/* Lock reconstructor from store */
    virtual ~Lock ();
    
    /* non-virtual member functions */
    
    LockMode getLockMode () const;
    const Uid& getCurrentOwner () const;
    const ActionHierarchy& getAllOwners () const;
    LockStatus getCurrentStatus () const;

    /* Used by stub system */

    void marshall (RpcBuffer&) const;
    void unmarshall (RpcBuffer&);
       
    /* manipulation functions */

    void changeHierarchy (const ActionHierarchy& newOwner);
    void propagate ();

    /* virtual member functions and operators */

    virtual Boolean conflictsWith  (const Lock& otherLock) const;
    virtual Boolean modifiesObject () const;
    
    virtual Boolean operator==  (const Lock& otherLock) const;

    /* inherited functions */

    virtual ostream& print (ostream& strm) const ;    
    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);    
    virtual const TypeName type () const;

private:

    /* List chaining routines for Locks */

    Lock *getLink () const;
    void setLink (Lock *pointTo);

    /* private state variables */

    LockStatus currentStatus;		/* Current status of lock */
    Lock *nextLock;
    LockMode lMode;			/* Typically READ or WRITE */
    ActionHierarchy* owners;		/* Uid of owner action (faked if no */
					/* action) */ 
};

#include <Arjuna/Lock.n>

#endif
#endif
