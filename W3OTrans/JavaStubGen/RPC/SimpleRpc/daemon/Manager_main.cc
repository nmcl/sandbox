/*
 * Copyright 1992, 1993,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Manager_main.cc,v 1.4 1996/04/03 13:00:39 nmcl Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

#ifndef CLIENTCONTROL_H_
#  include <RPC/SimpleRpc/ClientControl.h>
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MANAGER_H_
#  include "Manager.h"
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#define PATH_DELIM ":"
#define MAX_PATH 10


int main (int argc, char** argv)
{
    char* path[MAX_PATH+1];
    PortAddress globalPortAddr(MANAGER_PORT_GLOBAL);
    Port globalPort(globalPortAddr);

    if (!globalPort.valid())
    {
	::perror("MANAGER_MAIN: Valid Global Port ");
	exit(1);
    }

    for (int m = 0; m < MAX_PATH+1; m++)
	path[m] = (char*) 0;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-path <search path>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-path") == 0)
	{
	    char* ptr1 = ::new char[::strlen(argv[i+1])+1];
	    Boolean finished = FALSE;
	    int index = 0;

	    ::strcpy(ptr1, argv[i+1]);
	    char* ptr2 = ::strtok(ptr1, PATH_DELIM);

	    do
	    {
		ptr2 = ::strtok(NULL, PATH_DELIM);

		if (ptr2)
		{
		    path[index] = ::new char[::strlen(ptr1)+1];
		    ::memcpy(path[index], ptr1, (ptr2-ptr1));

		    index++;

		    if (index == MAX_PATH)
			finished = TRUE;
		    else
			ptr1 = ptr2++;
		}
		else
		{
		    path[index] = ::new char[::strlen(ptr1)+1];
		    ::strcpy(path[index], ptr1);
		    finished = TRUE;
		}
		    
	    } while (!finished);
	}
    }

    if (!path[0])
    {
	path[0] = ::new char[::strlen(SERVER_LOCATION_Q)+1];
	::strcpy(path[0], SERVER_LOCATION_Q);
    }
    
    Manager manager(path);

    while (TRUE)
    {
	Buffer      work;
	Buffer* RecBuff[1];
	int number = -1;
	PortAddress clientAddr;

	RecBuff[0] = &work;
    
	if (globalPort.receiveMessage (clientAddr, RecBuff, number) != Port::DONE)
	{
	    ::perror("MANAGER_MAIN: ReceiveMessage ");
	    exit(1);
	}
	else
	{
	    int    opcode, numberToSend = 1;
	    Buffer results;
	    Buffer* sendBuff[1];

	    sendBuff[0] = &results;

	    work.unpack(opcode);

#ifdef DEBUG
            cout << "**Work from " << clientAddr << " : " << opcode << endl;
#endif

	    switch (opcode)
	    {
                case DO_INITIATE:
 	            {
			char*        service = NULL;
			Uid          serverID, clientID;
			long         seqNo;
			PortAddress  serverAddr;
			Boolean      initiated;
			int          tmpVal = 0;
			ProtocolType protocol;
		
			work.unpack(service);
			serverID.unpack(work);
			clientID.unpack(work);
			work.unpack(seqNo);
			work.unpack(tmpVal);
			protocol = (ProtocolType) tmpVal;

			initiated = manager.Initiate(service, serverAddr, serverID, clientID,
						     seqNo, protocol);

			results.pack(initiated);
			serverAddr.pack(results);

			if (service != 0)
			    ::delete [] service;
		    }
		    break;
		case DO_TERMINATE:
 	            {
			char*       service = 0;
			Uid         serverID, clientID;
			long        seqNo;
		
			work.unpack(service);
			serverID.unpack(work);
			clientID.unpack(work);
			work.unpack(seqNo);

			manager.Terminate(service, serverID, clientID, seqNo);

			results.pack(1);

			if (service != 0)
			    ::delete [] service;
		    }
		    break;
		default:
		    perror("MANAGER_MAIN: opcode switch ");
		    break;
	    }

#ifdef DEBUG
            cout << "**Reply to " << clientAddr << endl;
#endif

	    if (globalPort.sendMessage (clientAddr, sendBuff, numberToSend) != Port::DONE)
	    {
		::perror("MANAGER_MAIN: sendMessage ");
		exit(1);
	    }
	}
    }

    for (int j = 0; j < MAX_PATH; j++)
	if (path[j])
	    ::delete [] path[j];
    
    return 0;
}
