/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StackDriver.cc,v 1.2 1997/10/02 12:59:11 nmcl Exp $
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
    ClientRpc* cRPC = (ClientRpc*) 0;
    ClientRpcManager* rpcC = (ClientRpcManager*) 0;
    char* hostname = (char*) 0;
    Uid u("80f09608:7214:2d6a1bae:8");
    Uid objID(NIL_UID);

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostname = argv[i+1];
	if (::strcmp(argv[i], "-uid") == 0)
	{
	    Uid tmpU(argv[i+1]);
	    objID = tmpU;
	}
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -host <host name> [-uid <uid>] [-help]" << endl;
	    return 0;
	}
    }

    if (hostname == (char*) 0)
    {
	cout << "Error. Hostname not given." << endl;
	return -1;
    }

    cRPC = new ClientRpc("Stack");
    (cRPC->control()).setHost(hostname);
    (cRPC->control()).setProtocol(STREAM);
    (cRPC->control()).setServerID(u);      // shared server id.
    (cRPC->control()).setObjectUid(objID);
    rpcC = new ClientRpcManager(cRPC);

    Stack *Use = new Stack(res, rpcC);
    
    if (!res)
    {
	cerr << "Constructor error" << endl;
    }
    else
	cout << "Server started ok" << endl;

    Boolean finished = FALSE;

    while (!finished)
    {
	char option;
	int value = 0;

	cout << endl;
	
	cout << "1: push" << endl;
	cout << "2: pop" << endl;
	cout << "3: quit" << endl;

	cin >> option;

	cout << endl;
	
	switch (option)
	{
	case '1':
	    {
		cout << "What is the value?" << endl;
		cin >> value;

		if (Use->push(value))
		    cout << "Value pushed ok" << endl;
		else
		    cout << "Push error" << endl;
	    }
	break;
	case '2':
	    {
		if (Use->pop(value))
		    cout << "Popped " << value << endl;
		else
		    cout << "Pop error" << endl;
	    }
	break;
	case '3':
	    finished = TRUE;
	    break;
	}
    }

    delete Use;
    delete rpcC;
    delete cRPC;

    return 0;
}

