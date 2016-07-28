/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: inet.h,v 1.3 1995/02/10 12:00:52 ngdp Exp $
 */

#ifndef ARPA_INET_H_
#define ARPA_INET_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef HIDE_INET_PROTOTYPES
#  include <arpa/inet.h>
#else
#  define NEED_INET_PROTOTYPES
#endif

#ifdef NEED_GCC_PROTOTYPES
#  define NEED_INET_PROTOTYPES
#endif

#ifdef NEED_INET_PROTOTYPES
extern "C" {
        unsigned long    inet_addr(const char*);
        char*            inet_ntoa(struct in_addr);
        struct in_addr   inet_makeaddr(int, int);
        unsigned long    inet_network(const char*);
        int              inet_netof(struct in_addr);
}
#endif

#endif
