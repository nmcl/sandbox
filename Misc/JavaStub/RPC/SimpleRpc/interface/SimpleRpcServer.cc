/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 */

/*
 * $Id: SimpleRpcServer.cc,v 1.4 1995/07/18 10:12:49 ngdp Exp $
 */

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

#ifndef SIMPLERPCSERVER_H_
#  include <RPC/SimpleRpc/SimpleRpcServer.h>
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
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
    exit(0);
}


SimpleRpcServer::SimpleRpcServer ()
				 : port(0),
				   rPort(0),
				   isStream(FALSE)
{
}

SimpleRpcServer::SimpleRpcServer ( PortAddress& portAddress, Boolean useStream )
				 : port(0),
				   rPort(0),
				   isStream(FALSE)
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

    if (clientAddr)
	delete clientAddr;
}

int SimpleRpcServer::initialise ( int argc, char** argv )
{
    PortAddress portAddress;

    ::chdir("/tmp");

    ::signal(SIGTERM, terminateServer);
    
    clientAddr = new PortAddress;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "DGRAM") == 0)
	    isStream = FALSE;

	if (::strcmp(argv[i], "STREAM") == 0)
	    isStream = TRUE;
    }

#ifdef DEBUG
    char outputFile[MAX_NAME], errorFile[MAX_NAME];

    ::memset(outputFile, '\0', MAX_NAME);
    ::memset(errorFile, '\0', MAX_NAME);
    
    ServerName = ::strdup(argv[0]);

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

    return 0;
}

SimpleRpcServer::SOutcome SimpleRpcServer::getWork ( Buffer* work[],
						     int& number )
{
#ifdef DEBUG
    cout << "**SimpleRpcServer::getWork - getting work" << endl;
#endif
    
    SOutcome res;
    
    if (isStream)
	res = getWorkSTREAM(work, number);
    else
	res = getWorkDGRAM(work, number);

#ifdef DEBUG
    cout << "**SimpleRpcServer::GetWork - returning " << res << endl;
#endif
    
    return res;
}

SimpleRpcServer::SOutcome SimpleRpcServer::getWorkSTREAM ( Buffer* work[],
							   int& number )
{
    ReliablePort::RPOutcome outcomeRP;
    
    if (rPort->valid())
    {
	if (rPort->acceptConnection(*clientAddr) == ReliablePort::DONE)
	{
	    outcomeRP = rPort->receiveMessage(work, number);

	    if (outcomeRP == ReliablePort::DONE)
		return DONE;
	    else if (outcomeRP == ReliablePort::NOTDONE)
		return NOTDONE;
	    else
		return UNKNOWN;
	}
    }
    
    return NOTDONE;
}
	
SimpleRpcServer::SOutcome SimpleRpcServer::getWorkDGRAM ( Buffer* work[],
							  int& number )
{
    Port::POutcome outcomeP;

    if (port->valid())
    {
	outcomeP = port->receiveMessage(*clientAddr, work, number);

	if (outcomeP == Port::DONE)
	    return DONE;
	else if (outcomeP == Port::NOTDONE)
	    return NOTDONE;
	else
	    return UNKNOWN;
    }
    else
	return NOTDONE;
}

SimpleRpcServer::SOutcome SimpleRpcServer::sendResult ( Buffer* result[],
							int number )
{
#ifdef DEBUG
    cout << "**SimpleRpcServer::sendResult - sending result" << endl;
#endif
    
    SOutcome res;
    
    if (isStream)
	res = sendResultSTREAM(result, number);
    else
	res = sendResultDGRAM(result, number);

#ifdef DEBUG
    cout << "**SimpleRpcServer::sendResult - returning " << res << endl;
#endif
    
    return res;
}

SimpleRpcServer::SOutcome SimpleRpcServer::sendResultSTREAM ( Buffer* result[],
							      int sendN )
{
    ReliablePort::RPOutcome outcomeRP;

    if (rPort->valid() && clientAddr->valid())
    {
	outcomeRP = rPort->sendMessage(result, sendN);
	rPort->breakConnection();
	
	if (outcomeRP == ReliablePort::DONE)
	    return DONE;
	else if (outcomeRP == Port::NOTDONE)
	    return NOTDONE;
	else
	    return UNKNOWN;
    }
    
    return NOTDONE;
}

SimpleRpcServer::SOutcome SimpleRpcServer::sendResultDGRAM ( Buffer* result[],
							     int sendN )
{
    Port::POutcome outcomeP;

    if (port->valid() && clientAddr->valid())
    {
	outcomeP = port->sendMessage(*clientAddr, result, sendN);

	if (outcomeP == Port::DONE)
	    return DONE;
	else if (outcomeP == Port::NOTDONE)
	    return NOTDONE;
	else
	    return UNKNOWN;
    }
    else
	return NOTDONE;
}

ostream& SimpleRpcServer::print ( ostream& strm ) const
{
    if (isStream)
	strm << "<SimpleRpcServer:" << *rPort << ',' << *clientAddr << '>';
    else
	strm << "<SimpleRpcServer:" << *port << ',' << *clientAddr << '>';
    
    return strm;
}

ostream& operator<< ( ostream& strm, const SimpleRpcServer& s )
{
    return s.print(strm);
}