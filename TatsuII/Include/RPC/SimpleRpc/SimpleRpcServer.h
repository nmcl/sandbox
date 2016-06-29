/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef SIMPLERPCSERVER_H_
#define SIMPLERPCSERVER_H_

/*
 * $Id: SimpleRpcServer.h,v 1.1 1997/06/09 19:51:56 nmcl Exp $
 */


#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

class Port;
class ReliablePort;
class PortAddress;

class SimpleRpcServer
{
public:
    enum SOutcome { DONE, NOTDONE, UNKNOWN };

    SimpleRpcServer ();
    SimpleRpcServer (PortAddress&, Boolean);
    virtual ~SimpleRpcServer ();

    int initialise (int, char**);

    SOutcome getWork (Buffer*[], int&);

    SOutcome sendResult (Buffer*[], int);

    ostream& print(ostream& strm) const;
    
private:
#ifdef WIN32
    void handleManager (Buffer*[], int&);
#endif

    SOutcome getWorkDGRAM (Buffer*[], int&);
    SOutcome getWorkSTREAM (Buffer*[], int&);

    SOutcome sendResultDGRAM (Buffer*[], int);
    SOutcome sendResultSTREAM (Buffer*[], int);

    Port         *port;
    ReliablePort *rPort;
    PortAddress  *clientAddr;
    Boolean      isStream;
#ifdef WIN32
    PortAddress  *managerAddr;
#endif
};

extern ostream& operator<<(ostream& strm, const SimpleRpcServer& server);

#endif
