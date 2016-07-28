/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: BootLockDaemon.cc,v 1.1 1995/03/06 10:23:33 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif 

#ifndef LOCKDAEMONIF_STUB_H_
#  include "LockDaemonIf_stub.h"
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

void usage ()
{
    cout << "usage: BootLockDaemon <host>" << endl;
    exit(-1);
}

static Boolean rpcFailureHandler (RPC_Status, Int32)
{
    return FALSE;
}

int main (int argc, char** argv)
{
    ClientRpcManager *clientManager = 0;
    ClientRpc *clientHandle = 0;
    LockDaemonIf *theDaemon = 0;
    char hostName[MAXHOSTNAMELEN+1];
 
    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);

    /* process arg list */

    if (argc <= 2)
    {
	if (argc == 2)
	{
	    Boolean hostValid = FALSE;
	    
	    /* assume arg is a host name */
	    argv++;
	    
	    if (::strlen(*argv) <= MAXHOSTNAMELEN)
	    {
		struct hostent *hp;
		
		::strcpy(hostName, *argv);
		hp = gethostbyname(hostName);

		if (hp)
		    hostValid = TRUE;
	    }

	    if (!hostValid)
	    {
		cout << "Illegal host name" << *argv << endl;
		usage();
	    }
	}
	else
	    ::gethostname(hostName, MAXHOSTNAMELEN);
    }
    else
	usage();
    

    setHandler(rpcFailureHandler);
    
    clientHandle = new ClientRpc("LockDaemon");

    if (clientHandle)
    {
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
	UNIQUEID rUid;

	rUid.host = rUid.sec = rUid.msec = DAEMONUID;
	
	clientHandle->setObjectUid(rUid);
	
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_)
	clientHandle->disableAtomic();
#endif
#endif

	clientHandle->setHost(hostName);
  	
	clientManager = new ClientRpcManager(clientHandle);

	if (clientManager)
	{
	    Boolean rVal = FALSE;
	    
	    theDaemon = new LockDaemonIf(rVal, clientManager);

	    if (!rVal)			/* check for good start up */
	    {
		cout << "An error occurred while attempting to start the Lock Daemon on " << hostName << endl;
		
		delete theDaemon;
		theDaemon = 0;
		return -1;
	    }
	    else
		cout << "Lock Daemon on " << hostName << " started ok" << endl;
	    
	}
    }

    return 0;
}

