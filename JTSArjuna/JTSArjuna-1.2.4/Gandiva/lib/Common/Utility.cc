/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Utility.cc,v 1.15 1998/01/12 13:05:53 nmcl Exp $
 */

#ifndef COMMON_UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef WIN32
#  include <os/sys/time.h>
#  include <os/strstream.h>
#else
#  include <os/time.h>
#  include <os/strstrea.h>
#endif

#include <os/iostream.h>
#include <os/string.h>
#include <os/sys/types.h>
#include <os/sys/select.h>

char *appendStrings (const char *n1, const char *n2)
{
    if ((!n1) && (!n2))
	return (char*) 0;
    
    size_t size1 = ((n1) ? ::strlen(n1) : 0);
    size_t size2 = ((n2) ? ::strlen(n2) : 0);
    size_t attrSize = size1 + size2;
    
    char *attributeName = ::new char[attrSize+1];
    ::memset(attributeName, '\0', attrSize+1);
    
    if (n1)
	::strcpy(attributeName, n1);

    if (n2)
	::strcat(attributeName, n2);

    return attributeName;
}

char *appendStrings (const char *n1, const char *n2, const char *n3)
{
    if ((!n1) && (!n2) && (!n3))
	return (char*) 0;
    
    size_t size1 = ((n1) ? ::strlen(n1) : 0);
    size_t size2 = ((n2) ? ::strlen(n2) : 0);
    size_t size3 = ((n3) ? ::strlen(n3) : 0);
    size_t attrSize = size1 + size2 + size3;
    
    char *attributeName = ::new char[attrSize+1];
    ::memset(attributeName, '\0', attrSize+1);
    
    if (n1)
	::strcpy(attributeName, n1);

    if (n2)
	::strcat(attributeName, n2);

    if (n3)
	::strcat(attributeName, n3);

    return attributeName;
}

#ifdef NEED_VOID_USLEEP
extern "C"
{
#ifdef WIN32
    void usleep(unsigned int useconds)
    {
        Sleep(useconds);
    }
#else
    void usleep(unsigned int useconds)
    {
        struct timeval delay;

        delay.tv_sec  = (long) (useconds / 1000000);
        delay.tv_usec = useconds % 1000000;

        (void) select(0, 0, 0, 0, &delay);
    }
#endif  
}
#endif

#ifdef NEED_INT_USLEEP
extern "C"
{
#ifdef WIN32
    int usleep(unsigned int useconds)
    {
        Sleep(useconds);

        return 0;
    }
#else
    int usleep(unsigned int useconds)
    {
        struct timeval delay;

        delay.tv_sec  = (long) (useconds / 1000000);
        delay.tv_usec = useconds % 1000000;

        (void) select(0, 0, 0, 0, &delay);

        return 0;
    }
#endif  
}
#endif

#ifdef WIN32

WinInit::WinInit ()
{
    WORD wVersionRequested = MAKEWORD(1, 1);
    WSADATA wsaData;

    if (WSAStartup(wVersionRequested, &wsaData) != 0)
	cerr << "WINSOCK startup failed!" << endl;
}

WinInit::~WinInit ()
{
    (void) WSACleanup();
}

const WinInit _win;

#endif
