/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AutoInterfaceDriver.cc,v 1.7 1995/02/22 14:56:12 ngdp Exp $
 */

#include <Config/Configure.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef COMMON_H_
#  include "common.h"
#endif

#ifndef INTERFACEARGS_H_
#  include "InterfaceArgs.h"
#endif

#include "Interface_stub.h"
#include "Transporter_stub.h"

int main(int argc, char** argv)
{
    if (!InterfaceArgs::CheckArgs(argc, argv))
    {
	cerr << "Usage: AutoInterfaceDriver -iname <interface name>\n" << flush;
	cerr << "                           [-uid <database UID>]\n" << flush;
	cerr << "                           [-name <database hostname]\n" << flush;
	return 1;
    }

    AtomicAction A;
    Buffer buff1, buff2, buff3, buff4;
    int i = 0, res;
    char *hostname = 0, *pathname = 0, *contents1 = 0;
    char c, contents2[filesize];
    Boolean opcode;
    Interface *itf = 0;

    ArjunaName ArjName;
    ArjName.setServiceName("Interface");

    if (Definitions::DatabaseLocation)
	ArjName.setHostName(Definitions::DatabaseLocation);

    ClientRpc *crpc = new ClientRpc(&ArjName);
    ClientRpcManager *rpcc = new ClientRpcManager(crpc);

    res = -1;

    if (Definitions::DatabaseUID)
    {
	Uid x(Definitions::DatabaseUID);
	itf = new Interface(x, res, rpcc);
    }
    else
    {
	itf = new Interface(res, rpcc);
    }

    if (res != 1)
    {
	cerr << "Could not start up Interface database\n" << flush;
	if (itf)
	    delete itf;
	return 2;
    }

    buff1.packMappedString(Definitions::InterfaceName);
    res = -1;

    A.Begin();

    opcode = itf->Get(buff1, buff2, res);
    if (res != 0)
    {
	cerr << "An error occurred while attempting to access database\n" << flush;
	if (itf)
	    delete itf;
	return 3;
    }

    if (!opcode)
    {
	cerr << "Could not find interface in database\n" << flush;
	A.Abort();
	if (itf)
	    delete itf;
	return 4;
    }
    else
    {
	if (A.End() != COMMITTED)
	{
	    cerr << "An error occurred while committing action\n" << flush;
	    if (itf)
		delete itf;
	    return 5;
	}

	buff2.unpack(hostname);
	buff2.unpack(pathname);

	ArjunaName AN;
	AN.setServiceName("Transporter");
	AN.setHostName(hostname);

	ClientRpc *RPCClient = new ClientRpc(&AN);
	ClientRpcManager *RPCCont = new ClientRpcManager(RPCClient);

	res = 0;
	Transporter T(res, RPCCont);
	if (res == 0)
	{
	    cerr << "An error occurred while attempting to access remote node\n" << flush;
	    if (itf)
		delete itf;
	    return 6;
	}

	res = -1;

	buff3.packMappedString(pathname);
	buff4 = T.Get(buff3, res);

	if (res != 0)
	{
	    cerr << "An error occurred while getting remote interface\n" << flush;
	    if (itf)
		delete itf;
	    return 7;
	}
	else
	    buff4.unpack(contents1);

	ifstream inFile(Definitions::InterfaceName, ios::in);

	if (inFile)
	{
	    Boolean different = FALSE;

	    while ((inFile.get(c)) && (i < filesize) && (c != EOF))
	    {
		contents2[i] = c;
		i++;
	    }

	    if (strlen(contents1) != strlen(contents2))
		different = TRUE;
	    else
		if (memcmp(contents1, contents2, strlen(contents1)) != 0)
		    different = TRUE;

	    if (!different)
	    {
		if (itf)
		    delete itf;
		return 0;
	    }
	    else
	    {
		ofstream outFile(Definitions::InterfaceName, ios::out);
		
		outFile << contents1;
		if (itf)
		    delete itf;
		return 0;
	    }
	}
	else
	{
	    ofstream outFile(Definitions::InterfaceName, ios::out);
		
	    outFile << contents1;
	    if (itf)
		delete itf;
	    return 1;
	}
    }
}


		   
