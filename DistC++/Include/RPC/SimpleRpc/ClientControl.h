/*
 * Copyright 1990, 1991, 1992-1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef CLIENTCONTROL_H_
#define CLIENTCONTROL_H_

/*
 * $Id: ClientControl.h,v 1.4 1998/11/20 08:28:38 nmcl Exp $
 */


#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

extern const char* defaultServer;

#ifndef DEBUG
const long DEFAULT_TIMEOUT = 20;  /* In milliseconds. */
const long DEFAULT_RETRY = 3;
const long INITTERM_TIMEOUT = 1000;
const long INITTERM_RETRY = 10;
const long BURST_SIZE = 4;
const long BURST_TIMEOUT = 10000 /* In microseconds */;
#else
const long DEFAULT_TIMEOUT = 1000;  /* In milliseconds. */
const long DEFAULT_RETRY = 3;
const long INITTERM_TIMEOUT = 2000;
const long INITTERM_RETRY = 10;
const long BURST_SIZE = 2;
const long BURST_TIMEOUT = 20000 /* In microseconds */;
#endif

enum ProtocolType { DGRAM, STREAM };

// @NoRemote, @NoMarshall
class ThreadMutex;

// @NoRemote, @NoMarshall
class ClientControl
{
public:
    ClientControl ();
    ClientControl (const ClientControl&);    
    virtual ~ClientControl ();

    ClientControl& operator= (const ClientControl&);
    
    Boolean valid () const;
    
    RPC_Status setServiceName (const char*);
    RPC_Status getServiceName (char*&) const;
    
    RPC_Status setHost (const char*);
    RPC_Status getHost (char*&) const;
    
    RPC_Status setTimeout (long = DEFAULT_TIMEOUT);
    RPC_Status getTimeout (long&) const;
    
    RPC_Status setRetry (long = DEFAULT_RETRY);
    RPC_Status getRetry (long&) const;

    RPC_Status setObjectUid (const Uid&);
    RPC_Status getObjectUid (Uid&) const;
    
    RPC_Status setServerID (const Uid&);
    RPC_Status getServerID (Uid&) const;
    
    RPC_Status setProtocol (ProtocolType);
    RPC_Status getProtocol (ProtocolType&) const;

    RPC_Status setInitTermTimeout (long = INITTERM_TIMEOUT);
    RPC_Status getInitTermTimeout (long&) const;

    RPC_Status setInitTermRetry (long = INITTERM_RETRY);
    RPC_Status getInitTermRetry (long&) const;

    RPC_Status setBurstTimeout (long = BURST_TIMEOUT);
    RPC_Status getBurstTimeout (long&) const;

    RPC_Status setBurstSize (long = BURST_SIZE);
    RPC_Status getBurstSize (long&) const;

    RPC_Status setPort (unsigned short);
    RPC_Status getPort (unsigned short&) const;
    
private:
    char *service;
    char *hostname;
    Uid  serverID;
    Uid  objectID;
    long timeout;
    long retry;
    long itTimeout;
    long itRetry;
    long burstSize;
    long burstTimeout;
    ProtocolType protocol;
    unsigned short port;

    ThreadMutex* _mutex;
    
    static char  *myHost;
};

#endif
