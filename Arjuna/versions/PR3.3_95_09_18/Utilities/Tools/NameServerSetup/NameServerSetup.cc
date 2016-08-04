/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServerSetup.cc,v 1.3 1994/10/10 11:14:03 ngdp Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef NAMESERV_H_
#  include <ArjServers/NameServ_stub.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif



int main (int argc, char** argv)
{
    ClientRpc* CRPC = (ClientRpc*) 0;
    ClientRpcManager* rpcMan = (ClientRpcManager*) 0;
    NameServer* nameServ = (NameServer*) 0;
    char* host = (char*) 0;
    int res = -1;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: NameServerSetup [-host <host>]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-host") == 0)
	    host = argv[i+1];
    }

    if (host)
    {
	CRPC = new ClientRpc("NameServer");
	CRPC->setHost(host);
	rpcMan = new ClientRpcManager(CRPC);
	nameServ = new NameServer(res, rpcMan);
    }
    else
	nameServ = new NameServer(res);

    if (res == 0)
	cout << "NameServer " << nameServ->get_uid() << " successfully created." << endl;
    else
	cout << "An error occurred while trying to create NameServer." << endl;

    if (nameServ)
	delete nameServ;

    if (rpcMan)
	delete rpcMan;

    if (CRPC)
	delete CRPC;

    return 0;
}
