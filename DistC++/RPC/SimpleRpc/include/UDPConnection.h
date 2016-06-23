/*
 * Copyright 1990-1997, 1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: UDPConnection.h,v 1.3 1998/11/20 08:28:47 nmcl Exp $
 */

#ifndef UDPCONNECTION_H_
#define UDPCONNECTION_H_

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef IOSTREAM_H_
# include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

class Port;
class PortAddress;

/*
 * A wrapper to the Port class so that we can have
 * a uniform way of interacting with udp and tcp
 * connections.
 *
 * No connection is actually established though.
 */

class UDPConnection
{
public:
    enum Outcome { DONE, NOTDONE, UNKNOWN };

    UDPConnection (Port*, const PortAddress&, Boolean canDelete = FALSE);
    UDPConnection (const UDPConnection&);
    ~UDPConnection ();

    Outcome sendMessage (Buffer*[], int);
    Outcome receiveMessage (Buffer*[], int&, long = -1);

    Outcome terminateConnection ();

    PortAddress getAddress () const;

    UDPConnection& operator= (const UDPConnection&);
    
private:
    Port* _port;
    PortAddress _addr;
    Boolean _connected;
    Boolean _canDelete;
};

#endif
