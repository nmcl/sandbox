/*
 * Copyright 1990, 1991,
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

#ifndef _PORTADDRESS_H_
#define _PORTADDRESS_H_

/*
 * $Id: PortAddress.h,v 1.2 1992/01/28 11:46:50 nmcl Exp $
 */

#include <Arjuna/Buffer.h>

#include <sys/types.h>
#include <netinet/in.h>

class PortAddress
{
public:
    PortAddress ();
    PortAddress ( unsigned short);
    PortAddress ( const char*,  unsigned short);
    PortAddress (struct sockaddr_in&);
    ~PortAddress ();

    void operator= (const PortAddress&);
    bool operator== (const PortAddress&) const;

    bool pack (Buffer&) const;
    bool unpack (Buffer&);

    bool GetSockAddr(struct sockaddr_in&) const;
    void SetSockAddr(struct sockaddr_in);

    bool GetHost(char*& host) const;
    bool GetPort(unsigned short& portnum) const;
    void SetPort(unsigned short portnum);

    bool Valid() const;

    ostream& print (ostream&) const;
private:
    bool ValidAddr;
    struct sockaddr_in sock_addr;
};

inline ostream& operator<< ( ostream& strm, const PortAddress& ua )
{
    return ua.print(strm);
}

#endif _PORTADDRESS_H_
