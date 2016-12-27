/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: inet.h,v 1.2 1996/01/02 13:13:35 nsmw Exp $
 */

#ifndef OS_ARPA_INET_H_
#define OS_ARPA_INET_H_

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
