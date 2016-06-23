/*
 * Copyright 1990, 1991, 1992-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Port.h,v 1.2 1998/11/20 08:28:46 nmcl Exp $
 */

#ifndef PORT_H_
#define PORT_H_

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

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

class BufferMap;
class SetupBuffer;
class ThreadMutex;

class Port
{
public:
    enum POutcome { DONE, NOTDONE, UNKNOWN };

    Port ();
    Port (PortAddress&);
    ~Port ();

    Boolean valid () const;

    POutcome sendMessage (const PortAddress&, Buffer*[], int);
    POutcome receiveMessage (PortAddress&, Buffer*[], int&, long = -1);
    POutcome pollForMessage (Boolean&, long = -1) const;
    
    POutcome getAddress (PortAddress&) const;

    /*
     * Operations for sending/receiving large udp messages.
     */
    
    void setBurstSize (long);
    void getBurstSize (long&) const;

    void setBurstTimeout (long);
    void getBurstTimeout (long&) const;

    ostream& print (ostream&) const;

private:
    int  createReceiveBuffers (const SetupBuffer&);
    void mapSendBuffers (Buffer*[], int);
    
    Boolean     validPort;
    SOCKET      sock;
    BufferMap** sendBuffers;
    BufferMap** receiveBuffers;
    long        burstTimeout;
    long        burstSize;

    ThreadMutex* _mutex;
};

extern ostream& operator<< (ostream&, const Port&);

#endif
