/*
 * Copyright 1990, 1991, 1992-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: RelPort.h,v 1.2 1998/11/20 08:28:47 nmcl Exp $
 */

#ifndef RELPORT_H_
#define RELPORT_H_

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
class TCPConnection;

class ReliablePort
{
    friend TCPConnection;
    
public:
    enum RPOutcome { DONE, NOTDONE, UNKNOWN };

    ReliablePort ();
    ReliablePort (Boolean);
    ReliablePort (PortAddress&);
    ~ReliablePort ();
    
    Boolean valid () const;

    /*
     * timeout in microseconds.
     */
    
    RPOutcome pollForMessage (long timeout) const;

    /*
     * Server side.
     */
    
    TCPConnection* acceptConnection (long timeout = -1);

    /*
     * Client side.
     */
    
    TCPConnection* setupConnection (const PortAddress&);

    RPOutcome getAddress (PortAddress&) const;

    ostream& print (ostream&) const;

protected:
    RPOutcome endConnection ();
    
private:
    Boolean validStream;
    Boolean isConnected;

    SOCKET sock;
    struct sockaddr_in _sockAddr;
};

extern ostream& operator<< (ostream&, const ReliablePort&);

#endif

