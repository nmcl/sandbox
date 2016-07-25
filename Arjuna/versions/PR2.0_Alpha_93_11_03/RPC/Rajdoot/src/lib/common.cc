/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: common.cc,v 1.1 1993/11/03 14:41:47 nmcl Exp $
 */

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
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

#if defined(__linux__) || defined(USESG)
#  ifndef ERRNO_H_
#    include <System/errno.h>
#  endif
#  ifndef STDLIB_H_
#    include <System/stdlib.h>
#  endif
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

static const char RCSid[] = "$Id: common.cc,v 1.1 1993/11/03 14:41:47 nmcl Exp $";

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

#if defined(__linux__) || defined(USESG)

#define PACKETSIZE  MAX_SIZE+sizeof(struct info)

int sendmsg (int s, struct msghdr* msg, int flags)
{
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "Within sendmsg" << endl;
#endif

  int len = 0, rcode = 0, bufPtr = 0;
  long converter = 0;

  // calculate size of packet

  len += msg->msg_namelen + msg->msg_accrightslen + 3*sizeof(long);
  for (int j = 0; j < msg->msg_iovlen; j++)
    len += msg->msg_iov[j].iov_len + sizeof(long);

  if (len > PACKETSIZE)
    return EMSGSIZE;

  char* buf = ::new char[len];
  ::memset(buf, '\0', len); // maximum size can transmit in one go

  // convert scatter/gather buffer into single buffer

  converter = htonl(msg->msg_accrightslen);
  ::memcpy(buf, (char*) &converter, sizeof(converter));
  bufPtr += sizeof(converter);

  if (msg->msg_accrightslen > 0)
  {
    ::memcpy(buf+bufPtr, (char*) msg->msg_accrights, msg->msg_accrightslen);
    bufPtr += msg->msg_accrightslen;
  }

  converter = htonl(msg->msg_namelen);
  ::memcpy(buf+bufPtr, (char*) &converter, sizeof(converter));
  bufPtr += sizeof(converter);

  if (msg->msg_namelen > 0)
  {
    ::memcpy(buf+bufPtr, (char*) msg->msg_name, msg->msg_namelen);
    bufPtr += msg->msg_namelen;
  }

  converter = htonl(msg->msg_iovlen);
  ::memcpy(buf+bufPtr, (char*) &converter, sizeof(converter));
  bufPtr += sizeof(converter);

  for (int i = 0; i < msg->msg_iovlen; i++)
  {
      converter = htonl(msg->msg_iov[i].iov_len);
      ::memcpy(buf+bufPtr, (char*) &converter, sizeof(converter));
      bufPtr += sizeof(converter);

      if (msg->msg_iov[i].iov_len > 0)
      {
	  ::memcpy(buf+bufPtr, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len);
	  bufPtr += msg->msg_iov[i].iov_len;
      }
  }

  rcode = sendto(s, buf, (int) PACKETSIZE, flags, (struct sockaddr *) msg->msg_name, msg->msg_namelen);

#ifndef __GNUG__
  ::delete [] buf;
#else
  ::delete buf;
#endif

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "returning " << rcode << endl;
#endif
  
  return rcode;
}

int recvmsg (int s, struct msghdr* msg, int flags)
{
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "Within recvmsg" << endl;
#endif

  int len = (int) PACKETSIZE, rcode, bufPtr = 0;
  char* recBuf = ::new char[len];

  ::memset(recBuf, '\0', len);

  // attempt to read data
  rcode = recvfrom(s, recBuf, len, flags, (struct sockaddr*) msg->msg_name, &msg->msg_namelen);

  if (rcode <= 0)
  {
#ifndef __GNUG__
      ::delete [] recBuf;
#else
      ::delete recBuf;
#endif    
      return rcode;
  }

  // now do the necessary manipulation
  ::memcpy((char*)&msg->msg_accrightslen, recBuf, sizeof(long));
  msg->msg_accrightslen = (int) ntohl(msg->msg_accrightslen);
  bufPtr += sizeof(long);

  if (msg->msg_accrightslen > 0)
  {
    if (!msg->msg_accrights)
      msg->msg_accrights = ::new char[msg->msg_accrightslen];
    ::memcpy(msg->msg_accrights, recBuf+bufPtr, msg->msg_accrightslen);
    bufPtr += msg->msg_accrightslen;
  }

  ::memcpy((char*)&msg->msg_namelen, recBuf+bufPtr, sizeof(long));
  msg->msg_namelen = (int) ntohl(msg->msg_namelen);
  bufPtr += sizeof(long);

  if (msg->msg_namelen > 0)
  {
    if (!msg->msg_name)
      msg->msg_name = ::new char[msg->msg_namelen];
    ::memcpy(msg->msg_name, recBuf+bufPtr, msg->msg_namelen);
    bufPtr += msg->msg_namelen;
  }

  ::memcpy((char*)&msg->msg_iovlen, recBuf+bufPtr, sizeof(long));
  msg->msg_iovlen = (int) ntohl(msg->msg_iovlen);
  bufPtr += sizeof(long);

  if (msg->msg_iovlen > 0)
  {
    for (int i = 0; i < msg->msg_iovlen; i++)
    {
	::memcpy((char*)&msg->msg_iov[i].iov_len, recBuf+bufPtr, sizeof(long));
	msg->msg_iov[i].iov_len = (int) ntohl(msg->msg_iov[i].iov_len);
	bufPtr += sizeof(long);

	if (msg->msg_iov[i].iov_len > 0)
	{
	    if (!msg->msg_iov[i].iov_base)
		msg->msg_iov[i].iov_base = ::new char[msg->msg_iov[i].iov_len];
	    ::memcpy(msg->msg_iov[i].iov_base, recBuf+bufPtr, msg->msg_iov[i].iov_len);
	    bufPtr += msg->msg_iov[i].iov_len;
	}
    }
  }

#ifndef __GNUG__
  ::delete [] recBuf;
#else
  ::delete recBuf;
#endif
  
  return rcode;
}

#endif
