/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DaemonList.cc,v 1.6 1996/08/21 10:23:46 nmcl Exp $
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

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <ArjServers/RPObjStore_stub.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef DAEMONLIST_H_
#  include <DaemonList.h>
#endif

#ifndef DAEMONDATA_H_
#  include <DaemonData.h>
#endif

#ifndef MEMBERS_H_
#  include <Members.h>
#endif

Boolean (*old_handler) (RPC_Status, Int32);

static Boolean rpc_handler ( RPC_Status, Int32 )
{
#ifdef DEBUG    
    error_stream << WARNING << "DaemonList::rpc_handler - RPObjStore daemon timeout occurred when terminating." << endl;
#endif
    return FALSE;
}

DaemonList::DaemonList ()
		       : next(0),
			 data(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "DaemonList::DaemonList ()\n" << flush;
#endif
}

DaemonList::~DaemonList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "DaemonList::~DaemonList ()\n" << flush;
#endif

    if (data)
    {
#ifdef DEBUG
	debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
	debug_stream << "DaemonList: calling destructor for RPObjStore on machine " << data->hostName << endl;
#endif
	
	if (data->memb)
	{
	    old_handler = setHandler(rpc_handler);
	
	    // if RPObjStore was detected to have failed then do not call terminate

#ifdef FASTTERMINATE
#ifdef DEBUG
	    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
	    debug_stream << "DaemonList: using fast terminate < " << data->memb->CRPC->getServerStatus() << " >" << endl;
#endif	    
	    if ((data->memb->rpobjstore_daemon) &&
		((data->memb->CRPC->getServerStatus() != INOPERATIONAL)) &&
		(data->memb->CRPC->getServerStatus() != UNINITIATED))
#else
#ifdef DEBUG
	    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
	    debug_stream << "DaemonList: using normal terminate" << endl;
#endif	    
	    if (data->memb->rpobjstore_daemon)
#endif
	    {
		delete data->memb->rpobjstore_daemon;

		if (data->memb->RPCCont)
		    delete data->memb->RPCCont;

		if (data->memb->CRPC)
		    delete data->memb->CRPC;
	    }

	    delete data->memb;

	    setHandler(old_handler);
	}

	if (data->hostName)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] data->hostName;
#else
	    ::delete data->hostName;
#endif

	delete data;
    }

    if (next)
	delete next;
}

Members* DaemonList::get (const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Members* DaemonList::get ( " << name << " )\n" << flush;
#endif

    if ((name == (char*) 0) || (data == (DaemonData*) 0))
	return (Members*) 0;

    DaemonList* dmarker = this;

    // do this check multiple times as group may have replicas on same node

    while (dmarker != 0)
    {
	if ((::strcmp(name, dmarker->data->hostName) == 0) && (dmarker->data->available))
	{
	    // recheck the replicas availability

	    if (dmarker->data->memb->CRPC->getServerStatus() != OPERATIONAL)
		dmarker->data->available = FALSE;
	    else
		return dmarker->data->memb;
	}

	dmarker = dmarker->next;
    }

    return (Members*) 0;
}


void DaemonList::put (Members* toadd, const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void DaemonList::put ( " << name << " )\n" << flush;
#endif

    DaemonData* adding = new DaemonData;

    adding->memb = toadd;
    adding->hostName = ::new char[::strlen(name)+1];
    ::memset(adding->hostName, '\0', ::strlen(name)+1);
    ::memcpy(adding->hostName, name, ::strlen(name));

    if (data == (DaemonData*) 0)
	data = adding;
    else
    {
	DaemonList* dmarker = this;
	while (dmarker->next != (DaemonList*) 0)
	    dmarker = dmarker->next;

	dmarker->next = new DaemonList;
	dmarker->next->data = adding;
    }
}

void DaemonList::remove (const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void DaemonList::remove ( " << name << " )\n" << flush;
#endif

    if ((name) && (data))
    {
	DaemonList *dmarker = this;

	while (dmarker != (DaemonList*) 0)
	{
	    if (::strcmp(name, dmarker->data->hostName) == 0)
	    {
		dmarker->data->available = FALSE;
		return;
	    }
	    else
		dmarker = dmarker->next;
	}
    }
}
