/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Utility.cc,v 
 */

#ifndef SYS_TYPES_H_
#   include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef SYS_SELECT_H_
#  include <System/sys/select.h>
#endif

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef STRING_H_
#   include <System/string.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

unsigned long checkSum ( const void *buffer, size_t len )
{
    unsigned long g, h = 0;
    const char *p = (const char *)buffer;

    if (buffer)
    {
	while (len-- > 0)
	{
	    h = (h << 4) + (*p);
	    if ((g = h & 0xf0000000))
	    {
		h = h ^ (g >> 24);
		h = h ^ g;
	    }
	    p++;
	}
    }
    
    return h;
}

char* clone ( const char * const string )
{
    char* res;

    if (string == 0)
	res = 0;
    else
    {
	res  = new char[strlen(string) + 1];
	strcpy(res, string);
    }

    return res;
}


/*
 *   The OS uses key_t values to identify its shared memory regions
 *   and semaphores. So need to convert the object's type to a key_t.
 *   For simplicity we hash the type into a signed quantity.
 *   Make sure we do not generate IPC_PRIVATE by mistake
 */

key_t typeToKey ( const char *tname )
{
    char buffer[1024];
    ostrstream os(buffer, 1024, ios::out);
    unsigned long h = 0;

    /* Convert uid to string */

    os << tname << ends;

    h = checkSum(buffer, ::strlen(buffer));

    if (h == IPC_PRIVATE)
	h++;
    
    return (key_t) h;
}
 
#if defined(hpux) || defined (SYSV) || defined(__ultrix)
int usleep ( unsigned int useconds )
{
    struct timeval delay;

    delay.tv_sec = (long) (useconds/1000000);
    delay.tv_usec = useconds%1000000;

    (void) select (0, 0, 0, 0, &delay);

    return 0;
}
#endif
