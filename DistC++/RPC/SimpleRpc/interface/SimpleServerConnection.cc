/*
 * Copyright 1990-1997, 1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 */

#ifdef WIN32
#  include <System/direct.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef SIMPLESERVERCONNECTION_H_
#  include <RPC/SimpleRpc/SimpleServerConnection.h>
#endif

#ifndef TCPCONNECTION_H_
#  include "TCPConnection.h"
#endif

#ifndef UDPCONNECTION_H_
#  include "UDPConnection.h"
#endif

SimpleServerConnection::SimpleServerConnection (TCPConnection* conn, long seq)
					       : tcpConn(conn),
						 udpConn(NULL),
						 seqNumb(seq)
{
}

SimpleServerConnection::SimpleServerConnection (UDPConnection* conn, long seq)
					       : tcpConn(NULL),
						 udpConn(conn),
						 seqNumb(seq)
{
}

SimpleServerConnection::~SimpleServerConnection ()
{
    if (tcpConn)
    {
	tcpConn->terminateConnection();  // tear down tcp connection
	
	delete tcpConn;
    }

    if (udpConn)
	delete udpConn;
}

SimpleServerConnection::Outcome SimpleServerConnection::sendResult ( Buffer* result[], int number )
{
#ifdef DEBUG
    cout << "**SimpleServerConnection::sendResult - sending result" << endl;
#endif
    
    Outcome res;
    
    if (tcpConn)
	res = sendResultSTREAM(result, number);
    else
	res = sendResultDGRAM(result, number);

#ifdef DEBUG
    cout << "**SimpleServerConnection::sendResult - returning " << res << endl;
#endif
    
    return res;
}

SimpleServerConnection::Outcome SimpleServerConnection::sendResultSTREAM ( Buffer* result[], int sendN )
{
    if (sendN <= 0)
	return NOTDONE;
    
    Buffer** reply = new Buffer* [sendN+1];

    for (int i = 0; i < sendN; i++)
	reply[i] = result[i];
    
    reply[sendN] = new Buffer;
    reply[sendN]->pack(seqNumb);

    TCPConnection::Outcome outcome = tcpConn->sendMessage(reply, sendN+1);

    delete reply[sendN];
    delete reply;
    
    switch (outcome)
    {
    case TCPConnection::DONE:
	return DONE;
    case TCPConnection::NOTDONE:
	return NOTDONE;
    default:
	return UNKNOWN;
    }
}

SimpleServerConnection::Outcome SimpleServerConnection::sendResultDGRAM ( Buffer* result[],
									  int sendN )
{
    if (sendN <= 0)
	return NOTDONE;
    
    Buffer** reply = new Buffer* [sendN+1];

    for (int i = 0; i < sendN; i++)
	reply[i] = result[i];
    
    reply[sendN] = new Buffer;
    reply[sendN]->pack(seqNumb);

    UDPConnection::Outcome outcome = udpConn->sendMessage(reply, sendN+1);

    delete reply[sendN];
    delete reply;
    
    switch (outcome)
    {
    case UDPConnection::DONE:
	return DONE;
    case UDPConnection::NOTDONE:
	return NOTDONE;
    default:
	return UNKNOWN;
    }
}
