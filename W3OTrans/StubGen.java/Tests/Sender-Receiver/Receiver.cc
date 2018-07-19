/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Receiver.cc,v 1.3 1994/09/02 14:10:11 ngdp Exp $
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

#ifndef PORT_H_
#  include "Port.h"
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

    Port myport(myaddress);
    
    if (!myport.valid())
    {
	cerr << "Error. Invalid Port\n";
	exit(1);
    }

    Buffer buff;
    Buffer *sendBuff[1], *recBuff[1];
    int number = -1;
    Port::POutcome res;
    char c;
    int i;
    int count = 0;
    
    recBuff[0] = &buff;
    res = myport.receiveMessage(from, recBuff, number);

    cout << "received returned " << res << endl;
    
    for (i = 0; i < 8096; i++)
	if (buff.unpack(c) && (c == 'a'))
	    count++;

    cerr << "Server received " << count << endl;

    Buffer buff1;

    for (i = 0; i < 8096; i++)
	buff1.pack('a');

    sendBuff[0] = &buff1;
    
    res = myport.sendMessage(from, sendBuff, 1);

    cout << "sent returned " << res << endl;
    
    return 0;
}
