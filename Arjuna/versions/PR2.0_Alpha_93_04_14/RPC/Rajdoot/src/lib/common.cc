/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: common.cc,v 1.14 1993/03/18 15:41:29 nmcl Exp $
 */

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

static const char RCSid[] = "$Id: common.cc,v 1.14 1993/03/18 15:41:29 nmcl Exp $";

int _rpcerr;		    /* Error diagnostic variable set by rpc mechanism */
jmp_buf  _rpcenv;	    /* Stack environment variable */
struct RPC_SN  h_lastrpcsn; /* Sequence number of last received call
			       for use from server routines */
  

unsigned long get_machine_id()
{
    struct hostent *a;
    struct in_addr b;
    char name[MAXHOSTNAMELEN];

    gethostname(name, MAXHOSTNAMELEN);
    a = gethostbyname(name);
    memcpy((caddr_t) &b, a->h_addr, a->h_length);

    return ((unsigned long) b.s_addr);
}
