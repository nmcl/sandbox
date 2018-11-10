/*
 * Copyright 1995, 1996,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: PortAddress.h,v 1.4 1996/02/12 14:42:31 nmcl Exp $
 */

#ifndef PORTADDRESS_H_
#define PORTADDRESS_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

class iostream;
class Buffer;

class PortAddress
{
public:
    PortAddress ();
    PortAddress (unsigned short);
    PortAddress (const char*, unsigned short);
    PortAddress(struct sockaddr_in&);
    ~PortAddress ();

    Boolean valid () const;

    void operator= (const PortAddress&);
    Boolean operator== (const PortAddress&) const;
    Boolean operator!= (const PortAddress&) const;

    Boolean getHost (char*&) const;
    Boolean getPort (unsigned short&) const;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    Boolean getSockAddr (struct sockaddr_in&) const;
    void setSockAddr (struct sockaddr_in);

    ostream& print(ostream&) const;

private:
    Boolean validAddr;
    struct sockaddr_in sockAddr;
};

extern ostream& operator<<(ostream&, const PortAddress&);

#endif
