/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Lock.h,v 1.1 1993/11/03 14:27:50 nmcl Exp $
 */

#ifndef LOCK_H_
#define LOCK_H_

#ifdef STUB
#  pragma @NoRename
#endif

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

typedef unsigned int LockMode;

extern const LockMode READ;
extern const LockMode WRITE;

enum LockStatus
{
    LOCKFREE, LOCKHELD, LOCKRETAINED
};


//  @NoWarnings, @NoRemote, @NoMarshall
class Lock : public StateManager
{
    friend class LockList;
    friend class LockListI;

public:
    /* Constructors and destructor */

    Lock (LockMode lm);			/* Lock constructor */
    Lock (const Uid& storeUid);		/* Lock reconstructor from object store */
    ~Lock ();
    
    /* non-virtual member functions */
    
    LockMode getLockMode () const;
    const Uid& getOwner () const;
    LockStatus getStatus () const;
    Boolean isAction () const;

    /* manipulation functions */

    void setOwner (const Uid& newOwner, Boolean isAction);
    void setStatus (LockStatus newStatus);

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

    LockStatus currentStatus;	       /* Current status of lock */
    Lock *nextLock;
    Boolean isActionLock;	       /* True if lock set as part of action */
    LockMode lMode;		       /* Typically READ or WRITE */
    Uid owner;			       /* Uid of owner action (faked if no */
				       /* action) */ 
};

#include <Arjuna/Lock.n>

#endif
