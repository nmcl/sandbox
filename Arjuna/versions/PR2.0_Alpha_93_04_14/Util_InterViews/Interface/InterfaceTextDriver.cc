/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InterfaceTextDriver.cc,v 1.9 1993/03/18 15:46:04 nsmw Exp $
 */

#include <Configure.h>

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#include <System/fstream.h>
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

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef CLIENTRPC_H_
#include <StubGen/ClientRpc.h>
#endif

#ifndef RPCCONTROL_H_
#include <StubGen/RpcControl.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#define _INIT_
#ifndef INTERFACEARGS_H_
#include "InterfaceArgs.h"
#endif

#include "Interface_stub.h"
#include "Transporter_stub.h"

class list
{
public:
    list ();
    ~list ();

    State* me;
    list *prev, *next;
};

list::list ()
{
    prev = 0;
    next = 0;
    me = 0;
}

list::~list () {}

list *head = 0, *ptr = 0;

void free_list ()
{
    if (head != 0)
    {
	ptr = head;
	while (ptr->next != 0)
	{
	    ptr = ptr->next;
	    delete ptr->prev->me;
	    delete ptr->prev;
	}

	delete ptr->me;
	delete ptr;
    }

    head = 0;
    ptr = head;
}

void insert (State elem)
{
    if (head == 0)
    {
	head = new list;
	head->me = new State(elem);
	ptr = head;
	ptr->next = 0;
	ptr->prev = 0;
    }
    else
    {
	ptr->next = new list;
	ptr->next->me = new State(elem);
	ptr->next->prev = ptr;
	ptr = ptr->next;
	ptr->next = 0;
    }
}

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    if (!InterfaceArgs::CheckArgs(argc, argv, FALSE))
    {
	cerr << "Usage: InterfaceDriver [-uid <database UID>]\n" << flush;
	cerr << "                       [-name <database hostname]\n" << flush;
	exit(0);
    }

    State a;
    char what[1024], name[20], where[100], placement[100], dummy[2];
    int i = 0, position = 0, select, number, res;
    Boolean temp;
    Interface *itf = 0;

    ArjunaName ArjName;
    ArjName.SetServiceName("Interface");
    if (Definitions::DatabaseLocation)
	ArjName.SetHostName(Definitions::DatabaseLocation);

    ClientRpc *crpc = new ClientRpc(&ArjName);
    RpcControl *rpcc = new RpcControl(crpc);

    res = 0;
    if (Definitions::DatabaseUID)
    {
	Uid x(Definitions::DatabaseUID);
	itf =  new Interface(x, res, rpcc);
    }
    else
    {
	itf = new Interface(res, rpcc);
    }

    if (res != 1)
    {
	cerr << "Constructor error\n";
	if (itf)
	    delete itf;
	exit(0);
    }

    do
    {
	memset(what, '\0', 1024);
	memset(name, '\0', 20);
	memset(where, '\0', 100);
	memset(placement, '\0', 100);

	cout << "1: Add entry to list.\n" << flush;
	cout << "2: Inspect list.\n" << flush;
	cout << "3: Get interface.\n" << flush;
	cout << "4: Delete interface.\n" << flush;
	cout << "5: Quit.\n" << flush;
	do
	{
	    cin >> select;
	    if (select == 2)
		temp = TRUE;
	    else
		temp = FALSE;
	} while ((select < 1) || (select > 5));
	if (!temp)
	    cin.getline(dummy, 2);

	switch (select)
	{
	case 1: cout << "What is the interface description?\n";
		cin.getline(what, 200);
		cout << "What is the hostname for the server?\n";
		cin.getline(name, 20);
		cout << "What is the location of the interface?\n";
		cin.getline(where, 200);
		{
		    Buffer buff;

		    buff.pack(what);
		    buff.pack(name);
		    buff.pack(where);
		    buff.pack("dummy");
		    buff.pack(0);

		    AtomicAction Z;
		    Boolean result = FALSE;
	
		    Z.Begin();

		    res = -1;
		    result = itf->Add(buff, res);

		    if (res != 0)
		    {
			cerr << "Addition error.\n" << flush;
			Z.Abort();
		    }
		    else
		    {
			if (!result)
			{
			    cerr << "Addition error.\n" << flush;
			    Z.Abort();
			}
			else
			    if (Z.End() != COMMITTED)
				cerr << "Z.End() : Not Done\n";
		    }
	        }
		break;
	    case 2:
		free_list();
		cout << "\n\n" << flush;
		number = 1;
		Boolean result;
		do
		{
		    AtomicAction Y;
		    result = FALSE;
		    Buffer buff;
		    char *interfaces = 0, *hostname = 0, *pathname = 0, *servers = 0;
		    int u_id;

		    a.Initialize();

		    Y.Begin();

		    res = -1;

		    result = itf->Inspect(buff, position, res);
		    if (res == -1)
		    {
			cerr << "Inspection error.\n" << flush;
			Y.Abort();        // lock conflict
			break;
		    }

		    if (!result)
		    {
			cerr << "End of list\n" << flush;
			Y.Abort();
			break;
		    }

		    buff.unpack(interfaces);
		    buff.unpack(hostname);
		    buff.unpack(pathname);
		    buff.unpack(servers);
		    buff.unpack(u_id);
		    a.Set_State(interfaces, hostname, pathname, servers, u_id);
		    insert(a);
		    if (Y.End() != COMMITTED)
			cerr << "Y.End() : Not Done\n";

		    cout << "" << position << ": On machine " << a.hostname << " there is:\n";
		    cout << a.interfaces << "\n\n";
		    cout << "\n\nLocated at " << a.pathname << "\n" << flush;
		} while (res == 0);

		position = 0;
		break;
	    case 3:
		if (head == 0)
		{
		    cerr << "Error: you have not inspected the list yet.\n";
		    break;
		}

		cout << "Which number?\n";
		cin >> number;		
		cin.getline(dummy, 2);
		cout << "Where do you want the file placed?\n";
		cin.getline(placement, 100);
	        {
		    ofstream outFile(placement, ios::out);
		    if (!outFile)
		    {
			cerr << "Cannot open " << placement << "\n";
			break;
		    }

		    list *marker = head;
		    for (int k = 0; ((k < number-1) && (marker != 0)); k++, marker = marker->next);

		    ArjunaName AN;
		    AN.SetServiceName("Transporter");
		    AN.SetHostName(marker->me->hostname);

		    ClientRpc *RPCClient = new ClientRpc(&AN);
		    RpcControl *RPCCont = new RpcControl(RPCClient);

		    res = 0;
		    Transporter T(res, RPCCont);
		    if (res == 0)
		    {
			cerr << "Constructor error.\n";
			break;
		    }

		    Buffer buff1, buff2;
		    res = -1;

		    buff1.pack(marker->me->pathname);
		    buff2 = T.Get(buff1, res);

		    if (res != 0)
		    {
			cerr << "Transporter::Get error.\n";
			break;
		    }
		    else
		    {
			char* temp = 0;
			buff2.unpack(temp);
			outFile << temp;
			delete temp;
		    }
		}
		break;
	    case 4:
	    {
		if (head == 0)
		{
		    cerr << "Error: you have not inspected the list yet.\n";
		    break;
		}

		cout << "Which number?\n";
		cin >> number;		
		cin.getline(dummy, 2);

		Buffer buff;
		list *marker = head;
		AtomicAction B;
		Boolean result;

		for (int k = 0; ((k < number-1) && (marker != 0)); k++, marker = marker->next);

		buff.pack(marker->me->interfaces);
		buff.pack(marker->me->hostname);
		buff.pack(marker->me->pathname);
		buff.pack(marker->me->servers);
		buff.pack(marker->me->u_id);

		res = -1;

		B.Begin();
		
		result = itf->Delete(number, buff, res);

		if (res != 0)
		{
		    cerr << "Error. Could not delete interface.\n" << flush;
		    B.Abort();
		    break;
		}
		else
		{
		    if (!result)
		    {
			cerr << "Error. Could not delete interface. Possible lock conflict.\n" << flush;
			B.Abort();
			break;
		    }
		    else
		    {
			if (B.End() != COMMITTED)
			{
			    cerr << "B.End() : Not Done.\n" << flush;
			    break;
			}
			else
			    cout << "Interface deleted.\n" << flush;
		    }
		}
	    }
	    default: break;
	    }
    } while (select != 5);

    if (itf)
	delete itf;

}

