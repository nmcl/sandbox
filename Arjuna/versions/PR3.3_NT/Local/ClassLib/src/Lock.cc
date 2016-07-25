/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Lock.cc,v 1.13 1995/10/13 09:02:35 ngdp Exp $
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

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef RESTRICTED_ARJUNA

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif

#endif

const LockMode READ = 0;
const LockMode WRITE = 1;

static const char RCSid[] = "$Id: Lock.cc,v 1.13 1995/10/13 09:02:35 ngdp Exp $";

static Uid applicUid;		/* In case lock set outside AA */
static long applicPid = ::getpid();		/* process id */

/*
 * Public functions
 */

/*
 * Default constructor - used by RPC system
 */

Lock::Lock ()
	   : LocalStateManager(NEITHER),
	     currentStatus(LOCKFREE),
	     nextLock(0),
	     owners(new ActionHierarchy(0))
{
}

/*
 * Constructor : create a new Lock object and initialise it. Mode is
 * based upon argument. The value of AtomicAction::Current determines the 
 * values of the remainder of the fields.
 * If there is no action running the owner field is set to be the application
 * uid created when the application starts.
 */

Lock::Lock ( LockMode lm )
           : LocalStateManager(NEITHER),
	     currentStatus(LOCKFREE),
	     nextLock(0),
	     lMode(lm),
	     owners(new ActionHierarchy(0))
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::Lock(" << lm << ")" << endl;
    debug_stream.unlock();
    
#endif

    AtomicAction *curr = AtomicAction::Current();
    
    if (curr == 0)
    {
	long currentPid = ::getpid();
	ActionHierarchy ah(1);		/* max depth of 1 */
	
	if (applicPid != currentPid)
	{
	    Uid temp;
	    
	    /*
	     * Process id change probably due to a fork(). Get new pid 
	     * and generate a new Applic_Uid
	     */

	    applicPid = currentPid;
	    applicUid = temp;
	}

	ah.add(applicUid);
	*owners = ah;
    }
    else
    {
	ActionHierarchy *ah = curr->getHierarchy();
	
	*owners = *ah;
    }
}

/*
 * Constructor: This is used when re-initialising a Lock after
 * retreival from the object store.
 */

Lock::Lock ( const Uid& storeUid )
           : LocalStateManager(storeUid, NEITHER),
	     currentStatus(LOCKFREE),
	     nextLock(0),
	     owners(new ActionHierarchy(0))
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::Lock(" << storeUid << ")" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Destructor: General clean up as Lock is deleted.
 */

Lock::~Lock ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::~Lock()" << endl;
    debug_stream.unlock();
    
#endif

    LocalStateManager::terminate();
    
    if (owners)
	delete owners;
}

/*
 * Public utility operations. Most are sufficiently simple as to be self
 * explanatory!
 */

LockMode Lock::getLockMode () const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Lock::getLockMode()" << endl;
    debug_stream.unlock();
    
#endif

    return lMode;
}

const Uid &Lock::getCurrentOwner () const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Lock::getCurrentOwner()" << endl;
    debug_stream.unlock();
    
#endif

    return owners->getDeepestActionUid();
}

const ActionHierarchy& Lock::getAllOwners () const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Lock::getAllOwner()" << endl;
    debug_stream.unlock();
    
#endif

    return *owners;
}

LockStatus Lock::getCurrentStatus () const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Lock::getCurrentStatus()" << endl;
    debug_stream.unlock();
    
#endif

    return currentStatus;
}

#ifndef RESTRICTED_ARJUNA

/*
 * (Un)Marshall lock for RPC purposes. Since we cannot return
 * a status here there are no checks that the buffer pack/unpacks
 * are ok
 */

void Lock::marshall ( RpcBuffer& buff ) const
{
    buff.pack(currentStatus);
    buff.pack(lMode);
    owners->pack(buff);
}

void Lock::unmarshall ( RpcBuffer& buff )
{

    int retval;

    buff.unpack(retval);
    currentStatus = (LockStatus) retval;
    buff.unpack(lMode);
    owners->unpack(buff);
}

#endif

void Lock::changeHierarchy ( const ActionHierarchy& newOwner )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::changeHierarchy()" << endl;
    debug_stream << "Changing to :\n" << newOwner << endl;
    debug_stream.unlock();
    
#endif

    *owners = newOwner;

    if (currentStatus == LOCKFREE)
	currentStatus = LOCKHELD;
}

void Lock::propagate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::propagate()" << endl;
    debug_stream.unlock();
    
#endif

    owners->forgetDeepest();
    currentStatus = LOCKRETAINED;
}

Boolean Lock::modifiesObject () const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::modifiesObject()" << endl;
    debug_stream.unlock();
    
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
    debug_stream.lock();
    
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << OPERATORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::conflictsWith (" << (void *)&otherLock << ")\n"
	         << "\tLock 1 :\n" << *this << "\n"
		 << "\tLock 2 :\n" << otherLock << "" << endl;
    debug_stream.unlock();
    
#endif

    if (!(getCurrentOwner() == otherLock.getCurrentOwner()))
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
    debug_stream.lock();
    
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << OPERATORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::operator== (" << (void *)&otherLock << ")\n"
	         << "\tLock 1 :\n" << *this << "\n"
		 << "\tLock 2 :\n" << otherLock << endl;
    debug_stream.unlock();
    
#endif
    
    if ((lMode == otherLock.lMode) &&
	(*owners == *otherLock.owners) &&
	(currentStatus == otherLock.currentStatus))
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
#ifndef WIN32
	strm << "current_status : " << currentStatus << "\n";
#else
    strm << "current_status : " << (int) currentStatus << "\n";
#endif
    strm << "Mode : " << lMode << "\n";
    strm << "Owner List : " << *owners << "\n";
    return strm;
}

/*
 * Carefully restore the state of a Lock.
 */

Boolean Lock::restore_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::restore_state(" << (void *)&os << ", " << ot << ")" << endl;
    debug_stream.unlock();
    
#endif

    int cStat;
    LockMode lTemp;
    ActionHierarchy ah(0);

    if (os.unpack(cStat) && os.unpack(lTemp) && ah.unpack(os))
    {
	currentStatus = (LockStatus) cStat;
	lMode = lTemp;
	*owners = ah;
	return TRUE;
    }

    return FALSE;
}

/*
 * Save the state of a lock object.
 */

Boolean Lock::save_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "Lock::save_state(" << (void *)&os << ", " << ot << ")" << endl;
    debug_stream.unlock();
    
#endif

    os.pack((int) currentStatus);
    os.pack(lMode);
    owners->pack(os);

    if (os.readState() == BUFFER_GOOD)
		return TRUE;
    else
		return FALSE;
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
