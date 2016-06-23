/*
 * Copyright 1990, 1991, 1992-1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef SIMPLERPCSERVER_H_
#define SIMPLERPCSERVER_H_

/*
 * $Id: SimpleRpcServer.h,v 1.3 1998/11/20 08:28:40 nmcl Exp $
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
class SimpleServerConnection;

class SimpleRpcServer
{
public:
    enum SOutcome { DONE, NOTDONE, UNKNOWN };

    SimpleRpcServer ();
    SimpleRpcServer (PortAddress&, Boolean);
    virtual ~SimpleRpcServer ();

    virtual int initialise (int, char**);

    virtual SimpleServerConnection* getWork (Buffer*[], int&);

    ostream& print (ostream& strm) const;

    PortAddress address () const;
    
    static const char* getServerName ();
    
private:
    SimpleServerConnection* getWorkDGRAM (Buffer*[], int&);
    SimpleServerConnection* getWorkSTREAM (Buffer*[], int&);

#ifdef WIN32
    void handleManager (Buffer*[], int&);
#endif
    
    Port*         port;
    ReliablePort* rPort;
    Boolean       isStream;
#ifdef WIN32
    PortAddress*  managerAddr;
#endif
};

extern ostream& operator<< (ostream& strm, const SimpleRpcServer& server);

#endif
