/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: common.cc,v 1.1 1997/09/25 15:28:28 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
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

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

static const char RCSid[] = "$Id: common.cc,v 1.1 1997/09/25 15:28:28 nmcl Exp $";

#ifdef NEED_SENDMSG

int sendmsg (int s, struct msghdr* msg, int flags)
{
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "Within sendmsg" << endl;
#endif

  int len = 0, rcode = 0, bufPtr = 0;

  // calculate size of packet

  len = msg->msg_accrightslen;
  for (int j = 0; j < msg->msg_iovlen; j++)
    len += msg->msg_iov[j].iov_len;

  char* buf = ::new char[len];

  for (int i = 0; i < msg->msg_iovlen; i++)
  {
      if (msg->msg_iov[i].iov_len > 0)
      {
	  ::memcpy(buf+bufPtr, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len);
	  bufPtr += msg->msg_iov[i].iov_len;
      }
  }

  if (msg->msg_accrightslen > 0)
  {
    ::memcpy(buf+bufPtr, (char*) msg->msg_accrights, msg->msg_accrightslen);
    bufPtr += msg->msg_accrightslen;
  }

  rcode = sendto(s, buf, len, flags, (struct sockaddr *) msg->msg_name, msg->msg_namelen);

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

  int len = 0, rcode, i;
  int bufPtr = 0, sizeLeft = 0;

  len = msg->msg_accrightslen;

  for (i = 0; i < msg->msg_iovlen; i++)
      len += msg->msg_iov[i].iov_len;

  char* recBuf = ::new char[len];

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

  sizeLeft = rcode;

  for (i = 0; (i < msg->msg_iovlen) && (sizeLeft); i++)
  {
      int copySize = msg->msg_iov[i].iov_len;

      if (bufPtr+copySize > rcode)
	  copySize = rcode-bufPtr;

      ::memcpy(msg->msg_iov[i].iov_base, recBuf+bufPtr, copySize);
      bufPtr += copySize;
      sizeLeft -= copySize;
  }

  if ((msg->msg_accrightslen > 0) && (sizeLeft == msg->msg_accrightslen))
  {
      ::memcpy(msg->msg_accrights, recBuf+bufPtr, msg->msg_accrightslen);
      bufPtr += msg->msg_accrightslen;
  }
  else
  {
      if ((msg->msg_accrightslen > 0) || (sizeLeft > 0))
      {
	  errno = EFAULT;
	  rcode = -1;
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
