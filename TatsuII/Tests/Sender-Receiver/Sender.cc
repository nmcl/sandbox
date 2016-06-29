/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Sender.cc,v 1.1 1997/06/09 19:52:28 nmcl Exp $
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
#  include <RelPort.h>
#endif

#ifndef PORTADDRESS_H_
#  include <PortAddress.h>
#endif


int main(int, char**)
{
    PortAddress serveraddress(4000);
    if (!serveraddress.valid())
    {
	cerr << "Error. Invalid server address\n";
	exit(1);
    }

    ReliablePort myport;

    Buffer buff;
    Buffer *sendBuff[1], *recBuff[1];
    int number = -1;
    ReliablePort::RPOutcome res;
    char* message = "Hello World";
    char* recMsg = NULL;

    sendBuff[0] = &buff;

    buff.packMappedString(message);
    myport.setupConnection(serveraddress);
    res = myport.sendMessage(sendBuff, 1);

    Buffer buff1;
    recBuff[0] = &buff1;
    
    res = myport.receiveMessage(recBuff, number);
    
    buff1.unpack(recMsg);
    cout << "got back " << recMsg << endl;
    
    myport.endConnection();

    ::delete [] recMsg;
    
    return 0;
}
