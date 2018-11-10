/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TheatreDriver.cc,v 1.1 1994/07/01 10:53:47 ngdp Exp $
 */

#include <Config/Configure.h>

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
# include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
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

#ifndef SIMPLERPCCLIENT_H_
#  include <RPC/SimpleRpc/SimpleRpcClient.h>
#endif

#ifndef DISPATCHER_H_
#  include <Dispatcher/Dispatcher.h>
#endif

#ifndef THEATRE_H_
#  include "Theatre.h"
#endif

#define UIDSIZE 256


int main(int argc, char** argv)
{
    Buffer ToSend, Received;
    char* servicename = "ServerMain";
    SimpleRpcClient* service = (SimpleRpcClient*) 0;
    Boolean finished = FALSE;
    char* hostname = (char*) 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostname = argv[i+1];
	if (::strcmp(argv[i], "-service") == 0)
	    servicename = argv[i+1];
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: TheatreDriver -host <host name> [-service <service name>] [-help]" << endl;
	    return 0;
	}
    }

    if (hostname == (char*) 0)
    {
	hostname = ::new char[MAXHOSTNAMELEN+1];
	::memset(hostname, '\0', MAXHOSTNAMELEN+1);
	::gethostname(hostname, MAXHOSTNAMELEN);
    }

    service = new SimpleRpcClient(servicename, hostname);

    if (service->Initiate() != OPER_DONE)
    {
	cerr << "Constructor error\n" << flush;
	delete service;
	exit(0);
    }

    while (!finished)
    {
	char opt;
	Buffer *SendBuff[MAXBUFFERS], *RecBuff[MAXBUFFERS];
	Buffer work;
	Boolean returnCode;
	int numberReceived = -1;
	char uidName[UIDSIZE+1];

	::memset(uidName, '\0', UIDSIZE+1);

	for (int i = 0; i < MAXBUFFERS; i++)
	{
	    SendBuff[i] = (Buffer*) 0;
	    RecBuff[i] = (Buffer*) 0;
	}
	
	cout << "Menu.\n" << endl;
	cout << "a: Create object" << endl;
	cout << "b: Delete object" << endl;
	cout << "c: Reserve seat" << endl;
	cout << "d: Unreserve seat" << endl;
	cout << "e: Inspect seat" << endl;
	cout << "z: Quit" << endl;

	cin >> opt;

	switch (opt)
	{
	case 'a':
	    {
		Uid* dispatcherUid = (Uid*) 0;
		
		cout << "What is the dispatcher UID?" << endl;
		cin >> uidName;

		dispatcherUid = new Uid(uidName);

		work.pack(D_CREATE);
		dispatcherUid->pack(work);

		SendBuff[0] = &work;

		service->Call(SendBuff, 1, RecBuff, numberReceived);

		if (numberReceived > 0)
		{
		    RecBuff[0]->unpack(returnCode);

		    if (returnCode == TRUE)
			cout << "An error occurred during create." << endl;
		    else
		    {
			Uid dispatcherID;

			dispatcherID.unpack(*RecBuff[1]);
			
			cout << "Object created successfully with id " << dispatcherID << endl;
		    }
		}
		else
		    cout << "Error. Received " << numberReceived << " buffers!" << endl;

		delete dispatcherUid;
	    }
	    break;
	case 'c':
	    {
		Uid* objectUid = (Uid*) 0;
		Buffer objectWork;

		cout << "What is the object UID?" << endl;
		cin >> uidName;

		objectUid = new Uid(uidName);

		work.pack(D_USERCODE);
		objectUid->pack(work);
		objectWork.pack(RESERVE);

		SendBuff[0] = &work;
		SendBuff[1] = &objectWork;

		service->Call(SendBuff, 1, RecBuff, numberReceived);

		if (numberReceived > 0)
		{
		    RecBuff[0]->unpack(returnCode);

		    if ((returnCode == TRUE) || (numberReceived < 2))
			cout << "An error occurred during create." << endl;
		    else
		    {
			Boolean reserveCode;

			RecBuff[1]->unpack(reserveCode);

			cout << "Reserve code: " << reserveCode << endl;
		    }
		}
		else
		    cout << "Error. Received " << numberReceived << " buffers!" << endl;
	    }
	    break;
	case 'z':
	    finished = TRUE;
	    break;
	default:
	    break;
	}

	for (i = 0; i < numberReceived; i++)
	{
	    if (RecBuff[i])
	    {
		delete RecBuff[i];
		RecBuff[i] = (Buffer*) 0;
	    }
	}
    }
		
    service->Terminate();
    
    delete service;

    return 0;
}

