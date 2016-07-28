/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GroupsDriver.cc,v 1.9 1995/09/25 15:10:42 nmcl Exp $
 */


#include "BasicServer_stub.h"

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef PRIMARYOBJECT_H_
#  include <Replication/PrimaryObject.h>
#endif


const int numberOfServers = 4;
const char* serviceName = "BasicServer";
const char* dummyServiceName = "NoService";


Buffer Key, Data;

Boolean rpc_handler ( RPC_Status, Int32 )
{
    return FALSE;
}

/*
 * This function simply stores some bogus information in to the name server
 * so we can attempt to use it later.
 */

int setUpDatabase ()
{
    int res = -1;
    AtomicAction A;
    ArjunaName ANKey, ANData[numberOfServers];
    Uid keyUid;
    char hostName[MAXHOSTNAMELEN+1];
    Boolean done = TRUE;

    NSInterface NS(res);
    
    if (res != 0)
    {
	cerr << "Could not open/create name server" << endl;
	return -2;
    }
    
    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    ::gethostname(hostName, MAXHOSTNAMELEN);

    // set up key for database
    ANKey.setObjectName(serviceName);
    ANKey.setObjectUid(keyUid);
    
    for (int i = 0; (i < numberOfServers) && (done); i++)
    {
	ANData[i].setObjectName(serviceName);
	ANData[i].setHostName(hostName);

	if (i != numberOfServers -1)
	    ANData[i].setServiceName(dummyServiceName);  // attempt to simulate an unavailable server
	else
	    ANData[i].setServiceName(serviceName);
	done = ANData[i].pack(Data);
	if (done)
	{
	    done = Data.pack(FALSE);
	    if ((i == 0) && (done))
		Data.pack(numberOfServers);
	}
    }

    if (!done)
    {
	cerr << "An error occurred during packing names." << endl;
	return -3;
    }

    done = ANKey.pack(Key);
    if (!done)
    {
	cerr << "Packing of key failed." << endl;
	return -4;
    }
    
    done = FALSE;
    A.Begin();

    NS.storeName(done, Key, Data);
    if (!done)
    {
	cerr << "storeName operation failed" << endl;
	return -4;
    }
    else
    {
	if (A.End() != COMMITTED)
	{
	    cerr << "Commit of storeName operation failed" << endl;
	    return -5;
	}
	else
	    cout << "storeName operation performed successfully" << endl;
    }

    return 0;
}


int main (int, char**)
{
    int res = -1;
    Boolean created = FALSE;
    PrimaryObject serverGroup(res);
    ArjunaName* AN = (ArjunaName*) 0;
    BasicServer* bs = (BasicServer*) 0;
    ClientRpc* RpcClient = (ClientRpc*) 0;
    ClientRpcManager* rpcCont = (ClientRpcManager*) 0;
    
    if (res != 0)
    {
	cout << "Error - could not set up PrimaryObject." << endl;
	return -1;
    }
	
    res = setUpDatabase();

    setHandler(rpc_handler);
    
    if (res != 0)
	return res;
    
    serverGroup.Begin(); // must call this so we can guarantee we are within an action

    if (!serverGroup.fetchNameList(Key)) // get list to choose primary server from
    {
	cout << "Error - could not get name list" << endl;
	serverGroup.End();
	return -6;
    }

    do
    {
	AN = serverGroup.getPrimary();
	if (AN)
	{
	    cout << "Attempting to create service " << AN->getServiceName() << " on node " << AN->getHostName() << endl;
	    
	    // attempt to start server with this information
	    res = -1;

	    RpcClient = new ClientRpc(AN);
	    rpcCont = new ClientRpcManager(RpcClient);
	    bs = new BasicServer(res, rpcCont);
	    
	    if (res == 0)
	    {
		cout << "Created server successfully." << endl;
		created = TRUE;
	    }
	    else
	    {
	        delete bs;		
	        delete rpcCont;		
	        delete RpcClient;
		
		cout << "Server creation failed." << endl;
	    }
	}
	else
	    cout << "nothing left" << endl;
	
    } while ((AN) && (!created));
    
    if (created)
    {
	if (serverGroup.registerPrimary())
	    cout << "Registered primary successfully." << endl;
	else
	    cout << "Register of primary failed." << endl;
    }
    
    serverGroup.End();
    
    if (bs)
	delete bs;
    if (rpcCont)
	delete rpcCont;
    if (RpcClient)
	delete RpcClient;

    return 0;
}
