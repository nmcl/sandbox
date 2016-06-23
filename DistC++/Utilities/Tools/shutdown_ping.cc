/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: shutdown_ping.cc,v 1.1 1998/11/20 08:29:02 nmcl Exp $
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

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifdef SYSV

#ifndef LIMITS_H_
#  include <System/limits.h>
#endif

#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef PING_H_
#  include <RPC/SimpleRpc/Ping.h>
#endif

#ifndef PORT_H_
#  include "../../RPC/SimpleRpc/include/Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "../../RPC/SimpleRpc/include/PortAddress.h"
#endif


int main (int argc, char** argv)
{
    static char* hostName = (char*) 0;
    unsigned short port = PING_SOCKET;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostName = argv[i+1];
	if (::strcmp(argv[i], "-port") == 0)
	    port = (unsigned short) ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: shutdown_ping [-host <host name>] [-port <port number>] [-help]" << endl;
	    return 0;
	}
    }
    
    if (hostName == (char*) 0)
    {
	hostName = ::new char[MAXHOSTNAMELEN+1];
	::memset(hostName, '\0', MAXHOSTNAMELEN+1);
	::gethostname(hostName, MAXHOSTNAMELEN);
    }

    Ping ping(hostName, port);

    if (ping.shutdown())
	cout << "Ping daemon on " << hostName << " successfully shutdown." << endl;
    else
	cerr << "Did not receive acknowledgment from pingdaemon on " << hostName << "." << endl;

    return 0;
}
