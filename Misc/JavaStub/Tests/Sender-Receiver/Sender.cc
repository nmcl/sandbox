/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Sender.cc,v 1.3 1994/09/02 14:10:13 ngdp Exp $
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
#  include <Port.h>
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

    Port myport;

    Buffer buff;
    Buffer *sendBuff[1], *recBuff[1];
    int number = -1;
    Port::POutcome res;
    int i;

    sendBuff[0] = &buff;

    for (i = 0; i < 8096; i++)
	buff.pack('a');
    
    res = myport.sendMessage(serveraddress, sendBuff, 1);

    cout << "send returned " << res << endl;
    
    Buffer buff1;
    recBuff[0] = &buff1;
    
    res = myport.receiveMessage(serveraddress, recBuff, number);

    char c;
    int count = 0;
    
    for (i = 0; i < 8096; i++)
    {
	if (buff1.unpack(c) && (c == 'a'))
	    count++;
    }
	    
    cout << "got back " << count << endl;

    return 0;
}
