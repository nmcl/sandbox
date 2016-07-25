/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupView.cc,v 1.27 1993/04/30 12:09:32 nmcl Exp $
 */

#ifndef GROUPVIEW_H_
#include "GroupView.h"
#endif

#ifndef REPICALIST_H_
#include "ReplicaList.h"
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ObjectGroups::ObjectGroups ()
                            : me(0),
			      next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ObjectGroups::ObjectGroups ()" << endl;
#endif
}

ObjectGroups::~ObjectGroups ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ObjectGroups::~ObjectGroups ()" << endl;
#endif

    if (me)
	delete me;
}



toPurge::toPurge ()
                  : toRemove(0),
		    number(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "toPurge::toPurge ()\n" << flush;
#endif
}

toPurge::~toPurge ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "toPurge::~toPurge ()\n" << flush;
#endif

    if (number != 0)
	Purge();
}

ReplicaList* toPurge::Retrieve (Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList* toPurge::Retrieve (Uid& u)" << endl;
#endif

    Boolean found = FALSE;
    ReplicaList *toReturn = 0;
    ObjectGroups *ptr = toRemove, *marker = 0;
    
    if (!toRemove)
	return 0;
    
    while ((ptr) && (!found))
    {

	if (ptr->me->get_uid() == u)
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
    
Boolean toPurge::Add (ReplicaList* toadd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean toPurge::Add (ReplicaList* toadd)\n" << flush;
#endif

    Boolean outCome = TRUE;
    ObjectGroups* nElement = new ObjectGroups;
     
    nElement->me = toadd;
    nElement->next = toRemove;
    toRemove = nElement;
    number++;

    return outCome;
}

void toPurge::Purge ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void toPurge::Purge ()\n" << flush;
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
    AtomicAction A;

#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::GroupView (int& res)\n" << flush;
#endif

    A.Begin();

    head = 0;
    numberofgroups = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
	res = 0;
    else
	res = -1;

    if (res == 0)
    {
	if (A.End() != COMMITTED)
	    res = -1;
    }
    else
	A.Abort();
}

GroupView::GroupView (const Uid& u, int& res) : LockManager(u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
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
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::~GroupView()\n" << flush;
#endif

    terminate();

    deleteList.Purge();

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

/* Add a replica to the database. If no replica group exists with Uid then this is
 * treated as the first member of a new replica group and so this group is added to
 * the database list. Otherwise the replica is added to the list of existing members
 * for the specified replica group.
 */

Boolean GroupView::Add_Replicas (const ReplicaDescriptor& objects, const Uid& group, unsigned long& v_number)
{
    AtomicAction A;
    ObjectGroups *ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::Add_Replicas (ReplicaDescriptor objects, const Uid& group)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)  // only need READ lock on enclosing database for increased concurrency
    {
	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (*ptr->me->group == group)  // is group already mentioned in database?
	    {
		if (ptr->me->AddToList(objects, v_number)) // if so then add it to the list of group members
		{
		    if (A.End() != COMMITTED)
			return FALSE;
		    else
			return TRUE;
		}
		else
		{
		    A.Abort();
		    return FALSE;
		}
	    }
	    else
		ptr = ptr->next;
	}

	// This is a new group and so should be added to list of groups

	if (setlock(new Lock(WRITE), 0) == GRANTED) // must acquire WRITE lock as we are modifying database state
	{
	    int res = -1;

	    ptr = new ObjectGroups;
	    ptr->me = new ReplicaList(res); // create new replica group entry
	    ptr->next = head;
	    head = ptr;

	    numberofgroups++;
	    if (res != 0)
	    {
		A.Abort();
		return FALSE;
	    }
	    else
	    {
		if (!ptr->me->AddToList(objects, v_number)) // add replica to currently empty group
		{
		    A.Abort();
		    return FALSE;
		}

		ptr->me->group = new Uid(NIL_UID);
		*ptr->me->group = group;
	    }

	    if (A.End() != COMMITTED)
		return FALSE;
	    else
		return TRUE;
	}
	else
	{
	    A.Abort();
	    return FALSE;
	}
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

/* Delete replicas from replica group, or delete entire group.
 */

Boolean GroupView::Delete_Replicas (const ReplicaDescriptor& objects, const Uid& group, const Boolean deleteAll,
				    int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::Delete_Replicas (ReplicaDescriptor view, int& res)\n" << flush;
#endif

    AtomicAction A;
    Boolean found = FALSE, result = FALSE;
    long left = -1;
    ObjectGroups *ptr = 0, *marker = 0;

    A.Begin();

    res = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	ptr = head;
	marker = ptr;
	for (int i = 0; ((i < numberofgroups) && (!result) && (ptr)); i++)
	{
	    if (*ptr->me->group == group)
	    {
		if (!deleteAll)
		    found = ptr->me->DeleteFromList(objects, left, v_number);
		else
		{
		    if (deleteList.Add(ptr->me))
		    {
			if (ptr == head)
			    head = ptr->next;
			else
			    marker->next = ptr->next;

			result = TRUE;
			numberofgroups--;
			res = numberofgroups;
		    }
		}

		if ((found) && (!deleteAll))
		{
		    if (left == 0)
		    {
			if (deleteList.Add(ptr->me))
			{
			    if (ptr == head)
				head = ptr->next;
			    else
				marker->next = ptr->next;

			    numberofgroups--;
			    res = numberofgroups;
			    result = TRUE;
			}
		    }
		    else
			result = TRUE;
		}
	    }
	    else
	    {
		marker = ptr;
		ptr = ptr->next;
	    }
	}
    }

    if (result)
    {
	if (A.End() == COMMITTED)
	    return TRUE;
	else
	    return FALSE;
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

/* Decrement the UseCount for the given replica group. The UseCount is incremented everytime a
 * client requests the groupview. The hostname parameter indicates which host is releasing
 * (decrementing use_count) the group. At present this is not used.
 */

Boolean GroupView::Release_Group (const Uid& group, unsigned long& v_number, char* hostname)
{
    AtomicAction A;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::Release_Group (const Uid& group)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups == 0)
	{
	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (*ptr->me->group == group)
	    {
		if (ptr->me->ReleaseGroup(v_number, hostname))
		{
		    A.Abort();
		    return FALSE;
		}

		if (A.End() == COMMITTED)
		    return TRUE;
		else
		    return FALSE;
	    }
	    else
		ptr = ptr->next;
	}

	A.Abort();
	return FALSE;
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

/* This operation checks that the use_count is zero. If it is then a WRITE lock is obtained on
 * the group and the use_count is set to 1. Used for recovery and migration.
 */

Boolean GroupView::CheckAndSet (const Uid& group, unsigned long& v_number)
{
    AtomicAction A;
    Boolean opcode = FALSE;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::CheckAndSet (const Uid& group, unsigned long& v_number)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (*ptr->me->group == group)
	    {
		v_number = ptr->me->GetVersionNumber();

		if (ptr->me->GetUseCount() == 0)
		{
		    if (ptr->me->GetExclusiveAccess())
			opcode = TRUE;
		}

		if (A.End() == COMMITTED)
		    return opcode;
		else
		    return FALSE;
	    }
	    else
		ptr = ptr->next;
	}

	A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean GroupView::GetExclusiveGroupView (ReplicaDescriptor& rd, const Uid& group, unsigned long& v_number)
{
    AtomicAction A;
    Boolean opcode = FALSE;
    
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::GetExclusiveGroupView" << endl;
#endif

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (CheckAndSet(group, v_number))
	{
	    if (Get_View(rd, group, v_number))
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return opcode;
}

/* This operation simply alters the location (host name) of a given replica in a given replica
 * group. Used for migration.
 */

Boolean GroupView::AlterLocation (const Uid& group, const Uid& replica,
				  string location, unsigned long& v_number)
{
    AtomicAction A;
    Boolean opcode = FALSE;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::AlterLocation (const Uid& group, const Uid& replica, string location)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (*ptr->me->group == group)
	    {
		v_number = ptr->me->GetVersionNumber();
		opcode = ptr->me->AlterLocation(replica, location);
		    
		if (opcode)
		{
		    if (A.End() != COMMITTED)
			opcode = FALSE;
		}
		else
		    A.Abort();
		    
		return opcode;
	    }
	    else
		ptr = ptr->next;
	}

	A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

/* Scan the entire database returning information on all replica groups mentioned. This is primarily used
 * by the Database Editor program, and not by any Arjuna classes.
 */

Boolean GroupView::Traverse_Database (ReplicaDescriptor& objects, Uid& group,
				      int& position, unsigned long& v_number)
{
    Boolean found = FALSE;
    AtomicAction A;
    GroupData* x;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::Traverse_Database (ReplicaDescriptor& objects, const int position)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((numberofgroups == 0) || (position < 0) || (position >= numberofgroups))
	{
	    position = -1;
	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (int i = 0; i < position; i++, ptr = ptr->next);
	
	x = ptr->me->GetList(v_number);
	ReplicaDescriptor* p = &objects;

	while (x != 0)
	{
	    *p = *x;
	    group = *ptr->me->group;

	    if (x->next != 0)
	    {
		p->next = new ReplicaDescriptor(0);
		p = p->next;
	    }
	    else
		p->next = 0;
	    
	    x = x->next;
	}

	position++;
	if (A.End() != COMMITTED)
	    return FALSE;
	else
	    return TRUE;
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

/* This returns all of the necessary information about a replica group (if it exists). It
 * also increments the UseCount to reflect the number of users of the group. If mode = WRITE
 * then exclusive access is obtained to the group. This is required for recovering nodes, which
 * do not want replicas to be changing state while they are using them to update the states of
 * out-of-date replicas.
 * The hostname parameter is used so that replica groups can keep track of which nodes have
 * gained access to them. The Recover operation then scans the replica groups and decrements
 * the use counts for the replica groups with the given hostname.
 */

Boolean GroupView::Get_View (ReplicaDescriptor& objects, const Uid& group,
			     unsigned long& v_number, const LockMode mode, char* hostname)
{
    Boolean found = FALSE;
    AtomicAction A;
    GroupData *x;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::Get_View (ReplicaDescriptor& objects, const Uid& group, const LockMode mode)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(mode), 0) == GRANTED)
    {
	if (numberofgroups == 0)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	    debug_stream << "GroupView::Get_View - No replicas have been added to group yet.\n" << flush;
#endif

	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (*ptr->me->group == group)
	    {
		x = ptr->me->GetList(v_number, hostname);
		ReplicaDescriptor* p = &objects;

		while (x != 0)
		{
		    found = TRUE;
		    *p = *x;
		    
		    if (x->next != 0)
		    {
			p->next = new ReplicaDescriptor(0);
			p = p->next;
		    }
		    else
			p->next = 0;

		    x = x->next;
		}

		if (A.End() != COMMITTED)
		    return FALSE;
		else
		    return FALSE;
	    }
	    else
		ptr = ptr->next;
	}

	A.Abort();
	return FALSE;
    }
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	debug_stream << "GroupView::Get_View - Could not set lock\n" << flush;
#endif

	A.Abort();
	return FALSE;
    }
}

Boolean GroupView::Recover (const char* hostname)
{
    AtomicAction A;
    int i;
    Boolean opcode = FALSE, result;
    ObjectGroups* ptr = 0;

    if (!hostname)
	return FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups == 0)
	{
	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    result = ptr->me->Recover(hostname);
	    if (!result)
	    {
		A.Abort();
		break;
	    }
	    ptr = ptr->next;
	}

	if (A.End() == COMMITTED)
	    opcode = TRUE;
    }
    else
	A.Abort();

    return opcode;
}

/* Used to change the status (active or inactive) of a replica, i.e., this functions
 * both as the exclude and include operation depending on the value of the state parameter.
 */

Boolean GroupView::ChangeReplicaStatus (const ReplicaDescriptor& objects, const Uid& group,
					const Boolean state, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Within GroupView::ChangeReplicaStatus (ReplicaDescriptor objects, const Uid& group)\n" << flush;
#endif

    AtomicAction A;
    ObjectGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups == 0)
	{
	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (*ptr->me->group == group)
	    {
		if (ptr->me->ChangeStatus(objects, state, v_number))
		{
		    if (A.End() == COMMITTED)
			return TRUE;
		    else
			return FALSE;
		}
		else
		{
		    A.Abort();
		    return FALSE;
		}
	    }
	    else
		ptr = ptr->next;
	}

	if (i == numberofgroups)
	{
	    A.Abort();
	    return FALSE;
	}
	else
	{
	    if (A.End() == COMMITTED)
		return TRUE;
	    else
		return FALSE;
	}
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

/* Given a recovered replica state UID this operation will indicate whether that state is
 * now out-of-date.
 */

Boolean GroupView::NeedsUpdating (const Uid& recovered, ReplicaDescriptor& info, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::NeedsUpdating (const Uid& recovered, ReplicaDescriptor& info)\n" << flush;
#endif

    AtomicAction A;
    ObjectGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups == 0)
	{
	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    if (ptr->me->HasChanged(recovered))
	    {
		if (A.End() == COMMITTED)
		    return Get_View(info, *ptr->me->group, v_number);
		else
		    return FALSE;
	    }
	    else
		ptr = ptr->next;
	}

	A.Abort();
	return FALSE;
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

Boolean GroupView::GetState (ObjectState& os, unsigned long& v_number)
{
    AtomicAction A;
    Boolean result = FALSE;
    ObjectGroups* ptr = 0;
    
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean GroupView::GetState (ObjectState& os, unsigned long& v_number)\n" << flush;
#endif

    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED) // must get exclusive access to all objects!!
    {
	result = os.pack(numberofgroups);
	if (numberofgroups != 0)
	{
	    ptr = head;
	    for (int i = 0; ((i < numberofgroups) && (result) && (ptr)); i++)
	    {
		result = ptr->me->GetState(os, v_number);
		ptr = ptr->next;
	    }

	    if (result)
		result = (Boolean) (A.End() == COMMITTED);
	    else
		A.Abort();
	}
	else
	{
	    if (result)
		result = (Boolean) (A.End() == COMMITTED);
	    else
		A.Abort();
	}
    }
    else
	A.Abort();
    
    return result;
}

Boolean GroupView::ImposeState (ObjectState& os)
{
    AtomicAction A;
    Boolean result = FALSE, problem = FALSE;
    ObjectGroups* ptr = 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	ptr = head;
	for (int i = 0; ((i < numberofgroups) && (ptr) && (!problem)); i++)
	{
	    if (deleteList.Add(ptr->me))
		ptr = ptr->next;
	    else
		problem = TRUE;
	}
	
	if (!problem)
	{
	    head = 0;
	    problem = os.unpack(numberofgroups);
	    
	    if (problem)
	    {
		if (numberofgroups > 0)
		{
		    for (i = 0; ((i < numberofgroups) && (!problem)); i++)
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
			
			ptr->me = new ReplicaList(res);
			if (res != 0)
			{
			    A.Abort();
			    return FALSE;
			}
			else
			    problem = ptr->me->ImposeState(os);
		    }
		}
	    }
	    
	    if (problem)
		A.Abort();
	    else
		result = (Boolean) (A.End() == COMMITTED);
	}
    }
    else
	A.Abort();
    
    return result;
}

/* This operation will return a list of all of those replicas belonging to a given node which need
 * updating. Since some groups may be locked and these may then be updated after this operation
 * completes, those replicas will also be added to the returned list.
 */

Boolean GroupView::GetNodeUpdateList (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				      string hostname, Boolean& unknownState, unsigned long& v_number)
{
    AtomicAction A;
    Boolean result = FALSE, lockProblem = FALSE;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::GetNodeUpdateList ()\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofgroups == 0)
	{
	    A.Abort();
	    return FALSE;
	}

	ptr = head;
	for (int i = 0; (i < numberofgroups) && (ptr); i++)
	{
	    lockProblem = FALSE;
	    result = ptr->me->GetHostInfo(toupdate, unknownList, hostname, lockProblem, v_number);
		
	    if ((!result) && (!lockProblem))
	    {
		A.Abort();
		return FALSE;
	    }

	    if ((!result) && (lockProblem))
	    {
		unknownState = lockProblem;
		A.Abort();
	    }

	    ptr = ptr->next;
	}

	if (A.End() == COMMITTED)
	    return result;
	else
	    return FALSE;
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

Boolean GroupView::save_state (ObjectState& os, ObjectType)
{
    Boolean res;
    ObjectGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    res = os.pack(numberofgroups);

    ptr = head;
    for (int i = 0; (i < numberofgroups) && res; i++)
    {
	res = (ptr->me->get_uid()).pack(os);
	ptr = ptr->next;
    }

    return res;
}

Boolean GroupView::restore_state (ObjectState& os, ObjectType)
{
    Boolean res, found;
    int err, currentNumber;
    Uid* objList[groupnumbers];
    ObjectGroups *ptr = 0, *indx = 0;

   for (int g = 0; g < groupnumbers; g++)
	objList[g] = 0;

    currentNumber = numberofgroups;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupView::restore_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    res = os.unpack(numberofgroups);

    if (res)
    {
	if ((numberofgroups == 0) && (currentNumber > 0))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	    debug_stream << "GroupView::restore_state - delete current list" << endl;
#endif
	    deleteList.Purge();
	    ptr = head;
	    
	    for (int i = 0; i < currentNumber; i++)
	    {
		indx = ptr;
		ptr = ptr->next;
		delete indx;
	    }
	    
	    head = 0;
	}
	else
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	    debug_stream << "GroupView::restore_state - load old Uids" << endl;
#endif
	    for (int j = 0; (j < numberofgroups) && res; j++)
	    {
		objList[j] = new Uid(NIL_UID);
		res = objList[j]->unpack(os);
	    }
    
	    if (res)
	    {
		ptr = head;
		indx = ptr;
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
		debug_stream << "GroupView::restore_state - compare 'old' Uids to existing Uids" << endl;
#endif		
		for (int i = 0; (i < currentNumber) && (ptr); i++)
		{
		    found = FALSE;
		    
		    for (j = 0; (j < numberofgroups) && (!found); j++)
		    {
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "GroupView::restore_state - comparing" << endl;
			debug_stream << ptr->me->get_uid() << endl;
			debug_stream << " with " << endl;
			debug_stream << *objList[j] << endl;
#endif	
			if (ptr->me->get_uid() == *objList[j])
			    found = TRUE;
		    }
		    
		    if (!found)
		    {
			if (ptr == head)
			    head = ptr->next;
			else
			    indx->next = ptr->next;

#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "GroupView::restore_state - " << ptr->me->get_uid() << " not found. Will delete" << endl;
#endif
			delete ptr;
			ptr = indx->next;
		    }
		    else
		    {
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "ServerView::restore_state - " << ptr->me->get_uid() << " found" << endl;
#endif
			indx = ptr;
			ptr = ptr->next;
		    }
		}

		res = TRUE;
		for (i = 0; (i < numberofgroups) && (res); i++)
		{
#ifdef DEBUG
		    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
		    debug_stream << "GroupView::restore_state - checking for recreation" << endl;
#endif	
		    ReplicaList* x = 0;
		    found = FALSE;
		    ptr = head;
		    
		    for (j = 0; (j < currentNumber) && (!found) && (ptr); j++)
		    {
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "GroupView::restore_state - comparing " << *objList[i] << " with " << ptr->me->get_uid() << endl;
#endif	
			if (*objList[i] == ptr->me->get_uid())
			    found = TRUE;
			else
			    ptr = ptr->next;
		    }
	    
		    if (!found)
		    {
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "GroupView::restore_state - " << *objList[i] << " not found" << endl;
#endif		
			x = deleteList.Retrieve(*objList[i]);

			if (!x)
			{
			    err = -1;
			    x = new ReplicaList(*objList[i], err);
			    res = (Boolean) (err == 0);
			}
			else
			    res = TRUE;

			indx = new ObjectGroups;
			indx->me = x;
			indx->next = head;
			head = indx;
		    }
		    else
			res = TRUE;
		}
	    }
	}
    }
    
    for (int i = 0; i < groupnumbers; i++)
	if (objList[i])
	    delete objList[i];

    deleteList.Purge();

    return res;
}


const TypeName GroupView::type () const
{
    return "/StateManager/LockManager/GroupView";
}
