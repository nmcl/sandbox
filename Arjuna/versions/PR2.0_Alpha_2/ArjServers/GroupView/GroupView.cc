/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupView.cc,v 1.1 1993/11/03 12:27:06 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef REPICALIST_H_
#  include "ReplicaList.h"
#endif

#ifndef TEMPLATEOBJECT_H_
#  include "TemplateObject.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ObjectGroups::ObjectGroups ()
                            : me(0),
			      groupUid(0),
			      replicaListUid(0),
			      next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ObjectGroups::ObjectGroups ()" << endl;
#endif
}

ObjectGroups::ObjectGroups (ReplicaList* rl)
                                           : next(0),
					     groupUid(0),
					     replicaListUid(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ObjectGroups::ObjectGroups (ReplicaList& rl)" << endl;
#endif

    me = rl;
}

ObjectGroups::~ObjectGroups ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ObjectGroups::~ObjectGroups ()" << endl;
#endif

    if (me)
        ReplicaList::remove(me);
    if (replicaListUid)
	delete replicaListUid;
    if (groupUid)
	delete groupUid;
}



ToPurge::ToPurge ()
                  : toRemove(0),
		    number(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ToPurge::ToPurge ()\n" << flush;
#endif
}

ToPurge::~ToPurge ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ToPurge::~ToPurge ()\n" << flush;
#endif

    if (number != 0)
	purge();
}

ReplicaList* ToPurge::retrieve (Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaList* ToPurge::retrieve ( " << u << " )" << endl;
#endif

    Boolean found = FALSE;
    ReplicaList *toReturn = 0;
    ObjectGroups *ptr = toRemove, *marker = 0;
    
    if (!toRemove)
	return 0;
    
    while ((ptr) && (!found))
    {
	if (*ptr->replicaListUid == u)
	    found = TRUE;
	else
	{
	    marker = ptr;
	    ptr = ptr->next;
	}
    }
    
    if (found)
    {
	toReturn = ptr->me;
	number--;
	
	if (ptr == toRemove)
	    toRemove = toRemove->next;
	else
	    marker->next = ptr->next;
    }
    
    return toReturn;
}
    
Boolean ToPurge::add (ReplicaList* toadd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ToPurge::add (ReplicaList* toadd)\n" << flush;
#endif
    
    Boolean outCome = TRUE;
    ObjectGroups* nElement = new ObjectGroups;
    
    nElement->me = toadd;
    nElement->groupUid = new Uid(*toadd->group);
    nElement->replicaListUid = new Uid(toadd->get_uid());
    nElement->next = toRemove;
    toRemove = nElement;
    number++;
    
    return outCome;
}

void ToPurge::purge ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ToPurge::purge ()\n" << flush;
#endif

    if (number != 0)
    {
	ObjectGroups *ptr = toRemove;
	
	for (int i = 0; i < number; i++)
	{
	    ptr = ptr->next;
	    delete toRemove;
	    toRemove = ptr;
	}

	toRemove = 0;
	number = 0;
    }
}



GroupView::GroupView (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::GroupView (int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    head = 0;
    numberofgroups = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

GroupView::GroupView (const Uid& u, int& res) : LockManager(u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::GroupView (Uid& u, int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    head = 0;
    numberofgroups = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

GroupView::~GroupView ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::~GroupView ()\n" << flush;
#endif

    terminate();

    deleteList.purge();

    if (head)
    {
	ObjectGroups* ptr = head;
	while (head)
	{
	    head = head->next;
	    delete ptr;
	    ptr = head;
	}
    }
}

/*
 * Used to add a new group to the list of groups. We do it this way so that
 * we can experiment with other structures for the linked list.
 */

Boolean GroupView::extendList (ObjectGroups* toAdd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean GroupView::extendList (ObjectGroups* toAdd)" << endl;
#endif

    toAdd->next = head;
    head = toAdd;
    numberofgroups++;
    return TRUE;
}

/*
 * Searches for a replica group through the linked list. Again, we use this
 * approach so that the format of the list can be altered in a relatively
 * transparent way to the rest of the system.
 */

ObjectGroups* GroupView::searchAllLists (const Uid& group, ObjectGroups* searchFrom, Boolean useWildCard) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::searchAllLists (const Uid& group, ObjectGroups* searchFrom) const" << endl;
#endif

    ObjectGroups* ptr = searchList(group, searchFrom);
    
    if (!ptr)
	ptr = searchCompositeObject(group, searchFrom);
    
    if ((!ptr) && (useWildCard))
	ptr = searchWildCard(searchFrom);
    
    return ptr;
}

ObjectGroups* GroupView::searchList (const Uid& group, ObjectGroups* searchFrom) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList* GroupView::searchList (const Uid& group, ObjectGroups* searchFrom) const" << endl;
#endif

    ObjectGroups* ptr = searchFrom;
    
    if ((!head) || (!searchFrom))
	return 0;
    
    for (int i = 0; i < numberofgroups; i++)
    {
	if (*ptr->groupUid == group)
	{
	    if (ptr->me == 0)
		ptr->me = createReplicaList(*ptr->replicaListUid);

	    if (ptr->me)
	        return ptr;
	    else
	        return 0;
	}
	else
	    ptr = ptr->next;
    }
    
    return 0;
}

ObjectGroups* GroupView::searchCompositeObject (const Uid& group, ObjectGroups* searchFrom) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::searchCompositeObject (const Uid&, ObjectGroups* searchFrom) const" << endl;
#endif

    ObjectGroups* ptr = searchFrom;

    if ((!head) || (!searchFrom))
        return 0;

    for (int i = 0; i < numberofgroups; i++)
    {
        if (ptr->me == 0)
	    ptr->me = createReplicaList(*ptr->replicaListUid);

	if (ptr->me->searchComposite(group))
	    return ptr;
	else
	    ptr = ptr->next;
    }

    return 0;
}

ObjectGroups* GroupView::searchWildCard (ObjectGroups* searchFrom) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::searchWildCard (ObjectGroups* searchFrom) const" << endl;
#endif

    ObjectGroups* ptr = searchFrom;
    
    if ((!head) || (!searchFrom))
	return 0;
    
    for (int i = 0; i < numberofgroups; i++)
    {
	if (ptr->me == 0)
	    ptr->me = createReplicaList(*ptr->replicaListUid);
	
	if (ptr->me->searchWildCard())
	    return ptr;
	else
	    ptr = ptr->next;
    }
    
    return 0;
}

/*
 * Remove a replica group from the linked list. Separate method makes altering
 * structure of list transparent.
 */

Boolean GroupView::removeFromList (ObjectGroups* toGo, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean GroupView::removeFromList (ObjectGroups* toGo, int& res)" << endl;
#endif

    Boolean found = FALSE, result;
    ObjectGroups *marker = head, *trail = head;

    if (!head)
	return FALSE;

    for (int i = 0; (i < numberofgroups) && (!found); i++)
    {
	if (marker == toGo)
	    found = TRUE;
	else
	{
	    trail = marker;
	    marker = marker->next;
	}
    }

    if (marker == head)
	head = head->next;
    else
	trail->next = marker->next;

    numberofgroups--;
    res = numberofgroups;

    result = deleteList.add(toGo->me);
    toGo->me = 0;
    delete toGo;
    return result;
}

/*
 * Create new replica list dynamically.
 */

ReplicaList* GroupView::createReplicaList (const Uid& u) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList* GroupView::createReplicaList ( " << u << " ) const" << endl;
#endif
    
    int err = -1;
    ReplicaList* RLptr = ReplicaList::create(u, err);
    return RLptr;
}

/*
 * This operation returns the group Uid of the group to which the give replica
 * Uid belongs.
 */

Boolean GroupView::groupAssociation (const Uid& replicaUid, Uid& group,
				     unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::groupAssociation ( " << replicaUid << ", Uid& group )" << endl;
#endif
    
    AtomicAction A;
    ObjectGroups* ptr = 0;
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = head;
	    for (int i = 0; (i < numberofgroups) && (!result); i++)
	    {
		if (ptr->me != 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		result = ptr->me->isMember(replicaUid, v_number);

		if (result)
		    group = *ptr->groupUid;
	    }
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

/*
 * Add a replica to the database. If no replica group exists with Uid then this
 * is treated as the first member of a new replica group and so this group is
 * added to the database list. Otherwise the replica is added to the list of
 * existing members for the specified replica group.
 */

OutCome GroupView::addReplicas (const ReplicaDescriptor& objects, const Uid& group, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::addReplicas ( const ReplicaDescriptor& objects, " << group << " )\n" << flush;
#endif

    AtomicAction A;
    ObjectGroups *ptr = 0;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchAllLists(group, head, FALSE);

	if (ptr)
	{
	    // group is already present in database

	    result = ptr->me->addToList(objects, v_number);
	    if (result == Done)
	    {
		if (A.End() != COMMITTED)
		    result = NotDone;
	    }
	    else
		A.Abort();
	    
	    return result;
	}

	// This is a new group and so should be added to list of groups

	if (::strcmp(objects.getHost(), templateName) == 0)
	    error_stream << WARNING << "Cannot add template object without template." << endl;
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;

		// create new replica group entry

		ptr = new ObjectGroups(ReplicaList::create(res));
		if (res == 0)
		{
		    result = ptr->me->addToList(objects, v_number);
		    if (result == Done)
		    {
			ptr->me->group = new Uid(group);
			ptr->groupUid = new Uid(group);
			ptr->replicaListUid = new Uid(ptr->me->get_uid());
			(void) extendList(ptr);
		    }
		    else
			deleteList.add(ptr->me);
		}
		else
		    result = NotDone;
	    }
	    else
		result = Locked;
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

/*
 * Delete replicas from replica group, or delete entire group. If a group
 * becomes empty then it is deleted automatically.
 */

OutCome GroupView::deleteReplicas (const ReplicaDescriptor& objects, const Uid& group,
				   const Boolean& deleteAll, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::deleteReplicas (const ReplicaDescriptor& objects, " << group << ", "
		 << ((deleteAll == TRUE) ? "TRUE" : "FALSE") << " )\n" << flush;
#endif

    AtomicAction A;
    OutCome result = NotDone, found = NotDone;
    long left = -1;
    ObjectGroups *ptr = 0;

    A.Begin();

    res = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	ptr = searchAllLists(group, head);

	if (ptr)
	{
	    if (!deleteAll)
	    {
		found = ptr->me->deleteFromList(objects, left, v_number);
		if ((found == Done) && (left == 0))
		    result = ((removeFromList(ptr, res) == TRUE) ? Done : NotDone);
		else
		    result = Done;
	    }
	    else
		result = ((removeFromList(ptr, res) == TRUE) ? Done : NotDone);
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

/*
 * Return use count
 */

Boolean GroupView::getUseCount (const Uid& group, long& UseCount, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getUseCount ( " << group << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    ObjectGroups* ptr = 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = searchAllLists(group, head);
	    if (ptr)
		result = ptr->me->getUseCount(UseCount, v_number);
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


/*
 * The following methods are used for creating/deleting composite (envelope)
 * objects.
 */

OutCome GroupView::addTemplate (const Uid& groupAdd, Buffer& toAdd, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::addTemplate ( " << groupAdd << ", Buffer& toAdd )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    ObjectGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchList(groupAdd, head);
	if (ptr)
	    result = ptr->me->addTemplate(toAdd, v_number);
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;
		
		ptr = new ObjectGroups(ReplicaList::create(res));
		if (res == 0)
		{
		    result = ptr->me->addTemplate(toAdd, v_number);
		    if (result == Done)
		    {
			ptr->me->group = new Uid(groupAdd);
			ptr->groupUid = new Uid(groupAdd);
			ptr->replicaListUid = new Uid(ptr->me->get_uid());
			(void) extendList(ptr);
		    }
		}
	    }
	    else
		result = Locked;
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

Boolean GroupView::getTemplate (const Uid& groupUse, Buffer& toUse, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getTemplate ( " << groupUse << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    ObjectGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupUse, head);
	if (ptr)
	    result = ptr->me->getTemplate(toUse, v_number);
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

OutCome GroupView::deleteTemplate (const Uid& groupAdd, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::deleteTemplate ( " << groupAdd << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    ObjectGroups* ptr = 0;
    int res = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        ptr = searchList(groupAdd, head);
	if (ptr)
	    result = ptr->me->deleteTemplate(v_number);
	if (result == Done)
	    result = ((removeFromList(ptr, res) == TRUE) ? Done : NotDone);
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
      
OutCome GroupView::replaceTemplate (const Uid& groupAdd, Buffer& toUse, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::replaceTemplate ( " << groupAdd << ", Buffer& toUse )" << endl;
#endif

    AtomicAction A;
    ObjectGroups* ptr = 0;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchList(groupAdd, head);
	if (ptr)
	    result = ptr->me->replaceTemplate(toUse, v_number);
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

OutCome GroupView::assignCompositeObject (const Uid& groupAssignTo, const Uid& groupToAssign, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::assignCompositeObject ( " << groupAssignTo << ", " << groupToAssign << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    ObjectGroups *ptr = 0, *tempPtr = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupAssignTo, head);
	if (ptr)
	{
	    Boolean found = FALSE;
	    tempPtr = head;
	    while ((tempPtr) && (!found))
	    {
	        if (*tempPtr->groupUid == groupToAssign)
		    found = TRUE;
		else
		    tempPtr = tempPtr->next;
	    }

	    // found both group to assign to and group to assign to it.

	    if (found)
		result = ptr->me->assignCompositeObject(*tempPtr->replicaListUid, v_number);
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

OutCome GroupView::assignCompositeObject (const Uid& groupAssignTo, const ReplicaDescriptor& objects, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::assignCompositeObject ( " << groupAssignTo << " , const ReplicaDescriptor& objects )" << endl;
#endif

    AtomicAction A;
    ObjectGroups* ptr = 0;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupAssignTo, head);
	if (ptr)
	    result = ptr->me->assignCompositeObject(objects, v_number);
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

OutCome GroupView::removeCompositeObject (const Uid& groupAssignedTo, const Uid& groupAssigned, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::removeCompositeObject ( " << groupAssignedTo << ", " << groupAssigned << " )" << endl;
#endif

    AtomicAction A;
    ObjectGroups *ptr = 0, *tempPtr = 0;
    OutCome result = NotDone;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchList(groupAssignedTo, head);
	if (ptr)
	{
	    Boolean found = FALSE;
	    tempPtr = head;
	    while ((tempPtr) && (!found))
	    {
		if (*tempPtr->groupUid == groupAssigned)
		    found = TRUE;
		else
		    tempPtr = tempPtr->next;
	    }
	    
	    // found both group assigned to and group assigned to it.

	    if (found)
		result = ptr->me->removeCompositeObject(*tempPtr->replicaListUid, v_number);
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

OutCome GroupView::removeCompositeObject (const Uid& groupAssignedTo, const ReplicaDescriptor& objects, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::removeCompositeObject ( " << groupAssignedTo << ", const ReplicaDescriptor& objects )" << endl;
#endif

    AtomicAction A;
    ObjectGroups* ptr = 0;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupAssignedTo, head);
	if (ptr)
	    result = ptr->me->removeCompositeObject(objects, v_number);
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
 * Decrement the use_count for the given replica group. The use_count is
 * incremented everytime a client requests the groupview. The hostname
 * parameter indicates which host is releasing (decrementing use_count) the
 * group.
 */

OutCome GroupView::releaseGroup (const Uid& group, const char* hostname, pid_t userPid, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::releaseGroup ( " << group << ", " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction A;
    ObjectGroups* ptr = 0;
    OutCome result = NotDone;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = searchAllLists(group, head);
	    if (ptr)
		result = ptr->me->releaseGroup(hostname, userPid, v_number);
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

/*
 * This operation checks that the use_count is zero. If it is then a WRITE lock
 * is obtained on the group and the use_count is set to 1. Used for recovery
 * and migration.
 */

Boolean GroupView::testAndSet (ReplicaDescriptor& objects, const Uid& group, const char* hostname, pid_t userPid, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::testAndSet ( " << group << ", " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction A;
    Boolean opcode = FALSE;
    ObjectGroups* ptr = 0;
    long UseCount = -1;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchAllLists(group, head);
	if (ptr)
	{
	    if (ptr->me->getUseCount(UseCount, v_number))
	    {
	        if (UseCount == 0)
		    opcode = getView(objects, group, hostname, userPid, v_number, WRITE);
	    }
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

Boolean GroupView::getExclusiveAccess ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getExclusiveAccess ()" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
	result = TRUE;
    
    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

/*
 * This operation simply alters the location (host name) of a given replica in
 * a given replica group. Typically used for migration.
 */

OutCome GroupView::alterLocation (const Uid& group, const Uid& replica, const char* location, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::alterLocation ( " << group << ", " << replica << ", " << location << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    ObjectGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchAllLists(group, head);
	if (ptr)
	    result = ptr->me->alterLocation(replica, location, v_number);
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
 * Scan the entire database returning information on all replica groups
 * mentioned. This is used by the Database Editor program, and not by any
 * Arjuna classes.
 */

Boolean GroupView::traverseDatabase (ReplicaDescriptor& objects, Uid& group, int& position,
				     const char* hostName, pid_t userPid, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::traverseDatabase ( ReplicaDescriptor& objects, " << group
		 << ", " << position << ", " << hostName << ", " << userPid << " )\n" << flush;
#endif

    Boolean result = FALSE;
    AtomicAction A;
    ReplicaDescriptor* x = 0;
    ObjectGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((numberofgroups == 0) || (position < 0) || (position >= numberofgroups))
	{
	    position = -1;
	    result = TRUE;
	}
	else
	{
	    ptr = head;
	    for (int i = 0; i < position; i++, ptr = ptr->next);
	
	    if (ptr->me == 0)
		ptr->me = createReplicaList(*ptr->replicaListUid);

	    group = *ptr->groupUid;
	    x = ptr->me->getList(hostName, userPid, v_number);
	    if (x)
		objects = *x;
	    else
	    {
		if (ptr->me->mask)
		{
		    x = ptr->me->mask->getTemplate();
		    objects = *x;
		}
	    }

	    result = TRUE;
	    if (x)
		delete x;
	    position++;
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

/*
 * This returns all of the necessary information about a replica group (if it
 * exists). It also increments the use_count to reflect the number of users of
 * the group. If mode = WRITE then exclusive access is obtained to the group.
 * This is required for recovering nodes, which do not want replicas to be
 * changing state while they are using them to update the states of out-of-date
 * replicas. The hostname parameter is used so that replica groups can keep
 * track of which nodes have gained access to them. The recover(...) operation
 * then scans the replica groups and decrements the use_ counts for the replica
 * groups with the given hostname.
 */

Boolean GroupView::getView (ReplicaDescriptor& objects, const Uid& group,
			    const char* hostname, pid_t userPid,
			    unsigned long& v_number, const LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getView (ReplicaDescriptor&, " << group << ", " << hostname << ", " << userPid
		 << ", " << ((mode == READ) ? "READ" : "WRITE") << " )" << endl;
#endif

    AtomicAction A;
    ReplicaDescriptor* x = 0;
    ObjectGroups* ptr = 0;
    Boolean result = FALSE;
    
    A.Begin();

    if (setlock(new Lock(mode), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = searchAllLists(group, head);
	    if (ptr)
	    {
		x = ptr->me->getList(hostname, userPid, v_number);
		if (x)
		{
		    objects = *x;
		    result = TRUE;
		    delete x;
		}
	    }
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

/*
 * When a crashed node recovers it is necessary for it to remove any
 * out-of-date information from the database (such as the corresponding
 * use_count).
 */

OutCome GroupView::recover (const char* hostname, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::recover ( " << hostname << " )" << endl;
#endif
    
    AtomicAction A;
    AtomicAction* B = 0;
    OutCome result = NotDone, opcode = NotDone;
    Boolean success = FALSE;
    ObjectGroups* ptr = 0;

    if (!hostname)
	return result;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = head;
	    for (int i = 0; (i < numberofgroups) && (ptr); i++)
	    {
		if (B)
		    delete B;
	
		B = new AtomicAction;
	
		B->Begin();

		if (ptr->me == 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		result = ptr->me->recover(hostname, v_number);
		if (result != Done)
		{
		    B->Abort();

		    if ((result == InUse) || (result == Locked))
			opcode = InUse;
		}
		else
		{
		    if (B->End() != COMMITTED)
		    {
			result = NotDone;
			break;
		    }
		    else
			success = TRUE;
		}

		ptr = ptr->next;
	    }
	}
    }
    else
	result = Locked;

    if (success)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
	else
	{
	    if (opcode == InUse)
		result = InUse;
	    else
		result = Done;
	}
    }
    else
	A.Abort();

    return result;
}

/*
 * include a list of replicas into an already existing group
 */

OutCome GroupView::include (const ReplicaDescriptor& objects, const Uid& group, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::include (const ReplicaDescriptor& objects, " << group << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    ObjectGroups* ptr = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
      	if (numberofgroups > 0)
	{
	    ptr = searchAllLists(group, head);
	    if (ptr)
		result = ptr->me->changeStatus(objects, TRUE, v_number);
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

/*
 * exclude a list of replicas from a group
 */

OutCome GroupView::exclude (const ReplicaDescriptor& objects, const Uid& group, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::exclude (const ReplicaDescriptor& objects, " << group << " )" << endl;
#endif
   
    AtomicAction A;
    OutCome result = NotDone;
    ObjectGroups* ptr = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (numberofgroups > 0)
	{
	    ptr = searchAllLists(group, head);
	    if (ptr)
		result = ptr->me->changeStatus(objects, FALSE, v_number);
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


/*
 * Given a recovered replica state UID this operation will indicate whether
 * that state is now out-of-date.
 */

OutCome GroupView::needsUpdating (const Uid& recovered, Boolean& isModified, ReplicaDescriptor& info,
				  const char* nodename, pid_t userPid, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::needsUpdating ( " << recovered << ", ReplicaDescriptor&, "
		 << nodename << ", " << userPid << " )" << endl;
#endif

    OutCome result = NotDone;
    AtomicAction A;
    ObjectGroups* ptr = 0;
    Boolean isPresent, isLocked = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = head;
	    for (int i = 0; (i < numberofgroups) && (ptr); i++)
	    {
		if (ptr->me == 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		isModified = isPresent = FALSE;
		result = ptr->me->hasChanged(recovered, isPresent, isModified);
		
		if (result == Done)
		{
		    if (isPresent)
		    {
			if (A.End() != COMMITTED)
			    result = NotDone;
			else
			{
			    if (isModified)
				return ((getView(info, *ptr->groupUid, nodename, userPid, v_number) == TRUE) ? Done : NotDone);
			}
			
			return result;
		    }
		}

		if (result == Locked)
		    isLocked = TRUE;
		
		ptr = ptr->next;
	    }
	}
    }
    else
	result = Locked;
    
    if (isLocked)
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

Boolean GroupView::getState (ObjectState& os, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getState (ObjectState& os)\n" << flush;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    ObjectGroups* ptr = 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	result = os.pack(numberofgroups);
	if (numberofgroups != 0)
	{
	    ptr = head;
	    for (int i = 0; ((i < numberofgroups) && (result) && (ptr)); i++)
	    {
		if (ptr->me == 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		result = ptr->me->getState(os, v_number);
		ptr = ptr->next;
	    }
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

Boolean GroupView::imposeState (ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::imposeState (ObjectState& os)\n" << flush;
#endif

    AtomicAction A;
    Boolean result = FALSE, res = FALSE;
    ObjectGroups *ptr = 0, *indx;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	ptr = head;
	for (int i = 0; ((i < numberofgroups) && (ptr) && (!res)); i++)
	{
	    if (deleteList.add(ptr->me))
	    {
	        indx = ptr;
		ptr = ptr->next;
		indx->next = 0;
		delete indx;
	    }
	    else
		res = TRUE;
	}
	
	if (!res)
	{
	    head = 0;
	    res = os.unpack(numberofgroups);
	    
	    if (res)
	    {
		if (numberofgroups > 0)
		{
		    for (i = 0; ((i < numberofgroups) && (!res)); i++)
		    {
			int res = -1;
			
			if (!head)
			{
			    head = new ObjectGroups;
			    ptr = head;
			}
			else
			{
			    ptr->next = new ObjectGroups;
			    ptr = ptr->next;
			}
			
			ptr->me = ReplicaList::create(res);
			if (res != 0)
			{
			    A.Abort();
			    return FALSE;
			}
			else
			{
			    res = ptr->me->imposeState(os);
			    ptr->groupUid = new Uid(*ptr->me->group);
			    ptr->replicaListUid = new Uid(ptr->me->get_uid());
			}
		    }
		}
	    }
	    
	    if (res)
		A.Abort();
	    else
		result = (Boolean) (A.End() == COMMITTED);
	}
    }
    else
	A.Abort();
    
    return result;
}

/*
 * This operation will return a list of all of those replicas belonging to a
 * given node which need updating. Since some groups may be locked (or have
 * non-zero use_counts) and these may then be updated after this operation
 * completes, those replicas will also be added to the returned list.
 */

OutCome GroupView::getNodeUpdateList (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				      const char* hostname, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::getNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, "
		 << hostname << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone, opcode = NotDone;
    ObjectGroups* ptr = 0;
    ReplicaDescriptor *ptrRD = &toupdate, *indxRD = &unknownList;

    while (ptrRD->next != 0)
	ptrRD = ptrRD->next;

    while (indxRD->next != 0)
	indxRD = indxRD->next;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups > 0)
	{
	    ptr = head;
	    for (int i = 0; (i < numberofgroups) && (ptr); i++)
	    {
		if (ptr->me == 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		result = ptr->me->getHostInfo(*ptrRD, *indxRD, hostname, v_number);

		if ((result == InUse) || (result == Locked))
		    opcode = InUse;

		ptr = ptr->next;
	    }
	}
    }
    else
	result = Locked;
    
    if ((result == Done) || (opcode == InUse))
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
	else
	{
	    if (opcode == InUse)
		result = InUse;
	}
    }
    else
	A.Abort();
    
    return result;
}

Boolean GroupView::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::save_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;
    ObjectGroups* ptr = 0;

    res = os.pack(numberofgroups);

    ptr = head;
    for (int i = 0; (i < numberofgroups) && res; i++)
    {
	res = ((ptr->replicaListUid)->pack(os)) && ((ptr->groupUid)->pack(os));
	ptr = ptr->next;
    }

    return res;
}

Boolean GroupView::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;
    int currentNumber;
    ObjectGroups *ptr = 0, *indx = 0;
    
    currentNumber = numberofgroups;
    
    res = os.unpack(numberofgroups);
    
    if (res)
    {
	ptr = head;
	    
	for (int i = 0; i < currentNumber; i++)
	{
	    indx = ptr;
	    ptr = ptr->next;
	    if (indx->me)
		deleteList.add(indx->me);
	    indx->me = 0;
	    delete indx;
	}
	    
	head = ptr = 0;

	if ((numberofgroups == 0) && (currentNumber > 0))
	    deleteList.purge();
	else
	{
	    for (i = 0; (i < numberofgroups) && res; i++)
	    {
		indx = new ObjectGroups;
		indx->replicaListUid = new Uid(NIL_UID);
		indx->groupUid = new Uid(NIL_UID);
		res = indx->replicaListUid->unpack(os) && indx->groupUid->unpack(os);

		if (res)
		    indx->me = deleteList.retrieve(*indx->replicaListUid);

		if (head == 0)
		{
		    head = indx;
		    ptr = head;
		}
		else
		{
		    ptr->next = indx;
		    ptr = ptr->next;
		}
	    }
	}
    }

    deleteList.purge();
    
    return res;
}

const TypeName GroupView::type () const
{
    return "/StateManager/LockManager/GroupView";
}
