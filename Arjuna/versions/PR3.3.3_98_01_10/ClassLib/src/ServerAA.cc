/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerAA.cc,v 1.19 1996/03/29 14:10:14 ngdp Exp $
 */

/*
 *
 * ServerAtomicAction class
 *
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifdef DEBUG 
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

static const char RCSid[] = "$Id: ServerAA.cc,v 1.19 1996/03/29 14:10:14 ngdp Exp $";

ServerAtomicAction* ServerAtomicAction::currentSvrAct  = 0;
char*               ServerAtomicAction::coordinNodename = 0;

/* Constructors and destructor */
    
ServerAtomicAction::ServerAtomicAction (const Uid& U,
					AtomicAction::ActionType at) 
                                       : AtomicAction(U, at),
					 parentServerAction(0),
					 previousPrepareOutcome(PREP_NOTOK),
					 savingUid()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::ServerAtomicAction (const Uid&)" << endl;
    debug_stream.unlock();
    
#endif
}

ServerAtomicAction::~ServerAtomicAction ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::~ServerAtomicAction()" << endl;
    debug_stream.unlock();
    
#endif

    if ((parentSAA() == 0) && (coordinNodename))
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] coordinNodename;
#else
	delete coordinNodename;
#endif
	coordinNodename = 0;
    }
}

/* User Atomic action operations */

ActionStatus ServerAtomicAction::Begin ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::Begin()" << endl;
    debug_stream.unlock();
    
#endif

    if (status() == CREATED)
    {
        parentServerAction = currentSvrAct;
        currentSvrAct = this;

        return AtomicAction::Begin();
    }
    else
    {
        addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::Begin() : incorrect invocation sequence\n"));
        return status();
    }
  
}

ActionStatus ServerAtomicAction::End ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::End()" << endl;
    debug_stream.unlock();
    
#endif

    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
		       "ServerAtomicAction::End() : incorrect invocation sequence\n"));
	
    return status();
}

ActionStatus ServerAtomicAction::Abort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::Abort()" << endl;
    debug_stream.unlock();
    
#endif

    if (status() == RUNNING)
        return AtomicAction::Abort();
    else
    {
        addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::Abort() : incorrect invocation sequence\n"));
        return status();
    }
}

void ServerAtomicAction::setCoordinNodename ( const char *name )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::setCoordinNodename("
		 << name << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    if (coordinNodename)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] coordinNodename;
#else
	delete coordinNodename;
#endif	
	coordinNodename = 0;
    }
    
    if (name)
    {
	coordinNodename = clone(name);
    }
}

char *ServerAtomicAction::getCoordinNodename ()
{
    char *name = 0;
    
    if (coordinNodename)
    {
	name = clone(coordinNodename);
    }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::getCoordinNodename() "
		 << "returning (" << name << ")" << endl;
    debug_stream.unlock();
    
#endif

    return name;
}

Uid ServerAtomicAction::getSavingUid () const
{
    return savingUid;
}

void ServerAtomicAction::finish()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::finish()" << endl;
    debug_stream.unlock();
    
#endif

    if (this != currentSvrAct)
    {
        addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::finish() : incorrect invocation sequence\n"));
	return;
    }

    switch (status())
    {
    case CREATED:
	break;
    case RUNNING:
        AtomicAction::Abort();
	break;
    case ABORTED:
	break;
    case COMMITTED:
	break;
    case PREPARED:
	AtomicAction::phase2Abort();
	break;
    default:
	break;
    }
    
    currentSvrAct = parentServerAction;
}

ServerAtomicAction* ServerAtomicAction::currentSAA ()
{
    return currentSvrAct;
}

ServerAtomicAction* ServerAtomicAction::parentSAA ()
{
    return parentServerAction;
}

/* Primitive Atomic action operations */ 

Boolean ServerAtomicAction::doNestedAbort ( Uid& actionUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::doNestedAbort (Uid&) on "
		 << (void*) AtomicAction::Current() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean done =  FALSE;

    if (actionUid == get_uid())
    {
	if (status() == PREPARED)
	{
	    AtomicAction::phase2Abort();
	    done =  TRUE;
	}
	else if (status() == RUNNING)
	{
	    AtomicAction::Abort();
	    done =  TRUE;
	}
	else if (status() == ABORTED)
	    done =  TRUE;
	else
	{
	    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			       "ServerAtomicAction::doNestedAbort() : incorrect invocation sequence\n"));

	    done =  FALSE;
	}
    }
    else
    {
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::doNestedAbort() : Wrong action!\n"));
	done =  FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::doNestedCommit ( Uid& actionUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::doNestedCommit (Uid&) on "
		 << (void*) AtomicAction::Current() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean done =  FALSE;

    if (actionUid == get_uid())
    {
	if (status() == PREPARED)
	{
	    AtomicAction::phase2Commit();
	    done = TRUE;
	}
	else if (status() == COMMITTED)
	    done =  TRUE;
	else
	{
	    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			       "ServerAtomicAction::doNestedCommit() : incorrect invocation sequence\n"));
	    done =  FALSE;
	}
    }
    else
    {
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::doNestedCommit() : Wrong action!\n"));
	done =  FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::doNestedPrepare ( Uid& actionUid, PrepareOutcome& outCome )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::doNestedPrepare (Uid&, PrepareOutcome&) on "
		 << (void*) AtomicAction::Current() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean done =  FALSE;
    outCome = PREP_OK;

    if (actionUid == get_uid())
    {
	if (status() == RUNNING)
	{
	    outCome = AtomicAction::prepare();
	    previousPrepareOutcome = outCome;
	    done =  TRUE;
	}
	else if (status() == PREPARED)
	{
	    outCome = previousPrepareOutcome;
	    done =  TRUE;
	}
	else if (status() == ABORTED)
	{
	    outCome = PREP_NOTOK;
	    done = TRUE;
	}
	else
	{
	    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			       "ServerAtomicAction::doNestedPrepare() : incorrect invocation sequence\n"));
	    done = FALSE;
	}
    }
    else
    {
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::doNestedPrepare() : Wrong action!\n"));
	done = FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::doTopLevelAbort ( Uid& actionUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::doTopLevelAbort (Uid&) on "
		 << (void*) AtomicAction::Current() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean done = FALSE;

    if (actionUid == get_uid())
    {
	if (status() == PREPARED)
	{
	    AtomicAction::phase2Abort();
	    done =  TRUE;
	}
	else if (status() == RUNNING)
	{
	    AtomicAction::Abort();
	    done =  TRUE;
	}
	else if (status() == ABORTED)
	    done =  TRUE;
	else
	{
	    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			       "ServerAtomicAction::doTopLevelAbort() : incorrect invocation sequence\n"));
	    done = FALSE;
	}
    }
    else
    {
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::doTopLevelAbort() : Wrong action!\n"));
	done = FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::doTopLevelCommit ( Uid& actionUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::doTopLevelCommit (Uid&) on "
		 << (void*) AtomicAction::Current() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean done = FALSE;
    
    if (actionUid == get_uid())
    {
	if (status() == PREPARED)
	{
	    AtomicAction::phase2Commit();
	    done =  TRUE;
	}
	if (status() == COMMITTED)
	    done =  TRUE;
	else
	{
	    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			       "ServerAtomicAction::doTopLevelCommit() : incorrect invocation sequence\n"));
	    done = FALSE;
	}
    }
    else
    {
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::doTopLevelCommit() : Wrong action!\n"));
	done = FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::doTopLevelPrepare ( Uid& actionUid, PrepareOutcome& outCome )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::doTopLevelPrepare (Uid&, PrepareOutcome&) on "
		 << (void*) AtomicAction::Current() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean done = FALSE;
    outCome = PREP_OK;

    if (actionUid == get_uid())
    {
	if (status() == RUNNING)
	{
	    outCome = AtomicAction::prepare();
	    previousPrepareOutcome = outCome;

	    if (outCome == READONLY)
		AtomicAction::phase2Commit(); /* Co-ordinator will not talk to us again */

	    done =  TRUE;
	}
	else if (status() == PREPARED)
	{
	    outCome = previousPrepareOutcome;
	    done =  TRUE;
	}
	else if (status() == ABORTED)
	{
	    outCome = PREP_NOTOK;
	    done = TRUE;
	}
	else
	{
	    addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			       "ServerAtomicAction::doTopLevelPrepare() : incorrect invocation sequence\n"));
	
	    done = FALSE;
	}
    }
    else
    {
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER,
			   "ServerAtomicAction::doTopLevelPrepare() : Wrong action!\n"));
	done = FALSE;
    }

    return done;
}

/* inherited public virtual functions */

Boolean ServerAtomicAction::restore_state ( ObjectState& s, ObjectType )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::restore_state" << endl;
    debug_stream.unlock();
    
#endif

    return ( ( AtomicAction::restore_state(s, ANDPERSISTENT) &&
	       savingUid.unpack(s) && 
               s.unpack(coordinNodename) )
	     ? TRUE : FALSE );
}

Boolean ServerAtomicAction::save_state ( ObjectState& s, ObjectType )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::save_state" << endl;
    debug_stream.unlock();
    
#endif

    Boolean res = TRUE;
    Uid myUid(get_uid());

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::save_state: "
		 << "Co-ord = " << coordinNodename << ", Uid = "
		 << myUid << endl;
    debug_stream.unlock();
    
#endif

    if ( (coordinNodename == 0) || (::strcmp(coordinNodename, "") == 0) )
    {
	addError(new Error(FAC_ATOMIC_ACTION, SM_SAVE_FAILED,
			   "ServerAA::save_state: Co-ordinator nodename unkown\n"));
	res = FALSE;
    }
    else
    {
	res = AtomicAction::save_state(s, ANDPERSISTENT);

	/*
	 * Only save ServerAtomicAction extras if AtomicAction has packed
	 * something i.e. some records existed on the atomic action prepared list
	 */
	
	if (res && s.notempty())
	{
	    res = ( myUid.pack(s) &&
		    s.packNonMappedString(coordinNodename) );
	}
    }
    return res;
}

const TypeName ServerAtomicAction::type () const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::::type ()" << endl;
    debug_stream.unlock();
    
#endif

    return "/StateManager/AtomicAction/ServerAtomicAction";
}



