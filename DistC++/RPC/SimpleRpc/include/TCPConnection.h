/*
 * Copyright 1990-1997,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: TCPConnection.h,v 1.1 1997/09/25 15:28:50 nmcl Exp $
 */

#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

class iostream;
class ReliablePort;
class ThreadMutex;

/*
 * Server side connections only.
 */

class TCPConnection
{
public:
    enum Outcome { DONE, NOTDONE, UNKNOWN };

    TCPConnection (SOCKET, ReliablePort*, const PortAddress&);
    TCPConnection (const TCPConnection&);
    ~TCPConnection ();

    Outcome sendMessage (Buffer* [], int);
    Outcome receiveMessage (Buffer* [], int&);

    Outcome terminateConnection ();

    PortAddress getAddress () const;
    
    TCPConnection& operator= (const TCPConnection&);
    
private:
    SOCKET msgSock;
    ReliablePort* msgHandle;
    PortAddress addr;
    ThreadMutex* mutex;
};

#endif

