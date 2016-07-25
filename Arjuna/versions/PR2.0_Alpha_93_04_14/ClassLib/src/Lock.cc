/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Lock.cc,v 1.10 1993/03/22 09:19:44 ngdp Exp $
 */

/*
 *
 * Standard Lock class
 *
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

const LockMode READ = 0;
const LockMode WRITE = 1;

static const char RCSid[] = "$Id: Lock.cc,v 1.10 1993/03/22 09:19:44 ngdp Exp $";

static Uid applicUid;		/* In case lock set outside AA */
static long applicPid;		/* process id */
/*
 * Public functions
 */

/*
 * Constructor : create a new Lock object and initialise it. Mode is
 * based upon argument. The value of AtomicAction::Current determines the 
 * values of the remainder of the fields.
 * If there is no action running the owner field is set to be the application
 * uid created when the application starts.
 */

Lock::Lock ( LockMode lm )
           : LocalStateManager(NEITHER),
	     lMode(lm),
	     currentStatus(LOCKFREE),
	     nextLock(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::Lock(" << lm << ")\n" << flush;
#endif

    AtomicAction *curr = AtomicAction::Current();
    
    if (curr == 0)
    {
	long currentPid = ::getpid();

	if (applicPid != currentPid)
	{
	    /*
	     * Process id change probably due to a fork(). Get new pid 
	     * and generate a new Applic_Uid
	     */

	    applicPid = currentPid;
	    applicUid = *new Uid();
	}

	isActionLock = FALSE;
	owner = applicUid;
    }
    else
    {
	isActionLock = TRUE;
	owner = curr->get_uid();
    }
}

/*
 * Constructor: This is used when re-initialising a Lock after
 * retreival from the object store.
 */

Lock::Lock ( const Uid& storeUid )
           : LocalStateManager(storeUid),
	     currentStatus(LOCKFREE),
	     nextLock(0),
	     isActionLock(FALSE),
	     owner(NIL_UID)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::Lock(" << storeUid << ")\n" << flush;
#endif
}

/*
 * Destructor: General clean up as Lock is deleted.
 */

Lock::~Lock ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::~Lock()\n" << flush;
#endif

    LocalStateManager::terminate();
}

/*
 * Public utility operations. Most are sufficiently simple as to be self
 * explanatory!
 */

LockMode Lock::getLockMode () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::getLockMode()\n" << flush;
#endif

    return lMode;
}

const Uid &Lock::getOwner () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::getOwner()\n" << flush;
#endif

    return owner;
}

LockStatus Lock::getStatus () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::getStatus()\n" << flush;
#endif

    return currentStatus;
}

Boolean Lock::isAction () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::isAction()\n" << flush;
#endif

    return isActionLock;
}

void Lock::setOwner ( const Uid& newOwner, Boolean isAction )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::setOwner(" << newOwner << ", " << isAction << ")\n" << flush;
#endif

    isActionLock = isAction;

    owner = newOwner;

    if (currentStatus == LOCKFREE)
	currentStatus = LOCKHELD;
}

void Lock::setStatus ( LockStatus newStatus )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::setStatus(" << newStatus << ")\n" << flush;
#endif

    currentStatus = newStatus;
}

Boolean Lock::modifiesObject () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::modifiesObject()\n" << flush;
#endif

    return ((lMode == WRITE) ? TRUE : FALSE);
}

/*
 * Virtual functions
 */

/*
 * Implementation of Lock conflict check. Returns TRUE if there is
 * conflict FALSE otherwise. Does not take account of relationship in
 * the atomic action hierarchy since this is a function of
 * LockManager. 
 * We use the public operations of Lock to access the internals of
 * otherLock despite the fact that C++ allows us direct access to maintain
 * encapsulation. 
 */

Boolean Lock::conflictsWith ( const Lock& otherLock ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << OPERATORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::conflicts_with (" << (void *)&otherLock << ")\n"
	         << "\tLock 1 :\n" << *this << "\n"
		 << "\tLock 2 :\n" << otherLock << "\n" << flush;
#endif

    if (!(owner == otherLock.getOwner()))
	switch (lMode)
	{
	case WRITE:
	    return TRUE;		/* WRITE conflicts always */

	case READ:
	    if (otherLock.getLockMode() != READ)
		return TRUE;
	    break;
	}
    return FALSE;			/* no conflict between these locks */
}

Boolean Lock::operator== ( const Lock& otherLock ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << OPERATORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::operator== (" << (void *)&otherLock << ")\n"
	         << "\tLock 1 :\n" << *this << "\n"
		 << "\tLock 2 :\n" << otherLock << "\n" << flush;
#endif
    
    if ((lMode == otherLock.getLockMode()) &&
	(owner == otherLock.getOwner()))
	return TRUE;

    return FALSE;
}

/*
 * virtual functions inherited from StateManager
 */

ostream& Lock::print ( ostream& strm ) const
{
    strm << "Lock object at : " << (void *const)this << "\n";
    strm << "unique id is : " << get_uid() << "\n";
    strm << "isactionlock : " << isActionLock << "\n";
    strm << "current_status : " << currentStatus << "\n";
    strm << "Mode : " << lMode << "\n";
    strm << "Owner : " << owner << "\n";
    return strm;
}

/*
 * Restore the state of a Lock.
 */

Boolean Lock::restore_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::restore_state(" << (void *)&os << ", " << ot << ")\n" << flush;
#endif

    int retval;

    os.unpack(isActionLock);

    os.unpack(retval);
    currentStatus = (LockStatus) retval;
    os.unpack(lMode);
    owner.unpack(os);
    return(TRUE);
}

/*
 * Save the state of a lock object.
 */

Boolean Lock::save_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::save_state(" << (void *)&os << ", " << ot << ")\n" << flush;
#endif

    os.pack(isActionLock);
    os.pack(currentStatus);
    os.pack(lMode);
    owner.pack(os);
    return(TRUE);
}

const TypeName Lock::type () const
{
    return "/StateManager/LockManager";
}

/*
 * Private functions
 */

Lock *Lock::getLink () const
{
    return nextLock;
}

void Lock::setLink ( Lock *pointTo )
{
    nextLock = pointTo;
}

ostream& operator<< ( ostream& strm, LockStatus ls )
{
    strm << (ls == LOCKFREE ? "LOCKFREE"
	                    : (ls == LOCKHELD ? "LOCKHELD"
			                      : "LOCKRETAINED"));
    return strm;
}


#ifdef NO_INLINES
#  define LOCK_CC_
#  include "Arjuna/Lock.n"
#  undef LOCK_CC_
#endif
