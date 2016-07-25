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
#include <System/sys/param.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef STATEDAEMON_H_
#  include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef ARJNAME_H_
#  include <RPC/ArjServers/ArjName.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef RPCCONTROL_H_
#  include <StubGen/RpcControl.h>
#endif

#include <Configure.h>


#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    UNIQUEID rUid;
    ArjunaName AN;
    char hostName[MAXHOSTNAMELEN];
    Boolean isVolatile = FALSE;
    int value = -1;

    ::memset(hostName, '\0', MAXHOSTNAMELEN);
    ::gethostname(hostName, MAXHOSTNAMELEN);
    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;

    for (int i = 0; i < argc; i++)
    {
	if (strcmp(argv[i], "-h") == 0)
	{
	    cout << "Usage  : BootStateDaemon -type Volatile/Persistent" << endl;
	    return 0;
	}
	
	if (strcmp(argv[i], "-type") == 0)
	{
	    if (strcmp(argv[i+1], "Volatile") == 0)
		isVolatile = TRUE;
	}
    }

    AN.SetObjName("StateDaemon");
    AN.SetServiceName("StateDaemon");
    AN.SetHostName(hostName);
    ClientRpc* CRPC = new ClientRpc(&AN);
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

	return -1;
    }
    
    return 0;
}

