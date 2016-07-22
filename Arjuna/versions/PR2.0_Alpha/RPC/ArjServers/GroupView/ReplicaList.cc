/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaList.cc
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ERROR_H_
#include <Common/Error.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#include "GroupView.h"
#include "ReplicaList.h"

/* Note that at present the use_count is not used (this is because nested-top-level actiona
 * are not currently supported in Arjuna [and the use_count could be a new lock type anyway]).
 * Thus all references to incrementing or decrementing the use_count have been commented out.
 * These will be removed when the use_count is fully made use of.
 */

ReplicaList::ReplicaList (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::ReplicaList (int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	self = 0;
	group = 0;
	number = 0;
	use_count = 0;
	v_number = 0;
	for (int i = 0; i < groupnumbers; i++)
	    hosts[i] = 0;

	if (A.End() == COMMITTED)
	    res = 0;
	else
	    res = -1;
    }
    else
	A.Abort();
}

ReplicaList::ReplicaList (const Uid& u_id, int& res) : LockManager(u_id)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::ReplicaList (Uid& u_id, int& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    number = 0;
    self = 0;
    group = 0;
    use_count = 0;
    v_number = 0;

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
    GroupData* ptr = self, *marker;

#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::~ReplicaList()\n" << flush;
#endif

    terminate();

    if (group)
	delete group;

    for (int i = 0; i < number; i++)
    {
	marker = ptr;
	ptr = ptr->next;

	if (marker)
	    delete marker;
    }
}

long ReplicaList::GetUseCount () { return use_count; }

unsigned long ReplicaList::GetVersionNumber () { return v_number; }

Boolean ReplicaList::GetExclusiveAccess ()
{
    AtomicAction A;
    Boolean opcode = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (use_count == 0)
	{
//	    use_count++;
	    if (A.End() == COMMITTED)
		opcode = TRUE;
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean ReplicaList::AlterLocation (const Uid& replica, string location)
{
    AtomicAction A;
    GroupData *ptr;

    if (!location)
	return FALSE;
    
    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	ptr = self;

	while (ptr != 0)
	{
	    if (*ptr->GetUid() == replica)
	    {
		ptr->SetHost(location);

		if (A.End() == COMMITTED)
		    return TRUE;
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
    
    return FALSE;
}

Boolean ReplicaList::Recover (const char* hostname)
{
    AtomicAction A;
    Boolean found = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	for (int i = 0; ((i < use_count) && (!found)); i++)
	    if (strcmp(hosts[i], hostname) == 0)
		found = TRUE;

	if (!found)
	{
	    A.Abort();
	    return TRUE;
	}

	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    for (int k = 0; k < use_count; k++)
	    {
		delete hosts[i];
		hosts[i] = 0;
//		use_count--;
	    }

	    for (int j = 0; j < groupnumbers -1; j++)
		if (hosts[j] == 0)
		    hosts[j] = hosts[j+1];

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

GroupData* ReplicaList::GetList (unsigned long& version, const char* hostname)
{
    AtomicAction A;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::GetList()\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;

	if (use_count < groupnumbers)
	{
	    if (hostname)
	    {
		hosts[use_count] = new char[strlen(hostname)];
		::memset(hosts[use_count], '\0', strlen(hostname));
		::memcpy(hosts[use_count], hostname, strlen(hostname));
	    }
	    else
		hosts[use_count] = 0;

//	    use_count++;

	    if (A.End() != COMMITTED)
		return 0;
	    else
		return self;
	}
	else
	{
	    error_stream << WARNING << "use_count full.\n" << flush;
	    A.Abort();
	    return 0;
	}
    }
    else
    {
	A.Abort();
	return 0;
    }
}

Boolean ReplicaList::AddToList (const ReplicaDescriptor& rd, unsigned long& version)
{
    AtomicAction A;
    GroupData* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::AddToList (ReplicaDescriptor rd)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	version = v_number;
	
	if (self != 0)
	{
	    ptr = self;
	    while (ptr->next)
		ptr = ptr->next;

	    ptr->next = new GroupData(0);
	    ptr = ptr->next;
	}
	else
	{
	    self = new GroupData(0);
	    ptr = self;
	}

	*ptr = rd;
	number += rd.GetNumber();

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

Boolean ReplicaList::DeleteFromList (const ReplicaDescriptor& rd, long& left, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::DeleteFromList (ReplicaDescriptor rd)\n" << flush;
#endif

    AtomicAction A;
    const ReplicaDescriptor* ptr = &rd;
    Boolean found = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	version = v_number;

	if (self == 0)
	{
	    A.Abort();
	    return FALSE;
	}

	while (ptr)
	{
	    GroupData* marker = self, *pointer = self;

	    for (int i = 0; i < number; i++)
	    {
		if (*marker == *ptr)
		{
		    found = TRUE;

		    if (marker == self)
		    {
			self = marker->next;
			delete marker;
			marker = self;
			number--;
		    }
		    else
		    {
			pointer->next = marker->next;
			delete marker;
			marker = pointer->next;
			number--;
		    }
		}
		else
		{
		    pointer = marker;
		    marker = marker->next;
		}
	    }

	    ptr = ptr->next;
	}

	if (number == 0)
	    self = 0;

	left = number;

	if (found)
	{
	    if (A.End() == COMMITTED)
		return found;
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
    {
	A.Abort();
	return FALSE;
    }
}

Boolean ReplicaList::HasChanged (const Uid recovered)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::HasChanged (Uid recovered)\n" << flush;
#endif

    AtomicAction A;
    GroupData* marker = self;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	while (marker != 0)
	{
	    if ((*marker->GetUid() == recovered) && (!marker->GetActive()))
	    {
		if (A.End() == COMMITTED)
		    return TRUE;
		else
		    return FALSE;
	    }
	    else
		marker = marker->next;
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

Boolean ReplicaList::ChangeStatus (const ReplicaDescriptor& rd, const Boolean state, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Within ReplicaList::ChangeStatus (ReplicaDescriptor rd)\n" << flush;
#endif
    AtomicAction A;
    const ReplicaDescriptor* ptr = &rd;
    int n_active = 0, i;
    GroupData* marker;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	version = v_number;

	while (ptr)
	{
	    marker = self;

	    for (i = 0; i < number; i++)
	    {
		if (*marker == *ptr)
		{
		    if (ptr->GetActive() == state)
			marker->SetActive(state);
		    break;
		}
		else
		    marker = marker->next;
	    }
	
	    ptr = ptr->next;
	}
    }

    if (!state)
    {
	marker = self;
	for (i = 0; i < number; i++)
	{
	    if (marker->GetActive())
		n_active++;
	    marker = marker->next;
	}

	if (n_active > 0)
	{
	    if (A.End() == COMMITTED)
		return TRUE;
	    else
		return FALSE;
	}
	else
	{
#ifdef DEBUG
	    error_stream << WARNING << "Exclude operation would make entire group unavailable. Not done.\n" << flush;
#endif
	    A.Abort();
	    return FALSE;
	}
    }
    A.Abort();
    return FALSE;
}

Boolean ReplicaList::GetState (ObjectState& os, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::GetState (ObjectState& os, unsigned long& version)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	result = save_state(os, ANDPERSISTENT);
	version = v_number;
	
	if (result)
	    result = (Boolean) (A.End() == COMMITTED);
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return result;
}

Boolean ReplicaList::ImposeState (ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::ImposeState (ObjectState& os)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	result = restore_state(os, ANDPERSISTENT);
	
	if (result)
	    result = (Boolean) (A.End() == COMMITTED);
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return result;
}

Boolean ReplicaList::ReleaseGroup (unsigned long& version, char* hostname)
{
    AtomicAction A;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaList::ReleaseGroup ()\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (hostname)
	{
	    for (int i = 0; i < use_count; i++)
	    {
		if (strcmp(hosts[i], hostname) == 0)
		{
		    delete hosts[i];
		    hosts[i] = 0;
		    break;
		}
	    }

	    for (int k = 0; k < groupnumbers -1; k++)
		if (hosts[k] == 0)
		    hosts[k] = hosts[k+1];
	}

//	use_count--;
	v_number++;
	version = v_number;

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

Boolean ReplicaList::GetHostInfo (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				  const string hostname, Boolean& unknownState, unsigned long& version)
{
    AtomicAction A;
    GroupData* marker;
    ReplicaDescriptor *ptr = &toupdate, *indx = &unknownList;
    Boolean firstUpdate = TRUE, firstUnknown = TRUE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::GetHostInfo (ReplicaDescriptor&, string)\n" << flush;
#endif

// This operation should make use of the use_count (see paper).

    while (ptr->next != 0)
	ptr = ptr->next;

    while (indx->next != 0)
	indx = indx->next;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	marker = self;

	while (marker != 0)
	{
	    if (strcmp(marker->GetHost(), hostname) == 0)
	    {
		if (setlock(new Lock(WRITE), 0) == GRANTED) // Try to get exclusive access
		{
		    if (!marker->GetActive()) // only add if replica is now marked as unavailable
		    {
			if (!firstUpdate)
			{
			    ptr->next = new ReplicaDescriptor(0);
			    ptr = ptr->next;
			    *ptr = *marker;
			}
			else
			{
			    toupdate = *marker;
			    firstUpdate = FALSE;
			}
		    }
		}
		else // still notify recovering node as it may have to perform recover when other actions have ended
		{
		    // do not bother checking availability status

		    if (!firstUnknown)
		    {
			indx->next = new ReplicaDescriptor(0);
			indx = indx->next;
			*indx = *marker;
		    }
		    else
		    {
			unknownList = *marker;
			firstUnknown = FALSE;
		    }
		}
	    }

	    marker = marker->next;
	}

	if (A.End() == COMMITTED)
	    return TRUE;
	else
	    return FALSE;
    }
    else
    {
	unknownState = TRUE;
	A.Abort();
	return FALSE;
    }
}

Boolean ReplicaList::save_state (ObjectState& os, ObjectType)
{
    Boolean res;
    GroupData* ptr = self;
    char blank = ' ';

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::save_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    res = os.pack(number) && os.pack(v_number);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::saving " << number << " and " << v_number << "\n" << flush;
#endif

    if (number > 0)
    {
	res = (res && group->pack(os) && os.pack(use_count));

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	debug_stream << "ReplicaList::saving " << *group << " and " << use_count << "\n" << flush;
#endif

	for (int k = 0; k < use_count; k++)
	{
	    if (!hosts[k])
	    {
		res = res && os.pack(blank);
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
		debug_stream << "ReplicaList::saving " << blank << "\n" << flush;
#endif
	    }
	    else
	    {
		res = res && os.pack(hosts[k]);
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
		debug_stream << "ReplicaList::saving " << hosts[k] << "\n" << flush;
#endif
	    }
	}
    }

    for (int i = 0; (i < number) && res; i++)
    {
	res = os.pack(ptr->GetHost()) && ptr->GetUid()->pack(os) && os.pack(ptr->GetActive());

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	debug_stream << "ReplicaList::saving -" << endl;
	debug_stream << *ptr << endl;
#endif
	ptr = ptr->next;
    }

    return res;
}

Boolean ReplicaList::restore_state (ObjectState&os, ObjectType)
{
    Boolean res;
    GroupData* ptr;
    char blank = ' ';

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::restore_state (ObjectState&os, ObjectType)\n" << flush;
#endif

// If old list then delete it

    if (self)
    {
	GroupData* ptr = self, *marker;
	for (int i = 0; (i < number) && (ptr != 0); i++)
	{
	    if (ptr != 0)
	    {
		marker = ptr;
		ptr = ptr->next;
		
		if (marker)
		    delete marker;
	    }
	}

	self = 0;
	number = 0;
    }

    res = os.unpack(number) && os.unpack(v_number);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaList::restoring " << number << " and " << v_number << "\n" << flush;
#endif

    if (number > 0)
    {
	group = new Uid(NIL_UID);
	res = (res && group->unpack(os) && os.unpack(use_count));

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	debug_stream << "ReplicaList::restoring " << *group << " and " << use_count << "\n" << flush;
#endif

	for (int k = 0; k < use_count; k++)
	{
	    res = res && os.unpack(hosts[k]);
	    if (*hosts[k] == blank)
		hosts[k] = 0;

#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	    debug_stream << "ReplicaList::restoring " << hosts[k] << "\n" << flush;
#endif
	}
    }

    if ((res) && (number > 0))
    {
	self = new GroupData(0);
	ptr = self;

	for (int i = 0; (i < number) && res; i++)
	{
	    char* temp = 0;
	    Uid u_id;
	    Boolean tempVal;
	    

	    res = os.unpack(temp) && u_id.unpack(os) && os.unpack(tempVal);
	    
	    if (res)
	    {
		ptr->SetHost(temp);
		ptr->SetUid(u_id);
		ptr->SetActive(tempVal);
	    }
	    
	    delete temp;

#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	    debug_stream << "ReplicaList::restoring -" << endl;
	    debug_stream << *ptr << endl;
#endif

	    if ((i +1 < number) && res)
	    {
		ptr->next = new GroupData(0);
		ptr = ptr->next;
	    }
	    else
		ptr->next = 0;
	}
    }
    else
	self = 0;

    return res;
}

const TypeName ReplicaList::type () const
{
    return "/StateManager/LockManager/ReplicaList";
}
