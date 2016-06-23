/*
 * Copyright 1992, 1993-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Manager_main.cc,v 1.4 1998/11/20 08:28:45 nmcl Exp $
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

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
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

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef MANAGER_H_
#  include "Manager.h"
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

char* path[MAX_SEARCHPATH+1];
Manager* manager = 0;

Boolean singleUserMode = FALSE;

#ifdef HAVE_INTSIG
int terminateManager (int dummy DOTDOTDOT)
#else
void terminateManager (int dummy DOTDOTDOT)
#endif
{
    if (manager)
	delete manager;

    ::exit(0);
}

#ifdef HAVE_INTSIG
int managerMultiOn (int dummy DOTDOTDOT)
#else
void managerMultiOn (int dummy DOTDOTDOT)
#endif
{
    singleUserMode = FALSE;

    ::signal(SIGUSR1, managerMultiOn);
}

#ifdef HAVE_INTSIG
int managerMultiOff (int dummy DOTDOTDOT)
#else
void managerMultiOff (int dummy DOTDOTDOT)
#endif
{
    singleUserMode = TRUE;

    ::signal(SIGUSR2, managerMultiOff);    
}


void generatePath (const char* data)
{
    char* copyString = ::new char[::strlen(data)+1];
    char* ptr1 = copyString;
    Boolean finished = FALSE;
    int index = 0;

    ::strcpy(copyString, data);
    char* ptr2 = ::strtok(ptr1, PATH_DELIM);
    
    do
    {
	if (ptr2)
	{
	    path[index] = ::new char[::strlen(ptr1)+1];
	    ::strcpy(path[index], ptr1);

	    index++;

	    if (index == MAX_SEARCHPATH)
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

	ptr2 = ::strtok(NULL, PATH_DELIM);

	if (!ptr2)
	    finished = TRUE;
	
    } while (!finished);

    ::delete [] copyString;
}


int main (int argc, char** argv)
{
    unsigned short port = MANAGER_PORT_GLOBAL;

    char* p = ::getenv("MANAGER_ADDR");

    if (p)
	port = (unsigned short) atoi(p);
    
    for (int m = 0; m < MAX_SEARCHPATH+1; m++)
	path[m] = (char*) 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-path <search path>] [-port <port>] [-mode <MULTI | SINGLE>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-port") == 0)
	    port = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-mode") == 0)
	{
	    if (::strcmp(argv[i+1], "SINGLE") == 0)
		singleUserMode = TRUE;
	}
	if (::strcmp(argv[i], "-path") == 0)
	    generatePath(argv[i+1]);
    }

    PortAddress globalPortAddr(port);
    Port globalPort(globalPortAddr);

    if (!globalPort.valid())
    {
	::perror("MANAGER_MAIN: Invalid Global Port ");
	Thread::exit();
    }
    
    if (path[0] == 0)
    {
	char* envp = ::getenv("MANAGER_SEARCH_PATH");
		
	if (envp != 0)
	    generatePath(envp);
	else
	{
	    path[0] = ::new char[::strlen(SERVER_LOCATION_Q)+1];
	    ::strcpy(path[0], SERVER_LOCATION_Q);
	}
    }

    manager = new Manager(path);

    ofstream target(MANPID_FILE_Q);
    if (!target)
	perror("manager failed to open pid file\n");
    else
    {
	target << getpid();
	target.close();
    }
    
    ::signal(SIGHUP, terminateManager);
    ::signal(SIGINT, terminateManager);
    ::signal(SIGQUIT, terminateManager);
    ::signal(SIGABRT, terminateManager);
    
    ::signal(SIGUSR1, managerMultiOn);
    ::signal(SIGUSR2, managerMultiOff);
    
    while (TRUE)
    {
	Buffer      work;
	Buffer* RecBuff[1];
	int number = 1;
	PortAddress clientAddr;

	RecBuff[0] = &work;
    
	if (globalPort.receiveMessage (clientAddr, RecBuff, number) != Port::DONE)
	{
	    ::perror("MANAGER_MAIN: ReceiveMessage ");
	    Thread::exit();
	}
	else
	{
	    int opcode = 0;
	    int numberToSend = 1;
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
		    if (singleUserMode)
			break;
		    
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

		    initiated = manager->Initiate(service, serverAddr, serverID, clientID,
						  seqNo, protocol);

		    results.pack(initiated);
		    serverAddr.pack(results);

		    if (service != 0)
			::delete [] service;
		}
		break;
	    case DO_TERMINATE:
		{
		    if (singleUserMode)
			break;

		    char*       service = 0;
		    Uid         serverID, clientID;
		    long        seqNo;
		
		    work.unpack(service);
		    serverID.unpack(work);
		    clientID.unpack(work);
		    work.unpack(seqNo);

		    manager->Terminate(service, serverID, clientID, seqNo);

		    results.pack(1);

		    if (service != 0)
			::delete [] service;
		}
		break;
	    case MANAGER_DIE:
		{
		    if (singleUserMode)
			break;
		    
		    if (manager)
			delete manager;

		    results.pack(1);

		    globalPort.sendMessage (clientAddr, sendBuff, numberToSend);
		    
		    ::exit(0);
		}
	        break;
	    case BIND_INFO:
		{
		    if (singleUserMode)
			break;
		    
		    Buffer bindInfo;

		    if (manager->processInformation(bindInfo))
		    {
			results.pack(1);
			bindInfo.packInto(results);
		    }
		    else
			results.pack(0);
		}
	    break;
	    case MODE_CHANGE:
		{
		    int mode = 0;
		    Boolean response = TRUE;
		    
		    work.unpack(mode);

		    if (mode == MANAGER_SINGLEUSER)
			singleUserMode = TRUE;
		    else
		    {
			if (mode == MANAGER_MULTIUSER)
			    singleUserMode = FALSE;
			else
			    response = FALSE;
		    }
			
		    results.pack(response);
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
		Thread::exit();
	    }
	}
    }

    for (int j = 0; j < MAX_SEARCHPATH; j++)
	if (path[j])
	    ::delete [] path[j];
    
    if (manager)
	delete manager;

    return 0;
}
