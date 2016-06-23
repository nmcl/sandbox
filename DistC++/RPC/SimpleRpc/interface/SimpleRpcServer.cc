/*
 * Copyright 1990-1997, 1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 */

/*
 * $Id: SimpleRpcServer.cc,v 1.4 1998/11/20 08:28:50 nmcl Exp $
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

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef SIMPLERPCSERVER_H_
#  include <RPC/SimpleRpc/SimpleRpcServer.h>
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
#endif

#ifndef SIMPLESERVERCONNECTION_H_
#  include <RPC/SimpleRpc/SimpleServerConnection.h>
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef RELPORT_H_
#  include "RelPort.h"
#endif

#ifndef TCPCONNECTION_H_
#  include "TCPConnection.h"
#endif

#ifndef UDPCONNECTION_H_
#  include "UDPConnection.h"
#endif

#include <Config/Configure.h>

#ifndef MAX_NAME
#define MAX_NAME 256
#endif

static char* ServerName = (char*) 0;

#ifdef HAVE_INTSIG
int terminateServer (int dummy DOTDOTDOT)
#else
void terminateServer (int dummy DOTDOTDOT)
#endif
{
    ::exit(0);
}


SimpleRpcServer::SimpleRpcServer ()
				 : port(0),
				   rPort(0),
#ifndef WIN32				   
				   isStream(FALSE)
#else
				   isStream(FALSE),
				   managerAddr(0)
#endif     
{
}

SimpleRpcServer::SimpleRpcServer ( PortAddress& portAddress,
				   Boolean useStream )
				 : port(0),
				   rPort(0),
#ifndef WIN32				   
				   isStream(FALSE)
#else
				   isStream(FALSE),
				   managerAddr(0)
#endif     
{
    if (useStream)
	rPort = new ReliablePort(portAddress);
    else
	port = new Port(portAddress);
}

SimpleRpcServer::~SimpleRpcServer ()
{
    if (port)
	delete port;
    if (rPort)
	delete rPort;
#ifdef WIN32    
    if (managerAddr)
	delete managerAddr;
#endif    
}

int SimpleRpcServer::initialise ( int argc, char** argv )
{
    PortAddress portAddress;
    char* logdir = ::getenv("SERVER_LOGDIR");
    Boolean noManager = FALSE;

    if (!logdir)
	logdir = "/tmp";

#ifndef WIN32
   ::chdir(logdir);
#else
   _chdir(logdir);
#endif

    ::signal(SIGTERM, terminateServer);
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], DGRAM_PROTOCOL) == 0)
	    isStream = FALSE;
	if (::strcmp(argv[i], STREAM_PROTOCOL) == 0)
	    isStream = TRUE;
	if (::strcmp(argv[i], NO_MANAGER) == 0)
	    noManager = TRUE;
    }

    ServerName = ::strdup(argv[0]);
    
#ifdef DEBUG
    char outputFile[MAX_NAME], errorFile[MAX_NAME];

    ::memset(outputFile, '\0', MAX_NAME);
    ::memset(errorFile, '\0', MAX_NAME);
    
    sprintf(outputFile, "%s_out_II_%ld", ServerName, getpid());
    sprintf(errorFile, "%s_err_II_%ld", ServerName, getpid());
    
    ofstream target1(outputFile);
    ofstream target2(errorFile);
    
    if (!target1)
	cout << "SimpleRpcServer: cannot open file " << outputFile << endl;
    else
	if (dup2(target1.rdbuf()->fd(), 1) == -1)
	    cout << "SimpleRpcServer: cannot assign output file descriptor" << endl;
    
    if (!target2)
	cout << "SimpleRpcServer: cannot open file " << outputFile << endl;
    else
	if (dup2(target2.rdbuf()->fd(), 2) == -1)
	    cout << "SimpleRpcServer: cannot assign error file descriptor" << endl;
#endif
    
    if (port == 0)
	port = new Port;

    if (isStream)
    {
	if (rPort == 0)
	    rPort = new ReliablePort(TRUE);
	
	rPort->getAddress(portAddress);
    }
    else
	port->getAddress(portAddress);

    if (!noManager)
    {
	Buffer buff;
	portAddress.pack(buff);
	buff.pack(getpid());
	Buffer* sendBuff[1];

	sendBuff[0] = &buff;

#ifdef DEBUG
	cout << "**SimpleRpcServer: identity is " << *this << endl;
	cout << "**SimpleRpcServer: sending address to manager" << endl;
#endif
    
	PortAddress managerLocalAddr(MANAGER_PORT_LOCAL);
	port->sendMessage(managerLocalAddr, sendBuff, 1);

#ifdef WIN32
	managerAddr = new PortAddress(managerLocalAddr);
#endif
    }
    else
    {
	cout << "Server identity is: " << *this << endl;
    }
    
    return 0;
}

#ifdef WIN32
void SimpleRpcServer::handleManager (Buffer* work[], int& number)
{
    if (number != 1)
    {
	cerr << "SimpleRpcServer::handleManager - unexpected message size." << endl;
	return;     // not expected!
    }

    int dummy = -1;
    managerOpcodes opcode;

    work[0]->unpack(dummy);
    opcode = (managerOpcodes) dummy;

    switch (opcode)
    {
    case DO_TERMINATE:
	terminateServer(1);
	break;
    default:
	cerr << "SimpleRpcServer::handleManager - unexpected opcode " << (int) opcode << endl;
	break;
    }
}
#endif

SimpleServerConnection* SimpleRpcServer::getWork ( Buffer* work[], int& number )
{
#ifdef DEBUG
    cout << "**SimpleRpcServer::GetWork - getting work" << endl;
#endif

    if (isStream)
	return getWorkSTREAM(work, number);
    else
	return getWorkDGRAM(work, number);
}

/*
 * Because NT will not allow one process to signal another (kill) we must handle
 * messages from the manager differently.
 */

SimpleServerConnection* SimpleRpcServer::getWorkSTREAM ( Buffer* work[],
							 int& number )
{
    if (rPort->valid())
    {
	TCPConnection* conn = rPort->acceptConnection();

	if (conn)
	{
	    Buffer** theWork = new Buffer* [number+1];
	    int created = number;
	    
	    for (int i = 0; i < number; i++)
		theWork[i] = work[i];
	    
	    theWork[created] = new Buffer;

	    number++;
	    
	    if (conn->receiveMessage(theWork, number) == TCPConnection::DONE)
	    {
#ifdef WIN32
		/*
		 * Manager protocol messages do not have the sequence number
		 * buffer attached to them.
		 */

		delete theWork[created];
		delete theWork;

		number--;
		
		if (conn->getAddress() == *managerAddr)
		{
		    handleManager(work, number);

		    delete conn;
		    
		    return NULL;
		}
#endif
		long seqNumb = 0;

		theWork[number-1]->unpack(seqNumb);

		delete theWork[created];
		delete theWork;

		number--;

		return new SimpleServerConnection(conn, seqNumb);
	    }
	    else
	    {
		delete theWork[created];
		delete theWork;
		
		delete conn;
	    }
	}
    }
    
    return NULL;
}
	
SimpleServerConnection* SimpleRpcServer::getWorkDGRAM ( Buffer* work[],
							int& number )
{
    if (port->valid())
    {
	PortAddress clientAddr;
	int created = number;
	
	Buffer** theWork = new Buffer* [number+1];

	for (int i = 0; i < number; i++)
	    theWork[i] = work[i];

	theWork[created] = new Buffer;

	number++;
	
	if (port->receiveMessage(clientAddr, theWork, number) == Port::DONE)
	{
	    UDPConnection* conn = new UDPConnection(new Port, clientAddr, TRUE);

#ifdef WIN32
	    if (clientAddr == *managerAddr)
	    {
		delete theWork[created];
		delete theWork;
		
		number--;
		
		handleManager(work, number);
		return NULL;
	    }
#endif
	    long seqNumb = 0;

	    theWork[number-1]->unpack(seqNumb);
	    
	    delete theWork[created];
	    delete theWork;

	    number--;
	    
	    return new SimpleServerConnection(conn, seqNumb);
	}
	else
	{
	    delete theWork[created];
	    delete theWork;
	}
    }

    return NULL;
}

PortAddress SimpleRpcServer::address () const
{
    PortAddress addr;
    
    if (rPort)
	rPort->getAddress(addr);
    else
	port->getAddress(addr);

    return addr;
}

const char* SimpleRpcServer::getServerName ()
{
    return ServerName;
}

ostream& SimpleRpcServer::print ( ostream& strm ) const
{
    if (isStream)
	strm << "<SimpleRpcServer:" << *rPort << '>';
    else
	strm << "<SimpleRpcServer:" << *port << '>';
    
    return strm;
}

ostream& operator<< ( ostream& strm, const SimpleRpcServer& s )
{
    return s.print(strm);
}
