/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

#ifndef PORTADDRESS_H_
#define PORTADDRESS_H_

/*
 * $Id: PortAddress.h,v 1.1 1992/02/21 10:25:46 nmcl Exp $
 */

#include <iostream.h>

#include <Common/Boolean.h>
#include <Common/Buffer.h>

#include <sys/types.h>
#include <netinet/in.h>

class PortAddress
{
public:
    PortAddress ();                             // Create uninitialized PortAddress
    PortAddress (unsigned short portNumber);    // Initialize data for port on this node with given number
    PortAddress (const char* hostName, unsigned short portNumber);  // Initialize data for remote port
    ~PortAddress ();

    Boolean Valid() const;                     // Is PortAddress valid?

    void    operator=  (const PortAddress& portAddress); // Assign ports
    Boolean operator== (const PortAddress& portAddress) const; // Compare two ports

    Boolean GetHost(char*& hostName) const;   // Return host name for this port
    Boolean GetPort(unsigned short& portNumber) const;  // Return port number for this port

    Boolean pack   (Buffer& buffer) const;   // Pack port into buffer
    Boolean unpack (Buffer& buffer);         // Unpack port from buffer

private:
    Boolean ValidAddr;
    struct sockaddr_in sock_addr;

public:
    PortAddress (struct sockaddr_in&);

    Boolean GetSockAddr(struct sockaddr_in&) const;
    void    SetSockAddr(struct sockaddr_in);

    ostream& print (ostream&) const;
};

extern ostream& operator<< (ostream& strm, const PortAddress& portAddress);

#endif
