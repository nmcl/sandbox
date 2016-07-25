/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: inet.h,v 1.4 1993/03/22 09:43:24 ngdp Exp $
 */

#ifndef ARPA_INET_H_
#define ARPA_INET_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef __GNUG__
#  include <arpa/inet.h>
#else

extern "C" {
        unsigned long    inet_addr(const char*);
        char*            inet_ntoa(struct in_addr);
        struct in_addr   inet_makeaddr(int, int);
        unsigned long    inet_network(const char*);
        int              inet_netof(struct in_addr);
}
#endif

#endif
