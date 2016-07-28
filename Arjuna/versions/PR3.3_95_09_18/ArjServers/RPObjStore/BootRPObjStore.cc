/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: BootRPObjStore.cc,v 1.6 1995/10/02 15:34:57 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif 

#ifndef RPOBJSTORE_H_
#  include <ArjServers/RPObjStore_stub.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

int main(int, char**)
{
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;
#endif
    char hostName[MAXHOSTNAMELEN+1];
    int value = -1;

    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    ::gethostname(hostName, MAXHOSTNAMELEN);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    rUid.host = rUid.sec = rUid.msec = RPOBJSTOREUID;
#endif

    ClientRpc* CRPC = new ClientRpc("RPObjStore");
    CRPC->setHost(hostName);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC->setObjectUid(rUid);
#endif
    ClientRpcManager* RPCC = new ClientRpcManager(CRPC);
    
    RPObjStore* s = new RPObjStore(value, RPCC);
    
    if (value == 0)
    {
#ifdef DEBUG	
	cout << "RPObjStore started ok" << endl;
#endif
    }
    else
    {
	cout << "An error occurred while attempting to start RPObjStore" << endl;
	if (s)
	    delete s;

	if (RPCC)
	    delete RPCC;

	return -1;
    }
    
    return 0;
}

