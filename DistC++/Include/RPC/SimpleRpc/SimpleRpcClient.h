/*
 * Copyright 1990-1997, 1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef SIMPLERPCCLIENT_H_
#define SIMPLERPCCLIENT_H_

/*
 * $Id: SimpleRpcClient.h,v 1.3 1998/11/20 08:28:40 nmcl Exp $
 */


#ifndef RPCSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef PING_H_
#  include <RPC/SimpleRpc/Ping.h>
#endif

#ifndef CLIENTCONTROL_H_
#  include <RPC/SimpleRpc/ClientControl.h>
#endif

class iostream;

// @NoRemote, @NoMarshall
class TCPConnection;

// @NoRemote, @NoMarshall
class ThreadMutex;

// @NoRemote, @NoMarshall
class Port;

// @NoRemote, @NoMarshall
class ReliablePort;

// @NoRemote, @NoMarshall
class PortAddress;

//  @NoRemote, @NoMarshall
class SimpleRpcClient
{
public:
    SimpleRpcClient (ProtocolType = DGRAM);
    SimpleRpcClient (const char*, ProtocolType = DGRAM);
    SimpleRpcClient (const char*, const char*, ProtocolType = DGRAM);
    SimpleRpcClient (PortAddress&, ProtocolType = DGRAM);
    SimpleRpcClient (SimpleRpcClient* toCopy);
    SimpleRpcClient (const SimpleRpcClient& toCopy);
    virtual ~SimpleRpcClient ();

    virtual RPC_Status initiate ();

    virtual RPC_Status call (Buffer*[], int, Buffer*[], int&);

    virtual RPC_Status terminate ();

    ServerStatus getServerStatus () const;
    PortAddress getServerAddress () const;
    
    ClientControl& control ();

    ostream& print (ostream& strm) const;
    
private:
    RPC_Status callDGRAM (Buffer*[], int, Buffer*[], int&);
    RPC_Status callSTREAM (Buffer*[], int, Buffer*[], int&);

    Port* managerPort;
    Port* port;
    ReliablePort*  rPort;
    PortAddress*   serverAddr;
    ServerStatus   status;
    Uid            clientID;
    ClientControl  myControl;
    long           sequenceNumber;
    long           initSequenceNumber;
    Boolean        doTerminate;
    Ping           ping;

    ThreadMutex*  _mutex;
};

extern ostream& operator<< (ostream&, const SimpleRpcClient&);

#endif
