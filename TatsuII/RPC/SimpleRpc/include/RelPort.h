/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: RelPort.h,v 1.1 1997/06/09 19:52:06 nmcl Exp $
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

class ReliablePort
{
public:
    enum RPOutcome { DONE, NOTDONE, UNKNOWN };

    ReliablePort ();
    ReliablePort (Boolean);
    ReliablePort (PortAddress&);
    ~ReliablePort ();
    
    Boolean valid () const;

    RPOutcome sendMessage (Buffer* [], int);
    RPOutcome receiveMessage (Buffer*[], int&);
    
    RPOutcome pollForMessage (long timeout) const;

    /*
     * timeout in microseconds.
     */
    
    RPOutcome acceptConnection (PortAddress&, long timeout = -1);
    RPOutcome breakConnection ();

    RPOutcome setupConnection (const PortAddress&);
    RPOutcome endConnection ();
    
    RPOutcome getAddress (PortAddress&) const;

    ostream& print (ostream&) const;
    
private:
    Boolean validStream, isConnected;

    SOCKET sock, msgSock;
    struct sockaddr_in _sockAddr;
};

extern ostream& operator<< (ostream&, const ReliablePort&);

#endif

