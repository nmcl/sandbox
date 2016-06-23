/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: in.h,v 1.1 1997/09/25 15:27:23 nmcl Exp $
 */

#ifndef NETINET_IN_H_
#define NETINET_IN_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifdef STUB
#  pragma @NoRename
#endif

extern "C"
{
#if defined(__alpha__) && defined(__osf__)
    unsigned short ntohs (unsigned short);
    unsigned short htons (unsigned short);
    unsigned int htonl(unsigned int);
    unsigned int ntohl(unsigned int);
#endif

#if defined(__mips) && defined(__ultrix)
    unsigned short ntohs (unsigned short);
    unsigned short htons (unsigned short);
    unsigned long htonl(unsigned long);
    unsigned long ntohl(unsigned long);

#endif

#ifdef HIDE_INET_PROTOTYPES
#  define ntohs ntohs__
#  define ntohl ntohl__
#  define htons htons__
#  define htonl htonl__
#endif

#include <netinet/in.h>

#ifdef HIDE_INET_PROTOTYPES
#  undef ntohs
#  undef ntohl
#  undef htons
#  undef htonl
#endif

}

#endif
