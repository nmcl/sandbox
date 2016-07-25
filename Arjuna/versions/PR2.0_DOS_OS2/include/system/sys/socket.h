/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: socket.h,v 1.1 1993/11/03 14:34:35 nmcl Exp $
 */

#ifndef SYS_SOCKET_H_
#define SYS_SOCKET_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/socket.h>


#ifdef __linux__

/*
 * currently linux does not support sendmsg and recvmsg so
 * we must implement them ourselves.
 */

// first define the data structure used;

struct msghdr
{
  caddr_t       msg_name;          /* optional address */
  int           msg_namelen;       /* size of address */
  struct iovec  *msg_iov;          /* scatter/gather array */
  int           msg_iovlen;        /* # elements in msg_iov */
  caddr_t       msg_accrights;     /* access rights sent/received */
  int           msg_accrightslen;
};

extern int sendmsg (int, struct msghdr*, int);
extern int recvmsg (int, struct msghdr*, int);

#endif

#endif


