/*
 * Copyright 1990-1997,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Sender.cc,v 1.2 1998/02/06 12:10:30 nmcl Exp $
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

#ifndef TCPCONNECTION_H_
#  include <TCPConnection.h>
#endif

#ifndef PORTADDRESS_H_
#  include <PortAddress.h>
#endif

int main (int, char**)
{
    PortAddress serveraddress(4004);
    
    if (!serveraddress.valid())
    {
	cerr << "Error. Invalid server address\n";
	exit(1);
    }

    /*
     * We will use a TCP connection.
     */
    
    ReliablePort myport;
    Buffer buff;
    Buffer *sendBuff[1];
    Buffer *recBuff[1];
    int number = 1;
    char* message = "Hello World";  // the message to send
    char* recMsg = (char*) 0;       // the message received back

    sendBuff[0] = &buff;

    /*
     * Pack the message into the buffer. Use mapped string in case we
     * have already packed it!
     */
    
    buff.packMappedString(message);

    /*
     * Now setup the connection to the received.
     * (Should really do some error checking here.)
     */
    
    TCPConnection* connection = myport.setupConnection(serveraddress);

    /*
     * OK, now send the message.
     */
    
    connection->sendMessage(sendBuff, 1);

    /*
     * Prepare the receiving buffer.
     */
    
    Buffer buff1;
    recBuff[0] = &buff1;

    /*
     * Receive the message. Block forever, or until connection
     * terminates.
     */
    
    connection->receiveMessage(recBuff, number);

    /*
     * Unpack and print reply message.
     */
    
    buff1.unpack(recMsg);
    
    cout << "Sender got back " << recMsg << endl;

    /*
     * Finally break connection.
     */
    
    connection->terminateConnection();
    delete connection;

    ::delete [] recMsg;
    
    return 0;
}
