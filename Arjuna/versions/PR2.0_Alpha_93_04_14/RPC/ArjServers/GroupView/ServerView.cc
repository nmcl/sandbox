/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerView.cc
 */

#include "ServerView.h"
#include "ServerList.h"

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

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ServerGroups::ServerGroups ()
                            : me(0),
			      next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerGroups::ServerGroups ()" << endl;
#endif
}

ServerGroups::~ServerGroups ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerGroups::~ServerGroups ()" << endl;
#endif

    if (me)
	delete me;
}



toDestroy::toDestroy ()
                      : toRemove (0),
			number(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "toDestroy::toDestroy ()\n" << flush;
#endif
}

toDestroy::~toDestroy ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "toDestroy::~toDestroy ()\n" << flush;
#endif

    if (number != 0)
	Purge();
}

ServerList* toDestroy::Retrieve (Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList* toDestroy::Retrieve (Uid& u)" << endl;
#endif

    Boolean found = FALSE;
    ServerList *toReturn = 0;
    ServerGroups *ptr = toRemove, *marker = 0;
    
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
    
Boolean toDestroy::Add (ServerList* toadd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean toDestroy::Add (ServerList* toadd)\n" << flush;
#endif

    Boolean outCome = TRUE;
    ServerGroups* nElement = new ServerGroups;
    
    nElement->me = toadd;
    nElement->next = toRemove;
    toRemove = nElement;
    number++;

    return outCome;
}

void toDestroy::Purge ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void toDestroy::Purge ()\n" << flush;
#endif

    if (number != 0)
    {
	ServerGroups *ptr = toRemove;
	
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




ServerView::ServerView (int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerView::ServerView (int& res)" << endl;
#endif
    
    A.Begin();

    head = 0;
    numberofservers = 0;

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

ServerView::ServerView (const Uid& u, int& res) : LockManager(u)
{
    AtomicAction A;

#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerView::ServerView (const Uid& u, int& res)" << endl;
#endif
    
    A.Begin();

    head = 0;
    numberofservers = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

ServerView::~ServerView ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerView::~ServerView ()" << endl;
#endif
    
    terminate();

    deleteList.Purge();

    if (head)
    {
	ServerGroups* ptr = head;
	while (head)
	{
	    head = head->next;
	    delete ptr;
	    ptr = head;
	}
    }
}

void ServerView::StoreName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
    AtomicAction A;
    ServerGroups* ptr = 0;
    int res = -1;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void ServerView::StoreName (Boolean& done, Buffer key_buff, Buffer data_buff)\n" << flush;
#endif

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (numberofservers == groupnumbers)
	{
	    A.Abort();
	    return;
	}
	else
	{
	    ptr = head;
	    
	    for (int k = 0; k < numberofservers; k++)
	    {
		if (ptr->me->CompareBuffers(key_buff))
		{
		    done = FALSE;
		    A.Abort();
		    return;
		}

		ptr = ptr->next;
	    }

	    ptr = new ServerGroups;
	    ptr->me = new ServerList(res);
	    ptr->next = head;
	    head = ptr;
	    numberofservers++;

	    if (res != 0)
	    {
		A.Abort();
		return;
	    }

	    if (!head->me->StoreName(key_buff, data_buff, v_number))
	    {
		A.Abort();
		return;
	    }
	}

	if (A.End() == COMMITTED)
	    done = TRUE;
    }
    else
	A.Abort();
}

void ServerView::FetchName (Boolean& done, Buffer key_buff, Buffer& data_buff, unsigned long& v_number)
{
    AtomicAction A;
    Boolean result = FALSE;
    int i = 0;
    ServerGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void ServerView::FetchName (Boolean& done, Buffer key_buff, Buffer& data_buff)\n" << flush;
#endif

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofservers == 0)
	{
	    A.Abort();
	    return;
	}

	ptr = head;
	while (i < numberofservers)
	{
	    if (ptr->me->CompareBuffers(key_buff))
	    {
		result = ptr->me->FetchName(data_buff, v_number);
		break;
	    }

	    i++;
	    ptr = ptr->next;
	}

	if (!result)
	    A.Abort();
	else
	{
	    if (A.End() == COMMITTED)
		done = TRUE;
	}
    }
    else
	A.Abort();
}

void ServerView::RemoveName (Boolean& done, Buffer key_buff, int& left, unsigned long& v_number)
{
    AtomicAction A;
    Boolean found = FALSE;
    int i = 0;
    ServerGroups *ptr = 0, *indx = 0;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (numberofservers == 0)
	{
	    A.Abort();
	    return;
	}

	ptr = head;
	while (i < numberofservers)
	{
	    found = ptr->me->CompareBuffers(key_buff);

	    if (found)
	    {
		v_number = ptr->me->GetVersionNumber();
		deleteList.Add(ptr->me);

		if (ptr == head)
		    head = ptr->next;
		else
		    indx->next = ptr->next;

		numberofservers--;

		if (A.End() == COMMITTED)
		{
		    done = TRUE;
		    left = numberofservers;
		}

		return;
	    }
	    else
	    {
		i++;
		indx = ptr;
		ptr = ptr->next;
	    }
	}

	A.Abort();
    }
    else
	A.Abort();
}

void ServerView::ReplaceName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
    AtomicAction A;
    int i = 0;
    Boolean result = FALSE;
    ServerGroups *ptr = 0;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {

	if (numberofservers == 0)
	{
	    A.Abort();
	    return;
	}
	
	ptr = head;
	while (i < numberofservers)
	{
	    if (ptr->me->CompareBuffers(key_buff))
	    {
		result = ptr->me->ReplaceName(data_buff, v_number);
		break;
	    }

	    i++;
	    ptr = ptr->next;
	}

	if (!result)
	    A.Abort();
	else
	{
	    if (A.End() == COMMITTED)
		done = TRUE;
	}
    }
    else
	A.Abort();
}

void ServerView::GetNextName (Boolean& done, Buffer& key_buff, int& position, unsigned long& v_number)
{
    AtomicAction B;
    Boolean result = FALSE;
    ServerGroups* ptr = 0;

    done = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerView::GetNextName (Boolean& done, Buffer& key_buff, int& position)\n" << flush;
#endif

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((numberofservers == 0) || (position < 0) || (position >= numberofservers))
	{
	    position = -1;
	    B.Abort();
	    return;
	}

	ptr = head;
	for (int i = 0; i < position; i++, ptr = ptr->next);
	
	v_number = ptr->me->GetVersionNumber();
	result = ptr->me->GetKey(key_buff);

	if (result)
	{
	    position++;

	    if (B.End() == COMMITTED)
		done = TRUE;
	}
	else
	    B.Abort();
    }
    else
	B.Abort();
}

Boolean ServerView::save_state (ObjectState& os, ObjectType)
{
    Boolean res;
    ServerGroups* ptr = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ServerView::save_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    res = os.pack(numberofservers);

    ptr = head;
    for (int i = 0; (i < numberofservers) && res; i++)
    {
	res = (ptr->me->get_uid()).pack(os);
	ptr = ptr->next;
    }
    
    return res;
}

Boolean ServerView::restore_state (ObjectState& os, ObjectType)
{
    Boolean res, found;
    int err, currentNumber;
    Uid* objList[groupnumbers];
    ServerGroups *ptr = 0, *indx = 0;

    for (int g = 0; g < groupnumbers; g++)
	objList[g] = 0;

    currentNumber = numberofservers;
    
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ServerView::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    res = os.unpack(numberofservers);

    if (res)
    {
	if ((numberofservers == 0) && (currentNumber > 0))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	    debug_stream << "ServerView::restore_state - delete current list" << endl;
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
	    debug_stream << "ServerView::restore_state - load old Uids" << endl;
#endif
	    for (int j = 0; (j < numberofservers) && res; j++)
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
		debug_stream << "ServerView::restore_state - compare 'old' Uids to existing Uids" << endl;
#endif
		for (int i = 0; (i < currentNumber) && (ptr); i++)
		{
		    found = FALSE;
		    
		    for (j = 0; (j < numberofservers) && (!found); j++)
		    {
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "ServerView::restore_state - comparing" << endl;
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
			debug_stream << "ServerView::restore_state - " << ptr->me->get_uid() << " not found. Will delete" << endl;
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
		for (i = 0; (i < numberofservers) && (res); i++)
		{
#ifdef DEBUG
		    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
		    debug_stream << "ServerView::restore_state - checking for recreation" << endl;
#endif		    
		    ServerList* x = 0;
		    found = FALSE;
		    ptr = head;
		    
		    for (j = 0; (j < currentNumber) && (!found) && (ptr); j++)
		    {
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
			debug_stream << "ServerView::restore_state - comparing " << *objList[i] << " with " << ptr->me->get_uid() << endl;
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
			debug_stream << "ServerView::restore_state - " << *objList[i] << " not found" << endl;
#endif			
			x = deleteList.Retrieve(*objList[i]);

			if (!x)
			{
			    err = -1;
			    x = new ServerList(*objList[i], err);
			    res = (Boolean) (err == 0);
			}
			else
			    res = TRUE;

			indx = new ServerGroups;
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

const TypeName ServerView::type () const
{
    return "/StateManager/LockManager/ServerView";
}
