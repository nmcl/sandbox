/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: BootStateDaemon.cc,v 1.7 1993/08/25 14:55:48 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
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

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef STATEDAEMON_H_
#  include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef RPCCONTROL_H_
#  include <StubGen/RpcControl.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif


#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;
#endif
    char hostName[MAXHOSTNAMELEN];
    Boolean isVolatile = FALSE;
    int value = -1;

    ::memset(hostName, '\0', MAXHOSTNAMELEN);
    ::gethostname(hostName, MAXHOSTNAMELEN);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
#endif

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-h") == 0)
	{
	    cout << "Usage  : BootStateDaemon -type Volatile/Persistent" << endl;
	    return 0;
	}
	
	if (::strcmp(argv[i], "-type") == 0)
	{
	    if (::strcmp(argv[i+1], "Volatile") == 0)
	    {
	        /*
	         * Have to do this because of permanent server problem.
		 * Problem will disappear when we have multi-threading.
		 */
	        cout << "Volatile option currently disabled" << endl;
//		isVolatile = TRUE;
		return 0;
	    }
	}
    }

    ClientRpc* CRPC = new ClientRpc("StateDaemon");
    CRPC->SetHost(hostName);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC->SetObjectUid(rUid);
#endif
    RpcControl* RPCC = new RpcControl(CRPC);
    
    StateDaemon* s = new StateDaemon(value, isVolatile, RPCC);
    
    if (value == 0)
	cout << "StateDaemon started ok" << endl;
    else
    {
	cout << "An error occurred while attempting to start StateDaemon" << endl;
	if (s)
	    delete s;

	if (RPCC)
	    delete RPCC;

	return -1;
    }
    
    return 0;
}

