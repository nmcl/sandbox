/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: UpdateNameServer.cc,v 1.2 1994/10/10 11:14:05 ngdp Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NAMESERV_H_
#  include <ArjServers/NameServ_stub.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
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
    NSInterface* NSI = (NSInterface*) 0;
    char* host = (char*) 0;
    char* uidString = (char*) 0;
    Uid* u = (Uid*) 0;
    int res = -1;
    AtomicAction A;
    Boolean result = FALSE;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: UpdateNameServer -host <host> -uid <uid>" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-host") == 0)
	    host = argv[i+1];
	if (::strcmp(argv[i], "-uid") == 0)
	    uidString = argv[i+1];
    }

    if ((!host) || (!uidString))
    {
	cout << "Error. Usage: UpdateNameServer -host <host> -uid <uid>" << endl;
	return -1;
    }

    u = new Uid(uidString);
    
    CRPC = new ClientRpc("NameServer");
    CRPC->setHost(host);
    rpcMan = new ClientRpcManager(CRPC);
    nameServ = new NameServer(*u, res, rpcMan);

    if (res == 0)
    {
	res = -1;

	A.Begin();

	NSI = new NSInterface(res);
    
	if (res == 0)
	{
	    ObjectState os;

	    if (NSI->getGroupViewState(os))
	    {
		Boolean status = nameServ->imposeNewGroupViewState(os, res);
		if (res == 0)
		{
		    if (status)
		    {
			cout << "Imposed state successfully." << endl;
			result = TRUE;
		    }
		    else
			cout << "An error occurred while imposing state." << endl;
		}
		else
		    cout << "Name server error." << endl;
	    }
	    else
		cout << "NSInterface error." << endl;
	}
	else
	    cout << "An error occurred while creating NSInterface." << endl;
    }
    else
	cout << "An error occurred while creating name server." << endl;

    if (result)
    {
	if (A.End() != COMMITTED)
	    cout << "An error occurred while committing action." << endl;
    }
    else
	A.Abort();
		
    if (nameServ)
	delete nameServ;
    if (rpcMan)
	delete rpcMan;
    if (CRPC)
	delete CRPC;
    if (NSI)
	delete NSI;
    if (u)
	delete u;
    
    return 0;
}
