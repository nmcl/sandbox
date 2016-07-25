/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaList.cc,v
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

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef REPLICALIST_H_
#  include "ReplicaList.h"
#endif

#ifndef COMPOBJECT_H_
#  include "CompositeObject.h"
#endif

#ifndef TEMPLATEOBJECT_H_
#  include "TemplateObject.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ReplicaList* ReplicaList::headOfList;


UserData::UserData ()
                    : hosts(0),
                      pids(-1),
                      next(0)
{
}

UserData::~UserData ()
{
    if (hosts)
#ifndef __GNUG__
        ::delete [] hosts;
#else
        ::delete hosts;
#endif
}

Boolean UserData::pack (Buffer& packInto) const
{
    return (packInto.pack(hosts) && packInto.pack(pids));
}

Boolean UserData::unpack (Buffer& unpackFrom)
{
    if (hosts)
    {
#ifndef __GNUG__
        ::delete [] hosts;
#else
	::delete hosts;
#endif
	hosts = 0;
    }

    return (unpackFrom.unpack(hosts) && unpackFrom.unpack(pids));
}


ReplicaList::ReplicaList (int& res) : LockManager(ANDPERSISTENT),
                                      next(0),
                                      linkCount(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList::ReplicaList (int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    self = 0;
    envelope = 0;
    mask = 0;
    clients = 0;
    group = 0;
    use_count = 0;
    v_number = 1;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
	else
	    res = -1;
    }
    else
	A.Abort();
}

ReplicaList::ReplicaList (const Uid& u_id, int& res) : LockManager(u_id),
                                                       next(0),
                                                       linkCount(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList::ReplicaList (Uid& u_id, int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    self = 0;
    envelope = 0;
    mask = 0;
    clients = 0;
    group = 0;
    use_count = 0;
    v_number = 1;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

ReplicaList::~ReplicaList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList::~ReplicaList()\n" << flush;
#endif

    GroupData *ptr = self, *marker;
    UserData *indx1 = clients, *indx2 = 0;

    terminate();

    if (group)
	delete group;

    while (ptr)
    {
	marker = ptr;
	ptr = ptr->next;
	delete marker;
    }

    while (indx1)
    {
        indx2 = indx1;
	indx1 = indx1->next;
	delete indx2;
    }

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

    ReplicaList* rlPtr = headOfList;

    if (headOfList == 0)
    {
        headOfList = new ReplicaList(res);
	if (res != 0)
	{
	    delete headOfList;
	    headOfList = 0;
	}
	rlPtr = headOfList;
    }
    else
    {
        while (rlPtr->next)
            rlPtr = rlPtr->next;

	rlPtr->next = new ReplicaList(res);
	if (res != 0)
	{
	    delete rlPtr->next;
	    rlPtr->next = 0;
	}
	
	rlPtr = rlPtr->next;
    }
    
    if (rlPtr)
	rlPtr->linkCount++;

    return rlPtr;
}

ReplicaList* ReplicaList::create (const Uid& u, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList* ReplicaList::create (const Uid& u, int& res)" << endl;
#endif

    ReplicaList* rlPtr = headOfList;
    Boolean found = FALSE;
    
    if (headOfList == 0)
    {
        headOfList = new ReplicaList(u, res);
	rlPtr = headOfList;
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
		rlPtr->next = 0;
		return 0;
	    }
	    
	    rlPtr = rlPtr->next;
	}
    }

    rlPtr->linkCount++;
    return rlPtr;
}

void ReplicaList::remove (ReplicaList*& rlPtr)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaList::remove (ReplicaList*& rlPtr)" << endl;
#endif

    ReplicaList *tempPtr = headOfList, *indx = 0;
    Boolean found = FALSE;
    
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
	    if (tempPtr == headOfList)
	        headOfList = tempPtr->next;
	    else
	        indx->next = tempPtr->next;

	    delete tempPtr;
	}
    }
    else
        delete rlPtr;

    rlPtr = 0;
}

OutCome ReplicaList::assignCompositeObject (const Uid& toAssign, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::assignCompositeObject ( " << toAssign << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	version = v_number;
	
	if (use_count == 0)
	{
	    if (toAssign != *group) // try to avoid assigning to self!!
	    {
		if (envelope == 0)
		    envelope = new CompositeObject();

		result = ((envelope->addObject(toAssign, get_uid()) == TRUE) ? Done : NotDone);
	    }
#ifdef DEBUG
	    else
		error_stream << WARNING << "Attempt to assign group " << *group << " to itself!" << endl;
#endif	    
	}
	else
	    result = InUse;
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::assignCompositeObject (const ReplicaDescriptor& rd, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::assignCompositeObject (const ReplicaDescriptor& rd)" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
        result = addToList(rd, version);
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

OutCome ReplicaList::removeCompositeObject (const Uid& toRemove, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::removeCompositeObject ( " << toRemove << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    CompositeObject* tempPtr = 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	version = v_number;
	
	if (use_count == 0)
	{
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

		result = Done;
	    }
	}
	else
	    result = InUse;
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::removeCompositeObject (const ReplicaDescriptor& rd, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::removeCompositeObject (const ReplicaDescriptor& rd)" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    long left = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        result = deleteFromList(rd, left, version);
	if (left == 0)
	    result = NotDone;
    }
    else
        result = Locked;

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
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
    Boolean found = FALSE;
    GroupData* indx = 0;

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
    {
        if (A.End() != COMMITTED)
	    found = FALSE;
    }
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
    Boolean found = FALSE;
    ReplicaDescriptor* tmplate = 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (mask)
	{
	    tmplate = mask->getTemplate();
	    
	    if ((::strcmp(tmplate->getObjectName(), wildCardName) == 0) ||
		(::strcmp(tmplate->getObjectName(), globalWildCardName) == 0))
		found = TRUE;
	}
    }
    
    if (found)
    {
	if (A.End() != COMMITTED)
	    found = FALSE;
    }
    else
	A.Abort();
    
    return found;
}
    
OutCome ReplicaList::addTemplate (Buffer& toUse, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::addTemplate (Buffer& toUse)" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;

	/*
	 * Cannot create template if replicas already in group.
	 * Cannot add to template if there are users of group.
	 */

	if ((use_count == 0) || (self != 0))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		v_number++;

		// Cannot add template if one already exists.

		if (mask == 0)
		{
		    Boolean res;
		    GroupData *ptrEntry = new GroupData;
		    mask = new TemplateObject;
		    res = ptrEntry->unpack(toUse);
		    if (res)
		    {
			mask->tmplate = ptrEntry;
			result = Done;
		    }
		    else
		    {
			TemplateObject::deleteAll(mask);
			delete ptrEntry;
		    }
		}
		else
		    result = InUse;
	    }
	    else
	        result = Locked;
	}
	else
	    result = InUse;
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

Boolean ReplicaList::getTemplate (Buffer& toUse, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::getTemplate (Buffer& toUse)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;
        if (mask)
	    result = mask->tmplate->pack(toUse);
    }

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    return result;
}
	    
OutCome ReplicaList::replaceTemplate (Buffer& toUse, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::replaceTemplate (Buffer& toUse)" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;
	if (use_count == 0)
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
	        Boolean res;
		GroupData* dataToAdd = new GroupData;

		v_number++;
	        if (mask)
	            TemplateObject::deleteAll(mask);
		mask = new TemplateObject;
		res = dataToAdd->unpack(toUse);
		if (res)
		{
		    mask->tmplate = dataToAdd;
		    result = Done;
		}
		else
		{
		    TemplateObject::deleteAll(mask);
		    delete dataToAdd;
		}
	    }
	    else
	        result = Locked;
	}
	else
	    result = InUse;
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

int ReplicaList::numberOfTemplateReplicas () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "int ReplicaList::numberOfTemplateReplicas () const" << endl;
#endif

    GroupData* gdPtr = self;
    int numbTmp = 0;
    
    while (gdPtr)
    {
	if (::strcmp(gdPtr->getHost(), templateName) == 0)
	    numbTmp++;
	
	gdPtr = gdPtr->next;
    }

    return numbTmp;
}

OutCome ReplicaList::deleteTemplate (unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::deleteTemplate ()" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;

	// cannot delete template if template objects remain.

	if ((use_count == 0) && (numberOfTemplateReplicas() == 0))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
	        v_number++;
		if (mask)
		{
		    TemplateObject::deleteAll(mask);
		    result = Done;
		}
	    }
	    else
	        result = Locked;
	}
	else
	    result = InUse;
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
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
    Boolean opcode = FALSE;
    GroupData* indx;
    
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
    {
	if (A.End() != COMMITTED)
	    opcode = FALSE;
    }
    else
	A.Abort();
    
    return opcode;
}

Boolean ReplicaList::isMember (const GroupData& tempGD)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaList::isMember ( " << tempGD << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    GroupData* indx;
    
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
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
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
    {
	if (A.End() != COMMITTED)
	    opcode = FALSE;
    }
    else
	A.Abort();
    
    return opcode;
}

Boolean ReplicaList::getExclusiveAccess ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::getExclusiveAccess ()\n" << flush;
#endif

    AtomicAction A;
    Boolean opcode = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (use_count == 0)
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	        opcode = TRUE;
	}
    }

    if (opcode)
    {
	if (A.End() != COMMITTED)
	    opcode = FALSE;
    }
    else
	A.Abort();
    
    return opcode;
}

OutCome ReplicaList::alterLocation (const Uid& replica, const char* location,
				    unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::alterLocation ( " << replica << ", " << location << " )\n" << flush;
#endif

    AtomicAction A;
    GroupData* ptr;
    OutCome result = NotDone;

    if (!location)
	return NotDone;
    
    A.Begin();

/*
 * Check to see whether group is in use as we cannot
 * move replica otherwise.
 */

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        v_number++;
	version = v_number;

	if (use_count == 0)
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		ptr = self;

		while (ptr != 0)
		{
		    if (ptr->getUid() == replica)
		    {
			ptr->setHost((char*) location);
			result = Done;
		    }
		    else
			ptr = ptr->next;
		}
	    }
	    else
		result = Locked;
	}
	else
	    result = InUse;
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
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
    Boolean found = FALSE;
    OutCome result = NotDone;
    UserData* udPtr = 0;

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
	        UserData* tempPtr = 0;
		udPtr = clients;

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

		result = Done;
	    }
	    else
		result = Locked;
	}
	else
	    result = Done;
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

Boolean ReplicaList::isNew (const char* hostname, pid_t userPid) const
{
    UserData* indx = clients;
    for (int i = 0; i < use_count; i++)
    {
        if ((::strcmp(hostname, indx->hosts) == 0) && (indx->pids == userPid))
	    return FALSE;
	else
	    indx = indx->next;
    }
    
    return TRUE;
}

void ReplicaList::addElement (const ReplicaDescriptor& rd)
{
    GroupData *indx = self, *ptr = 0;

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

    if (self == 0)
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

ReplicaDescriptor* ReplicaList::getList (const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::getList ( " << hostname << ", " << userPid << " )\n" << flush;
#endif
    
    AtomicAction A;
    ReplicaDescriptor *resultData = 0, *tempData = 0;
    
    if (!hostname)
        return 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	
	if (isNew(hostname, userPid))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
	        // only increment use_count if this is a new user
		
	        UserData *tempUD = new UserData, *tPtr = clients;
		
		tempUD->hosts = ::new char[::strlen(hostname)+1];
		::memset(tempUD->hosts, '\0', ::strlen(hostname)+1);
		::memcpy(tempUD->hosts, hostname, ::strlen(hostname));
		tempUD->pids = userPid;
		tempUD->next = tPtr;
		clients = tempUD;
		use_count++;
	    }
	}
	
	// put template (if any) at front of list

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
	
	if (self)
	{
	    if (resultData == 0)
	    {
		resultData = new ReplicaDescriptor;
		*resultData = *self;
	    }
	    else
		*resultData += *self;
	}
	
	if (envelope)
	{
	    if (resultData == 0)
	    {
		resultData = new ReplicaDescriptor;
		*resultData = *self;
	    }
	    else
	    {
		tempData = envelope->getList(hostname, userPid);
		if (tempData)
		{
		    *resultData += *tempData;
		    delete tempData;
		}
	    }
	}
    }

    if (resultData != 0)
    {
	if (A.End() != COMMITTED)
	    resultData = 0;
    }
    else
	A.Abort();
    
    return resultData;
}

OutCome ReplicaList::addToList (const ReplicaDescriptor& rd, unsigned long& version, Boolean override)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::addToList (const ReplicaDescriptor& rd)\n" << flush;
#endif

    AtomicAction A;
    const ReplicaDescriptor* indx = &rd;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((use_count == 0) || (override))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		v_number++;
		version = v_number;
		
		if (*(ReplicaDescriptor*)indx != NIL_RD)
		{
		    while (indx)
		    {
			Boolean found = FALSE;

			// Only add if not already a member, and not overriding
			    
			while ((!found) && (indx))
			{
			    GroupData *tempGD = new GroupData;
			    *tempGD = *(ReplicaDescriptor*)indx;

			    if (isMember(*tempGD))
			    {
				found = FALSE;
				indx = indx->next;

				if (override)
				    result = Done;
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

			    delete tempGD;
			}
		    
			// found new member in list
			    
			if (found)
			{
			    addElement(*indx);
			    indx = indx->next;
			    result = Done;
			}
		    }
		}
		else
		    result = Locked;
	    }
	    else
		result = InUse;
	}
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::deleteFromList (const ReplicaDescriptor& rd, long& left,
				     unsigned long& version, Boolean override)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList::deleteFromList (const ReplicaDescriptor& rd)\n" << flush;
#endif

    AtomicAction A;
    const ReplicaDescriptor* ptr = &rd;
    Boolean found = FALSE;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((use_count == 0) || (override))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		v_number++;
		version = v_number;

		if (self != 0)
		{
		    while (ptr)
		    {
			GroupData *marker = self, *indx = self;

			while (marker)
			{
			    if (*marker == *ptr)
			    {
				found = TRUE;
				result = Done;

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

		    left = (((self == 0) && (mask == 0) && (envelope == 0)) ? 0 : 1);
		}
	    }
	    else
		result = Locked;
	}
	else
	    result = InUse;
    }
    else
	result = Locked;

    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::hasChanged (const Uid& recovered, Boolean& isPresent,
				 Boolean& changed)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::hasChanged ( " << recovered << " )\n" << flush;
#endif

    AtomicAction A;
    GroupData* marker = self;
    OutCome result = NotDone;

    changed = FALSE;
    isPresent = FALSE;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	result = Done;

	while ((marker != 0) && (!isPresent))
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
	result = Locked;

    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::changeTemplateStatus (const ReplicaDescriptor& rd, const Boolean& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::changeTemplateStatus (const ReplicaDescriptor& rd, const Boolean& state)" << endl;
#endif

    OutCome result = NotDone;
    const ReplicaDescriptor* ptr = &rd;
    GroupData* toPtr = 0;
    Boolean done;
    unsigned long dummy = 0;
    long left = 0;
    
    /*
     * If wildcard template and excluding, then we will add replicas to view.
     * If we are including then we will removed replicas from view.
     */

    if ((mask->tmplate->isWildCard()) || (mask->tmplate->isGlobalWildCard()))
    {
	if (state == FALSE)
	    return addToList(rd, dummy, TRUE);
	else
	    return deleteFromList(rd, left, dummy, TRUE);
    }

    // Currently exclude template and leave replicas alone - change later.

    while (ptr)
    {
	done = FALSE;
	toPtr = mask->tmplate;
	
	while ((toPtr) && (!done))
	{
	    if ((::strcmp(toPtr->getHost(), ptr->getHost()) == 0) && (ptr->getActive() == state))
	    {
		done = TRUE;
		toPtr->setActive(state);
	    }

	    toPtr = toPtr->next;
	}
	
	ptr = ptr->next;
    }
    
    toPtr = mask->tmplate;
    while ((toPtr) && (result != Done))
    {
	if (toPtr->getActive())
	    result = Done;
	else
	    toPtr = toPtr->next;
    }
    
    if (result != Done)
	error_stream << WARNING << "Template exclusion would result in fully excluded group. Will abort." << endl;
    
    return result;
}
    
OutCome ReplicaList::changeStatus (const ReplicaDescriptor& rd, const Boolean& state, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::changeStatus ( const ReplicaDescriptor& rd, "
		 << ((state == TRUE) ? "TRUE" : "FALSE") << " )" << endl;
#endif

    AtomicAction A;
    const ReplicaDescriptor* ptr = &rd;
    int n_active = 0;
    GroupData* marker = 0;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	version = v_number;

	/*
	 * Assume that is we have a template then everything after it is governed by it.
	 */

	if (mask)
	    result = changeTemplateStatus(rd, state);
/*
	// have excluded templates, now exclude replicas which may not be governed by it
	if ((result == Done) && (self))
*/
	else
	{
	    while (ptr)
	    {
		marker = self;
		
		while (marker)
		{
		    if (*marker == *ptr)
		    {
			if (ptr->getActive() == state)
			    marker->setActive(state);
			result = Done;
			break;
		    }
		    else
			marker = marker->next;
		}
	
		ptr = ptr->next;
	    }

	    if (!state)
	    {
		marker = self;
		while ((marker != 0) && (n_active == 0))
		{
		    if (marker->getActive())
			n_active++;
		    marker = marker->next;
		}

		if (n_active <= 0)
		{
		    result = NotDone;
		    error_stream << WARNING << "Exclude operation not done - would make entire group unavailable.\n" << flush;
		}
	    }
	}
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
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
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	result = save_state(os, ANDPERSISTENT);
	version = v_number;
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();

    return result;
}

Boolean ReplicaList::imposeState (ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::imposeState (ObjectState& os)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
	result = restore_state(os, ANDPERSISTENT);

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

OutCome ReplicaList::releaseGroup (const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome ReplicaList::releaseGroup ( " << hostname << ", " << userPid << " )\n" << flush;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    Boolean released = FALSE;
    UserData *udPtr = 0, *indx = 0;

    if (!hostname)
        return NotDone;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        udPtr = clients;
        for (int i = 0; ((udPtr) && (!released)); i++)
	{
	    if ((::strcmp(udPtr->hosts, hostname) == 0) && (udPtr->pids == userPid))
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
		released = TRUE;
	    }
	    else
	    {
	        indx = udPtr;
		udPtr = udPtr->next;
	    }
	}

	if (envelope)
	    result = envelope->releaseGroup(hostname, userPid);
	else
	    result = Done;

	v_number++;
	version = v_number;
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
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
    GroupData* marker = 0;
    ReplicaDescriptor *ptr = &toupdate, *indx = &unknownList;
    Boolean mayNeedUpdating = FALSE;
    OutCome result = NotDone;
    GroupData tempData;

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
 */

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	result = Done;

	// template currently governs all.

	if (mask)
	    marker = mask->tmplate;
	else
	    marker = self;

	while (marker != 0)
	{
	    tempData = *marker;

	    if (::strcmp(tempData.getHost(), hostname) == 0)
	    {
		mayNeedUpdating = FALSE;
		
		if (setlock(new Lock(WRITE), 0) == GRANTED)
		{
		    if (!tempData.getActive())
		    {
			if (toupdate == NIL_RD)
			{
			    toupdate = tempData;
			    ptr = &toupdate;
			}
			else
			{
			    ptr->next = new ReplicaDescriptor;
			    ptr = ptr->next;
			    *ptr = tempData;
			}
		    }
		    else
		    {
			if (use_count > 0)
			    mayNeedUpdating = TRUE;
		    }
		}
		else
		    mayNeedUpdating = TRUE;
		
		if (mayNeedUpdating)
		{
		    if (unknownList == NIL_RD)
		    {
			unknownList = tempData;
			indx = &unknownList;
		    }
		    else
		    {
			indx->next = new ReplicaDescriptor;
			indx = indx->next;
			*indx = tempData;
		    }

		    result = InUse;
		}
	    }

	    marker = marker->next;
	}
    }
    else
	result = Locked;

    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
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

    Boolean res;
    long number = 0;
    UserData* indx = clients;

    if (self)
	number = self->getNumber();

    res = os.pack(number) && os.pack(v_number);

    if (group == 0)
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

    Boolean res;
    GroupData *ptr = 0, *marker = 0;
    UserData *indx1 = clients, *indx2 = 0;
    long number = 0;
    int ptrType;

    if (group)
    {
	delete group;
	group = 0;
    }
    
    if (envelope)
        CompositeObject::deleteAll(envelope);

    if (mask)
        TemplateObject::deleteAll(mask);

    if (self)
    {
	ptr = self;
	while (ptr)
	{
	    marker = ptr;
	    ptr = ptr->next;
		
	    if (marker)
		delete marker;
	}
	self = 0;
    }
    
    while (indx1)
    {
	indx2 = indx1;
	indx1 = indx1->next;
	delete indx2;
    }
    clients = 0;

    res = os.unpack(number) && os.unpack(v_number) && os.unpack(ptrType);
    
    if ((res) && (ptrType == 0))
    {
	group = new Uid(NIL_UID);
	res = group->unpack(os);
    }
    
    res = res && os.unpack(use_count);

    for (int k = 0; k < use_count; k++)
    {
	indx1 = new UserData;
	if (clients == 0)
	    clients = indx1;
	else
	    indx2->next = indx1;

	indx2 = indx1;
	res = res && indx1->unpack(os);
    }
    
    res = os.unpack(ptrType);
    if (res)
    {
	if (ptrType == 0)
	{
	    self = new GroupData;
	    res = self->unpack(os);
	}
    }
	    
    res = os.unpack(ptrType);
    if (res)
    {
	if (ptrType == 0)
	{
	    envelope = new CompositeObject;
	    res = envelope->unpack(os);
	}
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
