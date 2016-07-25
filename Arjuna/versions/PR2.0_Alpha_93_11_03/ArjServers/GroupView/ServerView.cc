/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerView.cc,v
 */

#ifndef SERVERVIEW_H_
#  include "ServerView.h"
#endif

#ifndef SERVERLIST_H_
#  include "ServerList.h"
#endif

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

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ServerGroups::ServerGroups ()
                            : serverListUid(0),
                              me(0),
			      next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerGroups::ServerGroups ()" << endl;
#endif
}

ServerGroups::~ServerGroups ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerGroups::~ServerGroups ()" << endl;
#endif

    if (serverListUid)
        delete serverListUid;
    if (me)
        ServerList::remove(me);
}



ToDestroy::ToDestroy ()
                      : toRemove (0),
			number(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ToDestroy::ToDestroy ()\n" << flush;
#endif
}

ToDestroy::~ToDestroy ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ToDestroy::~ToDestroy ()\n" << flush;
#endif

    if (number != 0)
	purge();
}

ServerList* ToDestroy::retrieve (Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList* ToDestroy::retrieve (Uid& u)" << endl;
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
    
Boolean ToDestroy::add (ServerList* toadd)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ToDestroy::add (ServerList* toadd)\n" << flush;
#endif

    Boolean outCome = TRUE;
    ServerGroups* nElement = new ServerGroups;
    
    nElement->me = toadd;
    nElement->next = toRemove;
    toRemove = nElement;
    number++;

    return outCome;
}

void ToDestroy::purge ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ToDestroy::purge ()\n" << flush;
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
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerView::ServerView (int& res)" << endl;
#endif
    
    AtomicAction A;

    A.Begin();

    head = 0;
    numberofservers = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

ServerView::ServerView (const Uid& u, int& res) : LockManager(u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerView::ServerView (const Uid& u, int& res)" << endl;
#endif

    AtomicAction A;

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
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerView::~ServerView ()" << endl;
#endif
    
    terminate();

    deleteList.purge();

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

ServerList* ServerView::createServerList (const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList* ServerView::createServerList ( " << u << " )" << endl;
#endif

    int err = -1;
    ServerList* slPtr = ServerList::create(u, err);

    if (err != 0)
    {
        if (slPtr)
	    ServerList::remove(slPtr);
    }

    return slPtr;
}

void ServerView::storeName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ServerView::storeName (Boolean& done, Buffer key_buff, Buffer data_buff)\n" << flush;
#endif

    AtomicAction A;
    ServerGroups* ptr = 0;
    int res = -1;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        Boolean found = FALSE;
	ptr = head;
	    
	for (int k = 0; ((k < numberofservers) && (!found)); k++)
	{
	    if (ptr->me == 0)
	        ptr->me = createServerList(*ptr->serverListUid);

	    if (ptr->me->compareBuffers(key_buff, v_number))
	    {
	        found = TRUE;
		result = TRUE;
		break;
	    }

	    ptr = ptr->next;
	}

	if (!found)
	{
	    ptr = new ServerGroups;
	    ptr->me = ServerList::create(res);
	    ptr->serverListUid = new Uid(ptr->me->get_uid());
	    ptr->next = head;
	    head = ptr;
	    numberofservers++;

	    if (res == 0)
	    {
	        if (head->me->storeName(key_buff, data_buff, v_number))
		    result = TRUE;
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

    done = result;
}

void ServerView::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ServerView::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff)\n" << flush;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    int i = 0;
    ServerGroups* ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofservers > 0)
	{
	    ptr = head;
	    while (i < numberofservers)
	    {
	        if (ptr->me == 0)
		    ptr->me = createServerList(*ptr->serverListUid);

		if (ptr->me->compareBuffers(key_buff, v_number))
		{
		    result = ptr->me->fetchName(data_buff, v_number);
		    break;
		}

		i++;
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
    
    done = result;
}

void ServerView::removeName (Boolean& done, Buffer key_buff, int& left, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ServerView::removeName (Boolean& done, Buffer key_buff, int& left)" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;
    int i = 0;
    ServerGroups *ptr = 0, *indx = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (numberofservers > 0)
	{
	    Boolean found = FALSE;

	    ptr = head;
	    while ((i < numberofservers) && (!found))
	    {
	        if (ptr->me == 0)
		    ptr->me = createServerList(*ptr->serverListUid);

		found = ptr->me->compareBuffers(key_buff, v_number);

		if (found)
		{
		    deleteList.add(ptr->me);

		    if (ptr == head)
			head = ptr->next;
		    else
			indx->next = ptr->next;

		    numberofservers--;
		    left = numberofservers;
		    result = TRUE;
		}
		else
		{
		    i++;
		    indx = ptr;
		    ptr = ptr->next;
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
    
    done = result;
}

void ServerView::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ServerView::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff)" << endl;
#endif
    
    AtomicAction A;
    int i = 0;
    Boolean result = FALSE;
    ServerGroups *ptr = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberofservers > 0)
	{
	    ptr = head;
	    while (i < numberofservers)
	    {
	        if (ptr->me == 0)
		    ptr->me = createServerList(*ptr->serverListUid);

		if (ptr->me->compareBuffers(key_buff, v_number))
		{
		    result = ptr->me->replaceName(data_buff, v_number);
		    break;
		}

		i++;
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
    
    done = result;
}

void ServerView::getNextName (Boolean& done, Buffer& key_buff, int& position, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerView::getNextName (Boolean& done, Buffer& key_buff, int& position)\n" << flush;
#endif

    AtomicAction B;
    Boolean result = FALSE;
    ServerGroups* ptr = 0;

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if ((numberofservers == 0) || (position < 0) || (position >= numberofservers))
	    position = -1;
	else
	{
	    ptr = head;
	    for (int i = 0; i < position; i++, ptr = ptr->next);
	
	    if (ptr->me == 0)
	        ptr->me = createServerList(*ptr->serverListUid);

	    result = ptr->me->getKey(key_buff, v_number);

	    if (result)
		position++;
	}
    }
    
    if (result)
    {
	if (B.End() != COMMITTED)
	    result = FALSE;
    }
    else
	B.Abort();
    
    done = result;
}

Boolean ServerView::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerView::save_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;
    ServerGroups* ptr = 0;

    res = os.pack(numberofservers);

    ptr = head;
    for (int i = 0; (i < numberofservers) && res; i++)
    {
	res = ptr->serverListUid->pack(os);
	ptr = ptr->next;
    }
    
    return res;
}

Boolean ServerView::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerView::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;
    int currentNumber;
    ServerGroups *ptr = 0, *indx = 0;

    currentNumber = numberofservers;
    
    res = os.unpack(numberofservers);

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
	if ((numberofservers == 0) && (currentNumber > 0))
	    deleteList.purge();
	else
	{
	    for (i = 0; (i < numberofservers) && res; i++)
	    {
	        indx = new ServerGroups;
		indx->serverListUid = new Uid(NIL_UID);
		res = indx->serverListUid->unpack(os);

		if (res)
		    indx->me = deleteList.retrieve(*indx->serverListUid);

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

const TypeName ServerView::type () const
{
    return "/StateManager/LockManager/ServerView";
}
