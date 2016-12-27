/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: socket.h,v 1.9 1996/08/22 09:15:53 nmcl Exp $
 */

#ifndef OS_SYS_SOCKET_H_
#define OS_SYS_SOCKET_H_

#include <sys/socket.h>

#if defined(NEED_GCC_PROTOTYPES) && defined(SUNOS4)
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

#ifdef NEED_MSGHDR

/*
 * Old versions of linux did not support sendmsg and recvmsg so
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

extern int sendmsg (int, const struct msghdr*, unsigned int);
extern int recvmsg (int, struct msghdr*, unsigned int);

#endif

#endif


