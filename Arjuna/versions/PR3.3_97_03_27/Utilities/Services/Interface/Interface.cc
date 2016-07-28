/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define INTERFACE_SERVER

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#include "Interface_stub.h"

Interface::Interface (int &res) : LockManager(ANDPERSISTENT)
{
    _arjuna_debug->set_debuglevel(FULL_DEBUGGING);

#ifdef DEBUG
    cout << "Interface::Interface(int &res) : LockManager(ANDPERSISTENT)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    number = 0;
    head = 0;
    ptr = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 1;
    }
    else
	A.Abort();
}

Interface::Interface (const Uid& u, int &res) : LockManager(u)
{
    _arjuna_debug->set_debuglevel(FULL_DEBUGGING);

#ifdef DEBUG
    cout << "Interface::Interface(Uid &u, int &res) : LockManager(u)\n" << flush;
#endif

    res = 1;
}

Interface::~Interface ()
{
#ifdef DEBUG
    cout << "Interface::~Interface()\n" << flush;
#endif

    terminate();
}

Boolean Interface::Add (Buffer new_state, int& res)
{
    AtomicAction A;
    char *interfaces, *hostname, *pathname, *servers;
    int u_id;
    State to_add;

#ifdef DEBUG
    cout << "Boolean Interface::Add(State new_state, int& res)\n" << flush;
#endif

    res = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	new_state.unpack(interfaces);
	new_state.unpack(hostname);
	new_state.unpack(pathname);
	new_state.unpack(servers);
	new_state.unpack(u_id);
	to_add.Set_State(interfaces, hostname, pathname, servers, u_id);
	
	if (head == 0)
	{
	    head = new State(to_add);
	    head->u_id = 0;
	    ptr = head;
	}
	else
	{
	    ptr->Add(new State(to_add));
	    ptr = ptr->next;
	    ptr->u_id = ptr->prev->u_id +1;
	}

	number++;
	if (A.End() == COMMITTED)
	    return TRUE;
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

Boolean Interface::Delete (const int position, Buffer new_state, int& res)
{
    int id = 0;
    AtomicAction B;
    State to_delete;
    char *interfaces, *hostname, *pathname, *servers;
    int u_id;

#ifdef DEBUG
    cout << "Boolean Interface::Delete(int position, State to_delete, int& res)\n" << flush;
#endif

    B.Begin();

    res = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (number == 0)
	{
	    B.Abort();
	    return FALSE;
	}

	new_state.unpack(interfaces);
	new_state.unpack(hostname);
	new_state.unpack(pathname);
	new_state.unpack(servers);
	new_state.unpack(u_id);
	to_delete.Set_State(interfaces, hostname, pathname, servers, u_id);

	State *marker = head;

	for (int k = 0; ((k < position -1) && (marker != 0)); k++, marker = marker->next);
	    
	if (marker == 0)
	{
	    B.Abort();
	    return FALSE;
	}

	if (strcmp(marker->interfaces, to_delete.interfaces) != 0)
	{
	    B.Abort();
	    return FALSE;
	}

	if (marker->prev != 0)
	{
	    if (marker->next != 0)
		marker->next->prev = marker->prev->next;
	    marker->prev->next = marker->next;
	}
	else
	    head = marker->next;

	number--;

	marker = head;
	while (marker != 0)
	{
	    marker->u_id = id;
	    marker = marker->next;
	    id++;
	}

	if (B.End() == COMMITTED)
	    return TRUE;
	else
	{
	    B.Abort();
	    return FALSE;
	}
    }
    else
    {
	B.Abort();
	return FALSE;
    }
}

Boolean Interface::Inspect (Buffer& new_state, int &position, int& res)
{
    AtomicAction B;

#ifdef DEBUG
    cout << "Boolean Interface::Inspect(Buffer& new_state, int &position, int& res)\n" << flush;
#endif

    B.Begin();

    res = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (number == 0)
	{
	    B.Abort();
	    return FALSE;
	}

	State *marker = head;

	if ((marker != 0) && (position <= number -1))
	{
	    for (int i = 0; i < position; i++)
		marker = marker->next;
	    
	    position++;
	    if (B.End() == COMMITTED)
	    {
		new_state.packMappedString(marker->interfaces);
		new_state.packMappedString(marker->hostname);
		new_state.packMappedString(marker->pathname);
		new_state.packMappedString(marker->servers);
		new_state.pack(marker->u_id);
		return TRUE;
	    }
	    else
	    {
		B.Abort();
		return FALSE;
	    }
	}
	else
	{
	    B.Abort();
	    return FALSE;
	}
    }
    else
    {
	res = 1;
	B.Abort();
	return FALSE;
    }
}

Boolean Interface::Get (Buffer buff1, Buffer& buff2, int& res)
{
    AtomicAction A;
    char *header_name = 0;

#ifdef DEBUG
    cout << "Boolean Interface::Get (Buffer buff1, Buffer& buff2, int& res)\n" << flush;
#endif

    buff1.unpack(header_name);
    res = 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	State *marker = head;

	while (marker)
	{
	    if (strstr(marker->pathname, header_name))
	    {
		if (A.End() == COMMITTED)
		{
		    buff2.packMappedString(marker->hostname);
		    buff2.packMappedString(marker->pathname);
		    return TRUE;
		}
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

Boolean Interface::save_state (ObjectState &os, ObjectType)
{
    Boolean ok;
    State *marker = head;
    int x = 0;

#ifdef DEBUG
    cout << "Boolean Interface::save_state(ObjectState &os, ObjectType)\n" << flush;
#endif

    ok = os.pack(number);
    if (ok)
    {
	while ((x < number) && (ok))
	{
	    ok = os.packMappedString(marker->interfaces) && os.packMappedString(marker->hostname) &&
		os.packMappedString(marker->pathname) && os.packMappedString(marker->servers);

	    marker = marker->next;
	    x++;
	}
    }

    return ok;
}

Boolean Interface::restore_state (ObjectState &os, ObjectType)
{
    Boolean ok;
    int x = 0;

    if (head)
    {
	ptr = head->next;

	while (ptr)
	{
	    delete head;
	    head = ptr;
	    ptr = ptr->next;
	}

	delete head;
	
	head = 0;
	ptr = 0;
    }

#ifdef DEBUG
    cout << "Boolean Interface::restore_state(ObjectState &os, ObjectType)\n" << flush;
#endif

    ok = os.unpack(number);

    if (ok)
    {
	char *interfaces, *hostname, *pathname, *servers;

	while ((x < number) && (ok))
	{
	    interfaces = 0;
	    hostname = 0;
	    pathname = 0;
	    servers = 0;

	    if (head == 0)
	    {
		head = new State;
		head->u_id = 0;
		ptr = head;
	    }
	    else
	    {
		ptr->Add(new State);
		ptr = ptr->next;
		ptr->u_id = ptr->prev->u_id +1;
	    }

	    ok = os.unpack(interfaces) && os.unpack(hostname) &&
		os.unpack(pathname) && os.unpack(servers);

	    ptr->Set_State(interfaces, hostname, pathname, servers, ptr->u_id);

	    delete interfaces;
	    delete hostname;
	    delete pathname;
	    delete servers;
	    x++;
	}
    }

    return ok;
}

const TypeName Interface::type () const
{
    return "/StateManager/LockManager/Interface";
}
