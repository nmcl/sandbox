/*
 * Copyright 1990-1997, 1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef SIMPLESERVERCONNECTION_H_
#define SIMPLESERVERCONNECTION_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

class TCPConnection;
class UDPConnection;

class SimpleServerConnection
{
public:
    enum Outcome { DONE, NOTDONE, UNKNOWN };

    SimpleServerConnection (TCPConnection*, long seq);
    SimpleServerConnection (UDPConnection*, long seq);
    virtual ~SimpleServerConnection ();

    Outcome sendResult (Buffer*[], int);
    
private:
    Outcome sendResultDGRAM (Buffer*[], int);
    Outcome sendResultSTREAM (Buffer*[], int);

    TCPConnection* tcpConn;
    UDPConnection* udpConn;
    long seqNumb;
};

#endif
