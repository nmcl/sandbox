/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Register.cc
 */

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef PROTOS_H_
#include <System/protos.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
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

#ifndef NSINTERFACE_H_
#include <RPC/ArjServers/NSInterface.h>
#endif

#ifndef STATEDAEMON_H_
#include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef STATEDAEMON_H_
#include <RPC/ArjServers/NameServ_stub.h>
#endif

#ifndef CONFIGURE_H_
#include <Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


#include <RPC/ArjServers/Register.h>


const int hSize = 400;
const char* bogusName = "bogusMachineName";


ReplicaData::ReplicaData ()
                          : next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaData::ReplicaData ()\n" << flush;
#endif

    hostName = new char[strlen(bogusName)+1];
    ::memset(hostName, '\0', strlen(bogusName)+1);
    ::memcpy(hostName, bogusName, strlen(bogusName));
}

ReplicaData::ReplicaData (const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Replica::ReplicaData (const char* name)" << endl;
#endif

    hostName = new char[strlen(name) +1];
    ::memset(hostName, '\0', strlen(name)+1);
    ::memcpy(hostName, name, strlen(name));
}
    
ReplicaData::~ReplicaData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaData::~ReplicaData ()\n" << flush;
#endif

    if (next)
	delete next;

    if (hostName)
	delete hostName;
}

ReplicaData& ReplicaData::operator= (const char* name)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaData& ReplicaData::operator= (const char* name)" << endl;
#endif

    if (name)
    {
	if (hostName)
	    delete hostName;

	hostName = new char[strlen(name)+1];
	::memset(hostName, '\0', strlen(name)+1);
	::memcpy(hostName, name, strlen(name));
    }
    
    return *this;
}

ReplicaData& ReplicaData::operator= (const ReplicaData& toCopy)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaData& ReplicaData::operator= (const ReplicaData& toCopy)" << endl;
#endif

    if (toCopy.hostName)
    {
	if (hostName)
	    delete hostName;
	
	hostName = new char[strlen(toCopy.hostName) +1];
	::memset(hostName, '\0', strlen(toCopy.hostName) +1);
	::memcpy(hostName, toCopy.hostName, strlen(toCopy.hostName));
    }
    
    return *this;
}



StateData::StateData ()
                      : RpcClient(0),
		        RpcCont(0),
		        SD(0),
			next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "StateData::StateData ()\n" << flush;
#endif
    
    hostName = new char[strlen(bogusName)+1];
    ::memset(hostName, '\0', strlen(bogusName)+1);
    ::memcpy(hostName, bogusName, strlen(bogusName));
}

StateData::~StateData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "StateData::~StateData ()\n" << flush;
#endif

    if (SD)
	delete SD;

    if (hostName)
	delete hostName;

    if (next)
	delete next;
}


Register::Register ()
                    : group_uid(0),
		      register_uid(0),
		      rd(0),
		      tn(0),
		      head(0),
		      NS(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Register::Register ()\n" << flush;
#endif
}

Register::~Register ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Register::~Register ()\n" << flush;
#endif

    if (rd)
	delete rd;

    if (group_uid)
	delete group_uid;

    if (register_uid)
	delete register_uid;

    if (head)
	delete head;

    if (NS)
	delete NS;
}

void Register::RegisterGroupUid (const Uid& gid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void Register::RegisterGroupUid (const Uid& gid)\n" << flush;
#endif

    group_uid = new Uid(gid);
}

void Register::RegisterTypeName (TypeName name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void Register::RegisterTypeName (TypeName name)\n" << flush;
#endif

    tn = &name;
}

void Register::RegisterObject (const Uid& gid, TypeName name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void Register::RegisterObject (const Uid& gid, TypeName name)\n" << flush;
#endif

    register_uid = new Uid(gid);
    tn = &name;
}

Boolean Register::readObjectState (ReplicaData* marker, Buffer& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PRIVATE;
    debug_stream << "Boolean Register::readObjectState (ReplicaData* marker, Buffer& buff)\n" << flush;
#endif

    UNIQUEID rUid;
    Boolean result = FALSE;
    int res = -1;

    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
    head = new StateData;
    head->ArjName.SetObjName("StateDaemon");
    head->ArjName.SetServiceName("StateDaemon");
    head->ArjName.SetHostName(marker->hostName);
    memcpy(head->hostName, marker->hostName, strlen(marker->hostName));
    head->RpcClient = new ClientRpc(&head->ArjName);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    head->RpcClient->SetTimeout(3000);
    head->RpcClient->SetRetry(1);
    head->RpcClient->SetObjectUid(rUid);
#endif
    head->RpcCont = new RpcControl(head->RpcClient);

    head->SD = new RemoteStateDaemon(res, FALSE, head->RpcCont);
    if (res == 0)
    {
	Uid u_id(NIL_UID);
	rd = new ReplicaDescriptor(marker->hostName);
	rd->SetUid(u_id);

	res = -1;
	buff = head->SD->ReadState(*register_uid, *tn, res);
	if (res == 0)
	    result = TRUE;
    }

    return result;
}

RemoteStateDaemon* Register::checkDaemon (char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PRIVATE;
    debug_stream << "RemoteStateDaemon* Register::checkDaemon (char* name)\n" << flush;
#endif

    StateData* Y = head;

    while (Y)
    {
	if (strcmp(Y->hostName, name) == 0)
	    return Y->SD;
	else
	    Y = Y->next;
    }

    return 0;
}

Boolean Register::writeObjectState (ReplicaData* ptr, Buffer buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PRIVATE;
    debug_stream << "Boolean Register::writeObjectState (ReplicaData* ptr, Buffer buff)\n" << flush;
#endif

    UNIQUEID rUid;
    ReplicaDescriptor* marker = rd;
    RemoteStateDaemon* SDtoUse = 0;
    StateData* X = head;
    int res;
    Boolean result = TRUE;

    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
 
    while (ptr != 0)
    {
	SDtoUse = checkDaemon(ptr->hostName);

	if (!SDtoUse)
	{
	    X->next = new StateData;
	    X = X->next;

	    X->ArjName.SetObjName("StateDaemon");
	    X->ArjName.SetServiceName("StateDaemon");
	    X->ArjName.SetHostName(ptr->hostName);
	    memcpy(X->hostName, ptr->hostName, strlen(ptr->hostName));
	    X->RpcClient = new ClientRpc(&X->ArjName);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
	    X->RpcClient->SetTimeout(3000);
	    X->RpcClient->SetRetry(1);
	    X->RpcClient->SetObjectUid(rUid);
#endif
	    X->RpcCont = new RpcControl(X->RpcClient);

	    res = -1;
	    X->SD = new RemoteStateDaemon(res, FALSE, X->RpcCont);

	    if (res == 0)
		SDtoUse = X->SD;
	    else
		SDtoUse = 0;
	}
	
	if (SDtoUse)
	{
	    res = -1;
	    if (SDtoUse->WriteCommitted(*marker->GetUid(), *tn, buff, res))
	    {
		Uid u_id(NIL_UID);
		marker->next = new ReplicaDescriptor(ptr->hostName);
		marker = marker->next;
		marker->SetUid(u_id);
		result = TRUE;
	    }
	}

	ptr = ptr->next;
    }

    return result;
}

Boolean Register::ReplicateState (ReplicaData* toUse)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean Register::ReplicateState (ReplicaData* toUse)\n" << flush;
#endif

    Buffer buff;
    
    if ((register_uid == 0) || (tn == 0))
	return FALSE;

    if (!group_uid)
	group_uid = register_uid;

    if (toUse->next)
    {
	if (rd)
	{
	    delete rd;
	    rd = 0;
	}

	if (!readObjectState(toUse, buff))
	    return FALSE;

	if (!writeObjectState(toUse->next, buff))
	    return FALSE;
    }

    return registerInfo();
}

Boolean Register::registerInfo ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PRIVATE;
    debug_stream << "Boolean Register::registerInfo ()\n" << flush;
#endif

    int done = -1;
    Boolean result = FALSE;

    NS = new NSInterface(done);
    if (done == 0)
	result = NS->Add_Replicas(*rd, *group_uid);
#ifdef DEBUG
    else
    {
	debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
	debug_stream << "Error. Could not create name server\n" << flush;
    }
#endif

    return result;
}
