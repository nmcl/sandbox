/*
 * Copyright 1990-1997,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Receiver.cc,v 1.2 1998/02/06 12:10:29 nmcl Exp $
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

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef RELPORT_H_
#  include "RelPort.h"
#endif

#ifndef TCPCONNECTION_H_
#  include "TCPConnection.h"
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

void* func (void* param)
{
    Buffer *recBuff[1];
    int number = 1;
    char* recMsg = (char*) 0;       // the message received
    Buffer *sendBuff[1];
    char* message = "Test Message"; // the message to send back
    TCPConnection* connection = (TCPConnection*) param;
    Buffer buff;
    	
    recBuff[0] = &buff;
	
    /*
     * Block receiving messages.
     */

    connection->receiveMessage(recBuff, number);

    Thread::yield();
    
    /*
     * Unpack and print the message.
     */
    
    buff.unpack(recMsg);

    cerr << "Receiver got " << recMsg << "\n";

    ::delete [] recMsg;

    /*
     * Prepare the reply.
     */
    
    Buffer buff1;
    buff1.packMappedString(message);

    Thread::yield();
    
    sendBuff[0] = &buff1;

    /*
     * Send the reply.
     */
    
    connection->sendMessage(sendBuff, 1);

    /*
     * Break the connection before terminating.
     */

    connection->terminateConnection();
    delete connection;

    Thread::yield();
    
    return 0;
}


int main (int, char**)
{
    PortAddress myaddress(4004);
    PortAddress from;

    if (!myaddress.valid())
    {
	cerr << "Error. Invalid PortAddress\n";
	exit(1);
    }

    /*
     * Use a TCP connection.
     */
    
    ReliablePort myport(myaddress);
    
    if (!myport.valid())
    {
	cerr << "Error. Invalid Port\n";
	exit(1);
    }

    Thread* t[10];

    for (int i = 0; i < 10; i++)
    {
	/*
	 * Initialise the connection.
	 */

	TCPConnection* connection = myport.acceptConnection();

	t[i] = new Thread(func, connection);
	t[i]->resume();
	
	Thread::yield();
    }

    for (int j = 0; j < 10; j++)
    {
	t[j]->join();
	delete t[j];
    }
    
    return 0;
}
