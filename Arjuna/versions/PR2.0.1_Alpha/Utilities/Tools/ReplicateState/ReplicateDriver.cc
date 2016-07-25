/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicateDriver.cc,
 */

#include <Configure.h>

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef REGISTER_H_
#  include <Replication/Register.h>
#endif

#ifndef STATEDAEMON_H_
#  include <ArjServers/StateDaemon_stub.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#  ifndef RAJDOOT_RPCUSER_H_
#    include <RPC/Rajdoot/rpcuser.h>
#  endif
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    Register reg;
    Uid *groupUid = 0, *registerUid = 0;
    Uid *replica1 = 0, *replica2 = 0;
    ListData head, *marker;
    char uid[32], tn[MAXPATHLEN];
    char host[MAXHOSTNAMELEN+1], hostName[MAXHOSTNAMELEN+1];
    int number = 0;
    Boolean toCopy = FALSE;

    ::memset(uid, '\0', 32);
    ::memset(tn, '\0', MAXPATHLEN);
    ::memset(host, '\0', MAXHOSTNAMELEN+1);
    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);

    ::gethostname(hostName, MAXHOSTNAMELEN);
    head = hostName;

    for (int i = 0; i < argc; i++)
    {
	if (strcmp(argv[i], "-group") == 0)
	    groupUid = new Uid(argv[i+1]);

	if (strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: ReplicateDriver [-group UID] [-copy UID1 UID2 typename host]\n" << flush;
	    exit(0);
	}

	if (strcmp(argv[i], "-copy") == 0)
	{
	    replica1 = new Uid(argv[i+1]);
	    replica2 = new Uid(argv[i+2]);
	    ::memcpy(tn, argv[i+3], strlen(argv[i+3]));
	    ::memcpy(host, argv[i+4], strlen(argv[i+4]));
	    toCopy = TRUE;
	}
    }

    if (toCopy)
    {
	LocalPersistentObjectStore LocalStore;
	ArjunaName copyTo;
#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
	UNIQUEID rUid;
#endif
	int res = -1;

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
	rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
#endif
	ObjectState* stateToCopy = LocalStore.read_state(*replica1, tn);
	copyTo.SetObjName("StateDaemon");
	copyTo.SetServiceName("StateDaemon");
	copyTo.SetHostName(host);
	
	ClientRpc RpcClient(&copyTo);
#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
	RpcClient.SetTimeout(3000);
	RpcClient.SetRetry(1);
	RpcClient.SetObjectUid(rUid);
#endif
	RpcControl RpcCont(&RpcClient);
	StateDaemon* SD = new StateDaemon(res, FALSE, &RpcCont);
	if (res == 0)
	{
	    Buffer buff;

	    res = -1;
	    stateToCopy->packInto(buff);
	    SD->writeCommitted(*replica2, tn, buff, res);

	    if (res == 0)
		cout << "State copied ok\n" << flush;
	    else
		cout << "State not copied\n" << flush;
	}
	else
	    cout << "Could not access remote object store\n" << flush;

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
/*
 * Do not call destructor
 */
#else
	if (SD)
	    delete SD;
#endif
	exit(0);
    }

    cout << "\nHow many replicas do you require (excluding the original)?\n" << flush;
    cin >> number;

    cout << "\nWhat is the Uid of the object state to be copied?\n";
    cin >> uid;
    registerUid = new Uid(uid);

    cout << "\nWhat is the type name of the object state?\n";
    cin >> tn;

    marker = 0;
    for (int j = 0; j < number; j++)
    {
	if (!marker)
	    marker = &head;

	marker->next = new ListData;
	marker = marker->next;

	cout << "\nWhat is the destination node called?\n";
	::memset(hostName, '\0', MAXHOSTNAMELEN+1);
	cin >> hostName;
	*marker = hostName;
    }

    if (groupUid)
	reg.registerGroupUid(*groupUid);

    reg.registerObject(*registerUid, tn);

    if (!reg.replicateState(&head))
	cout << "\nAn error has occurred.\n" << flush;
    else
	cout << "\nState copied ok\n" << flush;

    return 0;
}

    
