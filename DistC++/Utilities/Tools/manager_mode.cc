/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: manager_mode.cc,v 1.1 1998/11/20 08:29:00 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#   include <System/stdlib.h>
#endif

#ifndef PORT_H_
#  include "../../RPC/SimpleRpc/include/Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "../../RPC/SimpleRpc/include/PortAddress.h"
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
#endif

int main (int argc, char** argv)
{
    unsigned short port = MANAGER_PORT_GLOBAL;
    char* host = (char*) 0;
    char* p = ::getenv("MANAGER_ADDR");
    ManagerModes request_code = MANAGER_MULTIUSER;
    
    if (p)
	port = (unsigned short) atoi(p);
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    host = argv[i+1];
	if (::strcmp(argv[i], "-multiuser") == 0)
	    request_code = MANAGER_MULTIUSER;
	if (::strcmp(argv[i], "-singleuser") == 0)
	    request_code = MANAGER_SINGLEUSER;
	if (::strcmp(argv[i], "-port") == 0)
	    port = atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: manager_mode [-host <host_name>] [-port <manager_port>] [-multiuser] [-singleuser]" << endl;
	    return 1;
	}
    }
    
    if (host == (char*) 0)
    {
	host = ::new char[MAXHOSTNAMELEN+1];
	::memset(host, '\0', MAXHOSTNAMELEN+1);
	::gethostname(host, MAXHOSTNAMELEN);
    }

    PortAddress managerAddr(host, port);
    Port managerPort;
    Buffer* work[1];
    Buffer request;

    request.pack(MODE_CHANGE);
    request.pack(request_code);

    if (managerPort.sendMessage(managerAddr, work, 1) == Port::DONE)
    {
	cout << "Send to manager successful" << endl;

	Buffer* response[1];
	Buffer reply;
	int rNumb = 1;
	PortAddress from;

	response[0] = &reply;

	if ((managerPort.receiveMessage(from, response, rNumb) == Port::DONE) && (from == managerAddr))
	{
	    cout << "Received acknowledgment from manager." << endl;

	    Boolean outcome = FALSE;

	    if (reply.unpack(outcome) && (outcome))
		cout << "Manager now in " << ((request_code == MANAGER_MULTIUSER) ? "multi-user" : "single-user") << " mode." << endl;
	    else
		cout << "Received unrecognised response from manager." << endl;
	}
	else
	    cout << "Did not received response from manager." << endl;
    }
    else
	cout << "Could not send request to manager." << endl;

    return 0;
}
