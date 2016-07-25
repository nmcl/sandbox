/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DaemonList.cc,v
 */


#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef RPCCONTROL_H_
#  include <StubGen/RpcControl.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef DAEMONLIST_H_
#  include <RPC/ArjServers/DaemonList.h>
#endif

#ifndef DAEMONDATA_H_
#  include <RPC/ArjServers/DaemonData.h>
#endif

#ifndef MEMBERS_H_
#  include <RPC/ArjServers/Members.h>
#endif

#ifndef STATEDAEMON_H_
#  include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


DaemonList::DaemonList ()
                        : next(0),
			  data(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonList::DaemonList ()\n" << flush;
#endif
}


DaemonList::~DaemonList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonList::~DaemonList ()\n" << flush;
#endif
    
    if (data)
    {
	if (data->memb)
	{
	    if (data->memb->r_state_daemon)
	    {
		delete data->memb->r_state_daemon;

		if (data->memb->RPCCont)
		    delete data->memb->RPCCont;

		if (data->memb->CRPC)
		    delete data->memb->CRPC;
	    }

	    delete data->memb;
	}
    
	if (data->hostName)
#ifndef __GNUG__
	    delete [] data->hostName;
#else
	    delete data->hostName;
#endif

	delete data;
    }

    if (next)
	delete next;
}

Members* DaemonList::get (const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Members* DaemonList::get ( " << name << " )\n" << flush;
#endif

    if ((name == 0) || (data == 0))
	return 0;

    DaemonList* dmarker = this;

    while (dmarker != 0)
    {
	if (dmarker->data->available)
	{
	    if (dmarker->data->memb->CRPC->GetServerStatus() == DEAD)
		dmarker->data->available = FALSE;
	}

	if ((::strcmp(name, dmarker->data->hostName) == 0) && (dmarker->data->available))
	    return dmarker->data->memb;
	else
	   dmarker = dmarker->next;
    }

    return 0;
}


void DaemonList::put (Members* toadd, const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void DaemonList::put ( " << name << " )\n" << flush;
#endif

    DaemonData* adding = new DaemonData;

    adding->memb = toadd;
    adding->hostName = ::new char[::strlen(name)+1];
    ::memset(adding->hostName, '\0', ::strlen(name)+1);
    ::memcpy(adding->hostName, name, ::strlen(name));

    if (data == 0)
	data = adding;
    else
    {
	DaemonList* dmarker = this;
	while (dmarker->next != 0)
	    dmarker = dmarker->next;

	dmarker->next = new DaemonList;
	dmarker->next->data = adding;
    }
}

void DaemonList::remove (const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void DaemonList::remove ( " << name << " )\n" << flush;
#endif

    if ((name) && (data))
    {
	DaemonList *dmarker = this;
	Boolean found = FALSE;

	while ((dmarker != 0) && (!found))
	{
	    if (::strcmp(name, dmarker->data->hostName) == 0)
	    {
		dmarker->data->available = FALSE;
		found = TRUE;
	    }
	    else
		dmarker = dmarker->next;
	}
    }
}
