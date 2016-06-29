/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef SIMPLERPCCLIENT_H_
#define SIMPLERPCCLIENT_H_

/*
 * $Id: SimpleRpcClient.h,v 1.1 1997/06/09 19:51:56 nmcl Exp $
 */


#ifndef RPCSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTCONTROL_H_
#  include <RPC/SimpleRpc/ClientControl.h>
#endif

class iostream;

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
    virtual ~SimpleRpcClient ();

    RPC_Status initiate ();

    RPC_Status call (Buffer*[], int, Buffer*[], int&);

    virtual RPC_Status terminate ();

    ServerStatus getServerStatus () const;
    
    ClientControl& control ();

    ostream& print(ostream& strm) const;
private:
    void initialise ();
    
    RPC_Status callDGRAM (Buffer*[], int, Buffer*[], int&);
    RPC_Status callSTREAM (Buffer*[], int, Buffer*[], int&);

    Port* port;
    ReliablePort* rPort;
    PortAddress*  serverAddr;
    ServerStatus  status;
    Uid           clientID;
    ClientControl myControl;
    long          sequenceNumber;
};

extern ostream& operator<< (ostream&, const SimpleRpcClient&);

#endif
