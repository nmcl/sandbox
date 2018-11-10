/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: socket.h,v 1.7 1996/02/12 14:40:11 nmcl Exp $
 */

#ifndef SYS_SOCKET_H_
#define SYS_SOCKET_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/socket.h>

#ifdef __ultrix
/*
 * Ultrix has some prototypes but not others!
 */

extern "C" {
    int bind(int, const struct sockaddr *, int);
    int getsockname(int, struct sockaddr *, int *);
}
#endif

#if defined(NEED_GCC_PROTOTYPES)
extern "C" {
    int bind(int, const struct sockaddr *, int);
    int getsockname(int, struct sockaddr *, int *);
    int recv(int, char *, int, int);
    int recvfrom(int, char *, int, int, struct sockaddr *, int *);
    int send(int, const char *, int, int);
    int sendto(int, const char *, int, int, const struct sockaddr *, int);
    int socket(int, int, int);
    int recvmsg(int, struct msghdr *, int);
    int sendmsg(int, struct msghdr *, int);
    int connect(int, struct sockaddr*, int);
    int accept(int, struct sockaddr*, int*);
    int listen(int, int);
}
#endif

#ifdef __LINUX__

/*
 * currently linux does not support sendmsg and recvmsg so
 * we must implement them ourselves.
 */

#ifdef NEED_MSGHDR

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

#endif

#ifdef NEED_SENDMSG

int recvmsg(int, struct msghdr *, int);
int sendmsg(int, struct msghdr *, int);

#endif

#endif

#endif


