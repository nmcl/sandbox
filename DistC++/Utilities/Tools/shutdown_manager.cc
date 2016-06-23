/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: shutdown_manager.cc,v 1.1 1998/11/20 08:29:00 nmcl Exp $
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

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
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

    if (p)
	port = (unsigned short) atoi(p);
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    host = argv[i+1];
	if (::strcmp(argv[i], "-port") == 0)
	    port = atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: shutdown_manager [-host <host_name>] [-port <manager_port>]" << endl;
	    return 1;
	}
    }
    
    if (host == (char*) 0)
    {
	host = ::new char[MAXHOSTNAMELEN+1];
	::memset(host, '\0', MAXHOSTNAMELEN+1);
	::gethostname(host, MAXHOSTNAMELEN);
    }

    PortAddress managerGlobalAddr(host, port);
    Port managerPort;
    Buffer* work[1];
    Buffer workBuf;

    workBuf.pack(MANAGER_DIE);

    work[0] = &workBuf;

    if (managerPort.sendMessage(managerGlobalAddr, work, 1) == Port::DONE)
    {
	cout << "Send to manager successful" << endl;
	
	PortAddress from;
	Buffer* reply[1];
	Buffer replyBuf;
	int rNumber = 1;
	int terminated = 0;
	
	reply[0] = &replyBuf;
	
	if (managerPort.receiveMessage(from, reply, rNumber, -1) == Port::DONE)
	{
	    replyBuf.unpack(terminated);

	    if (terminated)
		cout << "Received acknowledgment from manager." << endl;
	    else
	    {
		cerr << "Received unknown reply from manager." << endl;
		return -3;
	    }
	}
	else
	{
	    cerr << "Did not receive acknowledgment from manager." << endl;

	    return -1;
	}
    }
    else
    {
	cerr << "Send to manager failed" << endl;
	return -2;
    }
    
    return 0;
}

    
    
