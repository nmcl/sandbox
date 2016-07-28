/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaList.cc,v 1.23 1995/09/30 10:15:46 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef TEMPLATEOBJECT_H_
#  include <ArjServers/TemplateObject.h>
#endif

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef REPLICALIST_H_
#  include "ReplicaList.h"
#endif

#ifndef COMPOBJECT_H_
#  include "CompositeObject.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ReplicaList* ReplicaList::headOfList = (ReplicaList*) 0;


UserData::UserData ()
                   : hosts((char*) 0),
                     pids(-1),
		     useCount(0),
                     next(0)
{
}

UserData::UserData (const char* name, pid_t pID, unsigned long count)
                   : hosts((char*) 0),
                     pids(pID),
                     useCount(count),
                     next(0)
{
    if (name)
    {
	hosts = ::new char[::strlen(name)+1];
	::memset(hosts, '\0', ::strlen(name)+1);
	::memcpy(hosts, name, ::strlen(name));
    }
}

UserData::~UserData ()
{
    if (hosts)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] hosts;
#else
        ::delete hosts;
#endif
}

Boolean UserData::pack (Buffer& packInto) const
{
    return (packInto.packMappedString(hosts) && packInto.pack(pids) &&
	    packInto.pack(useCount));
}

Boolean UserData::unpack (Buffer& unpackFrom)
{
    if (hosts)
    {
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] hosts;
#else
	::delete hosts;
#endif
	hosts = (char*) 0;
    }

    return (unpackFrom.unpack(hosts) && unpackFrom.unpack(pids) && unpackFrom.unpack(useCount));
}

void UserData::deleteAll (UserData*& deleteFrom)
{
    if (deleteFrom)
    {
        UserData* indx = deleteFrom;
	
	while (indx)
	{
	    deleteFrom = indx->next;
	    delete indx;
	    indx = deleteFrom;
	}

	deleteFrom = (UserData*) 0;
    }
}


ReplicaList::ReplicaList (int& res)
			 : LockManager(ANDPERSISTENT),
			   use_count(0),
			   v_number(1),
			   group(0),
			   self(0),
			   mask(0),
			   envelope(0),
			   clients(0),
			   linkCount(0),
			   next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PROTECTED;
    debug_stream << "ReplicaList::ReplicaList (int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
        res = ((A.End() == COMMITTED) ? 0 : -1);
    else
	A.Abort();
}

ReplicaList::ReplicaList (const Uid& u_id, int& res)
			 : LockManager(u_id),
			   use_count(0),
			   v_number(0),
			   group(0),
			   self(0),
			   mask(0),
			   envelope(0),
			   clients(0),
			   linkCount(0),
			   next(0)  
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PROTECTED;
    debug_stream << "ReplicaList::ReplicaList (Uid& u_id, int& res)\n" << flush;
#endif

    AtomicAction A;

    res = -1;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
        res = ((A.End() == COMMITTED) ? 0 : -1);
    else
	A.Abort();
}

ReplicaList::~ReplicaList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PROTECTED;
    debug_stream << "ReplicaList::~ReplicaList ()\n" << flush;
#endif

    terminate();

    if (group)
	delete group;

    if (clients)
        UserData::deleteAll(clients);

    if (self)
        GroupData::deleteAll(self);

    if (envelope)
        CompositeObject::deleteAll(envelope);

    if (mask)
        TemplateObject::deleteAll(mask);
}

ReplicaList* ReplicaList::create (int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList* ReplicaList::create (int& res)" << endl;
#endif

    ReplicaList* rlPtr = new ReplicaList(res);

    if (res == 0)
    {
        rlPtr->next = ReplicaList::headOfList;
	ReplicaList::headOfList = rlPtr;
	if (rlPtr)
	    rlPtr->linkCount++;
    }
    else
    {
        delete rlPtr;
	rlPtr = 0;
    }

    return rlPtr;
}

ReplicaList* ReplicaList::create (const Uid& u, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList* ReplicaList::create (const Uid& u, int& res)" << endl;
#endif

    ReplicaList* rlPtr = ReplicaList::headOfList;
    Boolean      found = FALSE;

    if (ReplicaList::headOfList == (ReplicaList*) 0)
    {
        ReplicaList::headOfList = new ReplicaList(u, res);
	rlPtr = ReplicaList::headOfList;

	if (res != 0)
	{
	    delete ReplicaList::headOfList;
	    ReplicaList::headOfList = (ReplicaList*) 0;
	}
    }
    else
    {
        while ((rlPtr->next) && (!found))
	{
	    if (rlPtr->get_uid() == u)
	        found = TRUE;
	    else
	        rlPtr = rlPtr->next;
	}

	if (!found)
	{
	    rlPtr->next = new ReplicaList(u, res);
	    if (res != 0)
	    {
		delete rlPtr->next;
		rlPtr->next = (ReplicaList*) 0;
		return (ReplicaList*) 0;
	    }
	    
	    rlPtr = rlPtr->next;
	}
    }

    rlPtr->linkCount++;
    return rlPtr;
}

void ReplicaList::remove (ReplicaList*& rlPtr, Boolean canDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaList::remove (ReplicaList*& rlPtr)" << endl;
#endif

    ReplicaList* tempPtr = ReplicaList::headOfList;
    ReplicaList* indx    = (ReplicaList*) 0;
    Boolean      found   = FALSE;

    if (!rlPtr)
	return;

    while ((tempPtr) && (!found))
    {
        if (tempPtr == rlPtr)
	    found = TRUE;
	else
	{
	    indx = tempPtr;
	    tempPtr = tempPtr->next;
	}
    }

    if (found)
    {
        tempPtr->linkCount--;
	if (tempPtr->linkCount == 0)
	{
	    if (canDelete)
	    {
		if (tempPtr == ReplicaList::headOfList)
		    ReplicaList::headOfList = tempPtr->next;
		else
		    indx->next = tempPtr->next;
		
		delete tempPtr;
	    }
	    else
		tempPtr->linkCount++;
	}
    }
    else
        delete rlPtr;

    rlPtr = (ReplicaList*) 0;
}

/*
 * Locking policy for use_count "lock" is necessary for exclusive access:
 * - if use_count is 0 then can get lock;
 * - if use_count is 1 then can get lock only if this is the process which set
 *      the lock;
 * - otherwise cannot be granted lock.
 */

Boolean ReplicaList::canLock (const char* hostname, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::canLock ( " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (use_count == 0)
	    result = TRUE;
	else
	{
	    if (use_count == 1)
	    {
	        UserData* tmpUD = clients;
		Boolean   found = FALSE;
	
		while ((!found) && (tmpUD))
		{
		    if ((::strcmp(tmpUD->hosts, hostname) == 0) && (userPid == tmpUD->pids))
		    {
		        found = TRUE;
			result = (Boolean) (tmpUD->useCount == 1);
		    }
		    else
		        tmpUD = tmpUD->next;
		}
	    }
	}
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
        A.Abort();

    return result;
}

OutCome ReplicaList::assignCompositeObject (const Uid& toAssign, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::assignCompositeObject ( " << toAssign << " )" << endl;
#endif

    AtomicAction A;
    OutCome      result = NOTDONE;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	version = v_number++;
	
	if (canLock(hostname, userPid))
	{
	    if (toAssign != *group) // try to avoid assigning to self!!
	    {
		if (envelope == (CompositeObject*) 0)
		    envelope = new CompositeObject();

		result = ((envelope->addObject(toAssign, *group) == TRUE) ? DONE : NOTDONE);
	    }
#ifdef DEBUG
	    else
		error_stream << WARNING << "Attempt to assign group " << *group << " to itself!" << endl;
#endif	    
	}
	else
	    result = INUSE;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::assignCompositeObject (const ReplicaDescriptor& rd, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::assignCompositeObject (const ReplicaDescriptor& rd)" << endl;
#endif

    AtomicAction A;
    OutCome result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
        result = addToList(rd, hostname, userPid, version);
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

OutCome ReplicaList::removeCompositeObject (const Uid& toRemove, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::removeCompositeObject ( " << toRemove << " )" << endl;
#endif

    AtomicAction     A;
    OutCome          result  = NOTDONE;
    CompositeObject* tempPtr = (CompositeObject*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	version = v_number++;
	
	if (canLock(hostname, userPid))
	{
	    if (*group != toRemove)
	    {
		/* If we have a composite group then try it, otherwise simply return DONE. */

		if (envelope)
		{
		    if (envelope->getSubObject() == toRemove)
		    {
			tempPtr = envelope;
			envelope = envelope->next;
			delete tempPtr;
		    }
		    else
			envelope->removeObject(toRemove);
		}
		result = DONE;
	    }
	    else
		error_stream << WARNING << "Attempt to remove group " << *group << " from itself!" << endl;
	}
	else
	    result = INUSE;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::removeCompositeObject (const ReplicaDescriptor& rd, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::removeCompositeObject (const ReplicaDescriptor& rd)" << endl;
#endif

    AtomicAction A;
    OutCome      result = NOTDONE;
    long         left   = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        result = deleteFromList(rd, left, hostname, userPid, version);
	if (left == 0)
	    result = NOTDONE;
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

/*
 * If a replica group does not exist as a separate entity (ReplicaList) within
 * the database then it may exist solely as an entry within another
 * ReplicaList (i.e., as a composite object).
 */

Boolean ReplicaList::searchComposite (const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::searchComposite ( " << u << " )" << endl;
#endif

    AtomicAction A;
    Boolean      found = FALSE;
    GroupData*   indx  = (GroupData*) 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        indx = self;
	while ((indx) && (!found))
	{
	    if (indx->getGroupUid() == u)
	        found = TRUE;
	    else
	        indx = indx->next;
	}
    }

    if (found)
        found = (Boolean) (A.End() == COMMITTED);
    else
        A.Abort();

    return found;
}

Boolean ReplicaList::searchWildCard ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::searchWildCard ()" << endl;
#endif

    AtomicAction A;
    Boolean      found   = FALSE;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (mask)
	{
	    GroupData* tmplate = mask->tmplate;
	    found = (Boolean) (tmplate->isWildCard() || tmplate->isGlobalWildCard());
	}
    }
    
    if (found)
        found = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return found;
}
    
OutCome ReplicaList::addTemplate (Buffer& toUse, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::addTemplate (Buffer& toUse)" << endl;
#endif

    AtomicAction A;
    OutCome result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;

	/*
	 * Cannot create template if replicas already in group.
	 * Cannot add to template if there are users of group.
	 */

	if ((canLock(hostname, userPid)) || (self != (GroupData*) 0))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		v_number++;

		// Cannot add template if one already exists.

		if (mask == (TemplateObject*) 0)
		{
		    GroupData* ptrEntry = new GroupData;

		    mask = new TemplateObject;
		    if (ptrEntry->unpack(toUse))
		    {
			mask->tmplate = ptrEntry;
			result = DONE;
		    }
		    else
		    {
			TemplateObject::deleteAll(mask);
			delete ptrEntry;
		    }
		}
		else
		    result = INUSE;
	    }
	    else
	        result = LOCKED;
	}
	else
	    result = INUSE;
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

Boolean ReplicaList::getTemplate (Buffer& toUse, LockMode lMode, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::getTemplate (Buffer& toUse)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(lMode), 0) == GRANTED)
    {
        version = v_number;
        if (mask)
	    result = mask->tmplate->pack(toUse);
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
        A.Abort();

    return result;
}
	    
OutCome ReplicaList::replaceTemplate (Buffer& toUse, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::replaceTemplate (Buffer& toUse)" << endl;
#endif

    AtomicAction A;
    OutCome result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;
	if (canLock(hostname, userPid))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		GroupData* dataToAdd = new GroupData;

		v_number++;
	        if (mask)
	            TemplateObject::deleteAll(mask);
		mask = new TemplateObject;
		if (dataToAdd->unpack(toUse))
		{
		    mask->tmplate = dataToAdd;
		    result = DONE;
		}
		else
		{
		    TemplateObject::deleteAll(mask);
		    delete dataToAdd;
		}
	    }
	    else
	        result = LOCKED;
	}
	else
	    result = INUSE;
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

/*
 * Returns the number of replicas which are governed by the template
 * (if any).
 */

int ReplicaList::numberOfTemplateReplicas () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "int ReplicaList::numberOfTemplateReplicas () const" << endl;
#endif

    return ((self == 0) ? 0 : self->getNumber());
}

OutCome ReplicaList::deleteTemplate (const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::deleteTemplate ()" << endl;
#endif

    AtomicAction A;
    OutCome result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;

	// cannot delete template if template objects remain.

	if ((canLock(hostname, userPid)) && (numberOfTemplateReplicas() == 0))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
	        v_number++;
		if (mask)
		{
		    TemplateObject::deleteAll(mask);
		    result = DONE;
		}
	    }
	    else
	        result = LOCKED;
	}
	else
	    result = INUSE;
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

Boolean ReplicaList::isMember (const Uid& replicaUid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::isMember ( " << replicaUid << " )" << endl;
#endif

    AtomicAction A;
    Boolean      opcode = FALSE;
    GroupData*   indx   = (GroupData*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	indx = self;
	version = v_number;

	for (int i = 0; (indx) && (!opcode); i++)
	{
	    if (indx->getUid() == replicaUid)
		opcode = TRUE;
	    else
		indx = indx->next;
	}
    }
    
    if (opcode)
        opcode = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return opcode;
}

Boolean ReplicaList::isMember (const ReplicaDescriptor& tempRD)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::isMember ()" << endl;
#endif

    GroupData makeElement(tempRD);
    return isMember(makeElement);
}

Boolean ReplicaList::isMember (const GroupData& tempGD)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::isMember ( " << tempGD << " )" << endl;
#endif

    AtomicAction A;
    Boolean      result = FALSE;
    GroupData*   indx   = (GroupData*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	indx = self;
	for (int i = 0; (indx) && (!result); i++)
	{
	    if (*indx == tempGD)
		result = TRUE;
	    else
		indx = indx->next;
	}
    }
    
    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}
    
Boolean ReplicaList::getUseCount (long& UseCount, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::getUseCount (long& UseCount, unsigned long& version)" << endl;
#endif

    AtomicAction A;
    Boolean opcode = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	UseCount = use_count;
	opcode = TRUE;
    }
    
    if (opcode)
        opcode = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return opcode;
}

Boolean ReplicaList::lockGroup (const char* hostname, pid_t userPid, unsigned long& version, LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::lockGroup ()\n" << flush;
#endif

    AtomicAction A;
    Boolean opcode = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;

        // can only get exclusive access if use_count is zero

        if (mode == WRITE)
	{
	    if ((canLock(hostname, userPid) && (setlock(new Lock(WRITE), 0) == GRANTED)))
	        opcode = TRUE;
	}
    }

    if (opcode)
        opcode = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return opcode;
}

OutCome ReplicaList::alterLocation (const Uid& replica, const char* location, const char* hostname, pid_t userPid,
				    unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::alterLocation ( " << replica << ", " << location << " )\n" << flush;
#endif

    AtomicAction A;
    GroupData*  ptr    = (GroupData*) 0;
    OutCome     result = NOTDONE;

    if (!location)
	return NOTDONE;
    
    A.Begin();

    /*
     * Check to see whether group is in use as we cannot
     * move replica otherwise.
     */

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        v_number++;
	version = v_number;

	if (canLock(hostname, userPid))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		version = v_number++;
		ptr = self;
		while ((ptr != (GroupData*) 0) && (result != DONE))
		{
		    if (ptr->getUid() == replica)
		    {
			ptr->setHost(location);
			result = DONE;
		    }
		    else
			ptr = ptr->next;
		}
	    }
	    else
		result = LOCKED;
	}
	else
	    result = INUSE;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

/*
 * Remove any out-of-date information for this recovering node from the
 * database.
 */

OutCome ReplicaList::recover (const char* hostname, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::recover ( " << hostname << " )" << endl;
#endif
    
    AtomicAction A;
    Boolean      found  = FALSE;
    OutCome      result = NOTDONE;
    UserData*    udPtr  = (UserData*) 0;

    A.Begin();

/*
 * This may look convoluted, but we do it this way to try to
 * maximise concurrency and reduce time database is locked.
 */

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        udPtr = clients;
	for (int i = 0; ((i < use_count) && (!found)); i++)
	{
	    if (::strcmp(udPtr->hosts, hostname) == 0)
	        found = TRUE;
	    else
	        udPtr = udPtr->next;
	}

	version = v_number;
	
	if (found)
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
	        UserData* tempPtr = (UserData*) 0;
		udPtr = clients;

		version = v_number++;
		
		while (udPtr)
		{
		    if (::strcmp(udPtr->hosts, hostname) == 0)
		    {
		        if (udPtr == clients)
			{
			    clients = udPtr->next;
			    tempPtr = clients;
			}
			else
			{
			    tempPtr->next = udPtr->next;
			    tempPtr = tempPtr->next;
			}

			delete udPtr;
			udPtr = tempPtr;
			use_count--;
		    }
		    else
		    {
		        tempPtr = udPtr;
			udPtr = udPtr->next;
		    }
		}

		result = DONE;
	    }
	    else
		result = LOCKED;
	}
	else
	    result = DONE;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

/*
 * We must keep track of how many times a given process makes use of
 * a group view as well as how many processes use it. This is to guarantee
 * consistency within a process as well as between processes.
 */

Boolean ReplicaList::isNew (const char* hostname, pid_t userPid) const
{
    UserData* indx = clients;
    for (int i = 0; i < use_count; i++)
    {
	if ((::strcmp(hostname, indx->hosts) == 0) && (indx->pids == userPid))
	{
	    indx->useCount++;
	    return FALSE;
	}
	else
	    indx = indx->next;
    }
    
    return TRUE;
}

void ReplicaList::addElement (const ReplicaDescriptor& rd)
{
    GroupData* indx = self;
    GroupData* ptr  = (GroupData*) 0;

    while (indx)
    {
        // keep group members together in list

        if (indx->getGroupUid() == rd.getGroupUid())
	{
	    GroupData* tempPtr = indx->next;

	    indx->next = new GroupData;
	    indx = indx->next;
	    *indx = rd;
	    indx->next = tempPtr;
	    return;
	}

	ptr = indx;
	indx = indx->next;
    }

    if (self == (GroupData*) 0)
    {
        self = new GroupData;
	ptr = self;
    }
    else
    {
        ptr->next = new GroupData;
        ptr = ptr->next;
    }

    *ptr = rd;
}

/*
 * Because we must keep track of usage of each group, whenever a group view
 * is obtained this requires setting a WRITE lock. The replication protocol
 * does this within short-lived actions so the group is not locked for any
 * long duration. However, we can use this to obtain sustained exclusive
 * access to the group view.
 */

ReplicaDescriptor* ReplicaList::getList (const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::getList ( " << hostname << ", " << userPid << " )\n" << flush;
#endif
    
    AtomicAction A;
    ReplicaDescriptor* resultData = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* tempData   = (ReplicaDescriptor*) 0;
    
    if (!hostname)
        return (ReplicaDescriptor*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	version = v_number;
	
	if (isNew(hostname, userPid))
	{
	    UserData* tempUD = new UserData(hostname, userPid, 1);
	    tempUD->next = clients;
	    clients = tempUD;
	    use_count++;
	}
	
	/* put template (if any) at front of list. */

	if (mask)
	{
	    resultData = new ReplicaDescriptor;
	    tempData = mask->getTemplate();
	    if (tempData)
	    {
		*resultData = *tempData;
		delete tempData;
	    }
	}

	/*
	 * If we have some replica information then return it.
	 * We must return all information, including that on replicas
	 * which have been excluded. We do this for the caching policy
	 * which occurs at the user.
	 */

	if (self)
	{
	    if (resultData == (ReplicaDescriptor*) 0)
	    {
		resultData = new ReplicaDescriptor;
		*resultData = *self;
	    }
	    else
		*resultData += *self;
	}
	
	if (envelope)
	{
	    tempData = envelope->getList(hostname, userPid);

	    if (tempData)
	    {
		if (resultData == (ReplicaDescriptor*) 0)
		    resultData = tempData;
		else
		    *resultData += *tempData;

		delete tempData;
	    }
	}
    }

    if (resultData != (ReplicaDescriptor*) 0)
    {
	if (A.End() != COMMITTED)
	{
	    delete resultData;
	    resultData = (ReplicaDescriptor*) 0;
	}
    }
    else
	A.Abort();

    return resultData;
}

/*
 * This will add all replicas which are not already members of the group,
 * i.e., replicas are added even if they are marked as being excluded.
 */

OutCome ReplicaList::addToList (const ReplicaDescriptor& rd, const char* hostname, pid_t userPid,
				unsigned long& version, Boolean override)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::addToList (const ReplicaDescriptor& rd)\n" << flush;
#endif

    AtomicAction A;
    const ReplicaDescriptor* indx   = &rd;
    OutCome                  result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((canLock(hostname, userPid)) || (override))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		version = v_number++;

		if (*(ReplicaDescriptor*)indx != NIL_RD)
		{
		    while (indx)
		    {
			Boolean found = FALSE;

			/*
			 * Only add if not already a member & not overriding.
			 */
			    
			while ((!found) && (indx))
			{
			    if (isMember(indx))
			    {
				found = FALSE;
				indx = indx->next;
				if (override)
				    result = DONE;
			    }
			    else
			    {
				if (override)
				{
				    if (indx->getActive() == FALSE)
					found = TRUE;
				    else
					indx = indx->next;
				}
				else
				    found = TRUE;
			    }
			}
		    
			/* found new member in list. */
			    
			if (found)
			{
			    addElement(*indx);
			    indx = indx->next;
			    result = DONE;
			}
		    }
		}
		else
		    result = LOCKED;
	    }
	    else
		result = INUSE;
	}
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();

    return result;
}

/*
 * This will remove all marked replicas if they are members of the group,
 * even if they are marked as being excluded.
 */

OutCome ReplicaList::deleteFromList (const ReplicaDescriptor& rd, long& left, const char* hostname, pid_t userPid,
				     unsigned long& version, Boolean override)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::deleteFromList (const ReplicaDescriptor& rd)\n" << flush;
#endif

    AtomicAction A;
    const   ReplicaDescriptor* ptr = &rd;
    OutCome result                 = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((canLock(hostname, userPid)) || (override))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		version = v_number++;
		result = DONE;

		if (self != (GroupData*) 0)
		{
		    while (ptr)
		    {
			GroupData* marker = self;
			GroupData* indx   = self;

			while (marker)
			{
			    if (*marker == *ptr)
			    {
				if (marker == self)
				{
				    self = marker->next;
				    delete marker;
				    marker = self;
				}
				else
				{
				    indx->next = marker->next;
				    delete marker;
				    marker = indx->next;
				}
			    }
			    else
			    {
				indx = marker;
				marker = marker->next;
			    }
			}

			ptr = ptr->next;
		    }

		    left = (((self == (GroupData*) 0) && (mask == (TemplateObject*) 0) && (envelope == (CompositeObject*) 0)) ? 0 : 1);
		}
	    }
	    else
		result = LOCKED;
	}
	else
	    result = INUSE;
    }
    else
	result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

/*
 * Returns information on whether the replica is marked as inactive.
 */

OutCome ReplicaList::hasChanged (const Uid& recovered, Boolean& isPresent,
				 Boolean& changed)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::hasChanged ( " << recovered << " )\n" << flush;
#endif

    AtomicAction A;
    GroupData* marker = self;
    OutCome    result = NOTDONE;

    changed = FALSE;
    isPresent = FALSE;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	result = DONE;

	while ((marker != (GroupData*) 0) && (!isPresent))
	{
	    if (marker->getUid() == recovered)
	    {
		isPresent = TRUE;
		if (!marker->getActive())
		    changed = TRUE;
	    }
	    else
		marker = marker->next;
	}
    }
    else
	result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

/*
 * This routine scans through the entire replica group and checks that
 * for each group mentioned at least one member is marked as being
 * available. If this is not so then it returns FALSE, and the action
 * will abort. This is necessary for crash recovery.
 */ 

Boolean ReplicaList::checkGroupSize ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::checkGroupSize ()" << endl;
#endif

    AtomicAction A;
    GroupData*   gdPtr = (GroupData*) 0;
    Boolean      result = TRUE;
    long         numberOfActive, numberOfInactive;
    long         minimumSize = ((mask == (TemplateObject*) 0) ? 1 : mask->sizeofTemplate());
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	gdPtr = self;

	while ((gdPtr) && (result))
	{
	    GroupData* trail      = gdPtr;
	    Boolean    endOfGroup = FALSE;
	    
	    numberOfActive = 0;
	    numberOfInactive = 0;

	    while ((trail) && (!endOfGroup))
	    {
		if (trail->getGroupUid() == gdPtr->getGroupUid())
		{
		    if (trail->getActive())
			numberOfActive++;
		    else
			numberOfInactive++;

		    trail = trail->next;
		}
		else
		    endOfGroup = TRUE;
	    }

	    if (mask)
	    {
		if (numberOfInactive >= minimumSize)
		{
		    error_stream << WARNING << "Exclude operation not done - would make entire group unavailable.\n" << flush;
		    result = FALSE;
		}
	    }
	    else
	    {
		if (numberOfActive < minimumSize)
		{
		    error_stream << WARNING << "Exclude operation not done - would make entire group unavailable.\n" << flush;
		    result = FALSE;
		}
	    }

	    gdPtr = trail;
	}
    }
    else
	result = FALSE;
    
    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}

/*
 * When excluding/including replicas that belong to a template
 * group, we actually add/remove information rather than modifying
 * the template data. This is to make crash recovery faster.
 */
    
OutCome ReplicaList::changeTemplateStatus (const ReplicaDescriptor& rd, const Boolean& state,
					   const char* hostname, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::changeTemplateStatus ()" << endl;
#endif

    OutCome       result = NOTDONE;
    unsigned long dummy  = 0;
    long          left   = 0;

    /*
     * Since this is a template we actually add replica information to
     * the group view when we exclude replicas, and remove this information
     * when we include them. We do not touch the original data.
     *
     * We also currently assume that the objects to be added are valid, i.e.,
     * that they were generated using this template. [To Change Later.]
     */

    if (state == FALSE)
	result = addToList(rd, hostname, userPid, dummy, TRUE);
    else
	result = deleteFromList(rd, left, hostname, userPid, dummy, TRUE);

    if ((mask->tmplate->isWildCard()) || (mask->tmplate->isGlobalWildCard()))
	return result;

    if (result != DONE)
        return result;

    result = ((state == TRUE) ? DONE : ((checkGroupSize() == TRUE) ? DONE : NOTDONE));
    
    return result;
}
    
OutCome ReplicaList::changeStatus (const ReplicaDescriptor& rd, const Boolean& state, const char* hostname, pid_t userPid,
				   unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::changeStatus ( const ReplicaDescriptor& rd, "
		 << ((state == TRUE) ? "TRUE" : "FALSE") << " )" << endl;
#endif

    AtomicAction A;
    OutCome      result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	version = v_number++;

	if (canLock(hostname, userPid))
	{
	    if (mask)
		result = changeTemplateStatus(rd, state, hostname, userPid);
	    else
	    {
	        const ReplicaDescriptor* ptr = &rd;

		while (ptr)
		{
		    GroupData* marker = self;

		    while (marker)
		    {
			if (*marker == *ptr)
			{
			    if (ptr->getActive() == state)
				marker->setActive(state);
			    result = DONE;
			    break;
			}
			else
			    marker = marker->next;
		    }
	
		    ptr = ptr->next;
		}

		if (!state)
		    result = ((checkGroupSize() == TRUE) ? DONE : NOTDONE);
		else
		    result = DONE;
	    }
	}
	else
	    result = INUSE;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

Boolean ReplicaList::getState (ObjectState& os, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::getState (ObjectState& os, unsigned long& version)" << endl;
#endif

    AtomicAction A;
    Boolean      result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	result = save_state(os, ANDPERSISTENT);
	version = v_number;
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();

    return result;
}

Boolean ReplicaList::imposeState (ObjectState& os, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::imposeState (ObjectState& os)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	result = restore_state(os, ANDPERSISTENT);
	version = v_number;
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::releaseGroup (const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::releaseGroup ( " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction A;
    OutCome   result   = NOTDONE;
    Boolean   released = FALSE;
    UserData* udPtr    = (UserData*) 0;
    UserData* indx     = (UserData*) 0;

    if (!hostname)
        return NOTDONE;

    A.Begin();

    /* Do not set WRITE lock until we are sure we have something to release. */

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        udPtr = clients;
	result = DONE;
        for (int i = 0; ((udPtr) && (!released) && (result == DONE)); i++)
	{
	    if ((::strcmp(udPtr->hosts, hostname) == 0) && (udPtr->pids == userPid))
	    {
		if (setlock(new Lock(WRITE), 0) == GRANTED)
		{
		    udPtr->useCount--;
		    if (udPtr->useCount == 0)
		    {
			if (udPtr == clients)
			{
			    clients = udPtr->next;
			    indx = clients;
			}
			else
			    indx->next = udPtr->next;

			delete udPtr;
			udPtr = indx;
			use_count--;
		    }
		    
		    released = TRUE;
		}
		else
		    result = INUSE;
	    }
	    else
	    {
	        indx = udPtr;
		udPtr = udPtr->next;
	    }
	}

	if (result == DONE)
	{
	    if (envelope)
		result = ((envelope->releaseGroup(hostname, userPid) == CompositeObject::DONE) ? DONE : NOTDONE);
	}
	
	version = v_number;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::assembleView (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				   const char* hostname)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void ReplicaList::assembleView ( " << hostname << " )" << endl;
#endif

    OutCome            result          = NOTDONE;
    Boolean            mayNeedUpdating = FALSE;
    GroupData*         marker          = (GroupData*) 0;
    ReplicaDescriptor* ptr             = &toupdate;
    ReplicaDescriptor* indx            = &unknownList;
    GroupData          tempData;
    AtomicAction       A;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	marker = self;
	
	if (self == (GroupData*) 0)
	    result = DONE;
	
	while (marker != (GroupData*) 0)
	{
	    if (::strcmp(marker->getHost(), hostname) == 0)
	    {
		tempData = *marker;
		mayNeedUpdating = TRUE;
		
		if (setlock(new Lock(WRITE), 0) == GRANTED)
		{
		    if (use_count == 0)
			mayNeedUpdating = FALSE;  // not in use so examine current view
		
		    if (!tempData.getActive())
		    {
			if (toupdate == NIL_RD)
			    toupdate = tempData;
			else
			    ptr = ptr->addToList(tempData);

			mayNeedUpdating = FALSE;  // it definitely needs updating!
		    }

		    result = DONE;
		}
		
		if (mayNeedUpdating)
		{
		    if (unknownList == NIL_RD)
			unknownList = tempData;
		    else
			indx = indx->addToList(tempData);

		    result = INUSE;
		}
	    }

	    marker = marker->next;
	}
    }
    else
	result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::assembleViewFromTemplate (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList, const char* hostname)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "OutCome ReplicaList::assembleViewFromTemplate ( " << hostname << " )" << endl;
#endif

    OutCome            result          = NOTDONE;
    Boolean            mayNeedUpdating = FALSE;
    GroupData*         marker          = (GroupData*) 0;
    ReplicaDescriptor* ptr             = &toupdate;
    ReplicaDescriptor* indx            = &unknownList;
    AtomicAction       A;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	marker = mask->tmplate;

	while (marker != (GroupData*) 0)
	{
	    GroupData* t_groupD = (GroupData*) 0;

	    if (::strcmp(marker->getHost(), hostname) == 0)
	    {
		mayNeedUpdating = TRUE;
		
		if (setlock(new Lock(WRITE), 0) == GRANTED)
		{
		    if (use_count == 0)
			mayNeedUpdating = FALSE;  // not in use so examine current view

		    if ((t_groupD) && (!t_groupD->getActive()))
		    {
			if (toupdate == NIL_RD)
			    toupdate = *t_groupD;
			else
			    ptr = ptr->addToList(*t_groupD);
			
			mayNeedUpdating = FALSE;  // it definitely needs updating!
		    }

		    result = DONE;
		}

		// assemble template element to send back.

		if (mayNeedUpdating)
		{
		    if (t_groupD == (GroupData*) 0)
		    {
			t_groupD = new GroupData;
			*t_groupD = *marker;
			t_groupD->setHost(hostname);
		    }
		    
		    if (unknownList == NIL_RD)
			unknownList = *t_groupD;
		    else
			indx = indx->addToList(*t_groupD);

		    result = INUSE;
		}
	    }

	    if (t_groupD)
	    {
		delete t_groupD;
		t_groupD = (GroupData*) 0;
	    }
	    
	    marker = marker->next;
	}
    }
    else
	result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::getHostInfo (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				  const char* hostname, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::getHostInfo ( " << hostname << " )" << endl;
#endif

    AtomicAction A;
    OutCome      result = NOTDONE;

    A.Begin();

    /*
     * The three cases to consider are:
     *
     * 1: the group is not in use and replica has been excluded
     * 2: the group is in use, but not locked - replica *may* be excluded
     * 3: the group is in use and locked - replica *may* be/have been excluded
     *
     * This is further complicated by the fact that groups can be composite objects
     * and/or contain templates.
     *
     * If the group is write locked then there is nothing we can do for this
     * group and node must try again later.
     *
     * We return the actual group view and not the composite structure, for
     * crash recovery.
     */

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	result = DONE;

	if (mask)
	{
	    result = assembleViewFromTemplate(toupdate, unknownList, hostname);
	    if (result == DONE)
		result = assembleView(toupdate, unknownList, hostname);
	}
	else
	    result = assembleView(toupdate, unknownList, hostname);
    }
    else
	result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

Boolean ReplicaList::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::save_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean   res;
    long      number = 0;
    UserData* indx = clients;

    if (self)
	number = self->getNumber();

    res = os.pack(number) && os.pack(v_number);

    if (group == (Uid*) 0)
	res = res && os.pack(-1);
    else
	res = res && os.pack(0) && group->pack(os);
    
    res = res && os.pack(use_count);

    for (int k = 0; k < use_count; k++)
    {
	res = res && indx->pack(os);
	indx = indx->next;
    }

    if (self)
	res = res && os.pack(0) && self->pack(os);
    else
	res = res && os.pack(-1);

    if (envelope)
	res = res && os.pack(0) && envelope->pack(os);
    else
	res = res && os.pack(-1);

    if (mask)
	res = res && os.pack(0) && mask->pack(os);
    else
	res = res && os.pack(-1);

    return res;
}

Boolean ReplicaList::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res = FALSE;
    long number = 0;
    int ptrType = -1;

    if (group)
    {
	delete group;
	group = (Uid*) 0;
    }
    
    if (envelope)
        CompositeObject::deleteAll(envelope);

    if (mask)
        TemplateObject::deleteAll(mask);

    if (self)
        GroupData::deleteAll(self);

    if (clients)
        UserData::deleteAll(clients);

    res = os.unpack(number) && os.unpack(v_number) && os.unpack(ptrType);
    
    if (res)
    {
        if (ptrType == 0)
	{
	    group = new Uid(NIL_UID);
	    res = group->unpack(os);
	}
    
	res = res && os.unpack(use_count);

	for (int k = 0; (k < use_count) && (res); k++)
	{
	    UserData* udPtr = new UserData;

	    res = udPtr->unpack(os);
	    if (res)
	    {
	        udPtr->next = clients;
		clients = udPtr;
	    }
	}
    
	res = res && os.unpack(ptrType);
	if (res)
	{
	    if (ptrType == 0)
	    {
	        self = new GroupData;
		res = self->unpack(os);
	    }
	    
	    res = os.unpack(ptrType);
	    if (res)
	    {
	        if (ptrType == 0)
		{
		    envelope = new CompositeObject;
		    res = envelope->unpack(os);
		}
		
		res = os.unpack(ptrType);
		if (res)
		{
		    if (ptrType == 0)
		    {
		        mask = new TemplateObject;
			res = mask->unpack(os);
		    }
		}
	    }
	}
    }

    return res;
}

ostream& ReplicaList::printContents (ostream& strm) const
{
    UserData* udPtr = clients;

    strm << "Group ID : " << *group << "\n";
    if (self)
    {
        strm << "Number of replicas : " << self->getNumber() << "\n";
        strm << self;
    }
    else
        strm << "Number of replicas : 0" << "\n";

    if (envelope)
        strm << envelope;

    for (int i = 0; i < use_count; i++)
    {
        strm << "Client " << udPtr->pids << " on node " << udPtr->hosts << "\n";
	udPtr = udPtr->next;
    }
    strm << "Use count : " << use_count << endl;
    return strm;
}

ostream& operator<< (ostream& strm, const ReplicaList& rl)
{
    return rl.printContents(strm);
}

const TypeName ReplicaList::type () const
{
    return "/StateManager/LockManager/ReplicaList";
}
