/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: socket.h,v 1.11 1996/06/24 09:17:03 nmcl Exp $
 */

#ifndef SYS_SOCKET_H_
#define SYS_SOCKET_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef WIN32

#include <sys/socket.h>

#define SOCKET int
#define INVALID_SOCKET -1

#else

#include <winsock.h>

#endif

#ifdef __ultrix
/*
 * Ultrix has some prototypes but not others!
 */

extern "C" {
    int bind(SOCKET, const struct sockaddr *, int);
    int getsockname(SOCKET, struct sockaddr *, int *);
}
#endif

#if defined(NEED_GCC_PROTOTYPES)
extern "C" {
    int bind(SOCKET, const struct sockaddr *, int);
    int getsockname(SOCKET, struct sockaddr *, int *);
    int recv(SOCKET, char *, int, int);
    int recvfrom(SOCKET, char *, int, int, struct sockaddr *, int *);
    int send(SOCKET, const char *, int, int);
    int sendto(SOCKET, const char *, int, int, const struct sockaddr *, int);
    int socket(int, int, int);
    int recvmsg(SOCKET, struct msghdr *, int);
    int sendmsg(SOCKET, struct msghdr *, int);
    int connect(SOCKET, struct sockaddr*, int);
    int accept(SOCKET, struct sockaddr*, int*);
    int listen(SOCKET, int);    
}
#endif

#ifdef NEED_MSGHDR

struct msghdr
{
  caddr_t       msg_name;          /* optional address */
  int           msg_namelen;       /* size of address */
  struct iovec  *msg_iov;          /* scatter/gather array */
  int           msg_iovlen;        /* # elements in msg_iov */
  caddr_t       msg_accrights;     /* access rights sent/received */
  int           msg_accrightslen;
};

#endif

#ifdef NEED_SENDMSG

extern "C" int sendmsg (SOCKET, const struct msghdr*, unsigned int);
extern "C" int recvmsg (SOCKET, struct msghdr*, unsigned int);

#endif

#endif

