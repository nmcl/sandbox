/*
 * Copyright 1998,
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * UK.
 *
 * $Id: Ping.h,v 1.1 1998/11/20 08:28:39 nmcl Exp $
 */

#ifndef PING_H_
#define PING_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#define PING_SOCKET     4000  /* Socket for ping daemon to listen on */
#define PING_WAIT       1000  /* Wait time for ping in milliseconds */
#define PING_RETRY        10  /* Number of retries for ping */

#define TERMINATE_CODE     0  /* To kill ping daemon */
#define ARE_YOU_ALIVE      1

class Port;
class PortAddress;

class Ping
{
public:
    Ping (const char* host = 0, unsigned short port = PING_SOCKET);
    Ping (const PortAddress&, unsigned short port = PING_SOCKET);
    Ping (const Ping&);    
    ~Ping ();

    Boolean isAlive ();
    Boolean shutdown ();

    Boolean getAddr (PortAddress&) const;
    Boolean setAddr (const PortAddress&);
    
private:
    Port* pingPort;
    PortAddress* pingAddr;
};

#endif
