/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: pingdaemon.cc,v 1.1 1998/11/20 08:28:45 nmcl Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#include <System/iostream.h>
#include <System/fstream.h>
#include <System/stdio.h>
#include <System/unistd.h>
#include <System/stdlib.h>

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef PING_H_
#  include <RPC/SimpleRpc/Ping.h>
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

int main (int argc, char** argv)
{
    if (chdir("/tmp"))
	cerr << "pingdaemon: failed to chdir to /tmp" << endl;

    unsigned short port = PING_SOCKET;
    Boolean debugOn = FALSE;
#ifndef hpux
    int daemonPid = getpid();
#else
    pid_t daemonPid = getpid();
#endif    
    char outputFile[128], errorFile[128];

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-d") == 0)
	    debugOn = TRUE;
	if (::strcmp(argv[i], "-port") == 0)
	    port = (unsigned short) ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: pingdaemon [-d] [-port <port number>] [-help]" << endl;
	    return 0;
	}
    }
    
    sprintf(outputFile, "pingdaemon_out_%d", daemonPid);
    sprintf(errorFile, "pingdaemon_err_%d", daemonPid);

    ofstream target1(outputFile);
    if (!target1)
	printf("pingdaemon: Cannot open file %s\n", outputFile);
    else
    {
	if (debugOn)
	    cout = target1;
	else
	{
	    target1.close();
	    ::unlink(outputFile);
	}
    }
    
    ofstream target2(errorFile);
    if (!target2)
	printf("pingdaemon: Cannot open file %s\n", errorFile);
    else
    {
	if (debugOn)
	    cerr = target2;
	else
	{
	    target2.close();
	    ::unlink(errorFile);
	}
    }

    PortAddress myAddr(port);
    PortAddress from;
    Port myPort(myAddr);
    
    for (;;)
    {
	Buffer* work[1];
	Buffer request;
	int workN = 1;
	
	work[0] = &request;
	
	if (myPort.receiveMessage(from, work, workN) == Port::DONE)
	{
	    int opcode;
	    Buffer* response[1];
	    Buffer reply;
	    Boolean terminate = FALSE;
	    
	    response[0] = &reply;

	    request.unpack(opcode);

	    switch (opcode)
	    {
	    case TERMINATE_CODE:
		{
		    reply.pack(TRUE);
		    terminate = TRUE;
		}
	    break;
	    case ARE_YOU_ALIVE:
		{
		    reply.pack(TRUE);
		}
	    break;
	    default:
		{
		    error_stream << WARNING
				 << "pingdaemon: received unknown opcode " << opcode << endl;
		}
	    break;
	    };

	    if (myPort.sendMessage(from, response, 1) == Port::DONE)
	    {
		if (terminate)
		    return 0;
	    }
	    else
		error_stream << WARNING << "pingdaemon: could not respond to " << from << endl;
	}
    }

    return 0;
}

	    
		
