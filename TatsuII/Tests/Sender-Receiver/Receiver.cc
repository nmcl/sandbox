/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Receiver.cc,v 1.1 1997/06/09 19:52:28 nmcl Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef RELPORT_H_
#  include "RelPort.h"
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif


int main(int, char**)
{
    PortAddress myaddress(4000);
    PortAddress from;

    if (!myaddress.valid())
    {
	cerr << "Error. Invalid PortAddress\n";
	exit(1);
    }

    ReliablePort myport(myaddress);
    
    if (!myport.valid())
    {
	cerr << "Error. Invalid Port\n";
	exit(1);
    }

    Buffer buff;
    Buffer *sendBuff[1], *recBuff[1];
    int number = -1;
    ReliablePort::RPOutcome res;
    char* message = "Test Message";
    char* recMsg = NULL;

    recBuff[0] = &buff;
    myport.acceptConnection(from);
    res = myport.receiveMessage(recBuff, number);
    buff.unpack(recMsg);

    cerr << "Server received " << recMsg << "\n";

    ::delete [] recMsg;
    
    Buffer buff1;
    buff1.packMappedString(message);
    sendBuff[0] = &buff1;
    
    res = myport.sendMessage(sendBuff, 1);
    
    myport.breakConnection();
    
    return 0;
}
