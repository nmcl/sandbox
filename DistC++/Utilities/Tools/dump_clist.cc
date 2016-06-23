/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: dump_clist.cc,v 1.1 1998/11/20 08:28:59 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
#endif

#ifndef PORT_H_
#  include "../../RPC/SimpleRpc/include/Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "../../RPC/SimpleRpc/include/PortAddress.h"
#endif


void printOutput (Buffer& reply)
{
    int count = 0;

    if (reply.unpack(count))
    {
	cout << "There are currently " << count << " active processes." << endl;

	for (int i = 0; i < count; i++)
	{
	    pid_t pid;
	    int useCount;
	    char* service = 0;
	    Uid serverID(NIL_UID);

	    if (reply.unpack(pid) && reply.unpack(useCount) && reply.unpack(service) && serverID.unpack(reply))
	    {
		cout << "\nProcess:    " << i << endl;
		cout << "Name:       " << service << endl;
		cout << "Uid:        " << serverID << endl;
		cout << "Process id: " << pid << endl;
		cout << "Use count:  " << useCount << endl;

		if (service)
		    ::delete [] service;
	    }
	    else
	    {
		error_stream << WARNING << "Error unpacking process information." << endl;
		return;
	    }
	}
    }
    else
	error_stream << WARNING << "Error unpacking count value." << endl;
}

int main (int argc, char** argv)
{
    unsigned short port = MANAGER_PORT_GLOBAL;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-port") == 0)
	    port = (unsigned short) ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-port <port number>] [-help]" << endl;
	    return 0;
	}
    }

    PortAddress managerAddr(port);
    Port managerPort;
    Buffer* work[1];
    Buffer request;

    work[0] = &request;

    request.pack(BIND_INFO);

    if (managerPort.sendMessage(managerAddr, work, 1) == Port::DONE)
    {
	Buffer* response[1];
	Buffer reply;
	int rNumb = 1;
	PortAddress from;
	
	response[0] = &reply;

	if (managerPort.receiveMessage(from, response, rNumb) == Port::DONE)
	{
	    if ((from == managerAddr) && (rNumb == 1))
	    {
		printOutput(reply);
	    }
	    else
		error_stream << WARNING
			     << "Received unrecognised response." << endl;
	}
	else
	    error_stream << WARNING
			 << "Did not received response from manager." << endl;
    }
    else
	error_stream << WARNING
		     << "Could not send request to manager." << endl;

    return 0;
}
