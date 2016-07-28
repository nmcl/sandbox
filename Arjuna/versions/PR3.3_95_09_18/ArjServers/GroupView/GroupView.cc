/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupView.cc,v 1.21 1995/04/28 10:12:13 nmcl Exp $
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

#ifndef TEMPLATEOBJECT_H_
#  include <ArjServers/TemplateObject.h>
#endif

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef REPICALIST_H_
#  include "ReplicaList.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ObjectGroups* GroupView::head           = (ObjectGroups*) 0;
int           GroupView::numberofgroups = 0;
unsigned long GroupView::v_number       = 0;
ToPurge       GroupView::deleteList;



ObjectGroups::ObjectGroups ()
                            : me(0),
			      replicaListUid(0),
			      groupUid(0),
			      next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ObjectGroups::ObjectGroups ()" << endl;
#endif
}

ObjectGroups::ObjectGroups (ReplicaList* rl)
			   : me(rl),
			     replicaListUid(0),
			     groupUid(0),
			     next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ObjectGroups::ObjectGroups (ReplicaList& rl)" << endl;
#endif

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

    Boolean       found    = FALSE;
    ReplicaList*  toReturn = (ReplicaList*) 0;
    ObjectGroups* ptr      = toRemove;
    ObjectGroups* marker   = (ObjectGroups*) 0;
    
    if (!toRemove)
	return (ReplicaList*) 0;
    
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

	ptr->me = (ReplicaList*) 0;
	delete ptr;
    }
    
    return toReturn;
}
    
Boolean ToPurge::add (ReplicaList* toadd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ToPurge::add (ReplicaList* toadd)\n" << flush;
#endif
    
    Boolean       outCome  = TRUE;
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

    ObjectGroups *ptr = toRemove, *trail = (ObjectGroups*) 0;
    
    while (ptr)
    {
	trail = ptr->next;
	delete ptr;
	ptr = trail;
    }
    
    number = 0;
    toRemove = (ObjectGroups*) 0;
}



GroupView::GroupView (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::GroupView (int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

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

    res = -1;
    
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
    debug_stream << "GroupView::~GroupView ()" << endl;
#endif

    terminate();

    GroupView::deleteList.purge();

    /*
     * numberofgroups is the total number of group entries, but this need not
     * be the number of entries on the linked list. Therefore we cannot simply
     * use a for-loop to delete the list.
     */

    if (GroupView::head)
    {
	ObjectGroups* ptr = GroupView::head;
	while (ptr)
	{
	    GroupView::head = GroupView::head->next;
	    delete ptr;
	    ptr = GroupView::head;
	}
    }
    GroupView::numberofgroups = 0;
}

/*
 * Used to add a new group to the list of groups. We do it this way so that
 * we can experiment with other structures for the linked list.
 * This method will only ever be called with a valid ReplicaList method.
 *
 * At present the incrementing of the number of elements in the list is
 * performed outside of this routine because it is not always required
 * to be done. [To Change Later.]
 */

ObjectGroups* GroupView::extendList (ObjectGroups* toAdd) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::extendList () const" << endl;
#endif

    ObjectGroups* ptr = GroupView::head;

    while (ptr)
    {
	/*
	 * An entry for this replica may already exist in the list, but
	 * in an incomplete form, i.e., no ReplicaList object may exist.
	 * If this is so then we replace it with the one given, which must
	 * have an instance of this. extendList will never be called twice
	 * for the same replica, so we can be sure of no leaks.
	 */
	
	if (*toAdd->replicaListUid == *ptr->replicaListUid)
	{
	    ptr->me = toAdd->me;
	    toAdd->me = NULL;
	    toAdd->next = NULL;

	    delete toAdd;

	    return ptr;
	}
	else
	    ptr = ptr->next;
    }

    // not present, so add to list
    
    toAdd->next = GroupView::head;
    GroupView::head = toAdd;
    
    return toAdd;
}

/*
 * Searches for a replica group through the linked list. Again, we use this
 * approach so that the format of the list can be altered in a relatively
 * transparent way to the rest of the system.
 */

ObjectGroups* GroupView::searchAllLists (const Uid& group, ObjectGroups* searchFrom, Boolean useWildCard)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::searchAllLists ( " << group << " )" << endl;
#endif

    ObjectGroups* ptr = searchList(group, searchFrom);
    
    if (!ptr)
	ptr = searchCompositeObject(group, searchFrom);

    if ((!ptr) && (useWildCard))
	ptr = searchWildCard(searchFrom);
    
    return ptr;
}

ObjectGroups* GroupView::searchList (const Uid& group, ObjectGroups* searchFrom)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList* GroupView::searchList ()" << endl;
#endif

    ObjectGroups* ptr = searchFrom;
    
    if ((!GroupView::head) || (!searchFrom))
	return (ObjectGroups*) 0;

    while (ptr)
    {
        // scan list and create new object if necessary

	if (*ptr->groupUid == group)
	{
	    if (ptr->me == (ReplicaList*) 0)
		ptr->me = createReplicaList(*ptr->replicaListUid);
	    return ((ptr->me) ? ptr : 0);
	}
	else
	    ptr = ptr->next;
    }
    
    return (ObjectGroups*) 0;
}

ObjectGroups* GroupView::searchCompositeObject (const Uid& group, ObjectGroups* searchFrom)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::searchCompositeObject (const Uid&, ObjectGroups* searchFrom)" << endl;
#endif

    ObjectGroups* ptr = searchFrom;

    if ((!GroupView::head) || (!searchFrom))
        return (ObjectGroups*) 0;

    while (ptr)
    {
        if (ptr->me == (ReplicaList*) 0)
	    ptr->me = createReplicaList(*ptr->replicaListUid);

	if (ptr->me->searchComposite(group))
	    return ptr;
	else
	    ptr = ptr->next;
    }

    return (ObjectGroups*) 0;
}

ObjectGroups* GroupView::searchWildCard (ObjectGroups* searchFrom)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ObjectGroups* GroupView::searchWildCard (ObjectGroups* searchFrom)" << endl;
#endif

    ObjectGroups* ptr = searchFrom;
    
    if ((!GroupView::head) || (!searchFrom))
	return (ObjectGroups*) 0;

    while (ptr)
    {
	if (ptr->me == (ReplicaList*) 0)
	    ptr->me = createReplicaList(*ptr->replicaListUid);
	
	if (ptr->me->searchWildCard())
	    return ptr;
	else
	    ptr = ptr->next;
    }
    
    return (ObjectGroups*) 0;
}

/*
 * Remove a replica group from the linked list. Separate method makes altering
 * structure of list transparent.
 */

Boolean GroupView::removeFromList (ObjectGroups* toGo, int& res, const char* node, pid_t pID) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean GroupView::removeFromList (ObjectGroups* toGo, int& res) const" << endl;
#endif

    Boolean       result;
    Boolean       found  = FALSE;
    ObjectGroups* marker = GroupView::head;
    ObjectGroups* trail  = GroupView::head;

    if ((!toGo) || (!GroupView::head))
        return FALSE;

    /* Can we remove this group? */

    if ((toGo->me) && (!toGo->me->canLock(node, pID)))
        return FALSE;

    while ((!found) && (marker))
    {
	if (marker == toGo)
	    found = TRUE;
	else
	{
	    trail = marker;
	    marker = marker->next;
	}
    }

    if (marker == GroupView::head)
	GroupView::head = GroupView::head->next;
    else
	trail->next = marker->next;

    GroupView::numberofgroups--;
    res = GroupView::numberofgroups;
    result = GroupView::deleteList.add(toGo->me);
    toGo->me = (ReplicaList*) 0;
    delete toGo;

    return result;
}

/*
 * Create new replica list dynamically.
 */

ReplicaList* GroupView::createReplicaList (const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaList* GroupView::createReplicaList ( " << u << " )" << endl;
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
				     unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::groupAssociation ( " << replicaUid << ", Uid& group )" << endl;
#endif
    
    AtomicAction  A;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    Boolean       result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = GroupView::head;
	    while ((!result) && (ptr))
	    {
		if (ptr->me != (ReplicaList*) 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		result = ptr->me->isMember(replicaUid, version);
		if (result)
		    group = *ptr->groupUid;
	    }
	}
    }
    
    if (result)
        result = (Boolean) (A.End() == COMMITTED);
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

OutCome GroupView::addReplicas (const ReplicaDescriptor& objects, const Uid& group, const char* hostName, pid_t userPid,
				unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupView::addReplicas ( const ReplicaDescriptor& objects, " << group << " )\n" << flush;
#endif

    AtomicAction  A;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    OutCome       result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchAllLists(group, GroupView::head, FALSE);
	if (ptr)
	{
	    /* group is already present in database */

	    result = ptr->me->addToList(objects, hostName, userPid, version);
	    if (result == DONE)
	        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
	    else
		A.Abort();

	    return result;
	}

	/* This is a new group and so should be added to list of groups */

	if (objects.isNullHost())
	    error_stream << WARNING << "Cannot add template object without template." << endl;
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;

		v_number++;
		
		/* create new replica group entry */

		ptr = new ObjectGroups(ReplicaList::create(res));
		if (res == 0)
		{
		    result = ptr->me->addToList(objects, hostName, userPid, version);
		    if (result == DONE)
		    {
			ptr->me->group = new Uid(group);
			ptr->groupUid = new Uid(group);
			ptr->replicaListUid = new Uid(ptr->me->get_uid());
			ptr = extendList(ptr);
			GroupView::numberofgroups++;
		    }
		    else
			GroupView::deleteList.add(ptr->me);
		}
	    }
	    else
		result = LOCKED;
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
 * Delete replicas from replica group, or delete entire group. If a group
 * becomes empty then it is deleted automatically.
 */

OutCome GroupView::deleteReplicas (const ReplicaDescriptor& objects, const Uid& group, const Boolean& deleteAll, int& res,
				   const char* hostName, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::deleteReplicas (const ReplicaDescriptor& objects, " << group << ", "
		 << ((deleteAll == TRUE) ? "TRUE" : "FALSE") << " )\n" << flush;
#endif

    AtomicAction  A;
    OutCome       result = NOTDONE;
    OutCome       found  = NOTDONE;
    long          left   = -1;
    ObjectGroups* ptr    = (ObjectGroups*) 0;

    A.Begin();

    res = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	
	ptr = searchAllLists(group, GroupView::head);

	if (ptr)
	{
	    if (!deleteAll)
	    {
		found = ptr->me->deleteFromList(objects, left, hostName, userPid, version);

		/* If the group is now empty then remove it. */

		if ((found == DONE) && (left == 0))
		    result = ((removeFromList(ptr, res, hostName, userPid) == TRUE) ? DONE : NOTDONE);
		else
		    result = DONE;
	    }
	    else
		result = ((removeFromList(ptr, res, hostName, userPid) == TRUE) ? DONE : NOTDONE);
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
 * Return use count
 */

Boolean GroupView::getUseCount (const Uid& group, long& UseCount, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getUseCount ( " << group << " )" << endl;
#endif

    AtomicAction  A;
    Boolean       result = FALSE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(group, GroupView::head);
	    if (ptr)
		result = ptr->me->getUseCount(UseCount, version);
	}
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}

/*
 * The following methods are used for creating/deleting composite (envelope)
 * objects.
 */

OutCome GroupView::addTemplate (const Uid& groupAdd, Buffer& toAdd, const char* hostName, pid_t userPid,
				unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::addTemplate ( " << groupAdd << ", Buffer& toAdd )" << endl;
#endif

    AtomicAction  A;
    OutCome       result = NOTDONE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchList(groupAdd, GroupView::head);
	if (ptr)
	    result = ptr->me->addTemplate(toAdd, hostName, userPid, version);
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;
		
		v_number++;
		
		ptr = new ObjectGroups(ReplicaList::create(res));
		if (res == 0)
		{
		    result = ptr->me->addTemplate(toAdd, hostName, userPid, version);
		    if (result == DONE)
		    {
			ptr->me->group = new Uid(groupAdd);
			ptr->groupUid = new Uid(groupAdd);
			ptr->replicaListUid = new Uid(ptr->me->get_uid());
			ptr = extendList(ptr);
			GroupView::numberofgroups++;
		    }
		}
	    }
	    else
		result = LOCKED;
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

Boolean GroupView::getTemplate (const Uid& groupUse, Buffer& toUse, LockMode lMode, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getTemplate ( " << groupUse << " )" << endl;
#endif

    AtomicAction  A;
    Boolean       result = FALSE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupUse, GroupView::head);
	if (ptr)
	    result = ptr->me->getTemplate(toUse, lMode, version);
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
        A.Abort();

    return result;
}

OutCome GroupView::deleteTemplate (const Uid& groupAdd, const char* hostName, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::deleteTemplate ( " << groupAdd << " )" << endl;
#endif

    AtomicAction  A;
    OutCome       result = NOTDONE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    int           res = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	
        ptr = searchList(groupAdd, GroupView::head);
	if (ptr)
	    result = ptr->me->deleteTemplate(hostName, userPid, version);
	if (result == DONE)
	    result = ((removeFromList(ptr, res, hostName, userPid) == TRUE) ? DONE : NOTDONE);
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}
      
OutCome GroupView::replaceTemplate (const Uid& groupAdd, Buffer& toUse, const char* hostName, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::replaceTemplate ( " << groupAdd << ", Buffer& toUse )" << endl;
#endif

    AtomicAction  A;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    OutCome       result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchList(groupAdd, GroupView::head);
	if (ptr)
	    result = ptr->me->replaceTemplate(toUse, hostName, userPid, version);
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

OutCome GroupView::assignCompositeObject (const Uid& groupAssignTo, const Uid& groupToAssign, const char* hostName,
					  pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::assignCompositeObject ( " << groupAssignTo << ", " << groupToAssign << " )" << endl;
#endif

    AtomicAction  A;
    OutCome       result  = NOTDONE;
    ObjectGroups* ptr     = (ObjectGroups*) 0;
    ObjectGroups* tempPtr = (ObjectGroups*) 0;
    
    if (groupToAssign == groupAssignTo)
	return NOTDONE;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupAssignTo, GroupView::head);
	if (ptr)
	{
	    Boolean found = FALSE;

	    tempPtr = GroupView::head;
	    while ((tempPtr) && (!found))
	    {
	        if (*tempPtr->groupUid == groupToAssign)
		    found = TRUE;
		else
		    tempPtr = tempPtr->next;
	    }

	    /* found both group to assign to and group to assign to it. */

	    if (found)
		result = ptr->me->assignCompositeObject(groupToAssign, hostName, userPid, version);
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

OutCome GroupView::assignCompositeObject (const Uid& groupAssignTo, const ReplicaDescriptor& objects, const char* hostName,
					  pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::assignCompositeObject ( " << groupAssignTo << " , const ReplicaDescriptor& objects )" << endl;
#endif

    AtomicAction  A;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    OutCome       result = NOTDONE;

    /*
     * Try to prevent assigning to ourself!
     */

    if (objects.isMemberOfList(groupAssignTo))
	return NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupAssignTo, GroupView::head);
	if (ptr)
	    result = ptr->me->assignCompositeObject(objects, hostName, userPid, version);
    }
    else
        result = LOCKED;

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    return result;
}

OutCome GroupView::removeCompositeObject (const Uid& groupAssignedTo, const Uid& groupAssigned, const char* hostName,
					  pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::removeCompositeObject ( " << groupAssignedTo << ", " << groupAssigned << " )" << endl;
#endif

    AtomicAction  A;
    ObjectGroups* ptr     = (ObjectGroups*) 0;
    ObjectGroups* tempPtr = (ObjectGroups*) 0;
    OutCome       result  = NOTDONE;

    /*
     * Try to avoid assigning to outself.
     */

    if (groupAssigned == groupAssignedTo)
	return NOTDONE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchList(groupAssignedTo, GroupView::head);
	if (ptr)
	{
	    Boolean found = FALSE;

	    tempPtr = GroupView::head;
	    while ((tempPtr) && (!found))
	    {
		if (*tempPtr->groupUid == groupAssigned)
		    found = TRUE;
		else
		    tempPtr = tempPtr->next;
	    }

	    /* found both group assigned to and group assigned to it. */

	    if (found)
		result = ptr->me->removeCompositeObject(groupAssigned, hostName, userPid, version);
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

OutCome GroupView::removeCompositeObject (const Uid& groupAssignedTo, const ReplicaDescriptor& objects, const char* hostName,
					  pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::removeCompositeObject ( " << groupAssignedTo << ", const ReplicaDescriptor& objects )" << endl;
#endif

    AtomicAction  A;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    OutCome       result = NOTDONE;

    if (objects.isMemberOfList(groupAssignedTo))
	return NOTDONE;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        ptr = searchList(groupAssignedTo, GroupView::head);
	if (ptr)
	    result = ptr->me->removeCompositeObject(objects, hostName, userPid, version);
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
 * Decrement the use_count for the given replica group. The use_count is
 * incremented everytime a client requests the groupview. The hostname
 * parameter indicates which host is releasing (decrementing use_count) the
 * group.
 */

OutCome GroupView::releaseGroup (const Uid& group, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::releaseGroup ( " << group << ", " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction  A;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    OutCome       result = NOTDONE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(group, GroupView::head);
	    if (ptr)
		result = ptr->me->releaseGroup(hostname, userPid, version);
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
 * This operation checks that the use_count is zero. If it is then a WRITE lock
 * is obtained on the group and the use_count is set to 1. Used for recovery
 * and migration.
 */

Boolean GroupView::testAndSet (ReplicaDescriptor& objects, const Uid& group, const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::testAndSet ( " << group << ", " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction  A;
    Boolean       opcode   = FALSE;
    ObjectGroups* ptr      = (ObjectGroups*) 0;
    long          UseCount = -1;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchAllLists(group, GroupView::head);
	if (ptr)
	{
	    if (ptr->me->getUseCount(UseCount, version))
	    {
	        if (UseCount == 0)
		{
		    opcode = lockGroup(group, hostname, userPid, WRITE, version);
		    if (opcode)
		        opcode = getView(objects, group, hostname,
					 userPid, version);
		}
	    }
	}
    }

    if (opcode)
        opcode = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return opcode;
}

/*
 * This operation simply alters the location (host name) of a given replica in
 * a given replica group. Typically used for migration.
 */

OutCome GroupView::alterLocation (const Uid& group, const Uid& replica, const char* location, const char* hostName,
				  pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::alterLocation ( " << group << ", " << replica << ", " << location << " )" << endl;
#endif

    AtomicAction  A;
    OutCome       result = NOTDONE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = searchAllLists(group, GroupView::head);
	if (ptr)
	    result = ptr->me->alterLocation(replica, location, hostName, userPid, version);
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
 * Scan the entire database returning information on all replica groups
 * mentioned. This is used by the Database Editor program, and not by any
 * Arjuna classes.
 */

Boolean GroupView::traverseDatabase (ReplicaDescriptor& objects, Uid& group, int& position,
				     const char* hostName, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::traverseDatabase ( ReplicaDescriptor& objects, " << group
		 << ", " << position << ", " << hostName << ", " << userPid << " )\n" << flush;
#endif

    AtomicAction       A;
    Boolean            result = FALSE;
    ReplicaDescriptor* x      = (ReplicaDescriptor*) 0;
    ObjectGroups*      ptr    = (ObjectGroups*) 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((GroupView::numberofgroups == 0) || (position < 0) || (position >= GroupView::numberofgroups))
	{
	    position = -1;
	    result = TRUE;
	}
	else
	{
	    ptr = GroupView::head;
	    for (int i = 0; i < position; i++, ptr = ptr->next);

	    if (ptr->me == (ReplicaList*) 0)
		ptr->me = createReplicaList(*ptr->replicaListUid);

	    group = *ptr->groupUid;

	    x = ptr->me->getList(hostName, userPid, version);
	    if (x)
	    {
		objects = *x;
		delete x;
	    }

	    result = TRUE;
	    position++;
	}
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();

    return result;
}

Boolean GroupView::lockGroup (const Uid& group, const char* hostname, pid_t userPid,
			      LockMode mode, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::lockGroup (const Uid& group, const char* hostname, pid_t userPid, LockMode mode)" << endl;
#endif

    AtomicAction       A;
    ObjectGroups*      ptr    = (ObjectGroups*) 0;
    Boolean            result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(group, GroupView::head);
	    if (ptr)
		result = ptr->me->lockGroup(hostname, userPid, version, mode);
	}
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
        A.Abort();

    return result;
}

/*
 * This returns all of the necessary information about a replica group (if it
 * exists). It also increments the use_count to reflect the number of users of
 * the group. The hostname parameter is used so that replica groups can keep
 * track of which nodes have gained access to them. The recover(...) operation
 * then scans the replica groups and decrements the use_ counts for the replica
 * groups with the given hostname.
 */

Boolean GroupView::getView (ReplicaDescriptor& objects, const Uid& group,
			    const char* hostname, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getView (ReplicaDescriptor&, " << group
                 << ", " << hostname << ", " << userPid << " )" << endl;
#endif

    AtomicAction       A;
    ReplicaDescriptor* x      = (ReplicaDescriptor*) 0;
    ObjectGroups*      ptr    = (ObjectGroups*) 0;
    Boolean            result = FALSE;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(group, GroupView::head);
	    if (ptr)
	    {
		x = ptr->me->getList(hostname, userPid, version);
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
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}

/*
 * When a crashed node recovers it is necessary for it to remove any
 * out-of-date information from the database (such as the corresponding
 * use_count).
 */

OutCome GroupView::recover (const char* hostname, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::recover ( " << hostname << " )" << endl;
#endif
    
    AtomicAction  A;
    OutCome       result  = NOTDONE;
    OutCome       opcode  = NOTDONE;
    Boolean       success = FALSE;
    ObjectGroups* ptr     = (ObjectGroups*) 0;

    if (!hostname)
	return result;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = GroupView::head;
	    while (ptr)
	    {
		AtomicAction B;
		
		B.Begin();

		if (ptr->me == (ReplicaList*) 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		result = ptr->me->recover(hostname, version);
		if (result != DONE)
		{
		    B.Abort();
		    if ((result == INUSE) || (result == LOCKED))
			opcode = INUSE;
		}
		else
		{
		    if (B.End() != COMMITTED)
		    {
			result = NOTDONE;
			break;
		    }
		    else
			success = TRUE;
		}

		ptr = ptr->next;
	    }
	}
	else
	{
	    success = TRUE;  // if this is a new db then return TRUE
	    result = DONE;
	}
    }
    else
	result = LOCKED;

    if (success)
    {
	if (A.End() != COMMITTED)
	    result = NOTDONE;
	else
	{
	    if (opcode == INUSE)
		result = INUSE;
	    else
		result = DONE;
	}
    }
    else
	A.Abort();

    return result;
}

/*
 * include a list of replicas into an already existing group
 */

OutCome GroupView::include (const ReplicaDescriptor& objects, const Uid& group, const char* hostName, pid_t userPid,
			    unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::include (const ReplicaDescriptor& objects, " << group << " )" << endl;
#endif

    AtomicAction  A;
    OutCome       result = NOTDONE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
      	if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(group, GroupView::head);
	    if (ptr)
		result = ptr->me->changeStatus(objects, TRUE, hostName, userPid, version);
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
 * exclude a list of replicas from a group
 */

OutCome GroupView::exclude (const ReplicaDescriptor& objects, const Uid& group, const char* hostName, pid_t userPid,
			    unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::exclude (const ReplicaDescriptor& objects, " << group << " )" << endl;
#endif
   
    AtomicAction  A;
    OutCome       result = NOTDONE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(group, GroupView::head);
	    if (ptr)
		result = ptr->me->changeStatus(objects, FALSE, hostName, userPid, version);
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
 * Given a recovered replica state UID this operation will indicate whether
 * that state is now out-of-date.
 */

OutCome GroupView::needsUpdating (const Uid& recovered, Boolean& isModified, ReplicaDescriptor& info,
				  const char* nodename, pid_t userPid, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::needsUpdating ( " << recovered << ", ReplicaDescriptor&, "
		 << nodename << ", " << userPid << " )" << endl;
#endif

    AtomicAction  A;
    OutCome       result   = NOTDONE;
    ObjectGroups* ptr      = (ObjectGroups*) 0;
    Boolean       isLocked = FALSE;
    Boolean       isPresent;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = GroupView::head;
	    while (ptr)
	    {
		if (ptr->me == (ReplicaList*) 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		isModified = isPresent = FALSE;
		result = ptr->me->hasChanged(recovered, isPresent, isModified);
		
		if (result == DONE)
		{
		    if (isPresent)
		    {
			if (A.End() != COMMITTED)
			    result = NOTDONE;
			else
			{
			    if (isModified)
				return ((getView(info, *ptr->groupUid, nodename, userPid, version) == TRUE) ? DONE : NOTDONE);
			}
			
			return result;
		    }
		}

		if (result == LOCKED)
		    isLocked = TRUE;
		
		ptr = ptr->next;
	    }
	}
    }
    else
	result = LOCKED;
    
    if (isLocked)
	result = LOCKED;
    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
	A.Abort();
    
    return result;
}

/*
 * These routines impose a state on to the database (used for crash recovery).
 * If exclusive access is required then the appropriate operation should be
 * called *before* this method.
 */

Boolean GroupView::getState (ObjectState& os, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getState ()" << endl;
#endif

    AtomicAction  A;
    Boolean       result = FALSE;
    ObjectGroups* ptr = (ObjectGroups*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	result = save_state(os, ANDPERSISTENT);

	ptr = GroupView::head;
	while ((ptr) && (result))
	{
	    if (ptr->me == (ReplicaList*) 0)
		ptr->me = createReplicaList(*ptr->replicaListUid);
	    result = ptr->me->save_state(os, ANDPERSISTENT);
	    ptr = ptr->next;
	}
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}

Boolean GroupView::getReplicaState (const Uid& u, ObjectState& os, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::getState ( " << u << " )" << endl;
#endif

    AtomicAction  A;
    Boolean       result = FALSE;
    ObjectGroups* ptr = (ObjectGroups*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = searchAllLists(u, GroupView::head);
	    if (ptr)
		result = ptr->me->getState(os, version);
	}
    }

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();
    
    return result;
}

Boolean GroupView::imposeState (ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::imposeState ()" << endl;
#endif

    AtomicAction  A;
    Boolean       result = FALSE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    unsigned long dummy;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	// set up envelope object state first
	
	result = restore_state(os, ANDPERSISTENT);
	ptr = GroupView::head;
	
	while ((ptr) && (result))
	{
	    if (ptr->me == (ReplicaList*) 0) // replica not present in database so try to create it
	    {
		int res = -1;
		
		ptr = new ObjectGroups(ReplicaList::create(res));
		if (res == 0)
		{
		    ptr = extendList(ptr);
		    GroupView::numberofgroups++;
		}
		else
		{
		    GroupView::deleteList.add(ptr->me);
		    ptr->me = (ReplicaList*) 0;
		    delete ptr;
		    result = FALSE;
		}
	    }

	    // now call impose state on sub-objects
	    
	    if (result)
	    {
		if (ptr->me)
		    result = ptr->me->imposeState(os, dummy);

		ptr = ptr->next;
	    }
	}
    }
    
    if (result)
	result = (Boolean) (A.End() == COMMITTED);
    else
	A.Abort();

    return result;
}

Boolean GroupView::imposeReplicaState (const Uid& u, ObjectState& os, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::imposeState ( " << u << " )" << endl;
#endif

    AtomicAction  A;
    Boolean       result = FALSE;
    ObjectGroups* ptr    = (ObjectGroups*) 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	ptr = searchAllLists(u, GroupView::head);
	if (!ptr)  // replica not present in database so try to create it
	{
	    int res = -1;
	    
	    ptr = new ObjectGroups(ReplicaList::create(res));
	    if (res == 0)
	    {
		ptr = extendList(ptr);
		GroupView::numberofgroups++;
	    }
	    else
	    {
		GroupView::deleteList.add(ptr->me);
		ptr->me = (ReplicaList*) 0;
		delete ptr;
	    }
	}
	
	if (ptr)
	    result = ptr->me->imposeState(os, version);
    }

    if (result)
	result = (Boolean) (A.End() == COMMITTED);
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
				      const char* hostname, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome GroupView::getNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, "
		 << hostname << " )" << endl;
#endif

    AtomicAction       A;
    OutCome            result = NOTDONE;
    OutCome            opcode = NOTDONE;
    ObjectGroups*      ptr    = (ObjectGroups*) 0;
    ReplicaDescriptor* ptrRD  = &toupdate;
    ReplicaDescriptor* indxRD = &unknownList;

    while (ptrRD->next != (ReplicaDescriptor*) 0)
	ptrRD = ptrRD->next;

    while (indxRD->next != (ReplicaDescriptor*) 0)
	indxRD = indxRD->next;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GroupView::numberofgroups > 0)
	{
	    ptr = GroupView::head;
	    while (ptr)
	    {
		if (ptr->me == (ReplicaList*) 0)
		    ptr->me = createReplicaList(*ptr->replicaListUid);
		
		result = ptr->me->getHostInfo(*ptrRD, *indxRD, hostname, version);

		if ((result == INUSE) || (result == LOCKED))
		    opcode = INUSE;

		ptr = ptr->next;
	    }
	}
	else
	    result = DONE;  // success even if no entries found.
    }
    else
	result = LOCKED;

    if ((result == DONE) || (opcode == INUSE))
    {
	if (A.End() != COMMITTED)
	    result = ((opcode == INUSE) ? INUSE : NOTDONE);
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
    ObjectGroups* ptr = (ObjectGroups*) 0;

    res = os.pack(GroupView::numberofgroups) && os.pack(GroupView::v_number);

    ptr = GroupView::head;
    
    for (int i = 0; (i < GroupView::numberofgroups) && res; i++)
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

    ObjectGroups* ptr  = (ObjectGroups*) 0;
    ObjectGroups* indx = (ObjectGroups*) 0;
    int           currentNumber;
    Boolean       res;

    currentNumber = GroupView::numberofgroups;
    
    res = os.unpack(GroupView::numberofgroups) && os.unpack(GroupView::v_number);
    
    if (res)
    {
	ptr = GroupView::head;
	while (ptr)
	{
	    indx = ptr;
	    ptr = ptr->next;
	    if (indx->me)
		GroupView::deleteList.add(indx->me);
	    indx->me = (ReplicaList*) 0;
	    delete indx;
	}
	    
	GroupView::head = ptr = (ObjectGroups*) 0;

	if ((GroupView::numberofgroups == 0) && (currentNumber > 0))
	    GroupView::deleteList.purge();
	else
	{
	    for (int i = 0; (i < GroupView::numberofgroups) && res; i++)
	    {
		indx = new ObjectGroups;
		indx->replicaListUid = new Uid(NIL_UID);
		indx->groupUid = new Uid(NIL_UID);
		res = indx->replicaListUid->unpack(os) && indx->groupUid->unpack(os);

		if (res)
		    indx->me = GroupView::deleteList.retrieve(*indx->replicaListUid);

		if (GroupView::head == (ObjectGroups*) 0)
		{
		    GroupView::head = indx;
		    ptr = GroupView::head;
		}
		else
		{
		    ptr->next = indx;
		    ptr = ptr->next;
		}
	    }
	}
    }

    ptr = GroupView::head;
    int count = 0;

    while (ptr)
    {
	ptr = ptr->next;
	count++;
    }
    
    GroupView::deleteList.purge();
    
    return res;
}

const TypeName GroupView::type () const
{
    return "/StateManager/LockManager/GroupView";
}
