/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: RelPort.h,v 1.3 1996/02/12 14:42:32 nmcl Exp $
 */

#ifndef RELPORT_H_
#define RELPORT_H_

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
    
    RPOutcome pollForMessage (Boolean&) const;

    RPOutcome acceptConnection (PortAddress&);
    RPOutcome breakConnection ();

    RPOutcome setupConnection (const PortAddress&);
    RPOutcome endConnection ();
    
    RPOutcome getAddress (PortAddress&) const;

    ostream& print (ostream&) const;
    
private:
    Boolean validStream, isConnected;
    int sock, msgSock;
    struct sockaddr_in _sockAddr;
};

extern ostream& operator<< (ostream&, const ReliablePort&);

#endif

