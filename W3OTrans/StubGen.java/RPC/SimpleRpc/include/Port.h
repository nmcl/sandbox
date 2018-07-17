/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Port.h,v 1.3 1996/04/03 13:01:16 nmcl Exp $
 */

#ifndef PORT_H_
#define PORT_H_

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

class Port
{
public:
    enum POutcome { DONE, NOTDONE, UNKNOWN };

    Port ();
    Port (PortAddress&);
    ~Port ();

    Boolean valid() const;

    POutcome sendMessage (PortAddress, Buffer*[], int);
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
    int         sock;
    BufferMap** sendBuffers;
    BufferMap** receiveBuffers;
    long        burstTimeout;
    long        burstSize;
};

extern ostream& operator<< (ostream&, const Port&);

#endif
