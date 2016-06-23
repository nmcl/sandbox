/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StackDriver.cc,v 1.1 1997/09/25 15:32:08 nmcl Exp $
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

#include <fstream.h>

#include "cRBC_stub.h"


cRBC*   rbc1;
cRBC*   rbc2;

void initProcess()
{
  ifstream configFile( "config.dat");

    char objectName[255];
    char serviceName[255];
    char host[255];
    char uidString[255];
    Uid objUid( NIL_UID);
    Uid serverUid( NIL_UID);
    ProtocolType p;

    Boolean res = FALSE;
    ClientRpc* cRPC = (ClientRpc*) 0;
    ClientRpcManager* rpcC = (ClientRpcManager*) 0;

    configFile >> objectName;
    do {
	res = FALSE;
	
      cout << "object name " << objectName << endl;

      configFile >> serviceName;
      cout << "service name " << serviceName << endl;
      cRPC = new ClientRpc( serviceName);
	
      configFile >> host;
      cout << "host " << host << endl;
      (cRPC->control()).setHost( host);
	
      {
	configFile >> uidString;
	Uid u(uidString);
	objUid = u;
	cout << "object uid " << objUid << endl;
	(cRPC->control()).setObjectUid( objUid);
      }
      
      {
	configFile >> uidString;
	Uid u( uidString);
	serverUid = u;
	cout << "server uid " << serverUid << endl;
	(cRPC->control()).setServerID( serverUid);
      }
	
      configFile >> uidString;
      p = (ProtocolType)atoi( uidString);
      cout << "protocol " << p << endl;
      (cRPC->control()).setProtocol(p);

      // Create the client manager
      rpcC = new ClientRpcManager(cRPC);

      // create the server object
      if( strcmp( objectName, "rbc1") == 0)
	{
	cout << "creating rbc1\n";
	rbc1 = new cRBC(res, rpcC);
	}
      else if( strcmp( objectName, "rbc2") == 0)
	{
	cout << "creating rbc2\n";
	rbc2 = new cRBC(res, rpcC);
	}
      else cerr << "Unknown object " << objectName << endl;

      if (!res)
	{
	  cerr << "Constructor error" << endl << endl << endl;
	}
      else
	cout << "Server started ok" << endl << endl << endl;
	
      //delete rpcC;
      //delete cRPC;
      configFile >> objectName;
    } while( !configFile.eof());
}

int main (int argc, char** argv)
{
    initProcess();

    // --------------------------------- Start

    Boolean finished = FALSE;

    while (!finished)
    {
	char option;
	int value = 0;

	cout << endl;
	

	cout << "0: QUIT" << endl;
        cout << "1: rbc1::getStatus" << endl;
	cout << "2: rbc1::setStatus" << endl;
        cout << "3: rbc2::getStatus" << endl;
	cout << "4: rbc2::setStatus" << endl;

	cin >> option;

	cout << endl;
	
	switch (option)
	{
	case '1':
	    {
	      cout << "rbc1::status = " << rbc1->getStatus() << endl;
	    }
	break;
	case '2':
	    {
	      cout << "New status ?" << endl;
	      cin >> value;
	      rbc1->setStatus( value);
	      cout << " status = " << rbc1->getStatus() << endl;
	    }
	break;
	case '3':
	    {
	      cout << "rbc2::status = " << rbc2->getStatus() << endl;
	    }
	break;
	case '4':
	    {
	      cout << "New status ?" << endl;
	      cin >> value;
	      rbc2->setStatus( value);
	      cout << " rbc2::status = " << rbc2->getStatus() << endl;
	    }
	break;

	case '0':
	    finished = TRUE;
	    break;
	}
    }
    // --------------------------------- End

    //delete rbc1;
    //delete rbc2;

    return 0;
}

