/*
 * Copyright 1995, 1996,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: SetupBuffer.h,v 1.1 1997/06/09 19:52:06 nmcl Exp $
 */

#ifndef SETUPBUFFER_H_
#define SETUPBUFFER_H_

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

const int MAX_BUFFERS = MSG_MAXIOVLEN;

class SetupBuffer
{
public:
    SetupBuffer ();
    ~SetupBuffer ();

    size_t bufferSize[MSG_MAXIOVLEN];
    size_t packetNumber;
    size_t endOfMessage;
};

#endif
