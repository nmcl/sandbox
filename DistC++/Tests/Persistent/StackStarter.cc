/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StackStarter.cc,v 1.3 1998/02/06 12:10:25 nmcl Exp $
 */

#include <Config/Configure.h>

#include <System/unistd.h>

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#include "Stack_stub.h"

int main (int argc, char** argv)
{
    Boolean res = FALSE;
    ClientRpc* cRPC1 = (ClientRpc*) 0;
    ClientRpcManager* rpcC1 = (ClientRpcManager*) 0;
    ClientRpc* cRPC2 = (ClientRpc*) 0;
    ClientRpcManager* rpcC2 = (ClientRpcManager*) 0;
    char* hostname = (char*) 0;
    Uid u("80f09608:7214:2d6a1bae:8");

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostname = argv[i+1];
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -host <host name> [-help]" << endl;
	    return 0;
	}
    }

    if (hostname == (char*) 0)
    {
	cout << "Error. Hostname not given." << endl;
	return -1;
    }

    cRPC1 = new ClientRpc("Stack");
    (cRPC1->control()).setHost(hostname);
    (cRPC1->control()).setProtocol(STREAM);
    (cRPC1->control()).setServerID(u);      // shared server id.
    rpcC1 = new ClientRpcManager(cRPC1);
    
    Stack* Use1 = new Stack(res, rpcC1);
    
    if (!res)
    {
	cerr << "Constructor error" << endl;
    }
    else
	cout << "Server started ok" << endl;

    Uid objID(NIL_UID);

    Use1->rpcManager().getRpcHandle()->control().getObjectUid(objID);
    
    cout << "object 1 id: " << objID << endl;

    cRPC2 = new ClientRpc("Stack");
    (cRPC2->control()).setHost(hostname);
    (cRPC2->control()).setProtocol(STREAM);
    (cRPC2->control()).setServerID(u);      // shared server id.
    rpcC2 = new ClientRpcManager(cRPC2);

    res = FALSE;
    
    Stack* Use2 = new Stack(res, rpcC2);
    
    if (!res)
    {
	cerr << "Constructor error" << endl;
    }
    else
	cout << "Server started ok" << endl;

    Use2->rpcManager().getRpcHandle()->control().getObjectUid(objID);
    
    cout << "object 2 id: " << objID << endl;    
    
    /*
     * To leave the server running, we simply don't call
     * the destructor - hence create it on the heap!
     */
    
    return 0;
}

