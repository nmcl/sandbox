/*
 * Copyright 1990-1997, 1998,
 * Computing Laboratory,
 * University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: SimpleRpcClient.cc,v 1.5 1998/11/20 08:28:50 nmcl Exp $
 */

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
#endif

#ifndef SIMPLERPCCLIENT_H_
#  include <RPC/SimpleRpc/SimpleRpcClient.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
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

SimpleRpcClient::SimpleRpcClient ( const char* serv, const char* host,
				   ProtocolType p)
				 : managerPort(new Port),
				   port(0),
				   rPort(0),
				   serverAddr(0),
				   status(UNINITIATED),
				   sequenceNumber(0),
				   initSequenceNumber(0),
				   doTerminate(TRUE),
				   ping(host),
				   _mutex(ThreadMutex::create())
{
    myControl.setServiceName(serv);
    myControl.setHost(host);
    myControl.setProtocol(p);
}

SimpleRpcClient::SimpleRpcClient (ProtocolType p)
				 : managerPort(new Port),
				   port(0),
				   rPort(0),
				   serverAddr(0),
				   status(UNINITIATED),
				   sequenceNumber(0),
				   initSequenceNumber(0),
				   doTerminate(TRUE),
				   ping(),
				   _mutex(ThreadMutex::create())
{
    myControl.setProtocol(p);
}

SimpleRpcClient::SimpleRpcClient ( const char* serv, ProtocolType p )
				 : managerPort(new Port),
				   port(0),
				   rPort(0),
				   serverAddr(0),
				   status(UNINITIATED),
				   sequenceNumber(0),
				   initSequenceNumber(0),
				   doTerminate(TRUE),
				   ping(),
				   _mutex(ThreadMutex::create())
{
    myControl.setServiceName(serv);
    myControl.setProtocol(p);
}

SimpleRpcClient::SimpleRpcClient ( PortAddress& portAddress, ProtocolType p )
				 : managerPort(new Port),
				   port(0),
				   rPort(0),
				   serverAddr(0),
				   status(OPERATIONAL),
				   sequenceNumber(0),
				   initSequenceNumber(0),
				   doTerminate(FALSE),
				   ping(portAddress),
				   _mutex(ThreadMutex::create())
{
    serverAddr = new PortAddress(portAddress);
    myControl.setProtocol(p);
}

SimpleRpcClient::SimpleRpcClient ( const SimpleRpcClient& toCopy )
				 : managerPort(new Port),
				   port(0),
				   rPort(0),
				   serverAddr(0),
				   status(toCopy.status),
				   myControl(toCopy.myControl),
				   sequenceNumber(0),
				   initSequenceNumber(0),
				   doTerminate(toCopy.doTerminate),
				   ping(toCopy.ping),
				   _mutex(ThreadMutex::create())
{
    if (toCopy.serverAddr != 0)
	serverAddr = new PortAddress(*toCopy.serverAddr);
    
    if (toCopy.port != 0)
	port = new Port;

    if (toCopy.rPort != 0)
	rPort = new ReliablePort;
}

SimpleRpcClient::SimpleRpcClient ( SimpleRpcClient* toCopy )
				 : managerPort((toCopy) ? new Port : 0),
				   port(0),
				   rPort(0),
				   serverAddr(0),
				   status((toCopy) ? toCopy->status : DEAD),
				   sequenceNumber(0),
				   initSequenceNumber(0),
				   doTerminate((toCopy) ? toCopy->doTerminate : FALSE),
				   ping(),
				   _mutex(ThreadMutex::create())
{
    if (toCopy != 0)
    {
	if (toCopy->serverAddr != 0)
	    serverAddr = new PortAddress(*toCopy->serverAddr);
    
	if (toCopy->port != 0)
	    port = new Port;

	if (toCopy->rPort != 0)
	    rPort = new ReliablePort;

	PortAddress addr;

	toCopy->ping.getAddr(addr);
	ping.setAddr(addr);
	
	myControl = toCopy->myControl;
    }
}

SimpleRpcClient::~SimpleRpcClient ()
{
    if (_mutex != 0)
	_mutex->lock();
    
    if ((status == OPERATIONAL) || (status == INOPERATIONAL)
	|| (status == UNABLE) || (status == INIT_UNABLE))
    {
	terminate();
    }

    if (serverAddr)
	delete serverAddr;

    if (rPort)
	delete rPort;

    if (port)
	delete port;

    if (managerPort)
	delete managerPort;
    
    _mutex->unlock();

    delete _mutex;
}

ClientControl& SimpleRpcClient::control ()
{
    return myControl;
}

/*
 * Regardless of the connection mode for RPC we use datagrams
 * for communicating with the manager.
 */

RPC_Status SimpleRpcClient::initiate ()
{
#ifdef DEBUG
    cout << "**SimpleRpcClient::initiate called" << endl;
#endif
    
    RPC_Status opcode = OPER_UNKNOWN;

    if (_mutex && _mutex->lock())
    {
	if ((status != OPERATIONAL) && (status != DEAD))
	{
	    /*
	     * Do we have a supplied PortAddress in the control?
	     * If so, use this and don't do an initiate.
	     */
	    
	    unsigned short portNumber = 0;
	    char* hostName = 0;

	    if (myControl.getPort(portNumber) == OPER_DONE)
	    {
		if (myControl.getHost(hostName) == OPER_DONE)
		{
		    serverAddr = new PortAddress(hostName, portNumber);

		    opcode = OPER_DONE;
		    status = OPERATIONAL;
		    doTerminate = FALSE;
		}

		if (hostName)
		    ::delete [] hostName;
	    }
	    else
	    {
		/* Should do terminate if failed. */

		Buffer *sendBuff[1];
		Buffer *recBuff[1];
		Buffer work;
		Buffer result;
		long numberRetry = 0;
		Boolean initiated = FALSE;
		Uid serverID;
		char* serviceName = 0;
		char* hostName = 0;
		ProtocolType protocol;
		long it_timeout;
		long it_retry;
		int number = 1;

		if (!myControl.valid())
		    opcode = OPER_NOTDONE;
		else
		{
		    myControl.getServiceName(serviceName);
		    myControl.getHost(hostName);
		    myControl.getServerID(serverID);
		    myControl.getProtocol(protocol);
		    myControl.getInitTermTimeout(it_timeout);
		    myControl.getInitTermRetry(it_retry);

		    initSequenceNumber = ++sequenceNumber;
		    
		    if (work.pack(DO_INITIATE) && work.packMappedString(serviceName) &&
			serverID.pack(work) && clientID.pack(work) && work.pack(++initSequenceNumber) &&
			work.pack(protocol))
		    {
			sendBuff[0] = &work;
			recBuff[0] = &result;

			PortAddress managerGlobalAddr(hostName, MANAGER_PORT_GLOBAL), from;
		    
			do
			{
			    if (managerPort->sendMessage(managerGlobalAddr, sendBuff, 1) == Port::DONE)
			    {
				if (managerPort->receiveMessage(from, recBuff, number, it_timeout) == Port::DONE)
				{
				    result.unpack(initiated);

				    if (initiated)
				    {
					serverAddr = new PortAddress;

					serverAddr->unpack(result);

					status = OPERATIONAL;
					opcode = OPER_DONE;
				    }
				    else
					opcode = OPER_NOTDONE;
				}
				else
				{
				    if (!ping.isAlive())
				    {
					status = INOPERATIONAL;
				    }
				}
			    }
		    
			} while ((status != OPERATIONAL) && (numberRetry++ < it_retry));

			if ((opcode != OPER_DONE) && (status != INOPERATIONAL))
			{
			    RPC_Status termStatus;
		    
			    status = INOPERATIONAL;
			    termStatus = terminate();

			    status = ((termStatus == OPER_DONE) ? UNINITIATED : INIT_UNABLE);
			}
		    }
		    else
			opcode = OPER_NOTDONE;
   
		    if (serviceName)
			::delete [] serviceName;
	    
		    if (hostName)
			::delete [] hostName;
		}
	    }
	}
	else
	    opcode = OPER_DONE;  // already initiated

	if (_mutex != 0)
	    _mutex->unlock();
    }

#ifdef DEBUG
    cout << "**SimpleRpcClient::initiate - returning " << opcode << endl;
#endif

    return opcode;
}

RPC_Status SimpleRpcClient::terminate ()
{
#ifdef DEBUG
    cout << "**SimpleRpcClient::terminate called" << endl;
#endif

    RPC_Status outcome = OPER_NOTDONE;

    if (_mutex && _mutex->lock())
    {
	if (!doTerminate)
	    status = DEAD;
	
	if ((status != DEAD) && (status != UNINITIATED))
	{
	    Buffer *sendBuff[1];
	    Buffer *recBuff[1];
	    Buffer work;
	    Buffer result;
	    long numberRetry = 0;
	    Uid serverID;
	    char* serviceName = 0;
	    char* hostName = 0;
	    long it_timeout;
	    long it_retry;
	    int number = 1;

	    if (!myControl.valid())
		outcome = OPER_NOTDONE;
	    else
	    {
		myControl.getServiceName(serviceName);
		myControl.getHost(hostName);
		myControl.getServerID(serverID);
		myControl.getInitTermTimeout(it_timeout);
		myControl.getInitTermRetry(it_retry);
    
		if (work.pack(DO_TERMINATE) && work.packMappedString(serviceName) &&
		    serverID.pack(work) && clientID.pack(work) && work.pack(initSequenceNumber))
		{
		    sendBuff[0] = &work;
		    recBuff[0] = &result;

		    if ((status == OPERATIONAL) || (status == INOPERATIONAL))
		    {
			PortAddress managerGlobalAddr(hostName, MANAGER_PORT_GLOBAL), from;

			do
			{
			    if (managerPort->sendMessage(managerGlobalAddr, sendBuff, 1) == Port::DONE)
			    {
				if (managerPort->receiveMessage(from, recBuff, number, it_timeout) == Port::DONE)
				{
				    status = DEAD;
				    outcome = OPER_DONE;
				}
			    }
			    
			} while ((status != DEAD) && (numberRetry++ < it_retry));
		    }
		}
		else
		    outcome = OPER_NOTDONE;

		if (serviceName)
		    ::delete [] serviceName;
		
		if (hostName)
		    ::delete [] hostName;
	    }
	}
	else
	    outcome = OPER_DONE;  // already terminated

	_mutex->unlock();
    }
    
#ifdef DEBUG
    cout << "**SimpleRpcClient::terminate - returning " << outcome << endl;
#endif    
	
    return outcome;
}

RPC_Status SimpleRpcClient::call ( Buffer* work[], int sendN,
				   Buffer* result[], int& recN )
{
#ifdef DEBUG
    cout << "**SimpleRpcClient::call called" << endl;
#endif
    
    ProtocolType protocol;
    RPC_Status outcome = OPER_NOTDONE;

    if ((sendN <= 0) || (recN <= 0))
	return OPER_NOTDONE;
    
    if (_mutex->lock())
    {
	if ((status != DEAD) && (status != UNINITIATED))
	{
	    myControl.getProtocol(protocol);

	    switch (protocol)
	    {
	    case DGRAM:
		outcome = callDGRAM(work, sendN, result, recN);
		break;
	    case STREAM:
		outcome =  callSTREAM(work, sendN, result, recN);
		break;
	    default:
		break;
	    }
	}

	if (outcome == OPER_NOTDONE)
	    status = INOPERATIONAL;
	
	_mutex->unlock();
    }

#ifdef DEBUG
    cout << "**SimpleRpcClient::call - returning " << outcome << endl;
#endif
    
    return outcome;
}

/*
 * No need to protect with mutex since the wrapper function does this
 * for us.
 */

RPC_Status SimpleRpcClient::callDGRAM ( Buffer* work[], int sendN,
					Buffer* result[], int& recN )
{
    Port::POutcome outcomeP;
    long numberRetry = 0;
    long timeout = 0;
    long retry = 0;

    if (!port)
    {
	long burstSize = 0;
	long burstTimeout = 0;

	port = new Port;
	
	myControl.getBurstSize(burstSize);
	myControl.getBurstTimeout(burstTimeout);
	
	port->setBurstSize(burstSize);
	port->setBurstTimeout(burstTimeout);
    }

    if (!serverAddr)
	return OPER_NOTDONE;

    if (port->valid() && serverAddr->valid())
    {
	Buffer** theWork = new Buffer* [sendN+1];
	Buffer** theResult = new Buffer* [recN+1];
	int receiveNumber = recN+1;
	int created = recN;
	
	for (int i = 0; i < sendN; i++)
	    theWork[i] = work[i];

	theWork[sendN] = new Buffer;
	theWork[sendN]->pack(++sequenceNumber);

	for (int j = 0; j < recN; j++)
	    theResult[j] = result[j];

	theResult[recN] = new Buffer;
	
	myControl.getTimeout(timeout);
	myControl.getRetry(retry);

	Boolean machineFailure = FALSE;
	
	do
	{
	    outcomeP = port->sendMessage(*serverAddr, theWork, sendN+1);

	    if (outcomeP == Port::DONE)
	    {
		PortAddress fromAddr;
		
		receiveNumber = recN+1;

		outcomeP = port->receiveMessage(fromAddr, theResult, receiveNumber, timeout);

		/*
		 * In a multi-threaded environment we may get replies
		 * from a different thread and port to the one we
		 * sent the request. Therefore, don't check the fromAddr
		 * and assume that the sequence numbers will take care of
		 * any wrong messages.
		 */

		if (outcomeP == Port::DONE)
		{
		    long dummySequence = 0;

		    theResult[receiveNumber-1]->unpack(dummySequence);
		    
		    if (sequenceNumber != dummySequence)
		    {
			outcomeP = Port::NOTDONE;
			delete theResult[created];
			theResult[created] = 0;
		    }
		    else
			recN = receiveNumber-1;
		}
		else
		{
		    if (!ping.isAlive())
			machineFailure = TRUE;
		}
	    }
	    else
	    {
		if (!ping.isAlive())
		    machineFailure = TRUE;
	    }
	} while ((outcomeP != Port::DONE) && (numberRetry++ < retry) && (!machineFailure));

	if (machineFailure)
	    status = UNABLE;
	
	if (theWork[sendN])
	    delete theWork[sendN];
	if (theResult[created])
	    delete theResult[created];

	delete theWork;
	delete theResult;
	
        if (outcomeP == Port::DONE)
            return OPER_DONE;
        else if (outcomeP == Port::NOTDONE)
            return OPER_NOTDONE;
        else
            return OPER_UNKNOWN;
    }
    else
	return OPER_NOTDONE;
}

/*
 * Don't currently bother with pinging when using tcp. We assume
 * if we can't get a connection or make a call it's because the
 * machine has failed.
 */

RPC_Status SimpleRpcClient::callSTREAM ( Buffer* work[], int sendN,
					 Buffer* result[], int& recN )
{
    RPC_Status outcome = OPER_NOTDONE;

    if (!rPort)
	rPort = new ReliablePort;

    if (!serverAddr)
	return OPER_NOTDONE;

    if (rPort->valid() && serverAddr->valid())
    {
	TCPConnection* connection = rPort->setupConnection(*serverAddr);

	if (connection)
	{
	    Buffer** theWork = new Buffer* [sendN+1];
	    Buffer** theResult = new Buffer* [recN+1];
	    int receiveNumber = recN+1;
	    int created = recN;
	    
	    for (int i = 0; i < sendN; i++)
		theWork[i] = work[i];
	    
	    theWork[sendN] = new Buffer;
	    theWork[sendN]->pack(++sequenceNumber);
	    
	    for (int j = 0; j < recN; j++)
		theResult[j] = result[j];
	    
	    theResult[recN] = new Buffer;

	    if (connection->sendMessage(theWork, sendN+1) == TCPConnection::DONE)
	    {
		if (connection->receiveMessage(theResult, receiveNumber) != TCPConnection::DONE)
		    outcome = OPER_UNKNOWN;
		else
		{
		    long dummySequence = 0;

		    theResult[receiveNumber-1]->unpack(dummySequence);

		    if (sequenceNumber == dummySequence)
		    {
			recN = receiveNumber-1;
			outcome = OPER_DONE;
		    }
		}
	    }

	    if (theWork[sendN])
		delete theWork[sendN];
	    if (theResult[created])
		delete theResult[created];

	    delete theWork;
	    delete theResult;
	    
	    delete connection;
	}
	else
	    outcome = OPER_UNKNOWN;
    }

    return outcome;
}
	
ServerStatus SimpleRpcClient::getServerStatus () const
{
    ServerStatus toReturn = UNKNOWN_STATUS;

    if (_mutex->lock())
    {
	toReturn = status;

	_mutex->unlock();
    }

    return toReturn;
}

PortAddress SimpleRpcClient::getServerAddress () const
{
    PortAddress toReturn;

    if (_mutex->lock())
    {
	if (serverAddr)
	    toReturn = *serverAddr;

	_mutex->unlock();
    }

    return toReturn;
}

ostream& SimpleRpcClient::print ( ostream& strm ) const
{
    char* serviceName = 0;
    char* hostName = 0;
    
    myControl.getServiceName(serviceName);
    myControl.getHost(hostName);
    
    strm << "<SimpleRpcClient:";

    if (port)
	strm << *port;
    else
	if (rPort)
	    strm << *rPort;
    else
	strm << "No connection";

    strm << ',';
    
    if (serverAddr)
	strm << *serverAddr;
    else
	strm << "No address";

    strm << ',' << status << ',' << serviceName << ',' << hostName << '>';
    
    if (serviceName)
	::delete [] serviceName;
    
    if (hostName)
	::delete [] hostName;
    
    return strm;
}

ostream& operator<< ( ostream& strm, const SimpleRpcClient& c )
{
    return c.print(strm);
}
